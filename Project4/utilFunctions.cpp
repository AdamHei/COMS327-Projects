//
// Created by tesla517 on 12/3/15.
//

#include <tgmath.h>
#include "utilFunctions.h"

using namespace std;

/**
 * The utilFunctions source file serves as a collection of useful functions that can be shared across sndgen, sndmix,
 * sndcat, and sndmix. They involve everything from concatenation of objects, to outputing, to displaying helpful messages.
 */

cs229Object validatorAndBuilder(std::string filename)
{
    cs229Object toReturn;
    toReturn.setFilename(filename);
    ifstream myfile (filename);
    string line;

    if (!myfile.is_open())
    {
        displayError(ERR_OPEN_FILE);
    }

    string is229;
    getline(myfile, is229);
    is229 = tolowercase(is229.substr(0,5));

    if (is229.compare("cs229") != 0)
    {
        displayError(ERR_NOT_CS229);
    }
    else
    {
        toReturn.setFiletype("cs229");
    }
    bool startDataFound = false;
    while (getline(myfile, line) && !startDataFound)
    {
        if (line.find_first_not_of("\t\n\v\r") != std::string::npos && line[0] != '#')
        {
            if (line.length() > 6 && tolowercase(line.substr(0,7)).compare("samples") == 0)
            {
                toReturn.setNumSamples(stoi(line.substr(8)));
            }
            else if (line.length() > 7 && tolowercase(line.substr(0,8)).compare("channels") == 0)
            {
                toReturn.setChannels(stoi(line.substr(9)));
            }
            else if (line.length() > 6 && tolowercase(line.substr(0,6)).compare("bitres") == 0)
            {
                toReturn.setBit_depth(stoi(line.substr(7)));
            }
            else if (line.length() > 10 && tolowercase(line.substr(0,10)).compare("samplerate") == 0)
            {
                toReturn.setSample_rate(stoi(line.substr(11)));
            }
            else if (line.length() > 8 && tolowercase(line.substr(0,9)).compare("startdata") == 0)
            {
                if (toReturn.getBit_depth() == 0 || toReturn.getChannels() == 0 || toReturn.getSample_rate() == 0)
                {
                    displayError(ERR_NOT_DECLARED);
                }
                else
                {
                    startDataFound = true;
                }
            }
        }
    }

    int runningSamples = 0;
    vector<double> firstLine;
    vector<vector<double> > values;
    int val;

    if (line.find_first_not_of("\t\n\v\r") != std::string::npos && line[0] != '#')
    {
        stringstream firstlineStream(line);
        while (firstlineStream >> val)
        {
            if (val > pow(2, toReturn.getBit_depth()) || val < -1 * pow(2, toReturn.getBit_depth()))
            {
                displayError(ERR_OUT_OF_DEPTH);
            }
            firstLine.push_back(val);
        }
        if (firstLine.size() != toReturn.getChannels())
        {
            displayError(ERR_NUM_OF_CHANNELS);
        }
        else
        {
            values.push_back(firstLine);
            runningSamples += 1;
        }
    }

    while (getline(myfile, line))
    {
        if (line.find_first_not_of("\t\n\v\r") != std::string::npos && line[0] != '#')
        {
            stringstream lineStream(line);
            double value;
            vector<double> valueLine;

            while (lineStream >> value)
            {
                valueLine.push_back(value);
            }
            if (valueLine.size() != toReturn.getChannels())
            {
                displayError(ERR_NUM_OF_CHANNELS);
            }
            else
            {
                values.push_back(valueLine);
                runningSamples += 1;
            }
        }
    }
    if (runningSamples != toReturn.getNumSamples() && toReturn.getNumSamples() > 0)
    {
        displayError(ERR_NUM_OF_SAMPLES);
    }
    else
    {
        toReturn.setNumSamples(runningSamples);
    }
    toReturn.setSamples(values);
    toReturn.setLength_in_s(toReturn.getNumSamples() / toReturn.getSample_rate());
    myfile.close();
    return toReturn;
}

