//
//  HashMap.h
//  TablaHashLab
//
//  Created by Matias Barrientos on 11-08-18.
//  Copyright © 2018 Matias Barrientos. All rights reserved.
//

#ifndef HashMap_h
#define HashMap_h


typedef struct HashMap HashMap;

typedef struct Pair_map {
     char * key;
     void * value;
} Pair_map;

HashMap * createMap(long capacity);

void insertMap(HashMap * table, char * key, void * value);

void eraseMap(HashMap * table, char * key);

Pair_map * searchMap(HashMap * table, char * key);

Pair_map * firstMap(HashMap * table);

Pair_map * nextMap(HashMap * table);

void enlarge(HashMap * map);

#endif /* HashMap_h */