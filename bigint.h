#ifndef BIGINT_H
#define BIGINT_H

#include <string>
#include <cstdint>

class BigInt {
public:
    BigInt(long long val);
    BigInt(std::string val);
    BigInt(const BigInt& other);
    BigInt& operator=(const BigInt& other);

    BigInt negative() const;
    BigInt abs() const;

    std::string getVal() const;
    std::size_t size() const;
    bool getSign() const;

    std::uint8_t operator[](size_t index) const;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator-() const;
    BigInt operator*(const BigInt& other) const;
    bool operator==(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    BigInt operator<<(size_t n) const;

    static const BigInt zero;
    static const BigInt one;
private:
    BigInt singleDigitMultiply(std::uint8_t digit) const;
    static void borrow(char* digits, std::size_t pos);

    std::string val;
    std::size_t numDigits;
    // 0: negative, 1: positive
    bool sign;
};

#endif
