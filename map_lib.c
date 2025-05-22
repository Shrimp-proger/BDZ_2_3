#include <stdio.h>
#include <stdlib.h>
#include "map_lib.h"

Node* createNIL() {
    Node* nil = (Node*)malloc(sizeof(Node));
    nil->color = BLACK;
    nil->left = nil->right = nil->parent = nil;
    return nil;
}

Map* map_create() {
    Map* map = (Map*)malloc(sizeof(Map));
    map->nil = createNIL();
    map->root = map->nil;
    return map;
}

void destroyTree(Node* node, Node* nil) {
    if (node == nil) return;
    destroyTree(node->left, nil);
    destroyTree(node->right, nil);
    free(node);
}

void map_destroy(Map* map) {
    destroyTree(map->root, map->nil);
    free(map->nil);
    free(map);
}

void left_rotate(Map* map, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != map->nil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == map->nil)
        map->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void right_rotate(Map* map, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != map->nil)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == map->nil)
        map->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void insert_fixup(Map* map, Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(map, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(map, z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(map, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(map, z->parent->parent);
            }
        }
    }
    map->root->color = BLACK;
}

void map_insert(Map* map, int key, void* value) {
    Node* z = (Node*)malloc(sizeof(Node));
    z->key = key;
    z->value = value;
    z->color = RED;
    z->parent = z->left = z->right = map->nil;

    Node* y = map->nil;
    Node* x = map->root;

    while (x != map->nil) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y == map->nil)
        map->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    insert_fixup(map, z);
}

Node* map_find_node(Map* map, int key) {
    Node* x = map->root;
    while (x != map->nil) {
        if (key < x->key)
            x = x->left;
        else if (x->key < key)
            x = x->right;
        else
            return x;
    }
    return map->nil;
}

void* map_get(Map* map, int key) {
    Node* node = map_find_node(map, key);
    return (node != map->nil) ? node->value : NULL;
}

Node* minimum(Node* node, Node* nil) {
    while (node->left != nil)
        node = node->left;
    return node;
}


void transplant(Map* map, Node* u, Node* v) {
    if (u->parent == map->nil)
        map->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void delete_fixup(Map* map, Node* x) {
    while (x != map->root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(map, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(map, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(map, x->parent);
                x = map->root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(map, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(map, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(map, x->parent);
                x = map->root;
            }
        }
    }
    x->color = BLACK;
}

void map_erase(Map* map, int key) {
    Node* z = map_find_node(map, key);
    if (z == map->nil) return;

    Node* y = z;
    Node* x;
    Color y_original_color = y->color;

    if (z->left == map->nil) {
        x = z->right;
        transplant(map, z, z->right);
    } else if (z->right == map->nil) {
        x = z->left;
        transplant(map, z, z->left);
    } else {
        y = minimum(z->right, map->nil);
        y_original_color = y->color;
        x = y->right;
        transplant(map, y, x);
        y->right = z->right;
        y->right->parent = y;
        transplant(map, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == BLACK)
        delete_fixup(map, x);
    
    free(z);
}
