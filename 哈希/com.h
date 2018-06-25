#pragma once
#include "hash.h"

unsigned long GetPrime(unsigned long size);
size_t IntKeyHash(int key);
size_t StrKeyHash(char * key);