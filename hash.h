#include <pthread.h>

#define TAMBUCKET 1024
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
		int search(int chave);
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
