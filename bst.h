#ifndef BST_H
#define BST_H

#include <stdbool.h>

#define Tree Node

struct print_style {
    char *tip;
    char *left;
    char *right;
    char *limb;
    char *space;
    int size;
};

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    bool dynamic;
} Node;

Node *tree_new(int data);
void inorder_tree_walk(Tree *tree);
void tree_insert(Tree **tree, int data);
bool tree_delete(Tree **tree, int data);
Node *tree_search(Node *node, int data);
Node *tree_min(Node *node);
Node *tree_max(Node *node);
Node *tree_successor(Node *node);
Node *tree_predecessor(Node *node);
void print_node(Node *node, char *indent);
void print_tree(Tree *tree);
void generate_nodes(Tree **tree, int n, int max_value);
Tree *generate_tree(int n, int max_value);
void free_tree(Tree *tree);
int max_tree_depth(Tree *tree);
double avg_tree_depth(Tree *tree);
double test_average_depth(int n, int m);

#endif //BST_H
