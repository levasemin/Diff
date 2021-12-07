#include "differentiator.h"

node *read_graph_bracket(graph *diff_graph, const char **current_el);

void read_value(node *current_node, const char **current_el);

void get_graph(graph *diff_graph, const char *file_name)
{
    char *string;

    read_file(file_name, &string);
    
    assert(string != nullptr);

    const char * read_string = string;

    diff_graph->root_node = read_graph(diff_graph, &read_string);
    
    //DEBUG_GRAPHVIZ("graph.dot", diff_graph) 
}

#define DERIVATIVE(oper, symbols, level, diff_code, oper_code)             \
    if (strncmp(value, symbols, strlen(symbols)) == 0)                     \
    {                                                                      \
        return oper##_OPER;                                                \
    }                                                                      \
    else

float initializate_function(const char *value)
{    
    #include "derivative.h"
    
    //else
    {
        return -1;
    }
}