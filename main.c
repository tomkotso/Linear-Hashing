#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hash.h"
#define INITIAL_SIZE 2

int main(int argc, char *argv[]){

    int bucketentries;
    char registeredvoters[25];

    int i;
    for (i=1; i<argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                strcpy(registeredvoters,argv[i+1]);
                i++;  
            } else {
                printf("Missing argument for -f\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-b") == 0) {
            if (i + 1 < argc) {
                bucketentries = atoi(argv[i + 1]);
                i++; 
            } else {
                printf("Missing argument for -b\n");
                return 1;
            }
        }
    }

    struct hash_table* ht = create_table(INITIAL_SIZE,bucketentries);
    struct list* list = create_list();    

    read_voters(ht,registeredvoters);

    //print_ht(ht);
    //print_list(list);

    char command[5],lname[25],fname[25],fileofkeys[25];
    int pin,zip,test;

    while(1){

        printf(">>");
        scanf("%s",command);

        if ( strcmp(command,"l") == 0 ){

            test = scanf("%d",&pin);
            if ( test == 1 )
                find_pin(ht,pin);
            else
                printf("Malformed Pin\n");

        }else if ( strcmp(command,"i") == 0 ){

            test = scanf("%d %s %s %d", &pin, lname, fname, &zip);
            if ( test == 4 )
                insert_voter(ht,pin,fname,lname,zip);
            else
                printf("Malformed Input\n");

        }else if( strcmp(command,"m") == 0 ){

            test = scanf("%d",&pin);
            if ( test == 1 )
                just_voted(ht,pin,list);
            else
                printf("Malformed Pin\n");

        }else if ( strcmp(command,"bv") == 0 ){

            scanf("%s",fileofkeys);
            bv_voted(ht,list,fileofkeys);

        }else if ( strcmp(command,"v") == 0 ){

            how_many_voted(list);

        }else if ( strcmp(command,"perc") == 0 ){

            perc(ht,list);

        }else if ( strcmp(command,"z") == 0 ){

            scanf("%d",&zip);
            voted_zip(list,zip);

        }else if ( strcmp(command,"o") == 0 ){

            sort_list(list);

        }else if( strcmp(command,"exit") == 0 ){
            break;
        }

    }

    //print_ht(ht);
    //print_list(list);
    int freed;
    freed = free_everything(ht);
    freed += free_list(list);

    printf("%d of Bytes Released\n",freed);

    return 0;
}
