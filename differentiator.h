#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "stack.h"
#include "graph_operations.h"
#include "files_work.h"

const int MAX_LEN_VALUE = 20;

const int SIGN_COUNT = 3;

const char E = 'e' -'a';

void DEBUG_GRAPHVIZ_GRAPH(char *graph_file_name, graph *graph);

void DEBUG_GRAPHVIZ_NODE(char *graph_file_name, node *graph_node);

void get_graph(graph *diff_graph, const char *string);

void differentiate_graph(graph *diff_graph, int num, const char *dump_file_name = nullptr, const char *graph_file_name = nullptr, const char *png_file_name = nullptr);

node *differentiate_node(node *current_node, FILE *dump_file = nullptr, int num_derivative = 1);


void teilor(const char*name_dump_file, graph *diff_graph, float locality, int term);

void teilor(FILE *dump_file, graph *diff_graph, float locality, int term);


void be_simple(graph *graph, void (*simplify_exp)(node **) = nullptr);

void make_simple_node(node **current_node, void (*simplify_exp)(node **) = nullptr);

void simplify_exponential_function(node **current_node);