#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#define L 0.75


struct bucket{

    int cur_size; // current size
    struct voter** voters; // -b pointer(s) to voters in each bucket
    struct bucket* overflow_bucket; // pointer to next bucket

};

struct hash_table{

    int size;  // rows
    int keys;  // number of keys currently in table
    int m;  // starting size
    int b;  // -b parameter
    int p;
    int round;
    int prev_size;
    int to_free;
    struct bucket* buckets;

};

struct hash_table* create_table(int m,int b){

    struct hash_table* ht;
    ht = malloc(sizeof(struct hash_table));
    if ( ht == NULL){
        printf("Memory error\n");
        exit(0);
    }

    ht->m = m;
    ht->size = m;
    ht->keys = 0;
    ht->p = 0;
    ht->round = 0;
    ht->b = b;
    ht->prev_size = m;
    ht->to_free = 0;
    ht->buckets = malloc( m * sizeof(struct bucket) );
    if ( ht->buckets == NULL){
        printf("Memory error\n");
        exit(0);
    }

    int i,j;
    for (i=0;i<m;i++){
        ht->buckets[i].voters = malloc( b * sizeof(struct voter*));
        if ( ht->buckets[i].voters == NULL){
            printf("Memory error\n");
            exit(0);
        }
        for (j=0; j<b; j++)
            ht->buckets[i].voters[j] = NULL;
        ht->buckets[i].overflow_bucket = NULL;
        ht->buckets[i].cur_size = 0;
    }
    return ht;
}

int hash_f(int pin, int round, int m){
    return pin % ( (1 << round) * m );
}

int find_pos(Hash_Table* ht, int pin){
    
    int pos;

    pos = hash_f(pin, ht->round, ht->m); // h(i)
    if ( pos < ht->p )
        pos = hash_f(pin, (ht->round) + 1, ht->m); // h(i+1)
    //printf("round = %d   m = %d \n  to %d tha mpei sthn thesh %d\n\n", ht->round, ht->m, v->pin, pos);
    return pos;

}

void bucket_insert(Bucket* bucket, Voter* v, int max_size){

    int j;
    if ( bucket->cur_size == max_size ){

        if ( bucket->overflow_bucket == NULL ){ // create new overflow bucket
            bucket->overflow_bucket = malloc(sizeof(struct bucket));
            if ( bucket->overflow_bucket == NULL){
                printf("Memory error\n");
                exit(0);
            }
            bucket->overflow_bucket->overflow_bucket = NULL;
            bucket->overflow_bucket->cur_size = 0;
            bucket->overflow_bucket->voters = malloc(max_size * sizeof(struct voter*));
            if ( bucket->overflow_bucket->voters == NULL){
                printf("Memory error\n");
                exit(0);
            }
            for (j=0; j<max_size; j++)
                bucket->overflow_bucket->voters[j] = NULL;
        }
        bucket_insert(bucket->overflow_bucket, v, max_size); 

    }else{
        int ins_pos = bucket->cur_size; // insert into the first free position
        bucket->voters[ins_pos] = v;
        bucket->cur_size++;
    }

}

void ht_insert(Hash_Table* ht, Voter* v){

    // check if already exists

    int pos = find_pos(ht, v->pin); // we found the bucket to insert the voter

    bucket_insert(&ht->buckets[pos], v, ht->b); // pass the address because bucket can change, and allow to handle overflow buckets

    ht->keys++;
    if ( ( (float)(ht->keys) / (ht->size * ht->b)) > L )
        split_bucket(ht);

}

void split_bucket(Hash_Table* ht){

    int i,j,flag;
    ht->size++;             // add new bucket
    ht->buckets = realloc(ht->buckets,(ht->size*sizeof(struct bucket)));
    ht->buckets[ht->size-1].voters = malloc( ht->b * sizeof(struct voter*));
    if ( ht->buckets[ht->size-1].voters == NULL){
        printf("Memory error\n");
        exit(0);
    }
    for (j=0; j<ht->b; j++)
        ht->buckets[ht->size-1].voters[j] = NULL;
    ht->buckets[ht->size-1].overflow_bucket = NULL;
    ht->buckets[ht->size-1].cur_size = 0;

    // now temporally save the voters of the buckets that are going to be split
    // clear/free the memory, and re-insert the voters

    int count = 0;
    struct bucket* temp = &ht->buckets[ht->p];
    while ( temp != NULL ){
        count += temp->cur_size; // number of voters to re-insert
        temp = temp->overflow_bucket;
    }


    struct bucket* temp2;
    struct voter** revoters = malloc ( count * sizeof(struct voter*));
    if ( revoters == NULL ){
        printf("Memory error\n");
        exit(0);
    }
    i=0;
    flag=0;
    temp = &ht->buckets[ht->p];
    


    while ( temp != NULL ){
        for(j=0; j<temp->cur_size; j++){
            revoters[i++] = temp->voters[j];
            temp->voters[j] = NULL;
        }

        temp2 = temp;
        temp = temp->overflow_bucket;
        
        temp2->cur_size = 0;
        temp2->overflow_bucket = NULL;
        if(flag++ > 0){
            ht->to_free += sizeof(temp2->voters);
            free(temp2->voters);
            ht->to_free += sizeof(temp2);
            free(temp2); // don't free first bucket, only the overflows
        }

    }

    ht->keys = ht->keys - count; // since they will be re-inserted
    
    ht->p++;  // increase the p before the insertion, to ensure that the hash function that will be used is h(i+1) and not h(i)

    for (i=0; i<count; i++)
            ht_insert(ht,revoters[i]);
    

    if ( ht->size == ht->prev_size * 2 ){
        ht->prev_size = ht->size;
        ht->round++;
        ht->p = 0;
    }

    ht->to_free += sizeof(revoters);
    free(revoters);

}


