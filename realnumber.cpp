#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include "realnumber.h"


REAL_BEGIN

FixedReal::FixedReal(bool _Sign, bool _INFINITYbit) noexcept : infinity_bit_(_INFINITYbit) {
	if (infinity_bit_) {
		sign_ = _Sign;
		data_int_.clear();
		data_dec_.clear();
		return;
	}
	else {
		sign_ = false;
		data_int_.resize(10);
		data_dec_.resize(10);
	}
}
FixedReal::FixedReal(const unsigned long long& _Val, bool _Sign, bool _Check2) noexcept :sign_(_Sign), infinity_bit_(0) {
	std::string val{ std::to_string(_Val) };
	data_int_.resize(val.length());
	data_dec_.resize(val.length());
	for (int i = val.length() - 1;i >= 0;i--) {
		data_int_[val.length() - i - 1] = val.at(i) - '0';
	}
}

FixedReal::FixedReal() noexcept :infinity_bit_(0), sign_(0) {
	data_int_.resize(10);
	data_dec_.resize(10);
}
FixedReal::FixedReal(const std::string& _Val) noexcept
	:infinity_bit_(0), sign_(0) {
	std::string val = _Val;
	val.erase(
		std::remove_if(val.begin(), val.end(),
			[](char x)->bool {return (x == ',' || x == ' ' || x == '	' || x == '\n');}),
		val.end()
	);
	CapitalToSmall(val);
	if (_Val.find("-inf") != std::string::npos || _Val.find("-infinity") != std::string::npos) {
		*this = FixedReal(true, true);
		return;
	}
	if (_Val.find("inf") != std::string::npos || _Val.find("infinity") != std::string::npos) {
		*this = FixedReal(false, true);
		return;
	}
	if (_Val.find("nan") != std::string::npos) {
		*this = FixedReal(nullptr);
		return;
	}
	if (val.length() == 0) {
		*this = 0;
		return;
	}
	if (!IsNumber(val)) {
		std::cout << "It isn't a number." << std::endl;
		std::cout << "It'll be NaN instead." << std::endl;
		*this = FixedReal(nullptr);
		return;
	}
	switch (val.at(0))
	{
	case '+':
		val.erase(0, 1);
		break;
	case '-':
		sign_ = 1;
		val.erase(0, 1);
		break;
	}
	std::string int_part = val.substr(0, val.find('.'));
	std::string dec_part = val.substr(val.find('.') + 1);
	if (val.find('.') == std::string::npos) {
		dec_part = "";
	}
	int capacity_ = std::max(int_part.length(), dec_part.length());
	data_int_.resize(capacity_);
	data_dec_.resize(capacity_);
	int i = 0;
	for (;i != int_part.length();i++) {
		data_int_[i] = int_part.at(int_part.length() - i - 1) - '0';
	}
	for (i = 0;i != dec_part.length();i++) {
		data_dec_[i] = dec_part.at(i) - '0';
	}
	for (int i = capacity_ - 1;i >= 0;i--) {
		if (data_int_.at(i) != 0 || data_dec_.at(i) != 0) return;
	}
	sign_ = false;
}
FixedReal::FixedReal(const short _Val) noexcept {
	if (_Val >= 0) *this = std::move(FixedReal(_Val, false, true));
	else *this = std::move(FixedReal(static_cast<long long>(_Val) * -1, true, true));
}
FixedReal::FixedReal(const int _Val) noexcept {
	if (_Val >= 0) *this = std::move(FixedReal(_Val, false, true));
	else *this = std::move(FixedReal(static_cast<long long>(_Val) * -1, true, true));
}
FixedReal::FixedReal(const long _Val) noexcept {
	if (_Val >= 0) *this = std::move(FixedReal(_Val, false, true));
	else *this = std::move(FixedReal(static_cast<long long>(_Val) * -1, true, true));
}
FixedReal::FixedReal(const long long& _Val) noexcept {
	if (_Val >= 0) *this = std::move(FixedReal(_Val, false, true));
	else *this = std::move(FixedReal(_Val * -1, true, true));
}
FixedReal::FixedReal(const unsigned short _Val) noexcept {
	*this = std::move(FixedReal(_Val, false, true));
}
FixedReal::FixedReal(const unsigned int _Val) noexcept {
	*this = std::move(FixedReal(_Val, false, true));
}
FixedReal::FixedReal(const unsigned long _Val) noexcept {
	*this = std::move(FixedReal(_Val, false, true));
}
FixedReal::FixedReal(const unsigned long long& _Val) noexcept {
	*this = std::move(FixedReal(_Val, false, true));
}
FixedReal::FixedReal(const float _Val) noexcept {
	*this = std::move(FixedReal{ std::to_string(_Val) });
}
FixedReal::FixedReal(const double& _Val) noexcept {
	*this = std::move(FixedReal{ std::to_string(_Val) });
}
FixedReal::FixedReal(const long double& _Val) noexcept {
	*this = std::move(FixedReal{ std::to_string(_Val) });
}
FixedReal::FixedReal(const bool _Val) noexcept {
	*this = std::move(FixedReal{});
	if (_Val) {
		data_int_[0] = 1;
	}
}
FixedReal::FixedReal(const char* const _Val) noexcept {
	*this = std::move(FixedReal{ std::string(_Val) });
}
FixedReal::FixedReal(const std::nullptr_t& _nullptr) noexcept
	:infinity_bit_(0), sign_(0) {}

