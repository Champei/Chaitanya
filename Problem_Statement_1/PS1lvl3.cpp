#include <iostream>
#include <string>
#include <bitset>
using namespace std;

// ---------------- Base64 Table ----------------
static const string chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

// ---------------- Base64 Encode ----------------
string encode(const string &input) {
    string output;
    int val = 0, valb = -6;

    for (unsigned char c : input) {
        int asciiVal = int(c);

        // Show ASCII and binary
        cout << "Char: " << c 
             << " | ASCII: " << asciiVal 
             << " | Binary: " << bitset<8>(asciiVal) << endl;

        // Base64 encoding process
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            int index = (val >> valb) & 0x3F;
            cout << "  -> 6-bit group: " << bitset<6>(index) 
                 << " = " << chars[index] << endl;
            output.push_back(chars[index]);
            valb -= 6;
        }
    }

    if (valb > -6) {
        int index = ((val << 8) >> (valb + 8)) & 0x3F;
        cout << "  -> Remaining bits group: " 
             << bitset<6>(index) << " = " << chars[index] << endl;
        output.push_back(chars[index]);
    }

    while (output.size() % 4) {
        output.push_back('=');
    }

    return output;
}

// ---------------- Base64 Decode ----------------
string decode(const string &input) {
    int T[256];
    for (int i = 0; i < 256; i++) T[i] = -1;
    for (int i = 0; i < 64; i++) T[(unsigned char)chars[i]] = i;

    string output;
    int val = 0, valb = -8;
    for (unsigned char c : input) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            output.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return output;
}

// ---------------- Caesar Helper: uppercase shift ----------------
int getShift(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    return 0;
}

// ---------------- Keyword Caesar Encrypt ----------------
string keywordEncrypt(const string &text, const string &keyword) {
    string result;
    int k = 0, kwlen = keyword.length();

    for (char c : text) {
        if (c >= 'A' && c <= 'Z') {
            int shift = getShift(keyword[k % kwlen]);
            char enc = char((c - 'A' + shift) % 26 + 'A');
            result.push_back(enc);
            k++;
        }
        else if (c >= 'a' && c <= 'z') {
            int shift = getShift(keyword[k % kwlen]);
            char enc = char((c - 'a' + shift) % 26 + 'a');
            result.push_back(enc);
            k++;
        }
        else {
            result.push_back(c); // keep spaces/punctuation
        }
    }
    return result;
}

// ---------------- Keyword Caesar Decrypt ----------------
string keywordDecrypt(const string &text, const string &keyword) {
    string result;
    int k = 0, kwlen = keyword.length();

    for (char c : text) {
        if (c >= 'A' && c <= 'Z') {
            int shift = getShift(keyword[k % kwlen]);
            char dec = char((c - 'A' - shift + 26) % 26 + 'A');
            result.push_back(dec);
            k++;
        }
        else if (c >= 'a' && c <= 'z') {
            int shift = getShift(keyword[k % kwlen]);
            char dec = char((c - 'a' - shift + 26) % 26 + 'a');
            result.push_back(dec);
            k++;
        }
        else {
            result.push_back(c);
        }
    }
    return result;
}

// ---------------- MAIN ----------------
int main() {
    int choice;
    cout << "1. Encode\n2. Decode\nChoose option: ";
    cin >> choice;
    cin.ignore();

    string keyword, message;

    if (choice == 1) {
        cout << "Enter keyword: ";
        getline(cin, keyword);
        cout << "Enter message: ";
        getline(cin, message);

        // Caesar + Base64
        string caesarEnc = keywordEncrypt(message, keyword);
        cout << "\n[STEP 1] Caesar Encoded : " << caesarEnc << endl;

        string base64Enc = encode(caesarEnc);
        cout << "\n[STEP 2] Base64 Encoded : " << base64Enc << endl;

    } else if (choice == 2) {
        cout << "Enter keyword: ";
        getline(cin, keyword);
        cout << "Enter Base64 encoded message: ";
        getline(cin, message);

        // Base64 + Caesar
        string base64Dec = decode(message);
        cout << "\n[STEP 1] Base64 Decoded : " << base64Dec << endl;

        string caesarDec = keywordDecrypt(base64Dec, keyword);
        cout << "[STEP 2] Final Decrypted: " << caesarDec << endl;

    } else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
