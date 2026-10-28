#pragma once
#include <stdexcept>
#include <cstddef>
#include <iostream>
#include <algorithm>

//size -> max number [1; N]
class BitField {
private:
	unsigned int* data;
	size_t size;

	void mask_rest() noexcept;
public:
	BitField();
	BitField(size_t);
	BitField(const BitField&);
	BitField(BitField&&) noexcept;
	~BitField();

	size_t get_size() const noexcept;
	void set_bit(size_t);
	void reset_bit(size_t);
	bool get_bit(size_t) const;

	bool operator==(const BitField&) const noexcept;
	bool operator!=(const BitField&) const noexcept;
	BitField& operator=(const BitField&);
	BitField& operator=(BitField&&) noexcept;
	BitField operator|(const BitField&) const;
	BitField operator&(const BitField&) const;
	BitField& operator|=(const BitField&);
	BitField& operator&=(const BitField&);
	BitField operator~() const;

	//operator>> extends
	friend std::ostream& operator<<(std::ostream&, const BitField&);
	friend std::istream& operator>>(std::istream&, BitField&);
};
