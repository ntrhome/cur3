//#include "durModel.h"
#include "durView.h"
#include <stdio.h>
#include <pthread.h>

typedef

void *start_thread(void *message) {
    printf ("%s\n", (const char *) message);
    return message;
}

int main(int argc, char *argv[], char **penv) {
    //dur();
    pthread_t thing1, thing2;
    const char *message1 = "Thing 1";
    const char *message2 = "Thing 2";
    pthread_create (&thing1, NULL, start_thread, (void *) message1);
    pthread_create (&thing2, NULL, start_thread, (void *) message2);
    pthread_join (thing1, NULL);
    pthread_join (thing2, NULL);
    return 0;
}
