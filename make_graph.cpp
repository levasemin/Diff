#include "differentiator.h"

node *read_graph_bracket(graph *diff_graph, const char **current_el);

void read_value(node *current_node, const char **current_el);

int compare_floats(float val1, float val2)
{
    if (val1 - val2 > 0.001)
    {
        return 1;
    }

    else if (val1 - val2 < -0.001)
    {
        return -1;
    }
    
    else
    {
        return 0;
    }
}

void construct_graph(graph *graph)
{
    assert(graph != nullptr);
    
    construct_node(&graph->root_node, 0, 0);
    
    graph->count_nodes = 1;
}

void copy_node_with_childrens(node **new_node, node **old_node) 
{
    copy_node(new_node, old_node);

    if ((*old_node)->left_node != nullptr)
    {
        copy_node_with_childrens(&(*new_node)->left_node,  &(*old_node)->left_node);
    }

    if ((*old_node)->right_node != nullptr)
    {
        copy_node_with_childrens(&(*new_node)->right_node, &(*old_node)->right_node);
    }
}

void copy_node(node **new_node, node **old_node)
{
    assert(new_node != nullptr);

    *new_node = (node *)calloc(1, sizeof(node));
    
    assert(*new_node != nullptr);

    (*new_node)->value      = (*old_node)->value;
    (*new_node)->type       = (*old_node)->type;
    
    (*new_node)->left_node  = (*old_node)->left_node;
    (*new_node)->right_node = (*old_node)->right_node;
}

bool construct_node(node **new_node, int type, float value, node *left_node, node* right_node)
{
    assert(new_node != nullptr);

    *new_node = (node *)calloc(1, sizeof(node));
    
    assert(*new_node != nullptr);

    (*new_node)->value = value;

    (*new_node)->type           = type;
    (*new_node)->left_node      = left_node;
    (*new_node)->right_node     = right_node;

    return (bool)*new_node;
}

bool change_node(node **new_node, int type, float value, node *left_node, node* right_node)
{
    assert(new_node != nullptr);
    
    (*new_node)->value = value;

    (*new_node)->type           = type;
    (*new_node)->left_node      = left_node;
    (*new_node)->right_node     = right_node;

    return (bool)*new_node;
}

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