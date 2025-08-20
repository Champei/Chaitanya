#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

const int MAX_WORDS = 100000; // maximum words dictionary can hold
string dictionary[MAX_WORDS];
int dictSize = 0;

// Function to shift characters of a string
string shiftString(const string &s, int shift) {
    string result = s;
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        if (isalpha(c)) { // Only shift letters
            char base = islower(c) ? 'a' : 'A';
            result[i] = ( (c - base + shift) % 26 ) + base;
        }
    }
    return result;
}

// Load dictionary words from file into array
void loadDictionary(const string &filename) {
    ifstream file(filename);
    string word;
    dictSize = 0;
    while (file >> word && dictSize < MAX_WORDS) {
        dictionary[dictSize++] = word;
    }
}

// Check if a word exists in dictionary
bool isMeaningful(const string &word) {
    for (int i = 0; i < dictSize; i++) {
        if (dictionary[i] == word) return true;
    }
    return false;
}

int main() {
    // Load dictionary
    loadDictionary("words1.txt");
    if (dictSize == 0) {
        cout << "Dictionary file not found or empty!" << endl;
        return 1;
    }

    string input;
    cout << "Enter string: ";
    cin >> input;

    bool found = false;
    for (int shift = 0; shift < 26; shift++) {
        string shifted = shiftString(input, shift);
        if (isMeaningful(shifted)) {
            cout << "Found meaningful word: " << shifted 
                 << " (shift = " << shift << ")" << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No meaningful word found in dictionary." << endl;
    }

    return 0;
}
