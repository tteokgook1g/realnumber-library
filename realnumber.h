//This header file defines about class realnumber(math).

#pragma once
#ifndef REALNUMBER_REAL_H_
#define REALNUMBER_REAL_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifndef REAL_BEGIN
#define REAL_BEGIN namespace real {
#endif // !REAL_BEGIN
#ifndef REAL_END
#define REAL_END }
#endif // !REAL_END


REAL_BEGIN

namespace {
	//Change A,B,C... to a,b,c...
	inline void CapitalToSmall(std::string& val) noexcept {
		for (size_t i = 0;i != val.length();i++) {
			if ('A' <= val.at(i) && val.at(i) <= 'Z') {
				val.replace(i, 1, std::string(1, val.at(i) - 'A' + 'a'));
			}
		}
	}

	//Check that whether char type value is number or not.
	inline constexpr bool IsNumber(const char _Val) noexcept {
		return ('0' <= _Val && _Val <= '9');
	}
	inline bool IsNumber(const std::string& _Val) noexcept {
		std::string val(_Val);
		val.erase(std::remove(val.begin(), val.end(), ','), val.end());
		bool exist_point = false;
		switch (val.at(0))
		{
		case '+':case '-':
			break;
		case '.':
			exist_point = true;
			break;
		default:
			if (IsNumber(val.at(0))) break;
			return false;
		}
		for (size_t i = 1;i != val.length();i++) {
			if (val.at(i) == '.') {
				if (exist_point) return false;
				else
				{
					exist_point = true;
					continue;
				}
			}
			if (!IsNumber(val.at(i))) return false;
		}
		return true;
	}

	//Calculate power of ten.
	template<typename Ty>
	inline constexpr Ty PowerOfTen(const int index) noexcept {
		Ty answer = 1;
		if (index > 308 || index < -308) return NULL;
		if (index > 0) {
			for (int i = 0;i < index;i++) {
				answer *= 10;
			}
		}
		else
		{
			for (int i = 0;i < -1 * index;i++) {
				answer /= 10;
			}
		}
		return answer;
	}

	//Calculate square root of under 100.
	inline constexpr unsigned int SqrtUnder100(const unsigned int _Val) noexcept {
		if (_Val > 100) return NULL;
		if (_Val == 100) return 10;
		if (_Val >= 81) return 9;
		if (_Val >= 64) return 8;
		if (_Val >= 49) return 7;
		if (_Val >= 36) return 6;
		if (_Val >= 25)	return 5;
		if (_Val >= 16) return 4;
		if (_Val >= 9) return 3;
		if (_Val >= 4) return 2;
		if (_Val >= 1) return 1;
		return 0;
	}
}

//This class is about real number.
//This class is fixed point.
class FixedReal {
public:
	using DataT = short;
	//change the values according to the situation
	//The "accuracy" is how many digits are found after the decimal point.
	static const int kAccuracy = 30;

private:
	//true is -,false is +,0 is +
	bool sign_;
	std::vector<short> data_int_;
	std::vector<short> data_dec_;
	//0 means NaN or infinity.
	bool infinity_bit_;

	explicit FixedReal(bool _Sign, bool _INFINITYbit) noexcept;
	explicit FixedReal(const unsigned long long& _Val, bool _Sign, bool _Check2) noexcept;
	short operator[](const int index) const;
	short& operator[](const int index);
public:
	FixedReal() noexcept;
	FixedReal(const std::string& _Val) noexcept;
	FixedReal(const bool _Val) noexcept;
	FixedReal(const short _Val) noexcept;
	FixedReal(const int _Val) noexcept;
	FixedReal(const long _Val) noexcept;
	FixedReal(const long long& _Val) noexcept;
	FixedReal(const unsigned short _Val) noexcept;
	FixedReal(const unsigned int _Val) noexcept;
	FixedReal(const unsigned long _Val) noexcept;
	FixedReal(const unsigned long long& _Val) noexcept;
	FixedReal(const float _Val) noexcept;
	FixedReal(const double& _Val) noexcept;
	FixedReal(const long double& _Val) noexcept;
	FixedReal(const char* const _Val) noexcept;
	FixedReal(const std::nullptr_t& _nullptr) noexcept;
	FixedReal(const FixedReal& _Val) = default;
	FixedReal(FixedReal&& _Val) = default;

	~FixedReal() = default;

	//Reserves places that datas will be saved.
	void Reserve(const size_t _Size) noexcept;

