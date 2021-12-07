#include "differentiator.h"

int get_level(node *current_node)
{
    assert(current_node != nullptr);

    if (current_node->type == OPER_TYPE)
    {
        if (compare_floats(current_node->value, SUM_OPER) == 0 || compare_floats(current_node->value, SUB_OPER) == 0)
        {
            return 4;
        }
        
        else if  (compare_floats(current_node->value, DIV_OPER) == 0 || compare_floats(current_node->value,  MUL_OPER) == 0)
        {
            return 3; 
        }

        else if  (compare_floats(current_node->value, POW_OPER) == 0)
        {
            return 2;
        }
        
        else
        {
            return 1;
        }
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


void change_node(node *current_node, int type, float value, node *left_node, node *right_node)
{
    current_node->type       = type;
    current_node->value      = value;
    current_node->left_node  = left_node;
    current_node->right_node = right_node;
}


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


int get_count_params(node *current_node)
{
    assert(current_node != nullptr);

    if (current_node->type == OPER_TYPE)
    {
        if (compare_floats(current_node->value, SUM_OPER) == 0 || compare_floats(current_node->value, SUB_OPER) == 0  || \
            compare_floats(current_node->value, DIV_OPER) == 0 || compare_floats(current_node->value, MUL_OPER) == 0  || \
            compare_floats(current_node->value, POW_OPER) == 0 || compare_floats(current_node->value, LOG_OPER) == 0)
            {
                return 2;
            }

        else if(compare_floats(current_node->value, COS_OPER) == 0 || compare_floats(current_node->value, SIN_OPER) == 0 || \
                compare_floats(current_node->value, TG_OPER)  == 0 || compare_floats(current_node->value, CTG_OPER) == 0 || \
                compare_floats(current_node->value, LN_OPER)  == 0)
                {
                    return 1;
                }
    }

    return 0;
}