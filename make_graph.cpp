#include "differentiator.h"

node *read_graph_bracket(graph *diff_graph, const char **current_el);

void read_value(node *current_node, const char **current_el);

void construct_graph(graph *graph)
{
    assert(graph != nullptr);
    
    construct_node(&graph->root_node, 0, 0);
    
    graph->count_nodes = 1;
}

bool copy_node_with_childrens(node **new_node, node **old_node) 
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

bool copy_node(node **new_node, node **old_node)
{
    assert(new_node != nullptr);

    *new_node = (node *)calloc(1, sizeof(node));
    
    assert(*new_node != nullptr);

    (*new_node)->value      = (*old_node)->value;
    (*new_node)->type       = (*old_node)->type;
    
    (*new_node)->left_node  = (*old_node)->left_node;
    (*new_node)->right_node = (*old_node)->right_node;

    return (bool)*new_node;
}

bool construct_node(node **new_node, int type, int value, node *left_node, node* right_node)
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

bool change_node(node **new_node, int type, int value, node *left_node, node* right_node)
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

    diff_graph->root_node = read_graph(diff_graph, (const char **)&string);
    
    //DEBUG_GRAPHVIZ("graph.dot", diff_graph) 
}


node *read_graph_bracket(graph *diff_graph, const char **current_el)
{
    node *new_node = nullptr;
    
    while (**current_el == ' ')
    {
        *current_el += 1; 
    }

    if (**current_el == '(')
    {
        construct_node(&new_node, 0, 0);
        
        *current_el += 1;
    }

    else
    {
        return NULL;
    }

    while (**current_el == ' ')
    {
        *current_el += 1; 
    }

    if (**current_el == '(')
    {
        new_node->left_node = read_graph_bracket(diff_graph, current_el);

        *current_el += 1;
    }

    read_value(new_node, current_el);
    
    if (**current_el == '(')
    {
        new_node->right_node = read_graph_bracket(diff_graph, current_el);
        *current_el += 1;

    }   
    
    if (**current_el != ')')
    {
        return NULL;
    }
    
    return new_node;
}

void read_value(node *current_node, const char **current_el)
{        
    if (**current_el >= '0' && **current_el <= '9')
    {
        current_node->type = CONST_TYPE;

        int value = 0;

        for (int i = 0; **current_el != '(' && **current_el != ')'; ++i)
        {
            value = value * 10 + (**current_el - '0');
            
            *current_el += 1;
        }

        current_node->value = value;
    }

    else
    {
        current_node->type = OPER_TYPE;

        char value[MAX_LEN_VALUE] = {};

        for (int i = 0; **current_el != '(' && **current_el != ')'; ++i)
        {
            value[i] = **current_el;

            *current_el += 1; 
        }

        if ((current_node->value = initializate_function(value)) == -1)
        {
            current_node->type  = VAR_TYPE;

            current_node->value = *value - 'a';
        }
    }
}

#define DERIVATIVE(oper, symbols, level, diff_code, oper_code)             \
    if (strncmp(value, symbols, strlen(symbols)) == 0)                     \
    {                                                                      \
        return oper##_OPER;                                                \
    }                                                                      \

int initializate_function(const char *value)
{    
    #include "derivative.h"
    
    return -1;
}