//
// Created by tesla517 on 12/11/15.
//

#include "WaveGenerator.h"

using namespace std;

/**
 *  This class serves as something similar to an interface in that it will return a cs229Object resembling the
 *  waveform that was passed in during construction. Thus, new waveforms need only be added as another function
 *  within WaveGenerator without unecessary object construction. This resulting waveform is considered a "pure" wave,
 *  in that its properties are unaltered by the ADSREnvelope
 */

WaveGenerator::WaveGenerator(Type type, double len, double freq, double sr, int br, double pF) {
    t = type;
    length = len;
    frequency  = freq;
    sampleRate = sr;
    pulseFact = pF;
    bitres = br;
}

cs229Object WaveGenerator::generate() {
    cs229Object toReturn;
    toReturn.setSample_rate(sampleRate);
    toReturn.setChannels(1);
    toReturn.setNumSamples(sampleRate * length);
    toReturn.setBit_depth(bitres);
    toReturn.setFiletype("cs229");
    toReturn.setFilename("initial.txt");

    if (t == SINE) toReturn.setSamples(sinGen());
    else if (t == SAWTOOTH) toReturn.setSamples(sawtoothGen());
    else if (t == TRIANGLE) toReturn.setSamples(triangleGen());
    else toReturn.setSamples(pulseGen());

    return toReturn;
}

std::vector<std::vector<double> > WaveGenerator::pulseGen() {
    std::vector<std::vector<double> > toSet;
    double lengthbetweensamples = 1 / sampleRate;
    double period = (2 * PI) / frequency;
    if (pulseFact == 0) cout << "Warning: sustain volume may not have been set." << endl;
    for (double t = 0.0; t <= length; t += lengthbetweensamples)
    {
        std::vector<double> tempRow;
        double temp = fmod(t, period);
        if (temp <= pulseFact * period)
        {
            tempRow.push_back(1);
        }
        else
        {
            tempRow.push_back(-1);
        }
        toSet.push_back(tempRow);
    }
    return toSet;
}

std::vector<std::vector<double> > WaveGenerator::sawtoothGen() {
    std::vector<std::vector<double> > toSet;
    double lengthbetweensamples = 1 / sampleRate;
    double period = (2 * PI) / frequency;
    for (double t = 0.0; t <= length; t += lengthbetweensamples)
    {
        std::vector<double> tempRow;
        tempRow.push_back((-2 / PI) * atan(1 / tan((t * PI) / period)));
        toSet.push_back(tempRow);
    }
    return toSet;
}

std::vector<std::vector<double> > WaveGenerator::sinGen() {
    std::vector<std::vector<double> > toSet;
    double lengthbetweensamples = 1 / sampleRate;
    for (double t = 0.0; t <= length; t += lengthbetweensamples)
    {
        std::vector<double> tempRow;
        tempRow.push_back(sin(frequency * t));
        toSet.push_back(tempRow);
    }
    return toSet;
}

std::vector<std::vector<double> > WaveGenerator::triangleGen() {
    std::vector<std::vector<double> > toSet;
    double lengthbetweensamples = 1 / sampleRate;
    double period = (2 * PI) / frequency;
    for (double t = 0.0; t <= length; t += lengthbetweensamples)
    {
        std::vector<double> tempRow;
        double temp = fmod(t, period);
        if (temp <= (period / 4))
        {
            tempRow.push_back(-1 * 2 * frequency * temp / PI);
        }
        else if (temp > period / 4 && temp <= 3 * period / 4)
        {
            tempRow.push_back(2 * frequency * temp / PI - 2);
        }
        else
        {
            tempRow.push_back(-1 * 2 * frequency * temp / PI + 4);
        }
        toSet.push_back(tempRow);
    }
    return toSet;
}