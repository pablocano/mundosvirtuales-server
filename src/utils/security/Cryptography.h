#pragma once
#include <stdint.h>
#define VERSION "0.1"

class Cryptography
{
public:
	
	static void encrypt(uint32_t *v, unsigned n, uint32_t const key[4]);

	static void decrypt(uint32_t *v, unsigned n, uint32_t const key[4]);
};