cs229Object cinvalidtorAndBuilder()
{
    char * pointer = (char *) malloc(256 * sizeof(char *));
    long keyword;
    FILE * f;
    cs229Object toReturn;
    toReturn.setFilename("stdin");
    toReturn.setFiletype("cs229");
    f = stdin;
    fgets(pointer, 6, f);
    keyword = fscanf(f, "%s", pointer);
    bool startDataFound = false;

    while (keyword > 0 && !startDataFound)
    {
        pointer = charToLower(pointer);
        if (strncmp(pointer, "samples", 7) == 0)
        {
            fscanf(f, "%s", pointer);
            toReturn.setNumSamples(atoi(pointer));
        }
        else if (strncmp(pointer, "channels", 8) == 0)
        {
            fscanf(f, "%s", pointer);
            toReturn.setChannels(atoi(pointer));
        }
        else if (strncmp(pointer, "bitres", 8) == 0)
        {
            fscanf(f, "%s", pointer);
            toReturn.setBit_depth(atoi(pointer));
        }
        else if (strncmp(pointer, "samplerate", 10) == 0)
        {
            fscanf(f, "%s", pointer);
            toReturn.setSample_rate(atoi(pointer));
        }
        else if (strncmp(pointer, "startdata", 9) == 0)
        {
            startDataFound = true;
        }
        if (!startDataFound)
        {
            keyword = fscanf(f, "%s", pointer);
        }
    }

    int runningSamples = 0;
    vector<vector<double> > values;

    while (keyword)
    {
        keyword = fscanf(f, "%s", pointer);
        if (keyword)
        {
            vector<double> valueline;
            for (int i = 1; i < toReturn.getChannels(); i += 1)
            {
                valueline.push_back(atof(pointer));
                keyword = fscanf(f, "%s", pointer);
            }
            runningSamples += 1;
            values.push_back(valueline);
        }
    }

    toReturn.setSamples(values);
    if (toReturn.getNumSamples() == 0)
    {
        toReturn.setNumSamples(runningSamples);
    }
    toReturn.setLength_in_s(toReturn.getNumSamples() / toReturn.getSample_rate());
    free(pointer);
    return toReturn;
}

cs229Object mixem(double factors [], std::vector<cs229Object> &objects)
{
    cs229Object toreturn;
    double maxSamples = 0;
    int maxBitRes = 0;
    double lengthOfMaxSampleFile = 0.0;
    int runningChannels = 0;
    double minSamples = 0;
    int factorIndex = 0;
    for (cs229Object &obj: objects)
    {
        maxBitRes = max(obj.getBit_depth(), maxBitRes);
        minSamples = min(obj.getNumSamples(), minSamples);
        if (obj.getNumSamples() > maxSamples)
        {
            lengthOfMaxSampleFile = obj.getLength_in_s();
            maxSamples = obj.getNumSamples();
        }
        if (runningChannels != 0 && runningChannels != obj.getChannels())
        {
            displayError(ERR_DIFF_CHAN_NUM);
        }
        else
        {
            runningChannels = obj.getChannels();
        }

        //Multiplies each sample set by its given factor
        vector<vector<double> > toSet;
        for (vector<double> &row: obj.getSamples())
        {
            vector<double> temp;
            for (double &i: row)
            {
                temp.push_back(factors[factorIndex] * i);
            }
            toSet.push_back(temp);
        }
        obj.setSamples(toSet);
        factorIndex += 1;

    }

    //Create a blank 2d vector for the new values
    vector<vector<double> > newSamples;
    for (int i = 0; i < maxSamples; i += 1)
    {
        vector<double> newSampleRow;
        for (int j = 0; j < runningChannels; j += 1)
        {
            newSampleRow.push_back(0);
        }
        newSamples.push_back(newSampleRow);
    }

    //Merge and add all of the sample data
    for (cs229Object &obj: objects)
    {
        unsigned long newSamplesRowIndex = 0;
        for (vector<double> &row: obj.getSamples())
        {
            unsigned long newSamplesColumnIndex = 0;
            for (double &sample: row)
            {
                newSamples.at(newSamplesRowIndex).at(newSamplesColumnIndex) += sample;
                newSamplesColumnIndex += 1;
            }
            newSamplesRowIndex += 1;
        }
    }

    toreturn.setSamples(newSamples);
    toreturn.setLength_in_s(lengthOfMaxSampleFile);
    toreturn.setFiletype("cs229");
    toreturn.setSample_rate(maxSamples / lengthOfMaxSampleFile);
    toreturn.setBit_depth(maxBitRes);
    toreturn.setChannels(runningChannels);
    toreturn.setNumSamples(maxSamples);
    return toreturn;
}

