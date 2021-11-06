#include <algorithm>
#include <iostream>
#include <fstream>
#include <future>
#include <map>
#include <bits/stdc++.h>
#include <string>
#include <thread>
#include <vector>

using namespace std;
/* 
This Task Parallel version of the program is inspired by the code posted in
https://towardsdatascience.com/how-to-build-a-multi-threaded-pipeline-in-c-with-std-async-78edc19e862d

The general idea is to call functions with async that are passed the futures of functions
before them in the pipeline to ensure data is ready before execution continues.
*/

// Data structure to store words + their frequencies
typedef unordered_map<string, unsigned> StrFreqMap;

StrFreqMap stage1(string path);
StrFreqMap stage2(future<StrFreqMap> &&prev_stage_results, const char *delwords[20]);
void outputSummary(StrFreqMap freqs);

int main() {
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

    // List of noise words to delete from list
    const char *delwords[20] = {
        "a", "an", "the", "am", "is", "are", "was", "were", "being",
        "been", "seem", "become", "became", "to", "of", "in", "may", "and", "be", "on" };

    // Loop through each filename, sending them through the pipeline
    StrFreqMap freqmap;
    for (string fname : file_names) {
        string path = data_dir + (fname);
        

        // Create thread for stage1 and save its future
        future<StrFreqMap> future1 = async(stage1, path);
        // Create thread for stage2 and pass it future1, and save its future
        future<StrFreqMap> future2 = async(stage2, move(future1), delwords);

        // Get the result from future2 to combine in main
        StrFreqMap freqs = future2.get();

        // Loop through the resulting map and combine with the main map
        for (auto it = freqs.begin(); it != freqs.end(); it++) {
            // Increment frequency if key exists, or add new key to map
            if (freqmap.count(it->first)) {
                freqmap.at(it->first) += it->second;
            } else {
                freqmap.insert({{it->first, it->second}});
            }
        }
    }

    // Generate output summary
    outputSummary(freqmap);

    return 0;
}

StrFreqMap stage1(string path) {
    
    // MUCH INSPIRED BY CODE POSTED ON DISCUSSION BOARD
    string words, lowwords;
    StrFreqMap freqmap;

    // Open the provided file
    ifstream in(path.c_str(), ios::in);

    // Check for successful open
    if (!in.is_open()) {
        cout << path << " failed to open..." << endl << flush;
        return freqmap;
    }

    while (in) {
        // read the line
        getline(in, words);
        
        // remove symbols and numbers
        for (int j=0; j<words.size(); j++)
            if (!isalpha(words[j])) {
                words[j] = ' ';
            }

        // lowercase
        for (int i=0; i<words.size(); i++)
            words[i] = tolower(words[i]);

        // make into a list of words
        char charwords [words.length()+1];
        strcpy(charwords, words.c_str());
        char *token = strtok(charwords, " ");
        while (token != NULL) {
            // Increment frequency if key exists, or add new key to map
            if (freqmap.count(token)) {
                freqmap.at(token) += 1;
            } else {
                freqmap.insert({{token, 1}});
            }
            
            // Get next word
            token = strtok(NULL, " ");
        }
    }
    in.close();

    return freqmap;
}

StrFreqMap stage2(future<StrFreqMap> &&prev_stage_results, const char *delwords[20]) {
    // Ensure the previous stage has completed
    StrFreqMap freqmap = prev_stage_results.get();

    // Iterate through map, looking for delwords to remove
    vector<string> erase_list;
    for (auto it = freqmap.begin(); it != freqmap.end(); it++) {
        for (int i=0;i<20;i++) {
            // Save key for removal
            if (!it->first.compare(delwords[i])) {
                erase_list.push_back(it->first);
                break;
            }
        }
    }
    // Remove all keys that are delwords
    for (auto key : erase_list) {
        freqmap.erase(key);
    }
    
    return freqmap;
}

void outputSummary(StrFreqMap freqs) {
    //declare and open output file
    ofstream outfile;
    outfile.open("tp_results.txt");

    // Calculate what 10% of the total count is for keeping
    size_t keep_count = freqs.size() * 0.1;
    outfile << "Keeping " << keep_count << " words." << endl;

    // https://stackoverflow.com/questions/17963905/how-can-i-get-the-top-n-keys-of-stdmap-based-on-their-values
    // Use partial_sort_copy to extract the top *keep_count* values from the frequency map
    vector<pair<string, unsigned>> top_ten(keep_count);
    partial_sort_copy(freqs.begin(),
                    freqs.end(),
                    top_ten.begin(),
                    top_ten.end(),
                    [](pair<const string, unsigned> const& l,
                       pair<const string, unsigned> const& r)
                    {
                        return l.second > r.second;
                    });
    
    // Display the resulting top 10% and their frequencies
    outfile << "-----------------" << endl;
    for (auto p : top_ten) {
        outfile << p.first << " : " << p.second << endl;
    }

    //close outfile
    outfile.close();
}