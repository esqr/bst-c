#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "bst.h"

enum return_codes {
    QUIT,
    SUCCESS,
    ERR_EMPTY,
    ERR_ELEMENT_DOESNT_EXISTS
};

void clear_input() {
    char tmp;
    while ((tmp = getchar()) != '\n' && tmp != EOF); // ignorowanie pozostalych znakow
}

bool is_empty(Tree **tree) {
    return *tree == NULL;
}

int menu_print_help();

int menu_quit(Tree **tree) {
    free_tree(*tree);
    return QUIT;
}

int menu_reset_tree(Tree **tree) {
    if (!is_empty(tree)) {
        free_tree(*tree);
        *tree = NULL;
    }

    printf("Tree has been reset\n");
    return SUCCESS;
}

int menu_print_tree(Tree **tree) {
    if (is_empty(tree)) return ERR_EMPTY;

    print_tree(*tree);
    return SUCCESS;
}

int menu_tree_insert(Tree **tree) {
    printf("Enter data to insert: ");

    int data;
    scanf("%d", &data);
    clear_input();

    tree_insert(tree, data);
    return SUCCESS;
}

int menu_tree_delete(Tree **tree) {
    if (is_empty(tree)) return ERR_EMPTY;

    printf("Enter data to delete: ");

    int data;
    scanf("%d", &data);
    clear_input();

    if (tree_delete(tree, data)) return SUCCESS;
    else return ERR_ELEMENT_DOESNT_EXISTS;
}

int menu_tree_search(Tree **tree) {
    if (is_empty(tree)) return ERR_EMPTY;

    printf("Enter data to search: ");

    int data;
    scanf("%d", &data);
    clear_input();

    Node *node = tree_search(*tree, data);
    printf("Node %s\n", node ? "found" : "not found");
    return SUCCESS;
}

int menu_tree_successor(Tree **tree) {
    if (is_empty(tree)) return ERR_EMPTY;

    printf("Enter data to find successor: ");

    int data;
    scanf("%d", &data);
    clear_input();

    Node *node = tree_search(*tree, data);

    if (node) {
        Node *successor = tree_successor(node);
        if (successor) {
            printf("Successor of %d is %d\n", data, successor->data);
        } else {
            printf("There is no successor of %d\n", data);
        }
    } else {
        return ERR_ELEMENT_DOESNT_EXISTS;
    }

    return SUCCESS;
}

int menu_tree_predecessor(Tree **tree) {
    if (is_empty(tree)) return ERR_EMPTY;

    printf("Enter data to find predecessor: ");

    int data;
    scanf("%d", &data);
    clear_input();

    Node *node = tree_search(*tree, data);

    if (node) {
        Node *predecessor = tree_predecessor(node);

        if (predecessor) {
            printf("Predecessor of %d is %d\n", data, predecessor->data);
        } else {
            printf("There is no predecessor of %d\n", data);
        }
    } else {
        return ERR_ELEMENT_DOESNT_EXISTS;
    }

    return SUCCESS;
}

int menu_inorder_tree_walk(Tree **tree) {
    if (is_empty(tree)) return ERR_EMPTY;

    inorder_tree_walk(*tree);
    return SUCCESS;
}

int menu_generate_nodes(Tree **tree) {
    int max_value;
    int n;

    printf("Enter number of nodes to generate: ");

    scanf("%d", &n);
    clear_input();

    printf("Enter maximum value: ");

    scanf("%d", &max_value);
    clear_input();

    generate_nodes(tree, n, max_value);
    return SUCCESS;
}

int menu_generate_tree(Tree **tree) {
    free_tree(*tree);
    *tree = NULL;

    int max_value;
    int n;

    printf("Enter number of nodes to generate: ");

    scanf("%d", &n);
    clear_input();

    printf("Enter maximum value: ");

    scanf("%d", &max_value);
    clear_input();

    generate_nodes(tree, n, max_value);
    return SUCCESS;
}

int menu_test_average_depth() {
    int n;
    int m;

    printf("Enter tree size (number of nodes): ");

    scanf("%d", &n);
    clear_input();

    printf("Enter number of trees: ");

    scanf("%d", &m);
    clear_input();

    double average_depth = test_average_depth(n, m);
    printf("Average depth: %f\n", average_depth);
    return SUCCESS;
}

struct {
    char val;
    char *help_message;
    int (*function)();
} menu_items[] = {
    {'h', "print this help message", menu_print_help},
    {'q', "quit program", menu_quit},
    {'r', "reset tree", menu_reset_tree},
    {'p', "print tree", menu_print_tree},
    {'i', "insert element to tree", menu_tree_insert},
    {'d', "delete element from tree", menu_tree_delete},
    {'s', "search tree for element", menu_tree_search},
    {'S', "print successor of element", menu_tree_successor},
    {'P', "print predecessor of element", menu_tree_predecessor},
    {'w', "inorder tree walk", menu_inorder_tree_walk},
    {'g', "add n random elements to tree", menu_generate_nodes},
    {'G', "generate random n-element tree", menu_generate_tree},
    {'t', "test average tree depth", menu_test_average_depth}
};

int menu_print_help() {
    printf("Help:\n");

    for (int i = 0; i < (int) (sizeof (menu_items) / sizeof (menu_items[0])); i++) {
        printf("  %c - %s\n", menu_items[i].val, menu_items[i].help_message);
    }

    return 1;
}

void handle_error(int err) {
    switch (err) {
        case ERR_EMPTY:
            printf("Tree is empty\n");
            break;
        case ERR_ELEMENT_DOESNT_EXISTS:
            printf("There is no node with this data\n");
            break;
        default:
            printf("Undefined error\n");
            break;
    }
}

void menu(Tree **tree) {
    int result = SUCCESS;
    char val;

    do {
        printf("Command (h for help): ");
        scanf("%c", &val);
        clear_input();

        bool valid_command = false;
        
        for (int i = 0; i < (int) (sizeof (menu_items) / sizeof (menu_items[0])); i++) {
            if (val == menu_items[i].val) {
                result = menu_items[i].function(tree);
                valid_command = true;
                break;
            }
        }

        if (!valid_command) {
            printf("%c: unknown command\n", val);
        } else if (result > SUCCESS) {
            handle_error(result);
        }
    } while (result);
}

int main() {
    Tree *tree = NULL;
    menu(&tree);
    return 0;
}
