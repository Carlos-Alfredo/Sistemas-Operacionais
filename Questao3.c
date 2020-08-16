#include <stdio.h>
#include <pthread.h>
#define MAX 100000000 /* quantos numeros produzir */
pthread mutex_t the_mutex;
pthread cond_t condc, condp; /* usado para sinalizacao */
int buffer[100]; /* buffer usado entre produtor e consumidor */
int bufferCount=0; //Número de espaços ocupados no buffer
void *producer(void *ptr){ /* dados do produtor */ 
	int i;
	for (i=1;i<=MAX;i++){
		pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
		while (bufferCount>99){
			pthread_cond_wait(&condp, &the_mutex);
		}
		bufferCount++;
		buffer[bufferCount] = i; /*coloca item no buffer */
		pthread_cond_signal(&condc); /* acorda consumidor */
		pthread_mutex_unlock(&the mutex);/* libera acesso ao buffer */
	}
	pthread_exit(0);
}
void *consumer(void *ptr){ /* dados do consumidor */
	int i;
	for (i = 1; i <= MAX; i++){
		pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
		while (bufferCount==0){
			pthread_cond_wait(&condc, &the_mutex);
		}
		buffer[bufferCount]=0; /* retira o item do buffer */
		bufferCount--;
		pthread_cond_signal(&condp); /* acorda o produtor */
		pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
	}
	pthread_exit(0);
}
int main(int argc, char **argv){
	pthread_t pro, con;
	pthread_mutex_init(&the_mutex,0);
	pthread_cond_init(&condc,0);
	pthread_cond_init(&condp,0);
	pthread_create(&con, 0, consumer, 0);
	pthread_create(&pro, 0, producer, 0);
	pthread_join(pro, 0);
	pthread_join(con, 0);
	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&the_mutex);
}