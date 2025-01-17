#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "list.h"


Voter* create_voter (int pin, char* fname, char* lname, int tk){
    
    Voter* v;
    v = malloc(sizeof(struct voter));
    if ( v == NULL){
        printf("Memory error\n");
        exit(0);
    }

    v->pin = pin;
    v->tk = tk;
    v->status = 'N';
    strcpy(v->fname,fname);
    strcpy(v->lname,lname);

    return v;
}

void read_voters(Hash_Table* ht, char* registeredvoters){

    FILE *fp;
    fp = fopen(registeredvoters,"r");
    if ( fp == NULL ){
        printf("Error, couldn't open the file!\n");
        exit(0);
    }
    
    char line[70],lname[25],fname[25]; // max length name is 25
    int pin,tk;

    while ( fgets(line,70,fp) != NULL ){
        if( sscanf(line,"%d %s %s %d",&pin,fname,lname,&tk) != 4 ){
            printf("Error while reading tehe file!\n");
            exit(0);
        }

        Voter* v = create_voter(pin, fname, lname, tk);
        ht_insert(ht, v);
        //printf("%d %s %s %d %c\n",v->pin, v->fname, v->lname, v->tk, v->status);
        
    }

    fclose(fp);


}
