#include "set.h"
#include <iostream>


Set eratosthenes_sieve(size_t N) {
	Set result(N);
	for (size_t i = 2; i <= N; i++) {
		result.insert(i);
	}
	for (size_t i = 2; i * i <= N; i++) {
		if (result.is_member(i)) {
			for (size_t j = 2 * i; j <= N; j += i) {
				result.erase(j);
			}
		}
	}
	return result;
}

int main() {
	Set set = eratosthenes_sieve(10);
	std::cout << set << '\n';
}
