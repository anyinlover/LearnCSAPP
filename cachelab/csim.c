#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

int verbose = 0;
int s, E, b;
unsigned long S;
int h = 0;
int m = 0;
int e = 0;
char* infile;

struct Node {
    unsigned long key;
    struct Node* next;
};

struct Node** array;

void update_count(char label)
{
    switch (label) {
        case('h'):
            h++;
            if (verbose) printf("hit ");
            break;
        case('m'):
            m++;
            if (verbose) printf("miss ");
            break;
        case('e'):
            e++;
            if (verbose) printf("eviction ");
            break;
    }
}

void create_node(unsigned long tag, unsigned long seg)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = tag;
    node->next = array[seg];
    array[seg] = node;
}

void cal(unsigned long address)
{
    unsigned long tag = address >> (s + b);
    unsigned long seg = address << (64 - s - b) >> (64 - s);
    struct Node* ptr = array[seg];
    struct Node* old = NULL;
    int size = 0;

    if (ptr == NULL) {
        update_count('m');
        create_node(tag, seg);
    } else {
        while (ptr) {
            size++;
            if (ptr->key == tag) {
                update_count('h');
                // move to head
                if (old) {
                    old->next = ptr->next;
                    ptr->next = array[seg];
                    array[seg] = ptr;
                }
                break;
            }
            else if (ptr->next == NULL) {
                update_count('m');
                if (size == E) {
                    // need eviction
                    update_count('e');
                    free(ptr);
                    ptr = NULL;
                    if (old != NULL)
                        old->next = NULL;
                    else
                        array[seg] = NULL;
                }
                create_node(tag, seg);
                break;
            }
            old = ptr;
            ptr = ptr->next;
        }
    }
}

void sim()
{
    FILE* file = fopen(infile, "r");
    char op;
    unsigned long address;
    int size;
    while (fscanf(file, " %c %lx,%d", &op, &address, &size) > 0) {
        if (verbose && op != 'I') {
            printf("%c %lx,%d ", op, address, size);
        }
        switch (op) {
            case 'M':
                cal(address);
            case 'L':
            case 'S':
                cal(address);
                break;
        }
        if (verbose && op != 'I') {
            printf("\n");
        }
    }
    fclose(file);
}

void free_list(struct Node* head)
{
    if (head == NULL)
        return;
    free_list(head->next);
    free(head);
}


int main(int argc, char* argv[])
{
    int ch;
    while ((ch = getopt(argc, argv, "vs:E:b:t:")) != -1) {
        switch(ch)
        {
            case 'v':
                verbose = 1;
                break;
            case 's':
                s = atoi(optarg);
                S = 1 << s;
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                infile = malloc(strlen(optarg) + 1);
                strcpy(infile, optarg);
                break;
        }
    }

    array = malloc(S * sizeof(struct Node*));
    for (long i = 0; i < S; i++) {
        array[i] = NULL;
    }

    sim();
    for (long i = 0; i < S; i++) {
        free_list(array[i]);
    }
    free(array);
    
    printSummary(h, m, e);
    return 0;
}
