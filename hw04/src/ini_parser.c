#include "ini_parser.h"
#include "view.h"

node *add(linked_list *list, char *string) {
    node *new = malloc(sizeof(node));
    new->string = string;
    new->next = NULL;
    if (list->first == NULL) { list->first = list->last = new; }
    else {
        list->last->next = new;
        list->last = new;
    }
    return new;
}

void clear(linked_list *list) {
    node *to_remove = list->first;
    while (to_remove) {
        node *next = to_remove->next;
        free(to_remove->string);
        free(to_remove);
        to_remove = next;
    }
    list->first = list->last = NULL;
}

node *duplicate(linked_list *list, char *string) {
    node *current = list->first;
    while (current) {
        if (strcmp(current->string, string) == 0) { return current; }
        current = current->next;
    }
    return NULL;
}
