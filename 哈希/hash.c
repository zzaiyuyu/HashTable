#include "hash.h"
#include "com.h"
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//线性探测
size_t LineDet(size_t size, size_t pos)
{
	pos++;
	if (pos == size) {
		pos = 0;
	}
	return pos;
}
//二次探测
size_t DouDet(size_t size, size_t pos, int i)
{
	pos += pow(i, 2);
	//当越界时，不能直接置0.因为i可能增长至大于size
	if (pos >= size) {
		pos %= size;
	}
	return pos;
}

void HashTableInit(HashTable* ht, size_t size, HashFun pHf, DecFun pDf)
{
	assert(ht);
	//获得最接近素数
	ht->_size = GetPrime(size);
	HashNode *pNew = (HashNode*)malloc(sizeof(HashNode)*(ht->_size));
	if(NULL == pNew){
		exit(EXIT_FAILURE);
	}
	memset(pNew, 0, sizeof(HashNode)*(ht->_size));
	ht->_tables = pNew;
	ht->_total = 0;
	ht->pHf = pHf;
	ht->pDf = pDf;
	size_t i = 0;
	for (i = 0; i < ht->_size; i++) {
		ht->_tables[i]._status = EMPTY;
	}
}

size_t _Hash(HashTable *ht, KeyType key)
{
	return ht->pHf(key) % ht->_size;
}
void _Swap(HashTable *pOld, HashTable *pNew)
{
	HashNode *pTmp = pNew->_tables;
	pNew->_tables = pOld->_tables;
	pOld->_tables = pTmp;
	pOld->_size = pNew->_size;
}
void _CheckCapacity(HashTable *ht)
{
	assert(ht);
	//负载因子大于0.7扩容
	int a = ht->_total * 10 / ht->_size;
	if (a >= 7) {
		size_t newSize = ht->_size * 2;
		HashTable New;
		HashTableInit(&New, newSize, ht->pHf, ht->pDf);
		size_t i;
		for (i = 0; i < ht->_size; i++) {
			if (ht->_tables[i]._status == EXIST) {
				HashTableInsert(&New, ht->_tables[i]._key);
			}
		}
		//让原hash表指向新hash，销毁新表
		_Swap(ht, &New);
		HashTableDestory(&New);
	}
}
void HashTableInsert(HashTable* ht, KeyType key)
{
	assert(ht);
	_CheckCapacity(ht);
	size_t pos = _Hash(ht, key);
	int n = 0;//冲突次数
	//找到一个状态为EMPTY，或者DELETE的位置
	while (ht->_tables[pos]._status == EXIST) {
		if (ht->_tables[pos]._key == key) {
			//说明有相同key存在.不再插入
			return 0;
		}
		//选择探测函数
		if (ht->pDf == LineDet) {
			pos = ht->pDf(ht->_size, pos);
		}
		else {
			pos = ht->pDf(ht->_size, pos, ++n);
		}
	}
	ht->_tables[pos]._key = key;
	ht->_tables[pos]._status = EXIST;
	ht->_total++;
}
HashNode* HashTableFind(HashTable* ht, KeyType key)
{
	assert(ht);
	int pos = _Hash(ht, key);
	int index = pos;
	int n = 0;
	while (ht->_tables[index]._status != EMPTY) {
		if (ht->_tables[index]._status == EXIST) {
			if (ht->_tables[index]._key == key) {
				return  &ht->_tables[index];
			}
		}
		if (ht->pDf == LineDet) {
			index = ht->pDf(ht->_size, pos);
		}
		else {
			index = ht->pDf(ht->_size, pos, ++n);
		}
		//转了一圈回到起点，未找到
		if (index == pos) {
			return NULL;
		}
	}
	return NULL;

}
int HashTableRemove(HashTable* ht, KeyType key)
{
	assert(ht);
	HashNode *pDel = HashTableFind(ht, key);
	if (NULL == pDel) {
		return 0;
	}
	else {
		pDel->_status = DELETE;
		ht->_total--;
		return 1;
	}
}

int HashTableDestory(HashTable* ht)
{
	assert(ht);
	free(ht->_tables);
	ht->_size = ht->_total = 0;
	ht->_tables = NULL;
	return 1;
}

void PrintHashInt(HashTable *ht)
{
	size_t i;
	for (i = 0; i < ht->_size; i++) {
		printf("%d ", ht->_tables[i]._key);
	}
}

void TestHash()
{
	HashTable ht;
	int data[] = { 1,2,3,4,5,26,6,7,8,21 };
	size_t len = sizeof(data) / sizeof(data[0]);
	size_t i;
	HashTableInit(&ht, len, IntKeyHash, LineDet);
	for (i = 0; i < len; i++) {
		HashTableInsert(&ht, data[i]);
	}
	PrintHashInt(&ht);
	HashTableInsert(&ht, 61);
	HashTableInsert(&ht, 62);
	HashTableInsert(&ht, 63);
	HashNode *pRet = HashTableFind(&ht, 11);
	HashTableRemove(&ht, 3);
	pRet = HashTableFind(&ht, 4);
}


void PrintHashStr(HashTable *ht)
{
	int i;
	for (i = 0; i < ht->_size; i++) {
		printf("%s\n", ht->_tables[i]._key);
	}
}
void TestStrHash() {
	HashTable ht;
	char* data[] = {"hello", "girl", "hello"};
	int len = sizeof(data) / sizeof(data[0]);
	int i;
	HashTableInit(&ht, len, StrKeyHash, DouDet);
	for (i = 0; i < len; i++) {
		HashTableInsert(&ht, data[i]);
	}
	PrintHashStr(&ht);
}