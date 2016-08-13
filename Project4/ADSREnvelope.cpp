//
// Created by tesla517 on 12/10/15.
//

#include "ADSREnvelope.h"

/*
 * The ADSREnvelope constructor takes in the parameters necessary to describe the behavor of an envelope's
 * piece-wise curve. This means the slope-intercept forms for attack, decay, and release. The first two are simple
 * calculations, but release may change as the length of the file may be less that that of the three's sum. This
 * means that release will have a start amplitude other than that of the sustain volume.
 */
ADSREnvelope::ADSREnvelope(double bd, double len, double peakFact, double ta, double td, double susFact, double tr) {
    bit_depth = bd;
    length = len;
    peak_volume = peakFact * (pow(2.0, bit_depth) - 1);
    t_sub_a = ta;
    t_sub_d = td;
    sustain_volume = susFact * (pow(2.0, bit_depth) - 1);
    t_sub_r = tr;

    attack_slope = peak_volume / t_sub_a;
    decay_slope = (sustain_volume - peak_volume) / t_sub_d;
    b_sub_decay = peak_volume - ((sustain_volume - peak_volume) * t_sub_a / t_sub_d);

    t_sub_sus = length - t_sub_a - t_sub_r - t_sub_d;
    if (t_sub_sus > 0)
    {
        release_slope = -1 * sustain_volume / t_sub_r;
        b_sub_release = sustain_volume * length / t_sub_r;
    }
    else
    {
        t_sub_d += t_sub_sus;
        if (t_sub_d > 0)
        {
            double startRelease = decay_slope * (t_sub_a + t_sub_d) + b_sub_decay;
            release_slope = -1 * startRelease / t_sub_r;
            b_sub_release = startRelease * length / t_sub_r;
        }
        else
        {
            t_sub_a += t_sub_d;
            if (t_sub_a <= 0)
            {
                displayError(ERR_LENGTH_TOO_SHORT);
            }
            else
            {
                double startRelease = attack_slope * t_sub_a;
                release_slope = -1 * startRelease / t_sub_r;
                b_sub_release = startRelease * length / t_sub_r;
            }
        }
    }
}

/**
 * modify() takes in a cs229Object, pre-generated to symbolize an un-altered waveform with amplitude 1.
 * The sample values from this object are then modified by a piecewise function, thus conforming the waveform
 * to fit within the ADSREnvelope confines. The resulting values are returned and then set as the new data set.
 */
std::vector<std::vector<double> > ADSREnvelope::modify(cs229Object obj)
{
    double t = 0.0;
    double lengthBetweenSamples = 1 / obj.getSample_rate();
    std::vector<std::vector<double> > toReturn;
    for (std::vector<double> row: obj.getSamples())
    {
        std::vector<double> tempRow;
        if (t >= 0 && t <= t_sub_a)
        {
            tempRow.push_back(attack_slope * t * row.at(0));
        }
        else if (t > t_sub_a && t < t_sub_d + t_sub_a)
        {
            tempRow.push_back((decay_slope * t + b_sub_decay) * row.at(0));
        }
        else if (t > t_sub_d + t_sub_a && t <= t_sub_d + t_sub_a + t_sub_sus)
        {
            tempRow.push_back(sustain_volume * row.at(0));
        }
        else if (t > t_sub_d + t_sub_a + t_sub_sus && t <= length)
        {
            tempRow.push_back((release_slope * t + b_sub_release) * row.at(0));
        }
        else
        {
            std::cerr << "T was outside of the length" << std::endl;
            exit(0);
        }

        t += lengthBetweenSamples;
        toReturn.push_back(tempRow);
    }
    return toReturn;
}
