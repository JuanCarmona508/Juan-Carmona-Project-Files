#include <stdio.h>
#include <pthread.h>

#include <stdlib.h>
#include <time.h> 
#include <stdbool.h>
pthread_mutex_t deck_mutex;

pthread_cond_t round_end;
pthread_cond_t turn;
pthread_cond_t nextRound;
FILE *game;
#define NUM_THREADS	6
int give = 0;
int r = 0;
int deal = 1;
int size = 52;
int target = 0;
int order = 1;
bool win = false;
//bool deal_pass = false;
int summary = 0;
int deck[52] = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,10,11,12,13};
int pass[5];
void shuffle(){
    int j;// rand()%52;
    int temp;
    int i;
    for(i = 0; i < 52; i++){
        j = rand()%52;
        temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }    

}
void printfCard(int card){
    switch(card){
        case 1:
            fprintf(game,"%c",'A');
            break;
        case 11:
            fprintf(game,"%c",'J');
            break;
        case 12:
            fprintf(game,"%c",'Q');
            break;
        case 13:
            fprintf(game,"%c",'K');
            break;
        default:
            fprintf(game,"%d", card);
            break;
    }
}
void printCard(int card){
    switch(card){
        case 1:
            printf("%c",'A');
            break;
        case 11:
            printf("%c",'J');
            break;
        case 12:
            printf("%c",'Q');
            break;
        case 13:
            printf("%c",'K');
            break;
        default:
            printf("%d", card);
            break;
    }
}
void fprintDeck(int d[], int s){
    int i = 0;
    //printf("DECK: ");
    while(d[i] != 0 && i<s){
        //fprintf(game, "%d ", deck[i]);
        printfCard(deck[i]);
        fprintf(game," ");
        i++;
    }
    fprintf(game,"\n");
}
void printDeck(int d[], int s){
    int i = 0;
    //printf("DECK: ");
    while(d[i] != 0 && i<s){
        //printf("%d ", deck[i]);
        printCard(deck[i]);
        printf(" ");
        i++;
    }
    printf("\n");
}
int draw(){
    int card;
    int i;
    card  = deck[0];
    for( i = 1; i < 52; i++){
        //temp[i-1] = deck[i];
        deck[i-1] = deck[i];
    }
 
    deck[51] = 0;
    size--;
    return card;
}
void discard(int card){
    int i = 0;
    while(deck[i] != 0 && i < 52){
        i++;
    }
    deck[i] = card;
    size++;
}
void endRound(){//int id){
    r++;
    //printf("\nPlayer 2: wins",);
    fprintf(game, "PLAYER %d: Round ends\n",deal);
    deal++;
    win = false;
    give = 0; 
    discard(target);
}


