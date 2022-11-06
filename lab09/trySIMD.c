#include <x86intrin.h>
#include <string.h>
#include <stdio.h>

void print_vec(__m128i var);

int main() {
	__m128i max_short_vec = _mm_set1_epi16(0x7FFF);
	printf("max_short_vec = ");
	print_vec(max_short_vec);
	__m128i short_1_vec = _mm_set1_epi16(1);
	printf("short_1_vec = ");
	print_vec(short_1_vec);

	__m128i adds_res = _mm_adds_epi16(max_short_vec, short_1_vec);
	printf("adds_res = ");
	print_vec(adds_res);

	__m128i add_res = _mm_add_epi16(max_short_vec, short_1_vec);
	printf("add_res = ");
	print_vec(add_res);
}

void print_vec(__m128i var) {
	short val[8];
	memcpy(val, &var, sizeof(val));
	printf("[vec] %d %d %d %d %d %d %d %d\n", val[0], val[1], val[2],
			val[3], val[4], val[5], val[6], val[7]);
}
