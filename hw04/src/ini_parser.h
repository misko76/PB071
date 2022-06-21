#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <stdbool.h>

typedef struct node {
    char *string;
    struct node *next;
    char *special_mode;
} node;

typedef struct linked_list {
    node *first;
    node *last;
    char *special_mode;
} linked_list;

node *add(linked_list *list, char *string);

void clear(linked_list *list);

node *duplicate(linked_list *list, char *string);

#endif
