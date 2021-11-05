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

void countWords(ifstream &in, StrFreqMap &freqmap, const char *delwords[]);

int main()
{
    string data_dir = ".\\certdata\\";
    string file_name = ".\\file_names.txt";
    vector<string> file_names;

    // List of noise words to delete from list
    const char *delwords[20] = {
        "a", "an", "the", "am", "is", "are", "was", "were", "being",
        "been", "seem", "become", "became", "to", "of", "in", "may", "and", "be", "on" };

    // Load file_names.txt which contains list of all file names
    ifstream names_file;
    names_file.open(file_name);
    while (names_file) {
        string line;
        // read the line
        getline(names_file, line);

        if (!line.empty()) {
            string full_name = data_dir + line;
            file_names.push_back(full_name);
        }
    }

    // Loop through each filename
    StrFreqMap freqs;
    for (string fname : file_names) {
        // Load the current file
        ifstream inFile;
        inFile.open(fname);

        // Check for successful open
        if (!inFile.is_open()) {
            cout << fname << " failed to open..." << endl << flush;
            break;
        }
        
        // Pass the file, frequency map, and noise words to function
        countWords(inFile, freqs, delwords);

        inFile.close();
    }

    // Calculate what 10% of the total count is for keeping
    size_t keep_count = freqs.size() * 0.1;
    cout << "Keeping " << keep_count << " words." << endl;

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
    cout << "-----------------" << endl;
    for (auto p : top_ten) {
        cout << p.first << " : " << p.second << endl;
    }

    return 0;
}

void countWords(ifstream &in, StrFreqMap &freqmap, const char *delwords[20]) {
    // MUCH INSPIRED BY CODE POSTED ON DISCUSSION BOARD
    string words, lowwords;

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
    }
}