void FixedReal::Reserve(const size_t _Size) noexcept {
	if (infinity_bit_ || static_cast<size_t>(Capacity()) >= _Size
		|| _Size > static_cast<size_t>(INT_MAX)) return;
	data_int_.resize(_Size);
	data_dec_.resize(_Size);
}

int FixedReal::DigitInt() const noexcept {
	if (IsNan()) {
		return NULL;
	}
	int i = Capacity() - 1;
	for (;i >= 0;i--) {
		if (data_int_.at(i) != 0) break;
	}
	++i;
	return i;
}
int FixedReal::DigitDec() const noexcept {
	if (IsNan()) {
		return NULL;
	}
	int i = Capacity() - 1;
	for (;i >= 0;i--) {
		if (data_dec_.at(i) != 0) break;
	}
	++i;
	return i;
}
int FixedReal::DigitHighest() const noexcept {
	if (IsNan()) return NULL;
	if (DigitInt() > 0) return DigitInt() - 1;
	for (int i = 0;i <= Capacity();i++) {
		if (data_dec_.at(i) != 0) return -i - 1;
	}
	return 0;
}

short FixedReal::operator[](const int index) const {
	if (infinity_bit_) {
		std::cout << "REFERENCE INFINITY" << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		throw std::exception{};
	}
	if (IsNan()) {
		std::cout << "REFERENCE NaN" << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		throw std::exception{};
	}
	if (index >= DigitInt() || index < -1 * DigitDec()) return 0;
	if (index >= 0) return data_int_.at(index);
	else return data_dec_.at(-index - 1);
}
short& FixedReal::operator[](const int index) {
	if (infinity_bit_) {
		std::cout << "REFERENCE INFINITY" << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		throw std::exception{};
	}
	if (IsNan()) {
		std::cout << "REFERENCE NaN" << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		throw std::exception{};
	}
	if (index >= Capacity() || index < -1 * static_cast<int>(Capacity())) {
		std::cout << "Referenced value out of range." << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		std::cout << "capacity: " << Capacity() << ", index: " << index << std::endl;
		throw std::exception{};
	}
	if (index >= 0) return data_int_[index];
	else return data_dec_[-index - 1];
}


const FixedReal& FixedReal::operator+=(const FixedReal& _Val) noexcept {
	(*this) = (*this) + _Val;
	return *this;
}
const FixedReal& FixedReal::operator-=(const FixedReal& _Val) noexcept {
	(*this) = (*this) - _Val;
	return *this;
}
const FixedReal& FixedReal::operator*=(const FixedReal& _Val) noexcept {
	(*this) = (*this) * _Val;
	return *this;
}
const FixedReal& FixedReal::operator/=(const FixedReal& _Val) noexcept {
	(*this) = (*this) / _Val;
	return *this;
}
const FixedReal& FixedReal::operator<<=(const int _Val) noexcept {
	(*this) = (*this) << _Val;
	return *this;
}
const FixedReal& FixedReal::operator>>=(const int _Val) noexcept {
	(*this) = (*this) >> _Val;
	return *this;
}

