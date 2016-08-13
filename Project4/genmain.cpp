#include <iostream>
#include "utilFunctions.h"
#include "WaveGenerator.h"
#include "ADSREnvelope.h"

using namespace std;

/**
 * This program serves to produce a sound with specified frequency, waveform, and ADSR specifics.
 * It supports numerous switches, many of which are required to generate the sound.
 * Once the sound data has been parsed and checked, the initial sample data is generated
 * with WaveGenerator and stored in cs229Object initial. From there, the ADSREnvelope modifies the data, and it is
 * either outputed to a file or stdout.
 */

int main(int argc, char ** argv) {
    int c;
    string outputfilename = "";
    int bitRes = 0;
    double sampleRate = 0.0;
    double frequency = 0.0;
    double length = 0.0;
    double peakVol = 0.0;
    double attackTime = 0.0;
    double decayTime = 0.0;
    double sustainVol = 0.0;
    double releaseTime = 0.0;
    double pulseFact = 0.0;
    Type type = NIL;
    if (argc == 1)
    {
        displayError(ERR_NO_ARGS);
    }
    while (1) {
        static struct option long_options[] = {
                {"h",        no_argument,       0, 'h'},
                {"o",        required_argument, 0, 'o'},
                {"bits",     required_argument, 0, 'b'},
                {"sr",       required_argument, 0, 'e'},
                {"f",        required_argument, 0, 'f'},
                {"t",        required_argument, 0, 't'},
                {"v",        required_argument, 0, 'v'},
                {"a",        required_argument, 0, 'a'},
                {"d",        required_argument, 0, 'd'},
                {"s",        required_argument, 0, 's'},
                {"r",        required_argument, 0, 'r'},
                {"sine",     no_argument,       0, 'i'},
                {"triangle", no_argument,       0, 'g'},
                {"sawtooth", no_argument,       0, 'w'},
                {"pulse",    no_argument,       0, 'p'},
                {"pf",       required_argument, 0, 'u'},
                {0, 0,                          0, 0}
        };

        int option_index = 0;
        c = getopt_long(argc, argv, "ho:b:e:f:t:v:a:d:s:r:igwpc:", long_options, &option_index);

        if (c == -1) { break; }
        else if (c == 'h') displayHelpScreen(3);
        else if (c == 'o') outputfilename = optarg;
        else if (c == 'b') bitRes = atoi(optarg);
        else if (c == 'e') sampleRate = atof(optarg);
        else if (c == 'f') frequency = atof(optarg);
        else if (c == 't') length = atof(optarg);
        else if (c == 'v') peakVol = atof(optarg);
        else if (c == 'a') attackTime = atof(optarg);
        else if (c == 'd') decayTime = atof(optarg);
        else if (c == 's') sustainVol = atof(optarg);
        else if (c == 'r') releaseTime = atof(optarg);
        else if (c == 'i') type = SINE;
        else if (c == 'g') type = TRIANGLE;
        else if (c == 'w') type = SAWTOOTH;
        else if (c == 'p') type = PULSE;
        else if (c == 'u') pulseFact = atof(optarg);
    }

    if (type == NIL) displayError(ERR_NO_WAVE);
    if (optind < argc) displayError(ERR_TOO_MANY__ARGS);
    if (sustainVol > peakVol) displayError(ERR_SUSTAIN_VS_PEAK);

    WaveGenerator* waveGenerator = new WaveGenerator(type, length, frequency, sampleRate, bitRes, pulseFact);
    cs229Object initial = waveGenerator->generate();


    ADSREnvelope* envelope = new ADSREnvelope(bitRes, length, peakVol, attackTime, decayTime, sustainVol, releaseTime);
    initial.setSamples(envelope->modify(initial));

    if (outputfilename.length() > 0)
    {
        printToFile(initial, outputfilename);
    }
    else
    {
        printToStdout(initial);
    }

    delete(waveGenerator);
    delete(envelope);

    return 0;
}