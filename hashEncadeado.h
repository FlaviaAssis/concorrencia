#include <pthread.h>
#include<stdlib.h>
#include<iostream>

#define TAMBUCKET 1024
#define BUCKETSIZE 256
#define fhash(i) (i*2654435761) % TAMBUCKET

using namespace std;

typedef struct __node_t {
	int key;
	struct __node_t *next;
} node_t;

class list_t
{
	public:
		list_t()
		{
			head = NULL;
			pthread_mutex_init(&lock, NULL);
		}
		void insert(int chave);
		int lookup(int chave);
	private:
		node_t *head;
		pthread_mutex_t lock;
}
typedef struct __list_t {
	node_t *head;
	pthread_mutex_t lock;
} list_t;


void list_t::insert(int chave)
{
	node_t *new = malloc(sizeof(node_t));
	if(new == NULL)
	{
		perror("malloc");
		return;
	}
	new->key = key;
	pthread_mutex_lock(lock);
	new->next = head;
	head = new;
	pthread_mutex_unlock(lock);
}

int list_t::lookup(int chave)
{
	int rv = -1;
	pthread_mutex_lock(lock);
	node_t *curr = head;
	while (curr) {
		if (curr->key == key) {
			rv = 0;
			break;
		}
		curr = curr->next;
	}// qnto menos unlock melhor
	pthread_mutex_unlock(lock);
	return rv; // retorna o valor certo
}

class meuhash
{
	public:
		meuhash()
		{
			int i;
			for (i = 0; i < BUCKETSIZE; i++) {
				lists[i] = new list_t();
			}
		}
		void put(int chave);
		int get(int chave);
	private:
		list_t lists[BUCKETSIZE];
}

void meuhash::get(int chave)
{
	int hash = fhash(chave);
	return lists[hash].lookup(chave);
}

int meuhash::put(int chave)
{
	int hash = fhash(chave);
	return lists[hash].insert(chave);
}

#if 0 

typedef struct __hash_t {
	list_t lists[BUCKETSIZE];
} hash_t;

void Hash_Init(hash_t *H) {
	int i;
	for (i = 0; i < BUCKETSIZE; i++) {
		List_Init(&H->lists[i]);
	}
}

int Hash_Insert(hash_t *H, int key) {
	int bucket = key % BUCKETS;
	return List_Insert(&H->lists[bucket], key);
}

int Hash_Lookup(hash_t *H, int key) {
	int bucket = key % BUCKETS;
	return List_Lookup(&H->lists[bucket], key);
} 

#endif


#if 0
void List_Init(list_t *L) {
	L->head = NULL;
	pthread_mutex_init(&L->lock, NULL);
} 


void List_Insert(list_t *L, int key) {
// Nao precisa travar no inicio
	node_t *new = malloc(sizeof(node_t));
	if (new == NULL) {
		perror("malloc");
		return;
	}
	new->key = key;
// so na secao critica
	pthread_mutex_lock(&L->lock);
	new->next = L->head;
	L->head = new;
	pthread_mutex_unlock(&L->lock);
}

int List_Lookup(list_t *L, int key) {
	int rv = -1;
	pthread_mutex_lock(&L->lock);
	node_t *curr = L->head;
	while (curr) {
		if (curr->key == key) {
			rv = 0;
			break;
		}
		curr = curr->next;
	}// qnto menos unlock melhor
	pthread_mutex_unlock(&L->lock);
	return rv; // retorna o valor certo
}
#endif
