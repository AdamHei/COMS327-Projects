#include <iostream>
#include <getopt.h>
#include <vector>
#include "cs229Object.h"
#include "utilFunctions.h"

using namespace std;

/*Sound info is a basic info program that takes in cs229 filenames as arguments
 * and prints, either to stdout or a file, their basic information.
 * If no arguments are given, it reads from stdin
 */
int main(int argc, char ** argv){

    int c;
    int index;
    vector<string> fileList;

    while ((c = getopt(argc, argv, "h")) != -1)
    {
        if (c == 'h')
        {
            displayHelpScreen(0);
        }
    }

    for (index = optind; index < argc; index += 1)
    {
        fileList.push_back(argv[index]);
    }

    vector<cs229Object> soundList;

    for (string s: fileList)
    {
        soundList.push_back(validatorAndBuilder(s));
    }
    if (soundList.size() == 0)
    {
        cout << "Enter a filename: ";
        string s;
        cin >> s;
        soundList.push_back(validatorAndBuilder(s));
    }

    for (cs229Object obj: soundList)
    {
        printer(obj);
        cout << endl;
    }

    return 0;
}
