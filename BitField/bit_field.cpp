#include "bit_field.h"

static constexpr size_t bits_per_word() noexcept {
	return sizeof(unsigned int) * 8;
}

static size_t words_for(size_t n) noexcept {
	return (n + bits_per_word() - 1) / bits_per_word();
}

void BitField::mask_rest() noexcept {
	if (!data) {
		return;
	}
	size_t words = words_for(size);
	size_t r = size % bits_per_word();
	if (r == 0) {
		return;
	}
	else {
		data[words - 1] &= (1u << r) - 1u;
	}
}

BitField::BitField() {
	data = nullptr;
	size = 0;
}

BitField::BitField(size_t s) {
	if (s == 0) {
		data = nullptr;
		size = 0;
	}
	if (s > 0) {
		size = s;
		size_t words = words_for(s);
		data = new unsigned int[words];
		std::fill_n(data, words, 0u);
	}
}

BitField::BitField(const BitField& other) {
	if (other.size == 0) {
		data = nullptr;
		size = 0;
	}
	if (other.size != 0) {
		size_t words = words_for(other.size);
		data = new unsigned int[words];
		for (size_t i = 0; i < words; i++) {
			data[i] = other.data[i];
		}
		size = other.size;
	}
}

BitField::BitField(BitField&& other) noexcept {
	data = other.data;
	other.data = nullptr;
	size = other.size;
	other.size = 0;
}

BitField::~BitField() {
	delete[] data;
	size = 0;
}

BitField& BitField::operator=(const BitField& other) {
	if (this == &other) {
		return (*this);
	}
	if (other.size == 0) {
		delete[] data;
		data = nullptr;
		size = 0;
		return (*this);
	}
	else {
		size_t words = words_for(other.size);
		delete[] data;
		data = new unsigned int[words];
		for (size_t i = 0; i < words; i++) {
			data[i] = other.data[i];
		}
		size = other.size;
		return (*this);
	}
}

BitField& BitField::operator=(BitField&& other) noexcept {
	if (this == &other) {
		return (*this);
	}
	delete[] data;
	data = other.data;
	other.data = nullptr;
	size = other.size;
	other.size = 0;
	return (*this);
}

void BitField::set_bit(size_t i) {
	if (i == 0 || i > size) {
		throw std::out_of_range("wrong element");
	}
	size_t word = (i - 1) / bits_per_word();
	size_t pos = (i - 1) % bits_per_word();
	data[word] |= (1u << pos);
}

void BitField::reset_bit(size_t i) {
	if (i == 0 || i > size) {
		throw std::out_of_range("wrong element");
	}
	size_t word = (i - 1) / bits_per_word();
	size_t pos = (i - 1) % bits_per_word();
	data[word] &= ~(1u << pos);
}

bool BitField::get_bit(size_t i) const {
	if (i == 0 || i > size) {
		throw std::out_of_range("wrong element");
	}
	size_t word = (i - 1) / bits_per_word();
	size_t pos = (i - 1) % bits_per_word();
	return (data[word] >> pos) & 1u;
}

BitField BitField::operator~() const {
	BitField result(size);
	size_t words = words_for(size);
	for (size_t i = 0; i < words; i++) {
		result.data[i] = ~data[i];
	}
	result.mask_rest();
	return result;
}

BitField BitField::operator|(const BitField& other) const {
	size_t max_size = (size > other.size ? size : other.size);
	BitField result(max_size);
	size_t words_this = words_for(size);
	size_t words_other = words_for(other.size);
	size_t words_max = words_for(max_size);
	for (size_t i = 0; i < words_max; i++) {
		unsigned int operand1 = (i < words_this ? data[i] : 0u);
		unsigned int operand2 = (i < words_other ? other.data[i] : 0u);
		result.data[i] = operand1 | operand2;
	}
	result.mask_rest();
	return result;
}

BitField BitField::operator&(const BitField& other) const {
	size_t max_size = (size > other.size ? size : other.size);
	BitField result(max_size);
	size_t words_this = words_for(size);
	size_t words_other = words_for(other.size);
	size_t words_max = words_for(max_size);
	for (size_t i = 0; i < words_max; i++) {
		unsigned int operand1 = (i < words_this ? data[i] : 0u);
		unsigned int operand2 = (i < words_other ? other.data[i] : 0u);
		result.data[i] = operand1 & operand2;
	}
	result.mask_rest();
	return result;
}

BitField& BitField::operator|=(const BitField& other) {
	(*this) = (*this) | other;
	return (*this);
}

BitField& BitField::operator&=(const BitField& other) {
	(*this) = (*this) & other;
	return (*this);
}

bool BitField::operator==(const BitField& other) const noexcept {
	if (size != other.size) {
		return false;
	}
	if (size == 0) {
		return true;
	}
	size_t words = words_for(size);
	size_t count = 0;
	for (size_t i = 0; i < words; i++) {
		if (data[i] == other.data[i]) {
			count++;
		}
	}
	if (words == count) {
		return true;
	}
	return false;
}

bool BitField::operator!=(const BitField& other) const noexcept {
	return !((*this) == other);
}

std::ostream& operator<<(std::ostream& os, const BitField& other) {
	for (size_t i = 1; i < other.size; i++) {
		os << (other.get_bit(i) ? '1' : '0');
	}
	return os;
}

std::istream& operator>>(std::istream& is, BitField& other) {
	std::string tmp;
	is >> tmp;
	if (!is) {
		return is;
	}
	for (size_t i = 0; i < tmp.size(); i++) {
		if (tmp[i] != '1' && tmp[i] != '0') {
			is.setstate(std::ios::failbit);
			return is;
		}
	}
	delete[] other.data;
	size_t words = words_for(tmp.size());
	other.data = new unsigned int[words];
	std::fill_n(other.data, words, 0u);
	other.size = tmp.size();
	for (size_t i = 0; i < tmp.size(); i++) {
		if (tmp[i] == '1') {
			other.set_bit(i + 1);
		}
	}
	return is;
}