cs229Object concat(std::vector<cs229Object> objects)
{
    cs229Object toReturn;
    int runningChannels = 0;
    int runningSamples = 0;
    int maxBitres = 0;
    double totalLength = 0.0;
    vector<vector<double> > values;
    for (cs229Object obj: objects)
    {
        if (runningChannels != 0 && obj.getChannels() != runningChannels)
        {
            displayError(ERR_DIFF_CHAN_NUM);
        }
        else
        {
            runningChannels = obj.getChannels();
        }
        runningSamples += obj.getNumSamples();
        maxBitres = max(maxBitres, obj.getBit_depth());
        totalLength += obj.getLength_in_s();
        for (vector<double> j: obj.getSamples())
        {
            values.push_back(j);
        }
    }
    toReturn.setNumSamples(runningSamples);
    toReturn.setSamples(values);
    toReturn.setBit_depth(maxBitres);
    toReturn.setLength_in_s(totalLength);
    toReturn.setChannels(runningChannels);
    toReturn.setSample_rate(toReturn.getNumSamples() / totalLength);
    toReturn.setFiletype("cs229");
    toReturn.setFilename("Concatenation.txt");
    return toReturn;
}

void printer(cs229Object obj)
{
    cout << "Filename:\t" << obj.getFilename() << endl;
    cout << "Filetype:\t" << obj.getFiletype() << endl;
    cout << "Sample Rate:\t" << obj.getSample_rate() << endl;
    cout << "Bit Depth:\t" << obj.getBit_depth() << endl;
    cout << "# of Channels:\t" << obj.getChannels() << endl;
    cout << "# of Samples:\t" << obj.getNumSamples() << endl;
    cout << "Length:\t" << obj.getLength_in_s() << " seconds" << endl;
}

void printToStdout(cs229Object obj)
{
    cout << "cs229" << endl;
    cout << "Samples\t" << obj.getNumSamples() << endl;
    cout << "Channels\t" << obj.getChannels() << endl;
    cout << "Bitres\t" << obj.getBit_depth() << endl;
    cout << "SampleRate\t" << obj.getSample_rate() << endl;
    cout << "StartData\t" << endl;
    for (vector<double> row: obj.getSamples())
    {
        for (double i: row)
        {
            cout << i << "\t";
        }
        cout << endl;
    }
}

void printToFile(cs229Object obj, string filename)
{
    ofstream myfile;
    myfile.open(filename);
    if (!myfile.is_open())
    {
        displayError(ERR_OPEN_FILE);
        exit(0);
    }
    if (obj.getFiletype().compare("cs229") == 0)
    {
        myfile << "cs229\n";
    }
    myfile << "Samples\t" << obj.getNumSamples() << endl;
    myfile << "Channels\t" << obj.getChannels() << endl;
    myfile << "BitRes\t" << obj.getBit_depth() << endl;
    myfile << "SampleRate\t" << obj.getSample_rate() << endl;
    myfile << "StartData" << endl;
    for (vector<double> row: obj.getSamples())
    {
        for (double i: row)
        {
            myfile << i << "\t";
        }
        myfile << endl;
    }
    myfile.close();
}

string tolowercase(string input)
{
    for (int i = 0; i < input.length(); i += 1)
    {
        input[i] = (char) tolower(input[i]);
    }
    return input;
}

