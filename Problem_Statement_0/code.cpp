#include <iostream>
#include <string>
#include <sstream>
using namespace std;

const double PI = 3.141592653589793;
const double SMALL = 1e-10;

double mod(double x) {
    return (x < 0) ? -x : x;
}

double sqrt(double x) {
    double guess = x / 2.0;
    if (x == 0) return 0;
    while (mod(guess * guess - x) > SMALL) {
        guess = (guess + x / guess) / 2.0;
    }
    return guess;
}

double sin(double x) {
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    double term = x;
    double sum = x;
    for (int n = 1; n <= 10; n++) {
        term *= -x * x / ((2 * n) * (2 * n + 1));
        sum += term;
    }
    return sum;
}

double cos(double x) {
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    double term = 1;
    double sum = 1;
    for (int n = 1; n <= 10; n++) {
        term *= -x * x / ((2 * n - 1) * (2 * n));
        sum += term;
    }
    return sum;
}

double tan(double x) {
    return sin(x) / cos(x);
}

double exp(double x) {
    double term = 1;
    double sum = 1;
    for (int n = 1; n <= 20; n++) {
        term *= x / n;
        sum += term;
    }
    return sum;
}

double ln(double x) {
    double y = x - 1;
    for (int i = 0; i < 100; i++) {
        double e = exp(y);
        y -= (e - x) / e;
        if (abs(e - x) < SMALL) break;
    }
    return y;
}

double pow(double base, double exponent) {
    return exp(exponent * ln(base));
}

double calculate(string input) {
    stringstream ss(input);
    double a, b;
    char op;

    if (input.find("sqrt") != string::npos) {
        ss.ignore(5, '(');
        ss >> a;
        return sqrt(a);
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
    if (input.find("ln") != string::npos) {
        ss.ignore(3, '(');
        ss >> a;
        return ln(a);
    }
    if (input.find("exp") != string::npos) {
        ss.ignore(4, '(');
        ss >> a;
        return exp(a);
    }
    if (input.find("^") != string::npos) {
        size_t pos = input.find("^");
        a = stod(input.substr(0, pos));
        b = stod(input.substr(pos + 1));
        return pow(a, b);
    }

    ss >> a >> op >> b;
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : -1;
    }
    return -1;
}

int main() {
    string input;
    cout << "calculator with: +, -, *, /, ^, sqrt(x), sin(x), cos(x), tan(x), exp(x), ln(x)\n";

    while (true) {
        cout << "\nEnter expression (or 'exit'): ";
        getline(cin, input);
        if (input == "exit") break;
        cout << calculate(input) << endl;
    }
    return 0;
}

