#include "struct.h"

struct map_entry *create_map(char *attrname, struct constant *constant) {
    struct map_entry *entry = malloc(sizeof(struct map_entry));
    entry->key = attrname;
    entry->constant = constant;
    entry->next = NULL;
    entry->prev = NULL;
    return entry;
}

struct map_entry *push_back_to_map(struct map_entry *previous, struct map_entry *new) {
    previous->next = new;
    new->prev = previous;
    return new;
}

