
#include <pthread.h>
#include<stdlib.h>
#include<iostream>
 using namespace std;
 #define TAMBUCKET 1024
#define BUCKETSIZE 256
#define fhash(i) (i*2654435761) % TAMBUCKET

 typedef struct OpenAdressingHash {
	pthread_mutex_t lock;
} hash_oa;

 class meuhash{
	public:
		meuhash(){
			int i,a;
	 		for(i = 0; i < TAMBUCKET; i++){
				hashTable[i] = NULL;
				//nao eh garantia que todo ponteiro inicia com NULL, por isso precisamos fazer isso
			}
	 		for(i = 0; i < 64; i++){
				mutex_OA[i] = new hash_oa;
				a = pthread_mutex_init(&(mutex_OA[i]->lock), NULL);
				//cout<<i<<" "<<a<<endl;
			}
	 	}
	 	int get(int chave);
	 	void put(int chave);
	private:
 		int* hashTable[TAMBUCKET];
		hash_oa *mutex_OA[64];
};


void meuhash::put(int chave)
{
	// OpenAdressingMutex **hash_oa;
	int positionHash,i,cont;
	positionHash = fhash(chave);
	cont = 0;
	i = positionHash/16;
	pthread_mutex_lock(&mutex_OA[i]->lock);
	while( (hashTable[positionHash] != NULL) && cont < TAMBUCKET){
 		if(*hashTable[positionHash] == chave){
			pthread_mutex_unlock(&mutex_OA[i]->lock);
 			return; //se a chave for repetida, nao insere
		}
		pthread_mutex_unlock(&mutex_OA[i]->lock);
		positionHash = (positionHash + 1) % TAMBUCKET;
		cont++;
		i = positionHash/16;
		pthread_mutex_lock(&mutex_OA[i]->lock);
	}
	hashTable[positionHash] = new int;
	*hashTable[positionHash] = chave;
	//cout<<hashTable[positionHash]<<" "<<*hashTable[positionHash]<<endl;
	pthread_mutex_unlock(&mutex_OA[i]->lock);
}

int meuhash::get (int chave)
{
	int ret; /* variavel de retorno */
	int positionHash,i,cont;
	positionHash = fhash(chave);
	cont = 0;
	i = positionHash/16;
	pthread_mutex_lock(&mutex_OA[i]->lock);
	while( (*hashTable[positionHash] != chave) && cont < TAMBUCKET){
		pthread_mutex_unlock(&mutex_OA[i]->lock);
		positionHash = (positionHash + 1) % TAMBUCKET;
		cont++;
		i = positionHash/16;
		pthread_mutex_lock(&mutex_OA[i]->lock);
	}
	ret = *hashTable[positionHash];
	hashTable[positionHash] = NULL; /*não sei se é um delete ou receber um NULL então deixei assim (°.°')   */

	//cout<<hashTable[positionHash]<<" "<<*hashTable[positionHash]<<endl;
	pthread_mutex_unlock(&mutex_OA[i]->lock);
	return ret;
}

