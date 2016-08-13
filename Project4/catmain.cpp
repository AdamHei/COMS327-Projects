#include <iostream>
#include "utilFunctions.h"

using namespace std;

/*
 * The purpose of Sound concatenation is to take in one or more filenames,
 * construct the corresponding cs229Objects for each, then outputting, either
 * to a file or stdout, the resulting file information, followed by all of the files' 
 * data appended.
 */
int main(int argc, char ** argv) {

    int c;
    int index;
    vector<string> fileList;
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

    for (index = optind; index < argc; index += 1)
    {
        fileList.push_back(argv[index]);
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

    cs229Object toOut = concat(soundList);

    if (outFileName.compare("") != 0)
    {
        printToFile(toOut, outFileName);
    }
    else
    {
        printToStdout(toOut);
    }

    return 0;
}