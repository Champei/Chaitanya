#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    cout << "Enter your text: ";
    getline(cin, str);

    int shift;
    cout << "Enter shift value: ";
    cin >> shift;

    char choice;
    cout << "Encode (E) or Decode (D)? ";
    cin >> choice;

    // If decoding, we just reverse the shift
    if (choice == 'D' || choice == 'd') {
        shift = -shift;
    }

    for (int i = 0; i < str.size(); i++) {
        char ch = str[i];

        if (ch >= 'a' && ch <= 'z') { // lowercase
            ch = ( (ch - 'a' + shift + 26) % 26 ) + 'a';
        }
        else if (ch >= 'A' && ch <= 'Z') { // uppercase
            ch = ( (ch - 'A' + shift + 26) % 26 ) + 'A';
        }

        cout << ch;
    }

    cout << endl;
    return 0;
}