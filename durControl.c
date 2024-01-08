#include <stdio.h> // for getchar()
#include <ctype.h> // for getchar()

#include "durControl.h"

static char *suits  = "scdh";
static char *ranks  = "6789XJQKA";


static int durControl_step_getCommand() {


}


int durControl_step(dur_s_round *r) {
    fflush(stdout);
    fflush (stdin);
    char c1 = (char)toupper(getchar());
    char c2 = (char)tolower(getchar());
    if(c1 == 'Q' & c2 == 'q') return DUR_QUIT;
    if(c1 == 'T' & c2 == 't') return DUR_NONE; //take
    int rank = DUR_BAD;
    for (int i = 0; i < DUR_RANKS; ++i) {
        if (c1 == ranks[i]) {
            rank = i;
            break;
        }
    }
    if(rank == DUR_BAD) return DUR_BAD;
    //printf("rank = %d | ", rank);
    int suit = DUR_BAD;
    for (int i = 0; i < DUR_SUITS; ++i) {
        if(c2 == suits[i]) {
            suit = i;
            break;
        }
    }
    if(suit == DUR_BAD) return DUR_BAD;
    //printf("suit = %d | ", suit);
    int card = DUR_RANKS * suit + rank;
    //printf("card = %d | ", card);
    int desk = DUR_BAD;
    for (int i = 0; i < DUR_CARDS; ++i) {
        if(card == r->field.desk.card[i]) {
            desk = i;
            break;
        }
    }
    if(desk == DUR_BAD) return DUR_ERROR; //dbg assert
    //printf("desk = %d | ", desk);
    int fieldFrame = r->field.history.count - r->field.fieldHistory;
    int player = (fieldFrame % 2) ? !r->attacker : r->attacker;
    for (int i = 0; i < r->field.player[player].count; ++i) {
        if(desk == r->field.player[player].desk[i]) {
            return desk;
        }
    }
    return DUR_WRONG;
}


//#include <stdio.h>
//#include <pthread.h> //for pthread_create()... man 7 pthreads
//#include <unistd.h>  //for sleep(). man 3 sleep
//
//
//typedef struct DUR_S_CTRL {
//    char *s;
//    char c;
//} dur_s_ctrl;
//pthread_mutex_t mutex;
//
//void *newCtrl(void *arg) {
//    dur_s_ctrl *a = arg;
//    //printf ("%s\n", ((dur_s_ctrl*)arg)->s);
//    pthread_mutex_lock(&mutex);
//    printf("[%lu] s=%s. I lock mutex.\n", pthread_self(), a->s);
//    printf ("[%lu] I fall asleep for 3 seconds.\n", pthread_self());
//    sleep(3);
//    printf ("[%lu] I wake up! I unlock mutex.\n", pthread_self());
//    pthread_mutex_unlock(&mutex);
//    return arg; //return NULL;
//}
//
//int main(int argc, char *argv[], char *envp[]) {
//    //dur();
//    pthread_mutex_init(&mutex,NULL);
//    dur_s_ctrl arg1 = {"THREAD A", '0'};
//
//    pthread_t ctrl1, ctrl2; //структуры - инфа о потоках
//    if(pthread_create (&ctrl1, NULL, newThread, (void *) &arg1) != 0){printf("error1\n");}
//    if(pthread_create (&ctrl2, NULL, newThread, (void *) &arg2) != 0){printf("error2\n");}
//
//    printf("[%lu] MAIN. I join to threads.\n", pthread_self());
//    pthread_join (ctrl1, NULL); //ждем завершения потока
//    pthread_join (ctrl2, NULL);
//
//    printf ("%s\n", "MAIN. kurbasa!");
//    pthread_mutex_destroy(&mutex);
//
//    return 0;
//}




// int ch;
// printf("ch = ");
// fflush(stdout); // Сброс буфера вывода
// fflush (stdin); // Очистка буфера ввода
// ch = getchar();
// printf("%d\n", ch);
// printf("%c\n", (char) ch);
