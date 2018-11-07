#include"hash2.h"
#include<stdlib.h>
#include<iostream>

using namespace std;


void* acessa(void* arg){
  int i;
	float ratio = 800;
	meuhash* h = (meuhash*)arg;

	for(i=0;i<1000;i++){

		if(rand()%1000<ratio){
			h->put(rand()%2000);
		} else {
			//h->get(rand()%2000);
		}

		if(i%10==0){
			ratio=(150 + ratio)/2.71;
		}
	}

}

int main(int argc,char* argv[]){
	meuhash h;
	float ratio=800;
	int i;
	pthread_t p[10];

	srand(0);

	for(i=0;i<10;i++){
		pthread_create(&p[i],NULL,acessa,(void*)&h);
		pthread_join(p[i],NULL);
	}
	for(i=0;i<10;i++)
		pthread_join(p[i],NULL);
}
