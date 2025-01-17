#pragma once
#include "hash.h"

struct voter;

typedef struct sub_list_voter Sub_List_Voter;

typedef struct list List;

typedef struct node Node;

struct list* create_list();

void list_insert(List* list, struct voter* v);

struct node* create_node(int tk);

void print_list(List* list);

int free_list(List* list);

void how_many_voted(List* list);

int get_list_size(List* list);

void voted_zip(List* list, int tk);

void sort_list(List* list);

void sorted_insert(struct node** sorted, struct node* temp);

void sorted_print(List* list);