FixedReal FixedReal::operator<<(const int index) const noexcept {
	if (IsNan() || index == 0) {
		return *this;
	}
	if (index < 0) {
		return (*this) >> (-index);
	}
	FixedReal answer{ *this };
	answer.Reserve(Capacity() + index);
	for (int i = DigitHighest();i >= -1 * DigitDec();i--) {
		answer[i + index] = answer[i];
		answer[i] = 0;
	}
	return answer;
}
FixedReal FixedReal::operator>>(const int index) const noexcept {
	if (IsNan() || index == 0) {
		return *this;
	}
	if (index < 0) {
		return (*this) << (-index);
	}
	FixedReal answer{ *this };
	answer.Reserve(Capacity() + index);
	for (int i = -1 * DigitDec();i < DigitInt();i++) {
		answer[i - index] = answer[i];
		answer[i] = 0;
	}
	return answer;
}
const FixedReal& FixedReal::operator++() noexcept {
	if (data_int_.at(0) != 9) {
		data_int_[0] += 1;
	}
	else
	{
		*this += 1;
	}
	return *this;
}
FixedReal FixedReal::operator++(int) noexcept {
	FixedReal temp{ *this };
	if (data_int_.at(0) != 9) {
		data_int_[0] += 1;
	}
	else
	{
		*this += 1;
	}
	return temp;
}
const FixedReal& FixedReal::operator--() noexcept {
	if (data_int_.at(0) != 0) {
		data_int_[0] -= 1;
	}
	else
	{
		*this -= 1;
	}
	return *this;
}
FixedReal FixedReal::operator--(int) noexcept {
	FixedReal temp(*this);
	if (data_int_.at(0) != 0) {
		data_int_[0] -= 1;
	}
	else
	{
		*this -= 1;
	}
	return temp;
}
FixedReal FixedReal::operator-() const noexcept {
	FixedReal answer{ *this };
	answer.sign_ = !answer.sign_;
	return answer;
}


