#include <iostream>
#include "utilFunctions.h"

using namespace std;

/**
 * Sound mix serves to take in one or more files and their corresponding factors.
 * From there it multiplies the sample data of each file by the file's factor,
 * which leads to the merging of all the files' data. The resulting object is outputted
 * to either stdin or a file
 */
int main(int argc, char ** argv) {

    int c;
    int index;
    vector<string> fileList;
    vector<double> factlist;
    string outFileName = "";

    while ((c = getopt(argc, argv, "ho:")) != -1)
    {
        if (c == 'h')
        {
            displayHelpScreen(1);
            return 0;
        }
        else if (c == 'o')
        {
            outFileName = optarg;
        }
    }

    for (index = optind; index < argc - 1; index += 2)
    {
        if (atof(argv[index]) == 0) displayError(ERR_ZERO_FACTOR);
        else if (atof(argv[index]) > 10 || atof(argv[index]) < -10) displayError(ERR_FACT_OUT_RANGE);
        factlist.push_back(atof(argv[index]));
        fileList.push_back(argv[index + 1]);
    }

    double factors [factlist.size()];
    int ind = 0;
    for(double i: factlist)
    {
        factors[ind] = i;
        ind += 1;
    }

    if (fileList.size() == 0)
    {
        displayError(ERR_NO_INPUT);
    }
    vector<cs229Object> soundList;

    for (string s: fileList)
    {
        soundList.push_back(validatorAndBuilder(s));
    }

    cs229Object toPrint = mixem(factors, soundList);
    if (outFileName.length() > 0)
    {
        printToFile(toPrint, outFileName);
    }
    else
    {
        printToStdout(toPrint);
    }
    return 0;
}