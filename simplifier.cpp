#include "differentiator.h"

void be_simple(graph *graph)
{
    simplifier(graph->root_node);
}

int get_count_params(node *current_node)
{
    if (current_node->value == SUM_OPER || current_node->value == SUB_OPER /
        current_node->value == DIV_OPER || current_node->value == MUL_OPER /
        current_node->value == POW_OPER || current_node->value == LOG_OPER)
        {
            return 2;
        }

    else if(current_node->value == COS_OPER || current_node->value == SIN_OPER /
            current_node->value == TG_OPER  || current_node->value == CTG_OPER  /
            current_node->value == LN_OPER)
            {
                return 1;
            }
}

void check_node_nullptrs(node **)
{
    if (get_count_params(*current_node) == 2)
    {
        if ((*current_node)->left_node == nullptr && (*current_node)->right_node == nullptr)
        {
            *current_node = nullptr;
        }

        else if ((*current_node)->left_node != nullptr && (*current_node)->right_node == nullptr)
        {
            *current_node = (*current_node)->left_node;
        }

        else if ((*current_node)->left_node == nullptr && (*current_node)->right_node != nullptr)
        {
            *current_node = (*current_node)->right_node;
        }
    }

    else if (get_count_params((*current_node)) == 1)
    {
        if ((*current_node)->right_node == nullptr)
        {
            *current_node = nullptr;
        }
    }
}

void check_sum(node **current_node)
{
    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == SUM_OPER)
        {
            if ((*current_node)->left_node->value == 0)
            {
                *current_node = (*current_node)->right_node;
            }

            else if ((*current_node)->right_node->value == 0)
            {
                *current_node = (*current_node)->left_node;
            }
        }
}

void check_mul(node **current_node)
{
    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == MUL_OPER)
        {
            if ((*current_node)->left_node->value == 0 || (*current_node)->right_node->value == 0)
            {
                *current_node = nullptr;
            }

            else if ((*current_node)->left_node->value == 1)
            {
                *current_node = (*current_node)->right_node;
            }

            else if ((*current_node)->right_node->value == 1)
            {
                *current_node = (*current_node)->left_node;
            }
        }
}

void check_pow(node **current_node)
{
    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == POW_OPER)
        {
            if ((*current_node)->left_node->value == 0 || (*current_node)->right_node->value == 0)
            {
                *current_node = nullptr;
            }

            else if ((*current_node)->left_node->value == 1)
            {
                *current_node = (*current_node)->right_node;
            }

            else if ((*current_node)->right_node->value == 1)
            {
                *current_node = (*current_node)->left_node;
            }
        }
}

void check_useless_operations(node **current_node)
{
    check_sum(current_node);


     ((*current_node)->left_node)
}

void make_simple(node **current_node)
{
    check_node_nullptrs(current_node);


}

void simplifier(node *current_node)
{
    if (current_node->left_node != nullptr)
    {
        simplifier(current_node->left_node);
    }

    if (current_node->right_node != nullptr)
    {
        simplifier(current_node->right_node);
    }
}