void dealer(){
    int i;
    shuffle();
    target = draw();
    for( i = 0; i < 5; i++){
        pass[i] = draw();
    }
    
    summary = 5;
    pthread_cond_signal(&turn);
    
}
void *player(void *id){

    int hand[2];
    bool winner = false;

    long pid = (long) id+1;
    
    while(order != 6 && order < 7){///////////////////////////////////////////////////////////////ORDER
            pthread_mutex_lock(&deck_mutex);
            if(order+1 == pid && pid != deal){ //// locks in order
              //pthread_mutex_lock(&deck_mutex);

                //printf("\nPlayer %ld: I'm ordered", pid);
                order++;
                //printf(" order: %d dealer: %d\n", order, deal);
                pthread_cond_wait(&turn, &deck_mutex);
                //printf("Player %ld: I'm impatient\n", pid);
                order = 7;
              //  pthread_mutex_unlock(&deck_mutex);
            }////////////////////////////////////////////////////////////////////
            
            pthread_mutex_unlock(&deck_mutex);
        }
    
   
    while(r < 6){
        if(deal == pid){
            pthread_mutex_lock(&deck_mutex);
            order = 7;
            dealer();
            
            pthread_cond_wait(&round_end, &deck_mutex);
            endRound();// ends round///////////////////////////////////////////////////////
            order = deal;
            pthread_cond_broadcast(&nextRound);
            pthread_mutex_unlock(&deck_mutex);
        }
        else{
            
            pthread_mutex_lock(&deck_mutex);
            //pthread_cond_wait(&turn, &deck_mutex);
            hand[0] = pass[give];
            give++;
            //printf("\nPlayer %ld: Give Card %d\n",pid, give);
            pthread_mutex_unlock(&deck_mutex);
            while(!win){
                pthread_mutex_lock(&deck_mutex);
                fprintf(game,"PLAYER %ld: hand ", pid);
                printfCard(hand[0]);
                hand[1] = draw();
                fprintf(game,"\nPLAYER %ld: draws ", pid);
                printfCard(hand[1]);
                //printf("\nPlayer %d: Target Card %d\n", deal, target);
                printf("PLAYER %d: Target Card ",deal);
                printCard(target);
                //printf("Player %ld: hand %d,%d\n", pid, hand[0], hand[1]);
                printf("\nPLAYER %ld: hand ",pid);
                printCard(hand[0]);
                printf(",");
                printCard(hand[1]);
                printf("\n");
                if(hand[0] == target || hand[1] == target ){
                    printf("PLAYER %ld: WIN yes\n", pid);
                    win = true;
                    winner = true;
                    fprintf(game,"\nPLAYER %ld: hand (",pid);
                    printfCard(hand[0]);
                    fprintf(game,",");
                    printfCard(hand[1]);
                    fprintf(game,") <> Target card is ");
                    printfCard(target);
                    fprintf(game, "\n");
                    fprintf(game,"PLAYER %ld: wins round %d\n", pid, r+1);
                    
                    
                }
                else{
                    printf("PLAYER %ld: WIN no\n", pid);
                }
                printf("DECK: ");
                printDeck(deck, size);
                if(rand()%2 == 0){
                    discard(hand[0]);
                    if(!winner){
                        fprintf(game,"\nPLAYER %ld: discards ",pid);//, hand[0]);
                        printfCard(hand[0]);
                        fprintf(game," at random\n");
                    }
                    hand[0] = hand[1];
                    hand[1] = 0;
                    if(!winner){
                        fprintf(game,"PLAYER %ld: hand ", pid);
                        printfCard(hand[0]);
                        fprintf(game, "\n");
                    }
                }
                else{
                    discard(hand[1]);
                    //fprintf(game, "Player %ld: discards %d\n",pid, hand[1]);
                    if(!winner){
                        fprintf(game,"\nPLAYER %ld: discards ",pid);//, hand[1]);
                        printfCard(hand[1]);
                        fprintf(game," at random\n");
                        fprintf(game,"PLAYER %ld: hand ", pid);
                        printfCard(hand[0]);
                        fprintf(game, "\n");
                    }
                    hand[1] = 0;
                }
                
                if(!winner){
                    fprintf(game,"DECK: ");
                    fprintDeck(deck, size);
                }
                pthread_cond_signal(&turn);
                pthread_cond_wait(&turn, &deck_mutex);
                
                pthread_mutex_unlock(&deck_mutex);
            }
            pthread_mutex_lock(&deck_mutex);
           
            if(!winner){
                fprintf(game, "PLAYER %ld: lost round %d\n", pid, r+1);
            }
            discard(hand[0]);
            hand[0] = 0;
            summary--;
            winner = false;
            if(summary <= 0){
                //printf("\nPlayer %ld: size check %d\n", pid, size);
                pthread_cond_signal(&round_end);
            }
            else{
                //printf("\nPlayer %ld: size check %d\n", pid, size);
                pthread_cond_signal(&turn);
            }
            //pthread_mutex_lock(&deck_mutex);
            pthread_cond_wait(&nextRound, &deck_mutex);
            //pthread_mutex_unlock(&deck_mutex);
            pthread_mutex_unlock(&deck_mutex);

            
            
        }
        
        
        while(order+1 != deal && order < 7){///////////////////////////////////////////////////////////////ORDER
            pthread_mutex_lock(&deck_mutex);
            if(order+1 == pid && pid != deal){ //// locks in order
              //  pthread_mutex_lock(&deck_mutex);

                //printf("\nPlayer %ld: I'm ordered", pid);
                order++;
                //printf(" order: %d dealer: %d\n", order, deal);
                pthread_cond_wait(&turn, &deck_mutex);
                //printf("Player %ld: I'm impatient\n", pid);
                order = 7;
              //  pthread_mutex_unlock(&deck_mutex);
            }////////////////////////////////////////////////////////////////////
            else if(order == 6 && pid == 1){
                //printf(" \nPlayer %ld will be ordered", pid);
                order = 1;
                pthread_cond_wait(&turn, &deck_mutex);
                order = 7; //break;
            }
            
            pthread_mutex_unlock(&deck_mutex);
        }
        
    }
    //pthread_exit(NULL);
    pthread_cond_broadcast(&turn);
    pthread_exit(NULL);
}


int main(int argc, char **argv){
    int rs = atoi(argv[1]);
    srand(rs);
    char name[100];
    
    sprintf(name, "gameLog%d.txt", rs);
    game = fopen(name, "w");
    
    
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    int i;
    pthread_attr_t attr;
    
    pthread_mutex_init(&deck_mutex, NULL);
    pthread_cond_init (&round_end, NULL);
    pthread_cond_init (&turn, NULL);
    pthread_cond_init (&nextRound, NULL);
    pthread_attr_init(&attr);
    
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for(t=0; t<NUM_THREADS; t++){
       //printf("\nIn main: creating thread %ld\n", t);
       rc = pthread_create(&threads[t], &attr, player, (void *)t);
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }
    }
    
     for ( i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
     }
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&deck_mutex);
    pthread_cond_destroy(&round_end);
    pthread_cond_destroy(&turn);
    pthread_cond_destroy(&nextRound);
    fclose(game);
    
    pthread_exit(NULL);
    //return 0;
}