std::istream& operator>>(std::istream& _Istr, FixedReal& _Reference) noexcept {
	std::string _val;
	_Istr >> _val;
	_Reference = FixedReal{ _val };
	return _Istr;
}
std::ostream& operator<<(std::ostream& _Ostr, const FixedReal& _Val) noexcept {
	if (_Val.IsNan()) {
		if (_Val.infinity_bit_) {
			if (_Val.sign_) _Ostr << "-INFINITY";
			else _Ostr << "INFINITY";
		}
		else _Ostr << "NaN";
		return _Ostr;
	}
	if (_Val.sign_ == 1) _Ostr << '-';
	if (_Val.DigitInt() == 0) _Ostr << 0;
	for (int i = _Val.DigitInt() - 1;i >= 0;i--) _Ostr << _Val.data_int_.at(i);
	if (_Val.DigitDec() == 0) return _Ostr;
	_Ostr << '.';
	for (int i = 0;i < _Val.DigitDec();i++) _Ostr << _Val.data_dec_.at(i);
	return _Ostr;
}
FixedReal operator+(const FixedReal& _Val1, const FixedReal& _Val2) {
	if (_Val1.infinity_bit_) {
		if (_Val2.infinity_bit_) {
			if (_Val1.sign_ ^ _Val2.sign_) return FixedReal(nullptr);
		}
		return _Val1;
	}
	if (_Val2.infinity_bit_) return _Val2;
	if (_Val1.IsNan() || _Val2.IsNan()) return FixedReal(nullptr);
	FixedReal answer(nullptr);
	FixedReal val1{ _Val1 };
	FixedReal val2{ _Val2 };
	if (_Val1.sign_) {
		if (_Val2.sign_) {
			val1.sign_ = false, val2.sign_ = false;
			answer = val1 + val2;
			answer.sign_ = true;
			return answer;
		}
		else
		{
			val1.sign_ = false;
			answer = val2 - val1;
			return answer;
		}
	}
	else if (_Val2.sign_) {
		val2.sign_ = false;
		answer = val1 - val2;
		return answer;
	}
	answer.Reserve(std::max(std::max(_Val1.DigitDec(), _Val1.DigitInt()), std::max(_Val2.DigitDec(), _Val2.DigitInt())) + 1);
	for (int i = -1 * answer.Capacity();i < answer.Capacity();i++) {
		answer[i] = val1.At(i) + val2.At(i);
	}
	for (int i = -1 * answer.DigitDec();i <= answer.DigitInt();i++) {
		if (answer.At(i) > 9) {
			answer[i] -= 10;
			++answer[i + 1];
		}
	}
	return answer;
}
FixedReal operator-(const FixedReal& _Val1, const FixedReal& _Val2) {
	if (_Val1.infinity_bit_) {
		if (_Val2.infinity_bit_) {
			if (_Val1.sign_ ^ _Val2.sign_) return _Val1;
		}
		return FixedReal(nullptr);
	}
	if (_Val2.infinity_bit_) {
		if (_Val2.sign_) return FixedReal(false, true);
		else return FixedReal(true, true);
	}
	if (_Val1.IsNan() || _Val2.IsNan()) return FixedReal(nullptr);
	FixedReal answer(nullptr);
	FixedReal val1{ _Val1 };
	FixedReal val2{ _Val2 };
	if (_Val1.sign_) {
		if (_Val2.sign_) {
			val1.sign_ = false, val2.sign_ = false;
			answer = val2 - val1;
			return answer;
		}
		else
		{
			val1.sign_ = false;
			answer = val1 + val2;
			answer.sign_ = true;
			return answer;
		}
	}
	else if (_Val2.sign_) {
		val2.sign_ = false;
		answer = val1 + val2;
		return answer;
	}
	if (_Val1 < _Val2) {
		answer = val2 - val1;
		answer.sign_ = true;
		return answer;
	}
	answer.Reserve(std::max(std::max(_Val1.DigitDec(), _Val1.DigitInt()), std::max(_Val2.DigitDec(), _Val2.DigitInt())));
	for (int i = -1 * answer.Capacity();i < answer.Capacity();i++) {
		answer[i] = val1.At(i) - val2.At(i);
	}
	for (int i = -1 * answer.DigitDec();i <= answer.DigitInt();i++) {
		if (answer.At(i) < 0) {
			answer[i] += 10;
			--answer[i + 1];
		}
	}
	return answer;
}
FixedReal operator*(const FixedReal& _Val1, const FixedReal& _Val2) {
	if (_Val1.infinity_bit_ && _Val2.infinity_bit_) {
		if (_Val1.sign_ ^ _Val2.sign_) return FixedReal(true, true);
		else return FixedReal(false, true);
	}
	if (_Val1.infinity_bit_ || _Val2.infinity_bit_) {
		if (_Val1 == 0 || _Val2 == 0) return FixedReal(nullptr);
		if (_Val1.sign_ ^ _Val2.sign_) return FixedReal(true, true);
		else return FixedReal(false, true);
	}
	if (_Val1.IsNan() || _Val2.IsNan()) return FixedReal(nullptr);
	if (_Val1 == 0 || _Val2 == 0) return 0;
	const int to_alloc = std::max(_Val1.DigitInt() + _Val2.DigitInt(), _Val1.DigitDec() + _Val2.DigitDec());
	FixedReal answer(nullptr);
	answer.Reserve(to_alloc);
	if (_Val1.sign_ ^ _Val2.sign_) answer.sign_ = 1;
	int k;
	for (int i = -1 * _Val1.DigitDec();i < _Val1.DigitInt();i++) {
		for (int j = -1 * _Val2.DigitDec();j < _Val2.DigitInt();j++) {
			k = _Val1.At(i) * _Val2.At(j);
			answer[i + j] += k % 10;
			answer[i + j + 1] += k / 10;
		}
	}
	for (int i = -1 * answer.DigitDec();i < answer.DigitInt();i++) {
		if (answer.At(i) > 9) {
			k = answer.At(i) / 10;
			answer[i] -= k * 10;
			answer[i + 1] += k;
		}
	}
	return answer;
}
FixedReal operator/(const FixedReal& _Val1, const FixedReal& _Val2) {
	if (_Val1.infinity_bit_ && _Val2.infinity_bit_) return FixedReal(nullptr);
	if (_Val1.infinity_bit_) {
		if (_Val1.sign_ ^ _Val2.sign_) return FixedReal(true, true);
		else return FixedReal(false, true);
	}
	if (_Val2.infinity_bit_) return 0;
	if (_Val2 == 0) {
		if (_Val1 == 0) return FixedReal(nullptr);
		if (_Val1.sign_) return FixedReal(true, true);
		else return FixedReal(false, true);
	}
	if (_Val1.IsNan() || _Val2.IsNan()) return FixedReal(nullptr);
	if (_Val1 == 0) return 0;
	const int to_alloc = std::max(FixedReal::kAccuracy, static_cast<int>(_Val1.DigitInt() + _Val2.DigitDec()));
	FixedReal answer(nullptr);
	answer.Reserve(to_alloc);
	if (_Val1.sign_ ^ _Val2.sign_) answer.sign_ = 1;
	FixedReal remainder{ _Val1 };
	FixedReal divisor{ _Val2 };
	remainder.sign_ = false;
	divisor.sign_ = false;
	FixedReal k{};
	for (int i = _Val1.DigitInt() + _Val2.DigitDec() - 1;i >= -FixedReal::kAccuracy;i--) {
		for (int j = 9;j > 0;j--) {
			k = j * divisor * PowerOfTen(i);
			if (remainder >= k) {
				answer[i] = j;
				remainder -= k;
				if (remainder == 0) return answer;
				break;
			}
		}
	}
	return answer;
}
bool operator<(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	if (_Val1.infinity_bit_) {
		if (_Val2.infinity_bit_) {
			if (_Val1.sign_ ^ _Val2.sign_) return !_Val2.sign_;
			else return false;
		}
		else return _Val1.sign_;
	}
	else if (_Val2.infinity_bit_) return !_Val2.sign_;
	if (_Val1.IsNan() || _Val2.IsNan()) return false;
	if (_Val1.sign_ ^ _Val2.sign_) return !_Val2.sign_;
	if (_Val1.sign_) {
		FixedReal val1{ _Val1 };
		FixedReal val2{ _Val2 };
		val1.sign_ = false;
		val2.sign_ = false;
		if (val1 == val2) {
			return false;
		}
		return !(val1 < val2);
	}
	if (_Val1.DigitInt() < _Val2.DigitInt()) return true;
	if (_Val1.DigitInt() > _Val2.DigitInt()) return false;
	for (int i = _Val1.DigitInt() - 1;i >= -1 * std::max(_Val1.DigitDec(), _Val2.DigitDec());i--) {
		if (_Val1.At(i) < _Val2.At(i)) return true;
		if (_Val1.At(i) > _Val2.At(i)) return false;
	}
	return false;
}
bool operator>(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	if (_Val1.infinity_bit_) {
		if (_Val2.infinity_bit_) {
			if (_Val1.sign_ ^ _Val2.sign_) return !_Val1.sign_;
			else return false;
		}
		return !_Val1.sign_;
	}
	else if (_Val2.infinity_bit_) return _Val2.sign_;
	if (_Val1.IsNan() || _Val2.IsNan()) return false;
	if (_Val1.sign_ ^ _Val2.sign_) return !_Val1.sign_;
	if (_Val1.sign_) {
		FixedReal val1{ _Val1 };
		FixedReal val2{ _Val2 };
		val1.sign_ = false;
		val2.sign_ = false;
		if (val1 == val2) {
			return false;
		}
		return !(val1 > val2);
	}
	if (_Val1.DigitInt() > _Val2.DigitInt()) return true;
	if (_Val1.DigitInt() < _Val2.DigitInt()) return false;
	for (int i = _Val1.DigitInt() - 1;i >= -1 * std::max(_Val1.DigitDec(), _Val2.DigitDec());i--) {
		if (_Val1.At(i) > _Val2.At(i)) return true;
		if (_Val1.At(i) < _Val2.At(i)) return false;
	}
	return false;
}
bool operator<=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	return (_Val1 < _Val2 || _Val1 == _Val2);
}
bool operator>=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	return (_Val1 > _Val2 || _Val1 == _Val2);
}
bool operator==(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	if (_Val1.infinity_bit_) {
		if (_Val2.infinity_bit_) return !(_Val1.sign_ ^ _Val2.sign_);
		else return false;
	}
	else if (_Val2.infinity_bit_) return false;
	else if (_Val1.IsNan() && _Val2.IsNan()) return true;
	else if (_Val1.IsNan() || _Val2.IsNan()) return false;
	else if (_Val1.sign_ ^ _Val2.sign_) return false;
	else if (_Val1.DigitInt() != _Val2.DigitInt() || _Val1.DigitDec() != _Val2.DigitDec())
		return false;
	for (int i = _Val1.DigitInt() - 1;i >= 0;i--) {
		if (_Val1.data_int_.at(i) != _Val2.data_int_.at(i)) return false;
	}
	for (int i = _Val1.DigitDec() - 1;i >= 0;i--) {
		if (_Val1.data_dec_.at(i) != _Val2.data_dec_.at(i)) return false;
	}
	return true;
}
bool operator!=(const FixedReal& _Val1, const FixedReal& _Val2) noexcept {
	return !(_Val1 == _Val2);
}