char * charToLower(char * tolow)
{
    for (int i = 0; i < strlen(tolow); i += 1)
    {
        //May lead to problems
        tolow[i] = (char) tolower(tolow[i]);
    }
    return tolow;
}

void displayHelpScreen(int code)
{
    if (code == 0)
    {
        //Display sndinfo help
        cout << "This program takes in either cs229 files as arguments, validates them, then displays relevant information from the files" << endl;
    }
    else if (code == 1)
    {
        //Display sndcat help
        cout << "This program takes in either cs229 files as arguments, validates them, then outputs a concatenation of them to either a specified file or standard out." << endl;
    }
    else if (code == 2)
    {
        //Display sndmix help
        cout << "This program takes in one or more cs229 files with matching factors. The data in each file is multiplied by its given factor and then merged with every other file\n"
                        "to produce an output that is essential a mix of them all" << endl;
    }
    else if (code == 3)
    {
        //Display sndgen help
        cout << "This program produces a sound of a specified frequency and waveform by utilizing an ADSR envelope and outputing to either stdout or a specified file\n"
                "Switches: \n"
                        "-h:  show the help screen.\n"
                        "-o file: output file name; if omitted, outputs to standard output.\n"
                        "--bits n: bit depth of\n"
                        "--sr n: sample rate\n"
                        "-f r: frequency in Hz\n"
                        "-t r: total duration\n"
                        "-v p: peak volume factor\n"
                        "-a r: attack time\n"
                        "-d r: decay time\n"
                        "-s p: sustain volume\n"
                        "-r r: release time\n"
                        "--sine: generate a sine wave\n"
                        "--triangle: generate a triangle wave\n"
                        "--sawtooth: generate a sawtooth wave\n"
                        "--pulse: generate a pulse wave\n"
                        "--pf p : fraction of the time the pulse wave is up" << endl;
    }
    exit(0);
}

void displayError(int errorCode)
{
    if (errorCode == ERR_OPEN_FILE)
    {
        cerr << "There was an error opening the file" << endl;
    }
    else if (errorCode == ERR_NOT_CS229)
    {
        cerr << "The given file was not declared a cs229 file" << endl;
    }
    else if (errorCode == ERR_NOT_DECLARED)
    {
        cerr << "Either the bit depth, number of channels, or sample rate was not declared" << endl;
    }
    else if (errorCode == ERR_NUM_OF_CHANNELS)
    {
        cerr << "The samples did not match the number of channels" << endl;
    }
    else if (errorCode == ERR_NUM_OF_SAMPLES) {
        cerr << "The number of samples in the file did not match what was expected" << endl;
    }
    else if (errorCode == ERR_NO_INPUT)
    {
        cerr << "This program requires an input file from which to read." << endl;
    }
    else if (errorCode == ERR_ZERO_FACTOR)
    {
        cerr << "One of the supplied factors was either a string or zero" << endl;
    }
    else if (errorCode == ERR_DIFF_CHAN_NUM)
    {
        cerr << "Not all of the files had the same number of channels" << endl;
    }
    else if (errorCode == ERR_TOO_MANY__ARGS)
    {
        cerr << "Too many arguments/switches were given to sndgen. Check your spelling." << endl;
    }
    else if (errorCode == ERR_LENGTH_TOO_SHORT)
    {
        cerr << "The length provided was too short for the required components of the waveform." << endl;
    }
    else if (errorCode == ERR_SUSTAIN_VS_PEAK)
    {
        cerr << "The sustain volume factor cannot be higher than the peak volume factor." << endl;
    }
    else if (errorCode == ERR_OUT_OF_DEPTH)
    {
        cerr << "One of the samples was outside the allowed/specified bit depth." << endl;
    }
    else if (errorCode == ERR_NO_ARGS)
    {
        cerr << "No arguments were supplied." << endl;
    }
    else if (errorCode == ERR_NO_WAVE)
    {
        cerr << "No waveform was supplied." << endl;
    }
    else
    {
        cerr << "Error code not handled" << endl;
    }
    exit(0);
}
