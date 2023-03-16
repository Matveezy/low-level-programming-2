#include "struct.h"
#include "mm_malloc.h"

struct map_entry *create_map_entry_bool(char *alias, char *attrname, bool predicate) {
    struct map_entry *entry = malloc(sizeof(struct map_entry));
    entry->alias = alias;
    entry->attrname = attrname;
    entry->predicate = predicate;
    return entry;
}

struct map_entry *create_map_entry_char(char *alias, char *attrname, char *predicate) {
    struct map_entry *entry = malloc(sizeof(struct map_entry));
    entry->alias = alias;
    entry->attrname = attrname;
    entry->char_predicate = predicate;
    return entry;
}