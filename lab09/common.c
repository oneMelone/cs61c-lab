#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "common.h"

// void print_vec(__m128i var) {
//     int val[4];
//     memcpy(val, &var, sizeof(val));
//     printf("[vec] %d %d %d %d\n", val[0], val[1], val[2], val[3]);
// }

long long int sum(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();

	long long int sum = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS; i++) {
			if(vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	long long int sum = 0;

	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			if(vals[i] >= 128) sum += vals[i];
			if(vals[i + 1] >= 128) sum += vals[i + 1];
			if(vals[i + 2] >= 128) sum += vals[i + 2];
			if(vals[i + 3] >= 128) sum += vals[i + 3];
		}

		//This is what we call the TAIL CASE
		//For when NUM_ELEMS isn't a multiple of 4
		//NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
		for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_simd(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);		// This is a vector with 127s in it... Why might you need this?
	long long int result = 0;				   // This is where you should put your final result!
	/* DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE. */
	
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		__m128i sum_vec = _mm_setzero_si128();
		unsigned int i = 0;
		for (; i + 4 < NUM_ELEMS; i += 4) {
			__m128i cur_vec = _mm_loadu_si128((__m128i*)(vals + i));
			__m128i larger_than_127 = _mm_cmpgt_epi32(cur_vec, _127);
			cur_vec = _mm_and_si128(cur_vec, larger_than_127);
			sum_vec = _mm_add_epi32(cur_vec, sum_vec);
		}
		for (; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) result += vals[i];
		}
		int sum_arr[4];
		_mm_storeu_si128((__m128i*)sum_arr, sum_vec);
		for (int i = 0; i < 4; i++) {
			result += sum_arr[i];
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

long long int sum_simd_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);
	long long int result = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		__m128i sum_vec = _mm_setzero_si128();
		unsigned int i = 0;
		for (; i + 16 < NUM_ELEMS; i += 16) {
			__m128i cur_vec = _mm_loadu_si128((__m128i*)(vals + i));
			__m128i larger_than_127 = _mm_cmpgt_epi32(cur_vec, _127);
			cur_vec = _mm_and_si128(cur_vec, larger_than_127);
			sum_vec = _mm_add_epi32(cur_vec, sum_vec);
			cur_vec = _mm_loadu_si128((__m128i*)(vals + i + 4));
			larger_than_127 = _mm_cmpgt_epi32(cur_vec, _127);
			cur_vec = _mm_and_si128(cur_vec, larger_than_127);
			sum_vec = _mm_add_epi32(cur_vec, sum_vec);
			cur_vec = _mm_loadu_si128((__m128i*)(vals + i + 8));
			larger_than_127 = _mm_cmpgt_epi32(cur_vec, _127);
			cur_vec = _mm_and_si128(cur_vec, larger_than_127);
			sum_vec = _mm_add_epi32(cur_vec, sum_vec);
			cur_vec = _mm_loadu_si128((__m128i*)(vals + i + 12));
			larger_than_127 = _mm_cmpgt_epi32(cur_vec, _127);
			cur_vec = _mm_and_si128(cur_vec, larger_than_127);
			sum_vec = _mm_add_epi32(cur_vec, sum_vec);
		}
		for (; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) result += vals[i];
		}
		int sum_arr[4];
		_mm_storeu_si128((__m128i*)sum_arr, sum_vec);
		for (int i = 0; i < 4; i++) {
			result += sum_arr[i];
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}
