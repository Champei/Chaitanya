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

double atan(double x) {
    if (mod(x) <= 1.0) {
        double term = x;
        double sum = x;
        for (int n = 1; n <= 40; n++) {
            term *= -x * x;
            sum += term / (2 * n + 1);
        }
        return sum;
    } else {
        double s = (x > 0) ? (PI / 2) : (-PI / 2);
        return s - atan(1.0 / x);
    }
}

double asin(double x) {
    if (mod(x) > 1.0) return -1; // out of domain
    return atan(x / sqrt(1 - x * x));
}

double acos(double x) {
    if (mod(x) > 1.0) return -1; // out of domain
    return PI / 2 - asin(x);
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
        if (mod(e - x) < SMALL) break;
    }
    return y;
}

double pow(double base, double exponent) {
    return exp(exponent * ln(base));
}

string removeParentheses(string s) {
    string result;
    for (char c : s) {
        if (c != '(' && c != ')') result += c;
    }
    return result;
}

double calculate(string input) {
    input = removeParentheses(input);
    stringstream ss(input);
    double a, b;
    char op;

    auto readNumber = [&]() {
        ss >> a;
        return a;
    };

    if (input.find("sqrt") == 0) {
        ss.ignore(4);
        return sqrt(readNumber());
    }
    if (input.find("sin") == 0) {
        ss.ignore(3);
        return sin(readNumber());
    }
    if (input.find("cos") == 0) {
        ss.ignore(3);
        return cos(readNumber());
    }
    if (input.find("tan") == 0) {
        ss.ignore(3);
        return tan(readNumber());
    }
    if (input.find("asin") == 0) {
        ss.ignore(4);
        return asin(readNumber());
    }
    if (input.find("acos") == 0) {
        ss.ignore(4);
        return acos(readNumber());
    }
    if (input.find("atan") == 0) {
        ss.ignore(4);
        return atan(readNumber());
    }
    if (input.find("ln") == 0) {
        ss.ignore(2);
        return ln(readNumber());
    }
    if (input.find("exp") == 0) {
        ss.ignore(3);
        return exp(readNumber());
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
    cout << "calculator with: +, -, *, /, ^, sqrt(x), sin(x), cos(x), tan(x), asin(x), acos(x), atan(x), exp(x), ln(x)\n";

    while (true) {
        cout << "\nEnter expression (or 'exit'): ";
        getline(cin, input);
        if (input == "exit") break;
        cout << calculate(input) << endl;
    }
    return 0;
}

