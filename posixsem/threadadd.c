#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define NITER 1000000

int count = 0;
// sem_t mutex;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * ThreadAdd(void * a)
{
    int i, tmp;
    for(i = 0; i < NITER; i++)
    {
		//sem_wait(&mutex);
		pthread_mutex_lock(&mutex);
        tmp = count;      /* copy the global count locally */
        tmp = tmp+1;      /* increment the local copy */
        count = tmp;      /* store the local value into the global count */ 
		//sem_post(&mutex);
		pthread_mutex_unlock(&mutex);
    }
}

/**
 * MacOS X doesn't implement sem_init and sem_destroy.
 */
int main(int argc, char * argv[])
{
    pthread_t tid1, tid2;
	
	//sem_init(&mutex, 0, 1);

    if(pthread_create(&tid1, NULL, ThreadAdd, NULL))
    {
      printf("\n ERROR creating thread 1");
      exit(1);
    }

    if(pthread_create(&tid2, NULL, ThreadAdd, NULL))
    {
      printf("\n ERROR creating thread 2");
      exit(1);
    }

    if(pthread_join(tid1, NULL))	/* wait for the thread 1 to finish */
    {
      printf("\n ERROR joining thread");
      exit(1);
    }

    if(pthread_join(tid2, NULL))        /* wait for the thread 2 to finish */
    {
      printf("\n ERROR joining thread");
      exit(1);
    }

    if (count < 2 * NITER) 
        printf("\n BOOM! count is [%d], should be %d\n", count, 2*NITER);
    else
        printf("\n OK! count is [%d]\n", count);
  
	//sem_destroy(&mutex);
    pthread_exit(NULL);
}