FixedReal::operator std::string() const noexcept {
	if (infinity_bit_) {
		if (sign_) return std::string{ "-INFINITY" };
		else return std::string{ "INFINITY" };
	}
	if (IsNan()) return std::string{ "NaN" };
	std::string answer;
	if (sign_) answer.push_back('-');
	for (int i = DigitInt() - 1;i >= 0;i--) answer.push_back(data_int_.at(i) + '0');
	if (DigitDec() == 0) return answer;
	answer.push_back('.');
	for (int i = 0;i < DigitDec();i++) answer.push_back(data_dec_.at(i) + '0');
	return answer;
}
FixedReal::operator bool() const noexcept {
	if (*this == 0) return false;
	else return true;
}
FixedReal::operator short() const noexcept {
	if (IsNan()) return NULL;
	if (*this > SHRT_MAX - 1)return SHRT_MAX;
	if (*this < SHRT_MIN + 1)return SHRT_MIN;
	short answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) answer += data_int_.at(i) * PowerOfTen<short>(i);
	if (sign_) answer *= -1;
	return answer;
}
FixedReal::operator unsigned short() const noexcept {
	if (IsNan()) return NULL;
	if (*this > USHRT_MAX - 1) return USHRT_MAX;
	if (*this < 1) return 0;
	unsigned short answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) answer += data_int_.at(i) * PowerOfTen<unsigned short>(i);
	return answer;
}
FixedReal::operator int() const noexcept {
	if (IsNan()) return NULL;
	if (*this > INT_MAX - 1) return INT_MAX;
	if (*this < INT_MIN + 1) return INT_MIN;
	int answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) answer += data_int_.at(i) * PowerOfTen<int>(i);
	if (sign_) answer *= -1;
	return answer;
}
FixedReal::operator unsigned int() const noexcept {
	if (IsNan()) return NULL;
	if (*this > UINT_MAX - 1) return UINT_MAX;
	if (*this < 1) return 0;
	unsigned int answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) answer += data_int_.at(i) * PowerOfTen<unsigned int>(i);
	return answer;
}
FixedReal::operator long() const noexcept {
	if (IsNan()) return NULL;
	if (*this > LONG_MAX - 1) return LONG_MAX;
	if (*this < LONG_MIN + 1) return LONG_MIN;
	long answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) answer += data_int_.at(i) * PowerOfTen<long>(i);
	if (sign_) answer *= -1;
	return answer;
}
FixedReal::operator unsigned long() const noexcept {
	if (IsNan()) return NULL;
	if (*this > ULONG_MAX - 1) return ULONG_MAX;
	if (*this < 1) return 0;
	unsigned long answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) answer += data_int_.at(i) * PowerOfTen<unsigned long>(i);
	return answer;
}
FixedReal::operator long long() const noexcept {
	if (IsNan()) return NULL;
	if (*this > LLONG_MAX - 1) return LLONG_MAX;
	if (*this < LLONG_MIN + 1) return LLONG_MIN;
	long long answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) answer += data_int_.at(i) * PowerOfTen<long long>(i);
	if (sign_) answer *= -1;
	return answer;
}
FixedReal::operator unsigned long long() const noexcept {
	if (IsNan()) return NULL;
	if (*this > ULLONG_MAX - 1) return ULLONG_MAX;
	if (*this < 1) return 0;
	unsigned long long answer = 0;
	for (int i = DigitInt() - 1;i >= 0;i--) answer += data_int_.at(i) * PowerOfTen<unsigned long long>(i);
	return answer;
}
FixedReal::operator float() const noexcept {
	if (IsNan()) return NULL;
	if (*this >= FLT_MAX) return FLT_MAX;
	if (*this <= -FLT_MAX) return -FLT_MAX;
	if (*this == 0) return 0.0f;
	float answer = 0;
	for (int i = DigitHighest();i > DigitHighest() - 7;i--) answer += At(i) * PowerOfTen<float>(i);
	if (sign_) answer *= -1;
	return answer;
}
FixedReal::operator double() const noexcept {
	if (IsNan()) return NULL;
	if (*this >= DBL_MAX) return DBL_MAX;
	if (*this <= -DBL_MAX) return -DBL_MAX;
	if (*this == 0) return 0.0;
	double answer = 0;
	for (int i = DigitHighest();i > DigitHighest() - 15;i--) answer += At(i) * PowerOfTen<double>(i);
	if (sign_) answer *= -1;
	return answer;
}
FixedReal::operator long double() const noexcept {
	if (IsNan()) return NULL;
	if (*this >= LDBL_MAX) return LDBL_MAX;
	if (*this <= -LDBL_MAX) return -LDBL_MAX;
	if (*this == 0) return 0.0L;
	long double answer = 0;
	for (int i = DigitHighest();i > DigitHighest() - 15;i--) answer += At(i) * PowerOfTen<long double>(i);
	if (sign_) answer *= -1;
	return answer;
}

