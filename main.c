//#include "durModel.h"
#include "durView.h"
#include <stdio.h>
#include <pthread.h> //for pthread_create()... man 7 pthreads
#include <unistd.h>  //for sleep(). man 3 sleep


typedef struct DUR_S_CTRL {
    char *s;
} dur_s_ctrl;
pthread_mutex_t mutex;

void *newThread(void *arg) {
    dur_s_ctrl *a = arg;
    //printf ("%s\n", ((dur_s_ctrl*)arg)->s);
    pthread_mutex_lock(&mutex);
    printf("[%lu] s=%s. I lock mutex.\n", pthread_self(), a->s);
    printf ("[%lu] I fall asleep for 3 seconds.\n", pthread_self());
    sleep(3);
    printf ("[%lu] I wake up! I unlock mutex.\n", pthread_self());
    pthread_mutex_unlock(&mutex);
    return arg; //return NULL;
}

int main(int argc, char *argv[], char *envp[]) {
    //dur();
    pthread_mutex_init(&mutex,NULL);
    dur_s_ctrl arg1 = {"THREAD A"};
    dur_s_ctrl arg2 = {"THREAD B"};
    pthread_t ctrl1, ctrl2; //структуры - инфа о потоках
    if(pthread_create (&ctrl1, NULL, newThread, (void *) &arg1) != 0){printf("error1\n");}
    if(pthread_create (&ctrl2, NULL, newThread, (void *) &arg2) != 0){printf("error2\n");}

    printf("[%lu] MAIN. I join to threads.\n", pthread_self());
    pthread_join (ctrl1, NULL); //ждем завершения потока
    pthread_join (ctrl2, NULL);

    printf ("%s\n", "MAIN. kurbasa!");
    pthread_mutex_destroy(&mutex);

    return 0;
}
