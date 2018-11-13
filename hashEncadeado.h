#include <pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<iostream>

#define TAMBUCKET 1024
#define BUCKETSIZE 256
#define fhash(i) (i*2654435761) % BUCKETSIZE

using namespace std;

typedef struct __node_t {
	int chave;
	struct __node_t *next;
} node_t;

class list_t{
	public:
		pthread_mutex_t lock;
		list_t(){
			head = NULL;
			pthread_mutex_init(&lock, NULL);
		}
		void insert(int chave);
		int lookup(int chave);

	private:
		node_t *head;
};

void list_t::insert(int chave){
	node_t *novo = (node_t*) malloc(sizeof(node_t));
	if(novo == NULL)
	{
		perror("malloc");
		return;
	}
	novo->chave = chave;
	pthread_mutex_lock(&lock);
	novo->next = head;
	head = novo;
	pthread_mutex_unlock(&lock);
	//cout<<"foi normal"<<endl;
}

int list_t::lookup(int chave)
{
	//cout<<"move-"<<endl;
	int rv = -1;
	pthread_mutex_lock(&lock);
	if (head== NULL){
		pthread_mutex_unlock(&lock);
		return rv;
	}
	//cout<<"ok"<<endl;
	node_t *curr = head;
	//cout<<"move"<<endl;
	while (curr!=NULL) {
		if (curr->chave == chave) {
			rv = 0;
			break;
		}
		curr = curr->next;
	}// qnto menos unlock melhor
	pthread_mutex_unlock(&lock);
	return rv; // retorna o valor certo
}

class meuhash
{
	public:
		meuhash()
		{
			int i;
			for (i = 0; i < BUCKETSIZE; i++) {
				lists[i] = list_t();
			}
		}
		void put(int chave);
		int get(int chave);
	private:
		list_t lists[BUCKETSIZE];
};

int meuhash::get(int chave)
{
	int hash = fhash(chave);
	return lists[hash].lookup(chave);
}

void meuhash::put(int chave)
{
	int hash = fhash(chave);
	//cout<<hash<<endl;
	return lists[hash].insert(chave);
}
