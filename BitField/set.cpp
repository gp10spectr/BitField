#include "set.h"

Set::Set() {
	bitfield = BitField();
}

Set::Set(size_t s) {
	bitfield = BitField(s);
}

Set::Set(const Set& other) {
	bitfield = other.bitfield;
}

Set::Set(Set&& other) noexcept {
	bitfield = std::move(other.bitfield);
}

Set::Set(const BitField& bf) {
	bitfield = bf;
}

size_t Set::get_max() const noexcept {
	return bitfield.get_size();
}

bool Set::is_member(size_t i) const {
	if (i == 0 || i > bitfield.get_size()) {
		throw std::out_of_range("out");
	}
	return bitfield.get_bit(i);
}

void Set::insert(size_t i) {
	if (i == 0 || i > bitfield.get_size()) {
		throw std::out_of_range("out");
	}
	bitfield.set_bit(i);
}

void Set::erase(size_t i) {
	if (i == 0 || i > bitfield.get_size()) {
		throw std::out_of_range("out");
	}
	bitfield.reset_bit(i);
}

bool Set::operator==(const Set& other) const noexcept {
	return bitfield == other.bitfield;
}

bool Set::operator!=(const Set& other) const noexcept {
	return bitfield != other.bitfield;
}

Set& Set::operator=(const Set& other) {
	if (this == &other) {
		return (*this);
	}
	bitfield = other.bitfield;
	return (*this);
}

Set& Set::operator=(Set&& other) noexcept {
	if (this == &other) {
		return (*this);
	}
	bitfield = std::move(other.bitfield);
	return (*this);
}

Set Set::operator+(const Set& other) const {
	return Set(bitfield | other.bitfield);
}

Set Set::operator*(const Set& other) const {
	return Set(bitfield & other.bitfield);
}

Set Set::operator-(const Set& other) const {
	return Set(bitfield & ~(bitfield & other.bitfield));
}

Set& Set::operator+=(const Set& other) {
	(*this) = (*this) + other;
	return (*this);
}

Set& Set::operator*=(const Set& other) {
	(*this) = (*this) * other;
	return (*this);
}

Set& Set::operator-=(const Set& other) {
	(*this) = (*this) - other;
	return (*this);
}

Set Set::operator-() const {
	return Set(~bitfield);
}

Set::operator BitField() const {
	return bitfield;
}

std::ostream& operator<<(std::ostream& os, const Set& other) {
	os << "{ ";
	bool first_element = true;
	size_t max_N = other.get_max();
	for (size_t i = 1; i <= max_N; i++) {
		if (other.is_member(i)) {
			if (!first_element) {
				os << ", ";
			}
			os << i;
			first_element = false;
		}
	}
	os << " }";
	return os;
}

std::istream& operator>>(std::istream& is, Set& other) {
	char temp;
	is >> temp;
	if (!is) {
		return is;
	}
	if (temp != '{') {
		is.setstate(std::ios::failbit);
		return is;
	}
	is >> std::ws;
	if (is.peek() == '}') {
		is.get();
		other = Set();
		return is;
	}
	std::vector<size_t> numbers;
	while (true) {
		size_t value;
		if (!(is >> value)) {
			is.setstate(std::ios::failbit);
			return is;
		}
		if (value == 0) {
			is.setstate(std::ios::failbit);
			return is;
		}
		numbers.push_back(value);
		char seperate;
		if (!(is >> seperate)) {
			is.setstate(std::ios::failbit);
			return is;
		}
		if (seperate != '}') {
			break;
		}
		if (seperate == ',') {
			is.setstate(std::ios::failbit);
			return is;
		}
	}
	size_t max_N = 0;
	for (size_t i : numbers) {
		if (i > max_N) max_N = i;
	}
	Set result(max_N);
	for (size_t i : numbers) {
		result.insert(i);
	}
	other = std::move(result);
	return is;
}