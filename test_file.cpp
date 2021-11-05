#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <pthread.h>
#include <bits/stdc++.h>
#include <string>
#include <vector>

using namespace std;

typedef unordered_map<string, unsigned> StrFreqMap;

int main()
{
    string data_dir = "certdata/";
    vector<string> file_names =  {
	    "2013-01-08.txt","2013-01-09.txt","2013-01-11.txt","2013-01-10.txt",
        "2013-01-25.txt","2013-01-28.txt","2013-01-29.txt","2013-02-01.txt",
        "2013-02-07.txt","2013-02-14.txt","2013-02-22.txt","2013-03-05.txt",
        "2013-03-08.txt","2013-03-11.txt","2013-03-18.txt","2013-03-20.txt",
        "2013-03-21.txt","2013-04-03.txt","2013-04-05.txt","2013-04-02.txt",
        "2013-04-08.txt","2013-04-10.txt","2013-04-15.txt","2013-04-19.txt",
        "2013-04-25.txt","2013-04-26.txt","2013-04-29.txt","2013-04-30.txt",
        "2013-05-06.txt","2013-05-14.txt","2013-05-15.txt","2013-05-17.txt",
        "2013-06-05.txt","2013-06-07.txt","2013-06-10.txt","2013-06-11.txt",
        "2013-06-18.txt","2013-06-26.txt","2013-06-27.txt","2013-06-28.txt",
        "2013-07-12.txt","2013-07-15.txt","2013-07-26.txt","2013-08-01.txt",
        "2013-08-02.txt","2013-08-05.txt","2013-08-15.txt","2013-08-21.txt",
        "2013-08-23.txt","2013-08-26.txt","2013-08-30.txt","2013-09-03.txt",
        "2013-09-04.txt","2013-09-13.txt","2013-09-23.txt","2013-09-24.txt",
        "2013-10-01.txt","2013-10-04.txt","2013-10-10.txt","2013-10-15.txt",
        "2013-10-16.txt","2013-10-17.txt","2013-10-18.txt","2013-10-22.txt",
        "2013-10-25.txt","2013-10-28.txt","2013-10-30.txt","2013-10-31.txt",
        "2013-11-04.txt","2013-11-07.txt","2013-11-14.txt","2013-11-18.txt",
        "2013-11-22.txt","2013-12-02.txt","2013-12-03.txt","2013-12-05.txt",
        "2013-12-12.txt","2013-12-30.txt" };
    
    for (string fname : file_names) {
        string path = data_dir + (fname);
        ifstream inFile(path.c_str(), ios::in);
        //cout << "Opening " << full << endl;

        // Check for successful open
        if (!inFile.is_open()) {
            cout << fname.c_str() << endl << flush;
            cerr << strerror(errno) << endl;
            //cout << " failed to open..." << endl;
        } else {
            cout << fname.c_str() << " opened successfully." << endl;
        }

        while (inFile) {
            string temp;
            getline(inFile, temp);
            cout << ". ";
        }

        inFile.close();
    }    
    
    return 0;
}