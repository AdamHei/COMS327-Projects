//
// Created by tesla517 on 12/11/15.
//

#ifndef SNDGEN_WAVEGENERATOR_H
#define SNDGEN_WAVEGENERATOR_H

#include "cs229Object.h"
#include "utilFunctions.h"
#include <vector>
#include <math.h>

class WaveGenerator{
public:
    /**
     * The constructor just sets the values of the cs229Object to return
     */
    WaveGenerator(Type type, double len, double freq, double sr, int br, double pF);

    /**
     * Generate determines which wave to generate, then calls its corresponding function
     * to generate data set.
     */
    cs229Object generate();
    virtual ~WaveGenerator() { }

private:
    Type t;
    double length;
    double frequency;
    double sampleRate;
    double pulseFact;
    int bitres;

    /**
     * sinGen() uses the formula sin(frequency * time) to generate its values
     * Each function iterates from time = 0 to length, iterating by the length
     * between each sample, thus reaching the number of samples in the end
     */
    std::vector<std::vector<double> > sinGen();

    /**
     * triangleGen() uses a piece-wise function to determine what slope-intercept
     * form to apply to the time to find the data point it matches with
     */
    std::vector<std::vector<double> > triangleGen();

    /**
     * sawtoothGen() uses the formula: (-2 / PI) * atan(1 / tan((t * PI) / period))
     * to determine its data points
     */
    std::vector<std::vector<double> > sawtoothGen();

    /**
     * pulseGen() takes the modulo of the time and period, then compares that against
     * the fraction of the time the pulse is high to determine whether the data point is
     * -1 or 1
     */
    std::vector<std::vector<double> > pulseGen();
};


#endif //SNDGEN_WAVEGENERATOR_H
