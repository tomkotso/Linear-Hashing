#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "list.h"

struct sub_list_voter{

    struct voter* voter; // pointer to just one voter
    struct sub_list_voter* next;

};

struct node{

    int tk;
    int length;
    struct sub_list_voter* voters;
    struct node* next;

};
struct list{

    int size;
    struct node* head;

};

struct list* create_list(){

    struct list* list;
    list = malloc(sizeof(struct list));
    list->size = 0;
    list->head = NULL;

    return list;
}

void node_insert(Node* node,Voter* v){

    struct sub_list_voter* new_voter = malloc(sizeof(struct sub_list_voter));
    new_voter->voter = v;
    new_voter->next = node->voters;
    node->voters = new_voter;
    // insert at start
    node->length++;
}

void list_insert(List* list, Voter* v){

    struct node* temp;
    struct node* prev;
    temp = list->head;
    prev = NULL;

    while( temp != NULL ){
        if( temp->tk == v->tk){
            node_insert(temp, v);
            break;
        }

        prev = temp;
        temp = temp->next;
    }
    if( temp == NULL ){  // postal code not found, create new node
        if ( prev == NULL ){ // list is empty, create the first node
            list->head = create_node(v->tk);
            node_insert(list->head, v);
        }else{
            prev->next = create_node(v->tk);
            node_insert(prev->next, v);
        }
    }
    list->size++;
}

struct node* create_node(int tk){

    Node* node = malloc(sizeof(struct node));
    node->length = 0;
    node->tk = tk;
    node->next = NULL;
    node->voters = NULL;
    return node;

}

void print_list(List* list){

    struct node* temp;
    struct sub_list_voter* temp2;
    temp = list->head;

    while ( temp != NULL ){
        printf("tk = %d\n",temp->tk);
        temp2 = temp->voters;
        while ( temp2 != NULL){
            printf("%s me pin %d kai tk %d\n",temp2->voter->lname,temp2->voter->pin,temp2->voter->tk);
            temp2 = temp2->next;
        }
        // print voters
        printf("\n\n");
        temp = temp->next;
    }

}

int get_list_size(List* list){
    return list->size;
}

void how_many_voted(List* list){
    printf("Voted so far %d\n",list->size);
}


void voted_zip(List* list, int tk){

    struct node* temp;
    struct sub_list_voter* temp2;
    temp = list->head;

    while( temp != NULL){

        if ( temp->tk == tk ){ // found postal code
            temp2 = temp->voters; 
            printf("%d voted in postal code %d\n",temp->length,temp->tk);
            while ( temp2 != NULL ){
                printf("%d\n", temp2->voter->pin);
                temp2 = temp2->next;
            }
        }
        temp = temp->next;
    }
}

void sorted_insert(struct node** sorted, struct node* temp){

    struct node* temp2;
    struct node* prev;
    prev = (*sorted);
    temp->next = NULL;
    if ( (*sorted) == NULL ){  // first node
        (*sorted) = temp;
    }else{

        temp2 = (*sorted);
        if ( temp->length > temp2->length ){  // change head
            temp->next = (*sorted);
            (*sorted) = temp;
        }else{

            while( temp2 != NULL ){ // find the correct position to insert, according to the numbers of voters each node has ( length )                
                if ( temp->length > temp2->length )
                    break;
                prev = temp2;
                temp2 = temp2->next;
            }

            if ( temp2 == NULL ){
                prev->next = temp; // insert at end
            }else{
                prev->next = temp; // insert somewhere in the middle and tweak the pointers
                temp->next = temp2;
            }

        }
    }

}

void sorted_print(List* list){

    struct node* temp;
    temp = list->head;
    while ( temp != NULL ){
        printf("%d %d\n",temp->tk,temp->length);
        temp = temp->next;
    }
}

void sort_list(List* list){

    struct node* temp;
    struct node* sorted;
    struct node* next_node;
    sorted = NULL;

    // take each node one by one and sort-insert it in the new list

    temp = list->head;
    while ( temp != NULL ){

        next_node = temp->next;

        sorted_insert(&sorted,temp);

        temp = next_node;
    }

    list->head = sorted;

    sorted_print(list);

}

int free_list(List* list){

    struct node* temp;
    struct node* prev;
    struct sub_list_voter* temp2;
    struct sub_list_voter* prev2;

    temp = list->head;
    prev = NULL;
    int freed = 0;

    while ( temp != NULL ){

        temp2 = temp->voters;
        while ( temp2 != NULL ){
            prev2 = temp2;
            temp2 = temp2->next;
            freed += sizeof(prev2);
            free(prev2);
        }

        prev = temp;
        temp = temp->next;
        freed += sizeof(prev);
        free(prev);

    }

    freed += sizeof(list);
    free(list);
    return freed;
}
