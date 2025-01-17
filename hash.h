#pragma once
#include "list.h"

struct list;

typedef struct voter Voter;

typedef struct hash_table Hash_Table;

typedef struct bucket Bucket;

struct voter {
    int pin;
    int tk; // postal code
    char lname[25];
    char fname[25];
    char status;
};

Voter* create_voter (int pin, char* fname, char* lname, int tk);

void read_voters(Hash_Table* ht, char* registeredvoters);

Hash_Table* create_table(int m,int b);

int free_everything(struct hash_table* ht);

void ht_insert(Hash_Table* ht, Voter* v);

int hash_f(int pin, int round, int m);

int find_pos(Hash_Table* ht, int pin);

void print_bucket(Bucket* bucket);

void print_ht(Hash_Table* ht);

void split_bucket(Hash_Table* ht);

void find_pin(Hash_Table* ht, int pin);

void insert_voter(Hash_Table* ht, int pin, char* fname, char* lname, int tk);

void just_voted(Hash_Table* ht, int pin, struct list* list);

void bv_voted(Hash_Table* ht, struct list* list, char* filename);

void perc(Hash_Table* ht, struct list* list);
