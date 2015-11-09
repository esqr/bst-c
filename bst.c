#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "bst.h"

#define UTF_V   "\342\224\202"  // U+2502, pionowa kreska
#define UTF_VR  "\342\224\234"  // U+251C, pionowa kreska z bolcem na boku
#define UTF_H   "\342\224\200"  // U+2500, pozioma kreska
#define UTF_UR  "\342\224\224"  // U+2514, gorna polowa pionowej kreski z bolcem na boku
#define UTF_DR  "\342\224\214"  // U+250C, dolna polowa pionowej kreski z bolcem na boku

// niektore emulatory terminala nie wyswietlaja poprawnie znakow utf8
// utf8
static struct print_style print_chars = {
        UTF_VR UTF_H,
        UTF_UR UTF_H,
        UTF_DR UTF_H,
        UTF_V " ",
        "  ",
        2 * 4 // 2 znaki po 4 bajty
};

/*
// ascii
static struct print_style print_chars = {
    "|-",
    "`-",
    ",-",
    "| ",
    "  ",
    2*1 // 2 znaki po 1 bajcie
};
*/

Node *tree_new(int data) {
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->dynamic = true;

    return node;
}

void inorder_tree_walk(Tree *tree) {
    if (!tree) return;

    inorder_tree_walk(tree->left);
    printf("%d\n", tree->data);
    inorder_tree_walk(tree->right);
}

void tree_insert(Tree **tree, int data) {
    Node *node = tree_new(data);

    if (!*tree) {
        *tree = node;
        return;
    }

    Node **tmp = tree;
    Node *parent = NULL;

    while (*tmp) {
        parent = *tmp;

        if (node->data < (*tmp)->data) {
            tmp = &(*tmp)->left; // tmp wskazuje teraz na adres wskaznika do wezla
        } else {
            tmp = &(*tmp)->right; // jw
        }
    }

    *tmp = node; // przypisanie wskaznikowi do lewego/prawego wezla adresu wezla
    node->parent = parent;

    return;
}

bool tree_delete(Tree **tree, int data) {
    Node *node = tree_search(*tree, data);

    if (!node) return false;

    Node *tmp = NULL;

    if (node->left && node->right) {
        tmp = tree_successor(node);
        node->data = tmp->data;
        tree_delete(&tmp, tmp->data);
        return true;
    } else if (node->left) {
        tmp = node->left;
    } else if (node->right) {
        tmp = node->right;
    }

    if (node->parent) {
        if (tmp) {
            tmp->parent = node->parent;
        }

        if (node == node->parent->left) {
            node->parent->left = tmp;
        }
        else {
            node->parent->right = tmp;
        }
    } else {
        if (tmp) {
            tmp->parent = NULL;
        }

        *tree = tmp;
    }

    free(node);
    return true;
}

Node *tree_search(Node *node, int data) {
    Node *tmp = node;

    while (tmp && tmp->data != data) {
        if (tmp->data > data) {
            tmp = tmp->left;
        } else {
            tmp = tmp->right;
        }
    }

    return tmp;
}

Node *tree_min(Node *node) {
    if (!node) return NULL;

    Node *tmp = node;

    while (tmp->left) {
        tmp = tmp->left;
    }

    return tmp;
}


Node *tree_max(Node *node) {
    if (!node) return NULL;

    Node *tmp = node;

    while (tmp->right) {
        tmp = tmp->right;
    }

    return tmp;
}

Node *tree_successor(Node *node) {
    if (!node) return NULL;

    if (node->right) {
        return tree_min(node->right);
    }

    Node *result = node->parent;
    Node *tmp = node;

    while (result && tmp == result->right) {
        tmp = result;
        result = result->parent;
    }

    return result;
}

Node *tree_predecessor(Node *node) {
    if (!node) return NULL;

    if (node->left) {
        return tree_max(node->left);
    }

    Node *result = node->parent;
    Node *tmp = node;

    while (result && tmp == result->left) {
        tmp = result;
        result = result->parent;
    }

    return result;
}

/*void print_node(Node *node, char *indent) {
    if (!node || !indent) return;

    char *new_indent = malloc(strlen(indent) + print_chars.size + 1); // +1 bajt na \0

    strcpy(new_indent, indent);
    printf("%s", new_indent);

    if (node->parent) {
        if (node == node->parent->right || !node->parent->right) { // sprawdzanie czy wezel jest ostatni
            printf("%s", print_chars.left);
            strcat(new_indent, print_chars.space);
        } else {
            printf("%s", print_chars.tip);
            strcat(new_indent, print_chars.limb);
        }
    }

    printf("%d\n", node->data);

    if (node->left) {
        print_node(node->left, new_indent);
    }
    if (node->right) {
        print_node(node->right, new_indent);
    }

    free(new_indent);
}*/


void print_node(Node *node, char *indent) {
    if (!node) return;

    char *left_indent = malloc(strlen(indent) + print_chars.size + 1); // +1 bajt na \0
    char *right_indent = malloc(strlen(indent) + print_chars.size + 1);
    char *branch;

    strcpy(left_indent, indent);
    strcpy(right_indent, indent);

    if (node->parent) {
        if (node == node->parent->right) {
            branch = print_chars.right;
            strcat(right_indent, print_chars.space);
            strcat(left_indent, print_chars.limb);
        } else {
            branch = print_chars.left;
            strcat(left_indent, print_chars.space);
            strcat(right_indent, print_chars.limb);
        }
    } else {
        branch = "";
    }

    print_node(node->right, right_indent);
    printf("%s%s%d\n", indent, branch, node->data);
    print_node(node->left, left_indent);

    free(left_indent);
    free(right_indent);
}

void print_tree(Tree *tree) {
    print_node(tree, "");
}

void generate_nodes(Tree **tree, int n, int max_value) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        tree_insert(tree, rand() % max_value);
    }
}

Tree *generate_tree(int n, int max_value) {
    Tree *tree = NULL;
    generate_nodes(&tree, n, max_value);
    return tree;
}

void free_tree(Tree *tree) {
    if (!tree) return;

    free_tree(tree->left);
    free_tree(tree->right);

    if (tree->dynamic) {
        free(tree);
    }
}

int max_tree_depth(Tree *tree) {
    if (!tree) return 0;

    int left_depth = max_tree_depth(tree->left);
    int right_depth = max_tree_depth(tree->right);

    return (tree->parent ? 1 : 0) + ((left_depth > right_depth) ? left_depth : right_depth);
}

static void _avg_tree_depth(Node *node, int *leaves, int *sum_of_depths, int depth) {
    if (!node || !leaves || !sum_of_depths) return;

    if (!node->left && !node->right) {
        *leaves += 1;
        *sum_of_depths += depth;
    }

    _avg_tree_depth(node->left, leaves, sum_of_depths, depth + 1);
    _avg_tree_depth(node->right, leaves, sum_of_depths, depth + 1);
}

double avg_tree_depth(Tree *tree) {
    if (!tree) return 0;

    int leaves = 0;
    int sum_of_depths = 0;

    _avg_tree_depth(tree, &leaves, &sum_of_depths, 0);

    return (double) sum_of_depths / leaves;
}

double test_average_depth(int n, int m) {
    double sum_of_depths = 0;

    for (int i = 0; i < m; i++) {
        Tree *tree = generate_tree(n, INT_MAX);
        sum_of_depths += avg_tree_depth(tree);
        free_tree(tree);
    }

    return (double) sum_of_depths / m;
}
