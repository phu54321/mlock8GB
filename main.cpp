#include <iostream>
#include <sys/mman.h>
#include <stdint.h>

size_t allocSize = 8 * 0x40000000ULL;  // 8GiB

uint64_t xorshift64(uint64_t a)
{
	a ^= a << 7;
	a ^= a >> 9;
	return a;
}

int main() {
    const size_t arraySize = allocSize / sizeof(uint64_t);
    auto allocData = new uint64_t[arraySize];
    mlock(allocData, arraySize * sizeof(uint64_t));

    printf("Filling array with random data...\n");
    uint64_t* p = allocData;
    uint64_t* pEnd = allocData + arraySize;
    uint64_t seed = std::rand() + 1;
    while (p < pEnd) {
        seed = *(p++) = xorshift64(seed);
    }
    printf("OK! 8gb filled with random data!\n");
    printf("Press enter to quit!\n");
    getchar();
    munlock(allocData, arraySize * sizeof(uint64_t));
    delete[] allocData;
    return 0;
}
