#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
	int bit0 = *reg & 1;
	int bit2 = (*reg & (1 << 2)) >> 2;
	int bit3 = (*reg & (1 << 3)) >> 3;
	int bit5 = (*reg & (1 << 5)) >> 5;
	int shift_bit = bit0 ^ bit2 ^ bit3 ^ bit5;
	*reg = (*reg >> 1) + (shift_bit << 15);
}

