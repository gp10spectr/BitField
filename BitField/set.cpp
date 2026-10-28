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
		return false;
	}
	return bitfield.get_bit(i);
}

