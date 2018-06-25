#pragma once
#include <stdlib.h>
//typedef char* KeyType;
typedef int KeyType;
//typedef int ValueType;

typedef enum Status
{
	EMPTY,
	EXIST,
	DELETE,
}Status;

typedef struct HashNode
{
	KeyType _key;
	//ValueType _value;
	Status _status;
}HashNode;

typedef size_t (*HashFun)(KeyType key);
typedef size_t (*DecFun)(size_t size, size_t pos, ...);
typedef struct HashTable
{
	HashNode* _tables;
	size_t _size;//��ϣ������
	size_t _total;//��ǰ��ЧԪ�ظ���
	HashFun pHf;//��ϣ����
	DecFun	pDf;//̽�ⷽ��
}HashTable;

void HashTableInit(HashTable* ht, size_t size, HashFun pf);
void HashTableInsert(HashTable* ht, KeyType key);
HashNode* HashTableFind(HashTable* ht, KeyType key);
int HashTableRemove(HashTable* ht, KeyType key);
int HashTableDestory(HashTable* ht);
//����̽��
size_t LineDet(HashTable* ht, size_t pos);
//����̽��
size_t DouDet(HashTable* ht, size_t pos, int i);
