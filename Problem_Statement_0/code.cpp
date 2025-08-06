#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;

double evaluate(string input) {
stringstream ss(input);
double a, b;
char op;

if (input.find("sqrt") != string::npos) {
    ss.ignore(5, '(');   
    ss >> a;
    return sqrt(a);
}


if (input.find("ln") != string::npos) {
    ss.ignore(3, '(');   
    ss >> a;
    return log(a);       
}


if (input.find("log10") != string::npos) {
    ss.ignore(6, '(');   
    ss >> a;
    return log10(a);
}


if (input.find("exp") != string::npos) {
    ss.ignore(4, '(');   
    ss >> a;
    return exp(a);
}


if (input.find("sin") != string::npos) {
    ss.ignore(4, '(');   
    ss >> a;
    return sin(a);       
}
if (input.find("cos") != string::npos) {
    ss.ignore(4, '(');   
    ss >> a;
    return cos(a);
}
if (input.find("tan") != string::npos) {
    ss.ignore(4, '(');   
    ss >> a;
    return tan(a);
}


if (input.find("asin") != string::npos) {
    ss.ignore(5, '(');   
    ss >> a;
    return asin(a);
}
if (input.find("acos") != string::npos) {
    ss.ignore(5, '(');   
    ss >> a;
    return acos(a);
}
if (input.find("atan") != string::npos) {
    ss.ignore(5, '(');   
    ss >> a;
    return atan(a);
}


if (input.find("^") != string::npos) {
    size_t pos = input.find("^");
   
    a = stod(input.substr(0, pos));       
    b = stod(input.substr(pos + 1));      
    return pow(a, b);                     
}


ss >> a >> op >> b;

if (op == '+') return a + b;
if (op == '-') return a - b;
if (op == '*') return a * b;
if (op == '/') return a / b;

return NAN;  
}

int main() {
string input;

while (true) {
    cout << "\nEnter expression (or type 'exit'): ";
    getline(cin, input);  

    if (input == "exit") break;  

    try {
        double result = evaluate(input);  
        cout << "Output: " << result << endl;
    } catch (...) {
        cout << "Invalid input or math error." << endl;
    }
}

return 0;
}

