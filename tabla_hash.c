#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct Pair_map Pair_map;
typedef struct HashMap HashMap;
int enlarge_called=0;


struct HashMap {
    Pair_map ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair_map * createPair( char * key,  void * value) {
    Pair_map * new = (Pair_map *)malloc(sizeof(Pair_map));
    new->key = strdup(key);
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal_map(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void enlarge(HashMap * map);

void insertMap(HashMap * map, char * key, void * value) {
    long idx = hash(key, map->capacity);
    
    while (map->buckets[idx] != NULL && map->buckets[idx]->key != NULL){
        if(is_equal_map(map->buckets[idx]->key, key) == 1) return; //el dato ya existe
        idx = (idx + 1) % map->capacity;
    }
    
    if (map->buckets[idx] != NULL) {
        map->buckets[idx]->key = key;
        map->buckets[idx]->value = value;
    } else 
        map->buckets[idx] = createPair(key, value);
    
    
    map->size += 1;
    
    if ((double)map->size/(double)map->capacity > 0.75) enlarge(map);
}

void enlarge(HashMap * map) {
    map->capacity *= 2;

    Pair_map ** oldBucket = map->buckets;
    
    long oldCapacity = map->capacity/2;

    map->buckets = (Pair_map **)malloc(sizeof(Pair_map *) * map->capacity);
    memset(map->buckets, 0, map->capacity * sizeof(Pair_map *));
    map->size = 0;
    long i;
    
    for (i = 0; i < oldCapacity; i++) {
        if (oldBucket[i] != NULL) {
            if (oldBucket[i]->value != NULL) {
                insertMap(map, oldBucket[i]->key, oldBucket[i]->value);
            } else {
                free(oldBucket[i]);
            }
        }
    }
    
    
    free(oldBucket);
}


HashMap * createMap(long capacity) {
    HashMap * new = (HashMap *)malloc(sizeof(HashMap));
    new->buckets = (Pair_map **) calloc (capacity,sizeof(Pair_map *));
    new->size = 0;
    new->capacity = capacity;
    new->current = -1;
    return new;
}

void eraseMap(HashMap * map,  char * key) {    
    long idx = hash(key, map->capacity);
    while (map->buckets[idx] != NULL && is_equal_map(map->buckets[idx]->key, key) == 0)
        idx = (idx + 1) % map->capacity;
    
    if (map->buckets[idx] == NULL) return;

    map->buckets[idx]->key = NULL;

    map->size--;

}

Pair_map * searchMap(HashMap * map,  char * key) {   
    long idx = hash(key, map->capacity);
    while (map->buckets[idx] != NULL && is_equal_map(map->buckets[idx]->key, key) == 0) 
        idx = (idx + 1) % map->capacity;
    
    if (map->buckets[idx] == NULL || map->buckets[idx]->value == NULL) return NULL;
    
    map->current = idx;
    
    return (void *)map->buckets[idx];
}

Pair_map * firstMap(HashMap * map) {
    if (map == NULL || map->buckets == NULL) return NULL;
    
    long i;
    
    for (i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair_map * nextMap(HashMap * map) {
    if (map == NULL || map->buckets == NULL) return NULL;
    
    long i;
    
    for (i = (map->current + 1); i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    
    return NULL;
}