	//Check how many digit are saved in integer place.
	//If integer part is 0, Nan and infinity returns 0.
	int DigitInt() const noexcept;
	//Check how many digit are saved in decimal place.
	//If decimal part is 0, Nan and infinity returns 0.
	int DigitDec() const noexcept;
	//Check the highest digit.
	//0, Nan and infinity returns 0.
	int DigitHighest() const noexcept;
	//Check current reserved size.
	inline int Capacity() const noexcept {
		return data_dec_.size();
	}
	//Check whether it is infinity or not.
	inline bool IsInfinity() const noexcept {
		return infinity_bit_;
	}
	//Check whether it is not a number or a number.
	//Includes infinity.
	inline bool IsNan() const noexcept {
		return data_dec_.size() == 0;
	}
	inline bool IsInteger() const noexcept {
		return DigitDec() == 0;
	}

	FixedReal& operator=(const FixedReal&) = default;
	FixedReal& operator=(FixedReal&&) = default;
	const FixedReal& operator+=(const FixedReal& _Val) noexcept;
	const FixedReal& operator-=(const FixedReal& _Val) noexcept;
	const FixedReal& operator*=(const FixedReal& _Val) noexcept;
	const FixedReal& operator/=(const FixedReal& _Val) noexcept;
	const FixedReal& operator<<=(const int _Val) noexcept;
	const FixedReal& operator>>=(const int _Val) noexcept;

	FixedReal operator<<(const int index) const noexcept;
	FixedReal operator>>(const int index) const noexcept;
	const FixedReal& operator++() noexcept;
	FixedReal operator++(int) noexcept;
	const FixedReal& operator--() noexcept;
	FixedReal operator--(int) noexcept;
	FixedReal operator-() const noexcept;


	friend std::istream& operator>>(std::istream& _Istr, FixedReal& _Val) noexcept;
	friend std::ostream& operator<<(std::ostream& _Ostr, const FixedReal& _Val) noexcept;
	friend FixedReal operator+(const FixedReal& _Val1, const FixedReal& _Val2);
	friend FixedReal operator-(const FixedReal& _Val1, const FixedReal& _Val2);
	friend FixedReal operator*(const FixedReal& _Val1, const FixedReal& _Val2);
	friend FixedReal operator/(const FixedReal& _Val1, const FixedReal& _Val2);
	friend bool operator<(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator>(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator<=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator>=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator==(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;
	friend bool operator!=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept;


	explicit operator std::string() const noexcept;
	explicit operator bool() const noexcept;
	explicit operator short() const noexcept;
	explicit operator unsigned short() const noexcept;
	explicit operator int() const noexcept;
	explicit operator unsigned int() const noexcept;
	explicit operator long() const noexcept;
	explicit operator unsigned long() const noexcept;
	explicit operator long long() const noexcept;
	explicit operator unsigned long long() const noexcept;
	explicit operator float() const noexcept;
	explicit operator double() const noexcept;
	explicit operator long double() const noexcept;


	//If someone tries to reference out of range, it returns 0.
	//Read Only
	short At(const int index) const;
	FixedReal RoundOff(const int _digit = 0) noexcept;
	FixedReal RoundUp(const int _digit = 0) noexcept;
	FixedReal RoundDown(const int _digit = 0) noexcept;
	inline FixedReal& Abs() noexcept {
		sign_ = false;
		return *this;
	}


	friend FixedReal Power(const FixedReal& base, const FixedReal& index) noexcept;
	friend FixedReal PowerOfTen(const int index) noexcept;
	friend FixedReal SquareRoot(const FixedReal& _Val) noexcept;
	friend FixedReal PrintSquareRoot(const FixedReal& _Val, std::ostream& _Ostr) noexcept;
	friend FixedReal RoundOff(const FixedReal& _Val, const int _digit) noexcept;
	friend FixedReal RoundUp(const FixedReal& _Val, const int _digit) noexcept;
	friend FixedReal RoundDown(const FixedReal& _Val, const int _digit) noexcept;

	friend inline FixedReal Abs(const FixedReal& val) noexcept;
};

FixedReal Power(const FixedReal& base, const FixedReal& index) noexcept;
FixedReal PowerOfTen(const int index) noexcept;
FixedReal SquareRoot(const FixedReal& _Val) noexcept;
FixedReal PrintSquareRoot(const FixedReal& _Val, std::ostream& _Ostr = std::cout) noexcept;
FixedReal RoundOff(const FixedReal& _Val, const int _digit = 0) noexcept;
FixedReal RoundUp(const FixedReal& _Val, const int _digit = 0) noexcept;
FixedReal RoundDown(const FixedReal& _Val, const int _digit = 0) noexcept;

FixedReal Factorial(const unsigned int _Val);
inline FixedReal Abs(const FixedReal& val) noexcept {
	FixedReal answer(val);
	answer.sign_ = false;
	return answer;
}

REAL_END

#endif // !REALNUMBER_REAL_H_
