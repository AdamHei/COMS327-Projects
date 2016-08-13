//
// Created by tesla517 on 12/3/15.
//

#ifndef INC_327_PROJECT4_UTILFUNCTIONS_H
#define INC_327_PROJECT4_UTILFUNCTIONS_H

#include "cs229Object.h"
#include <iostream>
#include <getopt.h>
#include <bits/stl_list.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <string.h>

/**
 * Error codes for function displayError(), described within the function
 */
#define ERR_OPEN_FILE 2
#define ERR_NOT_CS229 3
#define ERR_NOT_DECLARED 4
#define ERR_NUM_OF_CHANNELS 5
#define ERR_NUM_OF_SAMPLES 6
#define ERR_DIFF_CHAN_NUM 7
#define ERR_NO_INPUT 8
#define ERR_ZERO_FACTOR 9
#define ERR_TOO_MANY__ARGS 10
#define ERR_LENGTH_TOO_SHORT 11
#define ERR_SUSTAIN_VS_PEAK 12
#define ERR_OUT_OF_DEPTH 13
#define ERR_FACT_OUT_RANGE 14
#define ERR_NO_ARGS 15
#define ERR_NO_WAVE 16

#define PI 3.14159265


/**
 * For setting and comparing waveforms
 */
typedef enum {SINE, TRIANGLE, SAWTOOTH, PULSE, NIL} Type;

/*
 * This parses a cs229 file and creates a representation in the form of a cs229Object, while performing error checking
 */
cs229Object validatorAndBuilder(std::string filename);

/*
 * This prints the basic information of a cs229Object to stdout
 */
void printer(cs229Object obj);

/*
 * Simply returns a lowercase string of the string provided
 */
std::string tolowercase(std::string input);

/*
 * Displays the help screen that corresponds to the argument passed in. Also exits the program.
 */
void displayHelpScreen(int code);

/*
 * Similar to displayHelpScreen, however it displays a useful error message for the user to consider
 */
void displayError(int errorCode);

/*
 *Similar to validatorAndBuilder, but it constructs a cs229Object from stdin
 */
cs229Object cinvalidtorAndBuilder();

/*
 * Simply returns a lowercase version of a char *
 */
char * charToLower(char * tolow);

/*
 * Concatenates the sample data several cs229Objects together and returns the summation
 */
cs229Object concat(std::vector<cs229Object> objects);

/*
 * Multiplies the data in each object by its respective factor, then merges all the
 * data into one resulting mixed object
 */
cs229Object mixem(double factors [], std::vector<cs229Object> &objects);

/*
 * Prints the contents of a cs229Object to a given file
 */
void printToFile(cs229Object obj, std::string filename);

/**
 * Prints the contents of a cs229Object to stdout
 */
void printToStdout(cs229Object obj);

#endif //INC_327_PROJECT4_UTILFUNCTIONS_H
