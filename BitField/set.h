#pragma once
#include "bit_field.h"

class Set {
private:
	BitField bitfield;
public:
	Set();
	Set(size_t);
	Set(const Set&);
	Set(Set&&) noexcept;
	Set(const BitField&);

	//i > size -> throw
	size_t get_max() const noexcept;
	bool is_member(size_t) const;
	void insert(size_t);
	void erase(size_t);

	//a - b -> result max(a, b)
	bool operator==(const Set&) const noexcept;
	bool operator!=(const Set&) const noexcept;
	Set& operator=(const Set&);
	Set& operator=(Set&&) noexcept;
	Set operator+(const Set&) const;
	Set operator-(const Set&) const;
	Set operator*(const Set&) const;
	Set& operator+=(const Set&);
	Set& operator-=(const Set&);
	Set& operator*=(const Set&);
	Set operator-() const;
	operator BitField() const;

	friend std::ostream& operator<<(std::ostream&, const Set&);
	friend std::istream& operator>>(std::istream&, Set&);
};
