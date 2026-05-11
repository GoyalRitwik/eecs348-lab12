#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <limits>

using namespace std;

const double INVALID = -999999.99;

double power10(int exp) {

    double result = 1.0;

    if (exp > 308 || exp < -308)
        return numeric_limits<double>::infinity();

    if (exp >= 0) {
        for (int i = 0; i < exp; i++)
            result *= 10.0;
    }
    else {
        for (int i = 0; i < -exp; i++)
            result /= 10.0;
    }

    return result;
}

double extractNumeric(const string& str) {

    int n = str.length();

    for (int i = 0; i < n; i++) {

        int j = i;

        bool negative = false;

        // Handle optional sign
        if ((str[j] == '+' || str[j] == '-') &&
            j + 1 < n &&
            (isdigit(str[j + 1]) || str[j + 1] == '.')) {

            negative = (str[j] == '-');
            j++;
        }

        else if (!(isdigit(str[j]) || str[j] == '.')) {
            continue;
        }

        bool hasDigits = false;

        double integerPart = 0.0;
        double fractionalPart = 0.0;
        double divisor = 10.0;

        // Integer part
        while (j < n && isdigit(str[j])) {

            hasDigits = true;

            // overflow protection
            if (integerPart > 1e15)
                return INVALID;

            integerPart = integerPart * 10 + (str[j] - '0');

            j++;
        }

        // Decimal part
        if (j < n && str[j] == '.') {

            j++;

            while (j < n && isdigit(str[j])) {

                hasDigits = true;

                fractionalPart += (str[j] - '0') / divisor;

                divisor *= 10.0;

                j++;
            }
        }

        // Must contain at least one digit
        if (!hasDigits)
            continue;

        double number = integerPart + fractionalPart;

        // Scientific notation
        if (j < n && (str[j] == 'e' || str[j] == 'E')) {

            j++;

            bool expNegative = false;

            if (j < n && (str[j] == '+' || str[j] == '-')) {
                expNegative = (str[j] == '-');
                j++;
            }

            // exponent must contain digits
            if (j >= n || !isdigit(str[j]))
                return INVALID;

            int exponent = 0;

            while (j < n && isdigit(str[j])) {

                exponent = exponent * 10 + (str[j] - '0');

                j++;
            }

            if (expNegative)
                exponent = -exponent;

            double pow10 = power10(exponent);

            if (isinf(pow10))
                return INVALID;

            number *= pow10;
        }

        // Invalid malformed continuation
        if (j < n) {

            if (isdigit(str[j]) ||
                str[j] == '.' ||
                ((str[j] == '+' || str[j] == '-') &&
                 j + 1 < n &&
                 isdigit(str[j + 1]))) {

                return INVALID;
            }
        }

        if (negative)
            number = -number;

        if (fabs(number) > numeric_limits<double>::max())
            return INVALID;

        return number;
    }

    return INVALID;
}

int main() {

    string input;

    while (true) {

        cout << "Enter a string (or 'END' to quit): ";

        getline(cin, input);

        if (input == "END") {

            cout << "Program terminated." << endl;

            break;
        }

        double result = extractNumeric(input);

        if (result == INVALID) {

            cout << "Invalid input: no valid floating-point number found\n" << endl;
        }

        else {

            cout << fixed << setprecision(4);

            cout << "Extracted number: " << result << "\n" << endl;
        }
    }

    return 0;
}
