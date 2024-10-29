#include "bigint.h"
#include <iostream>

using namespace std;

const BigInt BigInt::zero("0");
const BigInt BigInt::one("1");

string toString(long long val) {
    size_t numDigits = log10(abs(val)) + 1;
    char* digits = new char[numDigits + 1];
    digits[numDigits] = '\0';
    for (int i = 0; i < numDigits; i++) {
        int digit = val % 10;
        digits[numDigits - i - 1] = '0' + digit;
        val /= 10;
    }
    string str(digits);
    if (val < 0) {
        str = '-' + str;
    }
    delete [] digits;
    return str;
}

BigInt::BigInt(long long val) : BigInt(toString(val)) {};

BigInt::BigInt(string val) {
    if (val[0] == '-') {
        this -> sign = 0;
        val = val.substr(1);
    } else {
        this -> sign = 1;
    }

    int start = 0;
    if (val.size() > 1) {
        while (val[start] == '0') {
            start++;
        }
    }

    this -> val = val.substr(start);
    this -> numDigits = val.size() - start;
}

BigInt::BigInt(const BigInt& other) : val(other.val), numDigits(other.numDigits), sign(other.sign) {}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this == &other) {
        return *this;
    }

    val = other.val;
    numDigits = other.numDigits;
    sign = other.sign;
    return *this;
}

string BigInt::getVal() const {
    return val;
}

size_t BigInt::size() const {
    return numDigits;
}

bool BigInt::getSign() const {
    return sign;
}

BigInt BigInt::negative() const {
    BigInt output = *this;
    output.sign = !sign;
    return output;
}

BigInt BigInt::abs() const {
    BigInt output = *this;
    output.sign = 1;
    return output;
}

uint8_t BigInt::operator[](size_t index) const {
    return val[index] - '0';
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (!getSign()) {
        if (other.getSign()) {
            return other - abs();
        }
        else return (abs() + other.abs()).negative();
    } else if (!other.getSign()) {
        return *this - other.abs();
    }

    size_t newSize = max(numDigits, other.numDigits) + 1;
    char* digits = new char[newSize + 1];
    digits[newSize] = '\0';

    bool carry = false;
    for (int i = numDigits - 1, j = other.numDigits - 1, k = newSize - 1; i >= 0 || j >= 0; i--, j--, k--) {
        uint8_t aDigit = i >= 0 ? (*this)[i] : 0;
        uint8_t bDigit = j >= 0 ? other[j] : 0;
        uint8_t sum = aDigit + bDigit + carry;
        if (sum >= 10) {
            carry = true;
            sum -= 10;
        } else {
            carry = false;
        }
        digits[k] = sum + '0';
    }
    digits[0] = carry + '0';

    string str(digits);
    delete [] digits;
    return BigInt(str);
}

BigInt BigInt::operator-(const BigInt& other) const {
    /*if (*this == other) {
        return BigInt::zero();
    }*/

    if (!getSign()) {
        if (other.getSign()) {
            return (abs() + other).negative();
        }
        return other - abs();
    } else if (!other.getSign()) {
        return *this + other.abs();
    }

    /*if (*this < other) {
        return (other - *this).negative();
    }*/

    char* digits = new char[numDigits + 1];
    for (int i = 0; i < numDigits - other.numDigits; i++) {
        digits[i] = val[i];
    }
    digits[numDigits] = '\0';

    for (int i = numDigits - 1, j = other.numDigits - 1; i >= 0 || j >= 0; i--, j--) {
        uint8_t aDigit = (*this)[i];
        uint8_t bDigit = i >= numDigits - other.numDigits ? other[i - (numDigits - other.numDigits)] : 0;
        uint8_t diff;
        if (aDigit < bDigit) {
            borrow(digits, i);
            diff = aDigit + 10 - bDigit;
        } else {
            diff = aDigit - bDigit;
        }
        digits[i] = diff + '0';
    }

    string str(digits);
    delete [] digits;
    return BigInt(str);
}

void BigInt::borrow(char* digits, size_t pos) {
    
}

BigInt BigInt::operator-() const {
    return negative();
}

BigInt BigInt::singleDigitMultiply(uint8_t m) const {
    char* digits = new char[numDigits + 2];
    digits[numDigits + 1] = '\0';

    uint8_t carry = 0;
    for (int i = numDigits - 1; i >= 0; i--) {
        uint8_t digit = (*this)[i];
        uint8_t product = digit * m + carry;
        digits[i + 1] = product % 10 + '0';
        carry = product / 10;
    }
    digits[0] = carry + '0';

    string str(digits);
    delete [] digits;
    return BigInt(str);
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt output = BigInt::zero;
    for (int i = other.numDigits - 1; i >= 0; i--) {
        BigInt addend = singleDigitMultiply(other[i]) << (other.numDigits - 1 - i);
        output = output + addend;
    }
    return output;
}

bool BigInt::operator==(const BigInt& other) const {
    return val == other.val;
}

bool BigInt::operator<(const BigInt& other) const {
    if (numDigits < other.numDigits) {
        return true;
    }
    
    if (numDigits > other.numDigits) {
        return false;
    }

    
}

BigInt BigInt::operator<<(size_t n) const {
    BigInt output = *this;
    for (size_t i = 0; i < n; i++) {
        output.val += '0';
    }
    output.numDigits += n;
    return output;
}

void test() {
    time_t seed = time(0);
    cout << "Seed: " << seed << endl;
    srand(seed);

    bool passed = true;

    for (int i = 0; i < 100; i++) {
        long a = rand();
        long b = rand();
        BigInt bigA(a);
        BigInt bigB(b);
        BigInt sum = bigA + bigB;
        if (toString(a + b) != sum.getVal()) {
            passed = false;
            cout << "Error: a + b returns " << sum.getVal() << " but should return " << a + b << endl;
        }
    }

    if (passed) {
        cout << "Passed!" << endl;
    }
}

int main() {
    long long a = 89729372;
    long long b = 78483;
    BigInt bigA(a);
    BigInt bigB(b);
    cout << (bigA - bigB).getVal() << endl;
    cout << a - b << endl;
}
