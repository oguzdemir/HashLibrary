/* $Id: hash.c,v 1.2 2015/11/08 18:47:10 korpe Exp korpe $ */

/*
Oğuz Demir 21201712 CS342-1 Project 3

Thread-safe hash table library C file. 
Every bucket is designed as an object and stores its mutex.
Data is hold in linked list inside everybucket.  
*/

#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include <pthread.h>


/*
** Struct definitions for linked list and hash table
*/
struct Node
{
	int key;
	struct Node* next;
};

struct Node* Node_new(int given)
{
	struct Node* p = malloc(sizeof(struct Node));
	p->next = NULL;
	p->key = given;
	return p;
}

typedef struct
{
	struct Node* list;
	pthread_mutex_t* mymutex;
} Bucket;


/* 
   hash table definition here - array of linked lists. 
   it is a shared data structure 
*/
Bucket* hashTable = NULL;
int size;


void hash_init(int N)
{
	size = N;
	hashTable = malloc( N * sizeof(Bucket));
	
	int i;

	for(i = 0; i < N ; i++)
	{
		hashTable[i].mymutex= malloc(sizeof(*(hashTable[i].mymutex)));
		pthread_mutex_init(hashTable[i].mymutex , NULL);
	}
}

/*
**Hash function
*/
int hash(int k)
{
	return k % size;	
}


int hash_insert(int k)
{	
	int x = hash(k);
	//Lock
	pthread_mutex_lock(hashTable[x].mymutex);
	
	//Checking whether key is exist or not
	struct Node* temp = hashTable[x].list;
	while(temp!=NULL)
	{
		if(temp->key == k)
		{
			pthread_mutex_unlock(hashTable[x].mymutex);
			return -1;
		}	
		temp = temp->next;
	}

	//Creating node
	struct Node* newNode = Node_new(k);
	temp = hashTable[x].list;
	hashTable[x].list = newNode;
	newNode->next = temp;
	//Unlock
	pthread_mutex_unlock(hashTable[x].mymutex);
	
	return (0);
}

int hash_delete(int k)
{
	int x = hash(k);
	//Lock
	pthread_mutex_lock(hashTable[x].mymutex);
	if(hashTable[x].list == NULL)
	{
		//Unlock
		pthread_mutex_unlock(hashTable[x].mymutex);
		return -1;
	}
	struct Node* temp = hashTable[x].list;
	if(temp->key == k)
	{
		hashTable[x].list = temp->next;
		free(temp);
		//Unlock
		pthread_mutex_unlock(hashTable[x].mymutex);
		return 0;
	}
	while(temp->next != NULL && temp->next->key != k)
	{
		temp = temp->next;
	}
	if(temp->next == NULL)
	{
		//Unlock
		pthread_mutex_unlock(hashTable[x].mymutex);
		return -1;
	}
	struct Node *deleting = temp->next;
	temp->next = deleting->next;
	free(deleting);
	
	//Unlock
	pthread_mutex_unlock(hashTable[x].mymutex);
	
	return (0);
}

int hash_get(int k)
{
	int x = hash(k);
	//Lock
	pthread_mutex_lock(hashTable[x].mymutex);

	struct Node* temp = hashTable[x].list;
	while(temp!=NULL)
	{
		if(temp->key == k)
		{
			//Unlock
			pthread_mutex_unlock(hashTable[x].mymutex);
			return k;
		}
		else
			temp = temp->next;
	}
	//Unlock
	pthread_mutex_unlock(hashTable[x].mymutex);
	return -1;
}
