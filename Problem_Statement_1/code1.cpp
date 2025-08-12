#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    getline(cin, str);

    int increase_by = 3;

    for (int i = 0; i < str.size(); i++) {
        char ch = str[i];

        if (ch >= 'a' && ch <= 'z') { // lowercase
            ch = (ch - 'a' + increase_by) % 26 + 'a';
        }
        else if (ch >= 'A' && ch <= 'Z') { // uppercase
            ch = (ch - 'A' + increase_by) % 26 + 'A';
        }
        

        cout << ch ;
    }

    cout << endl;
    return 0;
}