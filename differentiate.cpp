#include "differentiator.h"

node *differentiate_internal(node* calc_node, node *external_derivative);


void differentiate_graph(graph *diff_graph)
{
    diff_graph->root_node = differentiate_node(diff_graph->root_node);

    //DEBUG_GRAPHVIZ("graph.dot", diff_graph) 

}


void change_node(node *current_node, int type, float value, node *left_node, node *right_node)
{
    current_node->type       = type;
    current_node->value      = value;
    current_node->left_node  = left_node;
    current_node->right_node = right_node;
}


#define DERIVATIVE(oper, symbols, level, diff_code, oper_code) \
    if (current_node->value == oper##_OPER)                                          \
        {                                                                       \
            diff_code                                                       \
        }                                              \
    else



node *differentiate_internal(node* calc_node, node *external_derivative)
{
    node *current_node = nullptr;

    node* internal_derivative = differentiate_node(calc_node);
    
    construct_node(&current_node, OPER_TYPE, MUL_OPER);

    copy_node_with_childrens(&current_node->left_node,  &external_derivative);

    copy_node_with_childrens(&current_node->right_node, &internal_derivative);

    return current_node;
}


node *differentiate_node(node *current_node)
{
    assert(current_node != nullptr);
    
    if (current_node->type == CONST_TYPE || current_node->type == EXP_TYPE)
    {
        construct_node(&current_node, CONST_TYPE, 0);
    }

    else if (current_node->type == VAR_TYPE)
    {
        construct_node(&current_node, CONST_TYPE, 1);
    }

    else
    {   
        #include"derivative.h"
        //else
        {
            assert(0);
        }
    }
    
    return current_node;
}