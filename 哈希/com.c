#include "com.h"

//素数表
#define _PrimeSize 30
//本源文件可见的静态数组，编译时创建，运行期间一直存在
static const unsigned long _PrimeList[_PrimeSize] = 
{ 11ul, 23ul, 53ul, 97ul, 193ul,  389ul,  769ul,1543ul,  3079ul,  6151ul, 12289ul, 
24593ul, 49157ul, 98317ul, 196613ul, 393241ul, 786433ul, 1572869ul, 3145739ul,
6291469ul, 12582917ul,  25165843ul,50331653ul, 100663319ul,  201326611ul, 
402653189ul, 805306457ul, 1610612741ul, 3221225473ul, 4294967291ul };

unsigned long GetPrime(unsigned long size)
{
	int i;
	for (i = 0; i < 28; i++) {
		if (size < _PrimeList[i]) {
			return _PrimeList[i];
		}
	}
	return _PrimeList[_PrimeSize-1];
}

size_t IntKeyHash(int key)
{
	return key;
}
size_t StrKeyHash(char * key)
{
	size_t seed = 131;
	size_t hash = 0;
	while (*key) {
		hash = hash * seed + (*key++);
	}
	return (hash & 0x7FFFFFFF);
}