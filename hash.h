#include <pthread.h>

#define TAMBUCKET 1024
#define BUCKETSIZE 256
#define fhash(i) (i*2654435761) % TAMBUCKET

class meuhash{
	public:
		meuhash()
		{
			for(int i=0; i<64;i++)
				mutex[i] = PHTHEAD_MUTEX_INITIALIZER;
		}
		int get(int chave);
		void put(int chave);
	private:
		pthread_mutex_t mutex[64];
		int hashTable[TAMBUCKET];
		int tamanho;
};

int meuhash::get(int chave)
{
	if(chave == NULL)
			return NULL;
		int hash  = fhash(chave);
		int i = 0;
		int mfind = hash/16;
		while(hashTable[hash] != NULL && hashTable[hash] != chave && i<TAMBUCKET)
		{
			i++;
			hash = (hash+1) % TAMBUCKET;
			mfind = hash/16;
		}
		pthread_mutex_lock(&mutex[mfind]);
		int ret = hashTable[hash];
		if(ret != NULL){
			hashTable[hash] = NULL;
			tamanho --;
		}
		pthread_mutex_unlock(&mutex[mfind]);
		return ret;

}

void meuhash::put(int chave)
{
	int hash = fhash(chave); 
	int mfind = hash/16;
	int i = 0;
	//se ele estiver cheio procura o proximo espaço vazio
	while(hashTable[hash] != NULL && hashTable[hash] != chave && i<TAMBUCKET)
	{
		i++;
		hash = (hash+1) % TAMBUCKET;
		mfind = hash/16;
	}
	pthread_mutex_lock(&mutex[mfind]);
	hashTable[hash] = chave;
	tamanho++;
	pthread_mutex_unlock(&mutex[mfind]);
}


typedef struct hinode
{
	hinode *prox;
	pthread_mutex_t mutex = PHTHEAD_MUTEX_INITIALIZER;
	int content;
}no;


class meuhashEncadeado
{
	public:
		meuhashEncadeado()
		{}
		void put(int chave);
		int get(int chave);
		int search(int chave);
	private:
		no *hashTable[BUCKETSIZE];
		int tamanho;
};


void meuhashEncadeado::put(int chave)
{
	int hash = fhash(chave);
	no *aux = hashTable[hash];
	//se ele não estiver vazio procura o fim da lista encadeada
	if(aux->content != NULL)
	{
		while(aux!= NULL)
		{
			aux = aux->prox;
		}
	}
	pthread_mutex_lock(&aux->mutex);
	aux->content = chave;
	pthread_mutex_unlock(&aux->mutex);
}

int meuhashEncadeado::get(int chave)
{
	int ret = -1;
	int hash = fhash(chave);
	no *aux = hashTable[hash];
	//se ele não estiver vazio procura o fim da lista encadeada
	if(aux->content != chave)
	{
		while(aux->prox!= NULL && aux->prox->content != chave)
		{
			aux = aux->prox;
		}
		if (aux->prox == NULL)
			return ret;
		//apaga o proximo da lista
		pthread_mutex_lock(&aux->mutex);
		ret = aux->prox->content;
		aux->prox = NULL;
		pthread_mutex_unlock(&aux->mutex);
		return ret;
	}
	//apaga o topo da lista
	pthread_mutex_lock(&aux->mutex);
	ret = aux->content;
	aux = NULL;
	pthread_mutex_unlock(&aux->mutex);
	return ret;
}
