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

	int* hashTable[TAMBUCKET];
	hash_oa *mutex_OA[64];

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


	int get(int chave){
	};


	void put(int chave){

		// OpenAdressingMutex **hash_oa;
		 int positionHash,i,cont;

		 positionHash = fhash(chave);

		 cont = 0;
		 while( (hashTable[positionHash] != NULL) && cont < TAMBUCKET){

			 if(*hashTable[positionHash] == chave){
				 return; //se a chave for repetida, nao insere
			 }

			 positionHash = (positionHash + 1) % TAMBUCKET;

			 cont++;
		}


		 i = positionHash/16;

		 pthread_mutex_lock(&mutex_OA[i]->lock);

		 hashTable[positionHash] = new int;
		 *hashTable[positionHash] = chave;

		 //cout<<hashTable[positionHash]<<" "<<*hashTable[positionHash]<<endl;

		 pthread_mutex_unlock(&mutex_OA[i]->lock);

	};
};




// class meuhashEncadeado
// {
// 	public:
// 		meuhashEncadeado()
// 		{}
// 		void put(int chave);
// 		int get(int chave);
// 		int search(int chave);
// 	private:
// 		no *hashTable[BUCKETSIZE];
// 		int tamanho;
// };
//
//
// void meuhashEncadeado::put(int chave){
// 	int hash = fhash(chave);
// 	no *topo = hashTable[hash];
// 	//se ele não estiver vazio insere depois do topo, e passa o fim da lista para ele
// 	if(topo->content != NULL){
// 		no *aux = new no;
// 		pthread_mutex_lock(&aux->mutex);
// 		aux->content = chave;
// 		aux->prox = topo->prox;
// 		topo->prox = aux;
// 		pthread_mutex_unlock(&aux->mutex);
//
// 	}else{
// 		pthread_mutex_lock(&topo->mutex);
// 		topo->content = chave;
// 		pthread_mutex_unlock(&topo->mutex);
// 	}
// }
//
// int meuhashEncadeado::get(int chave)
// {
// 	int ret = -1;
// 	int hash = fhash(chave);
// 	no *aux = hashTable[hash];
// 	//se ele não estiver vazio procura o fim da lista encadeada
// 	if(aux->content != chave)
// 	{
// 		while(aux->prox!= NULL && aux->prox->content != chave)
// 		{
// 			aux = aux->prox;
// 		}
// 		if (aux->prox == NULL)
// 			return ret;
// 		//apaga o proximo da lista
// 		pthread_mutex_lock(&aux->mutex);
// 		ret = aux->prox->content;
// 		aux->prox = NULL;
// 		pthread_mutex_unlock(&aux->mutex);
// 		return ret;
// 	}
// 	//apaga o topo da lista
// 	pthread_mutex_lock(&aux->mutex);
// 	ret = aux->content;
// 	aux = NULL;
// 	pthread_mutex_unlock(&aux->mutex);
// 	return ret;
// }
