#include "differentiator.h"

void differentiate_graph(graph *diff_graph)
{
    diff_graph->root_node = differentiate_node(diff_graph->root_node);

    DEBUG_GRAPHVIZ("graph.dot", diff_graph) 

}

void change_node(node *current_node, int type, int value, node *left_node, node *right_node)
{
    current_node->type       = type;
    current_node->value      = value;
    current_node->left_node  = left_node;
    current_node->right_node = right_node;
}

#define DERIVATIVE(oper, symbols, code) \
    case oper##_OPER:          \
        code                   \
        break;

node *differentiate_internal(node *current_node, node* calc_node, node *external_derivative)
{
    node* internal_derivative = differentiate_node(calc_node);

    if (internal_derivative->type == CONST_TYPE && internal_derivative->value == 1)
    {
        current_node = external_derivative;
    }

    else
    {
        construct_node(&current_node, OPER_TYPE, MUL_OPER, external_derivative, internal_derivative);
    }

    return current_node;
}

node *differentiate_node(node *current_node)
{
    assert(current_node != nullptr);
    
    if (current_node->type == CONST_TYPE)
    {
        construct_node(&current_node, CONST_TYPE, 0);
    }

    else if (current_node->type == VAR_TYPE)
    {
        construct_node(&current_node, CONST_TYPE, 1);
    }

    else
    {        
        switch(current_node->value)
        {
            #include"derivative.h"

            default:
                assert(0);
        }
    }
    
    return current_node;

}