#pragma once

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int key;
    void* value;
    Color color;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct {
    Node* root;
    Node* nil;
} Map;

Map* map_create();

void map_destroy(Map* map);

void map_insert(Map* map, int key, void* value);

Node* map_find_node(Map* map, int key);

void* map_get(Map* map, int key);

void map_erase(Map* map, int key);

