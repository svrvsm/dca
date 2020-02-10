#include <fstream>
#include <iostream>
#include <sstream>
#include "./include/cluster.h"
#include "./include/constants.h"


using namespace std;
using namespace DSA;

void GetOutputFileName(const string & in, string & out)
{
    out += "output_for_";
    size_t lastForwardSlash = in.find_last_of("/");
    if (lastForwardSlash == string::npos)
    {
        out += in;
        return;
    }

    out += in.substr(lastForwardSlash + 1);
}

int main(int argc, char const *argv[])
{

    Cluster cl;

    while (1)
    {
        string fileName, outputFileName;
        cout << "Enter file name or < Ctrl+C > to exit" << endl;
        cin >>  fileName;

        if ( fileName.empty() )
           break;

        GetOutputFileName(fileName, outputFileName);

        if ( cl.Process(fileName, outputFileName) != SUCCESS )
            cout << "Error processing please see log.txt(under current directory) for errors and debug info." << endl << endl;
        else 
            cout << "Success. Please refer to " << outputFileName <<  endl << endl;
    }
}