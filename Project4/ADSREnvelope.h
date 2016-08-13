//
// Created by tesla517 on 12/10/15.
//

#ifndef SNDGEN_ADSRENVELOPE_H
#define SNDGEN_ADSRENVELOPE_H

#include "cs229Object.h"
#include "utilFunctions.h"
#include <tgmath.h>

/**
 * This class serves to manipulate pre-generated sample data to conform to specific ADSR values.
 * The values are given in the constructor while modify() serves to change the sample data
 * of a given cs229Object
 */

class ADSREnvelope {
public:
    ADSREnvelope(double bd, double len, double peakFact, double ta, double td, double susFact, double tr);
    std::vector<std::vector<double> > modify(cs229Object obj);

private:
    double bit_depth;
    double length;
    double peak_volume;
    double t_sub_a;
    double t_sub_d;
    double sustain_volume;
    double t_sub_r;
    double t_sub_sus;

    double attack_slope;
    double decay_slope;
    double b_sub_decay;
    double release_slope;
    double b_sub_release;
};


#endif //SNDGEN_ADSRENVELOPE_H
