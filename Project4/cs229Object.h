//
// Created by tesla517 on 11/25/15.
//

#ifndef INC_327_PROJECT4_CS229OBJECT_H
#define INC_327_PROJECT4_CS229OBJECT_H

#include <stdio.h>
#include <string>
#include <vector>

/**
 * This class serves as an useable and versatile container for the data held within a .cs229 file, or that which
 * will soon be written to one. File parsing methods within utilFunctions use this object to store the data, while
 * WaveGenerator, for instance, will populate an instance with data representing a clean waveform.
 *
 * The variables within this class have simple getter and setter methods for ease of use.
 */

class cs229Object {
private:
    std::string filename;
    std::string filetype;
    std::vector<std::vector<double> > samples;
    double sample_rate;
    int bit_depth;
    int channels;
    double numsamples;
    double length_in_s;

public:
    std::vector<std::vector<double> > getSamples() {
        return samples;
    }

    void setSamples(std::vector<std::vector<double> > samples) {
        cs229Object::samples = samples;
    }
    std::string getFilename() {
        return filename;
    }

    void setFilename(std::string filename) {
        cs229Object::filename = filename;
    }

    double getLength_in_s() {
        return length_in_s;
    }

    void setLength_in_s(double length_in_s) {
        cs229Object::length_in_s = length_in_s;
    }

    double getNumSamples() {
        return numsamples;
    }

    void setNumSamples(double samples) {
        cs229Object::numsamples = samples;
    }

    int getChannels() {
        return channels;
    }

    void setChannels(int channels) {
        cs229Object::channels = channels;
    }

    int getBit_depth() {
        return bit_depth;
    }

    void setBit_depth(int bit_depth) {
        cs229Object::bit_depth = bit_depth;
    }

    double getSample_rate() {
        return sample_rate;
    }

    void setSample_rate(double sample_rate) {
        cs229Object::sample_rate = sample_rate;
    }

    std::string getFiletype() {
        return filetype;
    }

    void setFiletype(std::string filetype) {
        cs229Object::filetype = filetype;
    }

    cs229Object();

    virtual ~cs229Object() { }

};


#endif //INC_327_PROJECT4_ABSSOUNDPROG_H