void print_bucket(Bucket* bucket){
    int i;
    for(i=0; i<bucket->cur_size ;i++)
        printf(" | %d | ", bucket->voters[i]->pin);
    if ( bucket->overflow_bucket != NULL ){
        printf(" ---> ");
        print_bucket(bucket->overflow_bucket);
    }
}

void print_ht(Hash_Table* ht){
    int i;
    for(i=0; i<ht->size; i++){
        printf("%d: ",i);
        print_bucket(&ht->buckets[i]);
        printf("\n");
    }
}

void find_pin(Hash_Table* ht, int pin){

    int pos = find_pos(ht, pin); 
    int i,found;
    found = 0;
    struct bucket* bucket;
    bucket = &ht->buckets[pos];

    while ( bucket != NULL ){
        for (i=0; i<bucket->cur_size; i++){
            if ( bucket->voters[i]->pin == pin ){
                found++;
                printf("%d %s %s %d %c\n",bucket->voters[i]->pin,bucket->voters[i]->lname,bucket->voters[i]->fname,bucket->voters[i]->tk,bucket->voters[i]->status);
            }
        }
        bucket = bucket->overflow_bucket;
    }
    if ( found == 0 )
        printf("Participant %d not in cohort\n",pin);
}

void insert_voter(Hash_Table* ht, int pin, char* fname, char* lname, int tk){

    int pos = find_pos(ht, pin); 
    int i,found;
    found = 0;
    struct bucket* bucket;
    bucket = &ht->buckets[pos];

    while ( bucket != NULL ){
        for (i=0; i<bucket->cur_size; i++)
            if ( bucket->voters[i]->pin == pin )
                found++;
        bucket = bucket->overflow_bucket;
    }

    if ( found != 0 ){
        printf("%d already exist\n",pin);
    }else{
        Voter* v = create_voter(pin, fname, lname, tk);
        ht_insert(ht, v);
        printf("Inserted %d %s %s %d %c\n", v->pin, v->lname, v->fname, v->tk, v->status);
    }

}

void just_voted(Hash_Table* ht, int pin, struct list* list){

    int pos = find_pos(ht, pin); 
    int i,found;
    found = 0;
    struct bucket* bucket;
    bucket = &ht->buckets[pos];

    while ( bucket != NULL ){
        for (i=0; i<bucket->cur_size; i++){
            if ( bucket->voters[i]->pin == pin ){
                found++;
                if( bucket->voters[i]->status == 'N' ){
                    bucket->voters[i]->status = 'Y';
                    printf("|%d| Marked Voted\n",pin);
                    list_insert(list, bucket->voters[i]);
                }else{
                    printf("%d has already voted\n",pin);
                }
            }
        }
        bucket = bucket->overflow_bucket;
    }
    if ( found == 0 )
        printf("%d does not exist\n",pin);

}

void bv_voted(Hash_Table* ht, struct list* list, char* filename){

    FILE *fp;
    fp = fopen(filename,"r");
    if ( fp == NULL ){
        printf("%s could not be opened\n",filename);
        return;
    }

    char line[25];
    int key;
    while ( fgets(line,25,fp) != NULL ){
        if ( sscanf(line,"%d",&key) != 1 ){
            printf("Error while reading the file\n");
            return;
        }

        just_voted(ht, key, list);

    }

    fclose(fp);

}

void perc(Hash_Table* ht, struct list* list){
    
    int lsize = get_list_size(list);
    printf("percentage = %f\n",lsize/(float)ht->keys);

}


int free_everything(struct hash_table* ht){
    int i,j;
    struct bucket* temp;
    struct bucket* temp2;
    for (i=0; i<ht->size; i++){

        for(j=0; j<ht->buckets[i].cur_size; j++){ // only for the first bucket each time
            ht->to_free += sizeof(ht->buckets[i].voters[j]);
            free(ht->buckets[i].voters[j]);
        }
        ht->to_free += sizeof(ht->buckets[i].voters);
        free(ht->buckets[i].voters);
            
        // and then handle the overflow buckets
        temp = ht->buckets[i].overflow_bucket;
        while( temp != NULL ){
            for(j=0; j<temp->cur_size; j++){
                ht->to_free += sizeof(temp->voters[j]);
                free(temp->voters[j]);
            }
            ht->to_free += sizeof(temp->voters);
            free(temp->voters);
            temp2 = temp;
            temp = temp->overflow_bucket;
            ht->to_free += sizeof(temp2);
            free(temp2);
        }
    }

    ht->to_free += sizeof(ht->buckets);
    free(ht->buckets);

    ht->to_free += sizeof(ht);
    int freed = ht->to_free; // save the bytes because ht is about to get freed
    free(ht);

    return freed;
}

