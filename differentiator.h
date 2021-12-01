#include <stdio.h>
#include <assert.h>
#include "stack.h"

const int MAX_LEN_VALUE = 20;

#define DEBUG_GRAPHVIZ(graph_file_name, graph)                       \
    getchar();                                                       \
    dump_graph_graphviz(graph_file_name, graph);                     \
    system("dot -T png graph.dot > graph.png");                      \
    system("start graph.png");                                       \

void construct_graph(graph *graph);

bool copy_node(node **new_node, node **old_node);

bool construct_node(node **new_node, int type, int value, node *left_node = nullptr, node* right_node = nullptr);

bool copy_node(node **new_node, node **old_node);

bool copy_node_with_childrens(node **new_node, node **old_node);


void get_graph(graph *diff_graph, const char *string);

node *read_graph(graph *diff_graph, const char **current_el);

int initializate_function(const char *value);

void differentiate_graph(graph *diff_graph);

node *differentiate_node(node *current_node);

void simplifier(node *current_node);

void make_simple(node **current_node);


FILE *open_file(const char *file_name, const char *key);

size_t get_file_size(FILE *file);


size_t read_file(const char *file_name, char **string);

size_t read_file(FILE *file, char **string);


void dump_graph(const char* file_name, graph *graph);

void dump_graph(FILE *file, graph *graph);

void write_graph(FILE *file, node *current_node);

void write_node(FILE *file, node *current_node);


void dump_graph_graphviz(const char* graph_file_name, graph *graph, Stack* characteristics_object = nullptr, char *color = nullptr);

void dump_graph_graphviz(FILE* graphviz_file, graph *graph, Stack* characteristics_object = nullptr, char *color = nullptr);

void write_node_graphviz(FILE *file, node *current_node);


void fill_object_graphviz(const char *graphviz_file_name, Stack *stack, char *color);

void fill_object_graphviz(FILE *file, Stack *stack, char *color);


void print_nodes(node *my_node);


void create_png(const char *graph_file_name, const char* png_file_name);

void create_png(FILE *graph_file, FILE *png_file);
