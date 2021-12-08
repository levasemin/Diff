#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "stack.h"
#include "graph_operations.h"
#include "files_work.h"

const int MAX_LEN_VALUE = 20;

const int SIGN_COUNT = 3;

const char E = 'e' -'a';

#define DEBUG_GRAPHVIZ_GRAPH(graph_file_name, graph)                 \
    getchar();                                                       \
    dump_graph_graphviz(graph_file_name, graph);                     \
    system("dot -T png graph.dot > graph.png");                      \
    system("start graph.png");                                       \


void DEBUG_GRAPHVIZ_NODE(char *graph_file_name, node *graph_node);

void get_graph(graph *diff_graph, const char *string);

void differentiate_graph(graph *diff_graph, int num, const char *expression_file_name = nullptr, const char *graph_file_name = nullptr, const char *png_file_name = nullptr);

node *differentiate_node(node *current_node, FILE *expression_file = nullptr);


void teilor(const char*name_dump_file, graph *diff_graph, float locality, int term);

void teilor(FILE *dump_file, graph *diff_graph, float locality, int term);


void be_simple(graph *graph);

void make_simple(node **current_node);