short FixedReal::At(const int index) const {
	if (infinity_bit_) {
		std::cout << "$ERROR$ referenced infinity" << std::endl;
		std::cout << "ERROR position : " << __FUNCTION__ << std::endl;
		std::cout << "Class_Real_Number.cpp" << std::endl;
		return NULL;
	}
	if (index >= DigitInt() || index < -1 * DigitDec()) {
		return 0;
	}
	if (index >= 0) {
		return data_int_.at(index);
	}
	return data_dec_.at(-index - 1);
}
FixedReal FixedReal::RoundOff(const int _digit) noexcept {
	(*this) = real::RoundOff(*this, _digit);
	return *this;
}
FixedReal FixedReal::RoundUp(const int _digit) noexcept {
	(*this) = real::RoundUp(*this, _digit);
	return *this;
}
FixedReal FixedReal::RoundDown(const int _digit) noexcept {
	(*this) = real::RoundDown(*this, _digit);
	return *this;
}


FixedReal Power(const FixedReal& base, const FixedReal& exponent) noexcept {
	//Check special case
	{
		if (base.IsNan() || exponent.IsNan()) return FixedReal(nullptr);
		bool base0 = (base == 0), base_inf = (base == FixedReal(false, true));
		bool base_n_inf = (base == FixedReal(true, true)), expo0 = (exponent == 0);
		bool expo_inf = (exponent == FixedReal(false, true));
		bool expo_n_inf = (exponent == FixedReal(true, true));
		if (exponent > INT_MAX) expo_inf = true;
		else if (exponent < INT_MIN) expo_n_inf = true;
		if (((base0 || base_inf || base_n_inf) && expo0) ||
			(base_n_inf && expo_inf) ||
			(base0 && expo_n_inf)) return FixedReal(nullptr);
		if ((base0 && expo_inf) ||
			((base_inf || base_n_inf) && expo_n_inf)) return 0;
		if (base_inf && expo_inf) return FixedReal(false, true);
		//have to follow the order
		if (base0) return 0;
		if (expo0) return FixedReal(true); //1
		if (expo_n_inf) return 0;
		if (base_inf) return exponent > 0 ? FixedReal(false, true) : 0;
		if (expo_inf) return base > 0 ? FixedReal(false, true) : FixedReal(nullptr);
		if (base_n_inf) {
			if (exponent.IsInteger()) {
				if (exponent < 0) return 0;
				if (exponent.IsInteger() || exponent.data_int_[0] % 2 == 0)
					return FixedReal(false, true);
				else return FixedReal(true, true);
			}
			else return FixedReal(nullptr);
		}
	}
	if (exponent.IsInteger()) {
		FixedReal answer{ 1 };
		int _index = static_cast<int>(exponent);
		if (base == 10) return (answer << _index);

		if (_index > 0) {
			for (int i = _index - 1;i >= 0;i--) answer *= base;
		}
		else {
			for (int i = -_index - 1;i >= 0;i--) answer /= base;
		}
		return answer;
	}
#ifdef _CMATH_
	return std::pow(static_cast<double>(base), static_cast<double>(exponent));
#else
	FixedReal answer{ 1 };
	int _index = static_cast<int>(exponent);
	if (base == 10) return (answer << _index);
	if (_index > 0) {
		for (int i = _index - 1;i >= 0;i--) answer *= base;
	}
	else {
		for (int i = -_index - 1;i >= 0;i--) answer /= base;
	}
	return answer;
#endif // _CMATH_
}
FixedReal PowerOfTen(const int index) noexcept {
	FixedReal answer{ 1 };
	return answer << index;
}
FixedReal SquareRoot(const FixedReal& _Val) noexcept {
	if (_Val.sign_) return FixedReal(nullptr);
	if (_Val.infinity_bit_) return _Val;
	if (_Val.IsNan()) return FixedReal(nullptr);
	if (_Val == 0) return 0;
	FixedReal answer{};
	FixedReal remainder{ _Val };
	int highest;
	if (_Val.DigitHighest() >= 0) highest = _Val.DigitHighest() / 2;
	else highest = (_Val.DigitHighest() - 1) / 2;
	answer.Reserve(std::max(highest + 1, FixedReal::kAccuracy));
	answer[highest] = SqrtUnder100(10 * _Val.At(2 * highest + 1) + _Val.At(2 * highest));
	remainder -= FixedReal{ answer.At(highest) * answer.At(highest) } << (2 * highest);
	if (remainder == 0) return answer;
	FixedReal k{};
	for (int i = highest - 1;i >= -FixedReal::kAccuracy;i--) {
		for (int j = 9;j > 0;j--) {
			k = (((answer >> i) * 2 + j) * j) << 2 * i;
			if (k <= remainder) {
				answer[i] = j;
				remainder -= k;
				if (remainder == 0) return answer;
				break;
			}
		}
	}
	return answer;
}
FixedReal PrintSquareRoot(const FixedReal& _Val, std::ostream& _Ostr) noexcept {
	if (_Val.sign_) {
		_Ostr << "NaN" << std::endl;
		return FixedReal(nullptr);
	}
	if (_Val.infinity_bit_) {
		_Ostr << "INFINITY" << std::endl;
		return FixedReal(false, true);
	}
	if (_Val.IsNan()) {
		_Ostr << "NaN" << std::endl;
		return FixedReal(nullptr);
	}
	if (_Val == 0) {
		_Ostr << 0 << std::endl;
		return 0;
	}
	FixedReal answer{};
	FixedReal remainder{ _Val };
	int highest;
	if (_Val.DigitHighest() >= 0) highest = _Val.DigitHighest() / 2;
	else highest = (_Val.DigitHighest() - 1) / 2;
	answer.Reserve(std::max(highest + 1, FixedReal::kAccuracy));
	answer[highest] = SqrtUnder100(10 * _Val.At(2 * highest + 1) + _Val.At(2 * highest));
	remainder -= FixedReal{ answer.At(highest) * answer.At(highest) } << (2 * highest);
	if (remainder == 0) {
		if (highest >= 0) {
			_Ostr << answer.At(highest);
			for (int i = 0;i < highest;i++) _Ostr << 0;
		}
		else
		{
			_Ostr << "0.";
			for (int i = 0;i < -highest - 1;i++) _Ostr << '0';
			_Ostr << answer.At(highest);
		}
		std::cout << std::endl;
		return answer;
	}
	if (highest > 0) _Ostr << answer.At(highest);
	else if (highest == 0)
	{
		_Ostr << answer.At(highest);
		_Ostr << '.';
	}
	else
	{
		_Ostr << "0.";
		for (int i = 0;i < -highest - 1;i++)_Ostr << '0';
		_Ostr << answer.At(highest);
	}
	if (remainder == 0) {
		_Ostr << std::endl;
		return answer;
	}
	FixedReal k{};
	for (int i = highest - 1;i >= -FixedReal::kAccuracy;i--) {
		for (int j = 9;j > 0;j--) {
			k = (((answer >> i) * 2 + j) * j) << 2 * i;
			if (k <= remainder) {
				answer[i] = j;
				remainder -= k;
				_Ostr << j;
				if (remainder == 0) {
					_Ostr << std::endl;
					return answer;
				}
				break;
			}
		}
		if (answer.At(i) == 0) _Ostr << '0';
		if (i == 0) _Ostr << '.';
	}
	_Ostr << std::endl;
	return answer;
}
FixedReal RoundOff(const FixedReal& _Val, const int _digit) noexcept {
	if (_Val.IsNan()) return _Val;
	if (_digit > _Val.DigitInt()) return 0;
	if (_digit < -1 * _Val.DigitDec()) return _Val;
	FixedReal answer{};
	if (_digit == _Val.Capacity()) {
		answer.Reserve(_digit + 1);
		if (_Val.At(_digit - 1) > 4) {
			answer[_digit] = 1;
			return answer;
		}
		return 0;
	}
	answer = _Val;
	if (_Val.At(_digit - 1) > 4) {
		++answer[_digit];
		for (int i = _digit;i < answer.Capacity();i++) {
			if (answer.At(i) == 10) {
				answer[i] = 0;
				++answer[i + 1];
			}
		}
	}
	return RoundDown(answer, _digit);
}
FixedReal RoundUp(const FixedReal& _Val, const int _digit) noexcept {
	if (_Val.IsNan()) return _Val;
	if (_digit > _Val.DigitInt()) return 0;
	if (_digit < -1 * _Val.DigitDec()) return _Val;
	FixedReal answer{};
	if (_digit == _Val.Capacity()) {
		answer.Reserve(_digit + 1);
		answer[_digit] = 1;
		return answer;
	}
	answer = _Val;
	++answer[_digit];
	for (int i = _digit;i < answer.Capacity();i++) {
		if (answer.At(i) == 10) {
			answer[i] = 0;
			++answer[i + 1];
		}
	}
	return RoundDown(answer, _digit);
}
FixedReal RoundDown(const FixedReal& _Val, const int _digit) noexcept {
	if (_Val.IsNan()) return _Val;
	if (_digit >= _Val.DigitInt()) return 0;
	if (_digit < -1 * _Val.DigitDec()) return _Val;
	FixedReal answer{ _Val };
	for (int i = _digit - 1;i >= -1 * answer.Capacity();i--) answer[i] = 0;
	return answer;
}

FixedReal Factorial(const unsigned int _Val) {
	switch (_Val)
	{
	case 0:return 1;
	case 1:return 1;
	case 2:return 2;
	case 3:return 6;
	case 4:return 24;
	case 5:return 120;
	case 6:return 720;
	case 7:return 5040;
	case 8:return 40320;
	case 9:return 362880;
	case 10:return 3628800;
	case 11:return 39916800;
	case 12:return 479001600;
	default:
		FixedReal answer{ 479001600 };
		for (FixedReal i = 13;i <= _Val;i++) answer *= i;
		return answer;
	}
}

REAL_END
