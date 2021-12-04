#include "differentiator.h"

void be_simple(graph *graph)
{
    assert(graph != nullptr);

    simplifier(&graph->root_node);
}

int get_count_params(node *current_node)
{
    assert(current_node != nullptr);

    if (current_node->type == OPER_TYPE)
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

    return 0;
}


int get_level(node *current_node)
{
    assert(current_node != nullptr);

    if (current_node->type == OPER_TYPE)
    {
        if (current_node->value == SUM_OPER || current_node->value == SUB_OPER)
        {
            return 4;
        }
        
        else if  (current_node->value == DIV_OPER || current_node->value == MUL_OPER)
        {
            return 3; 
        }

        else if  (current_node->value == POW_OPER)
        {
            return 2;
        }
        
        else
        {
            return 1;
        }
    }

    return 0;
}

void check_node_nullptrs(node **current_node)
{
    assert(*current_node != nullptr);

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


void check_SUM(node **current_node)
{
    assert(*current_node != nullptr);
    
    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == SUM_OPER)
        {
           //printf("sum\n");
            if ((*current_node)->left_node->type == CONST_TYPE && (*current_node)->right_node->type == CONST_TYPE)
            {
                change_node(current_node, CONST_TYPE, (*current_node)->left_node->value + (*current_node)->right_node->value);
            }

            else if ((*current_node)->left_node->value == 0 && (*current_node)->left_node->type == CONST_TYPE)
            {
                *current_node = (*current_node)->right_node;
            }

            else if ((*current_node)->right_node->value == 0 && (*current_node)->right_node->type == CONST_TYPE)
            {
                *current_node = (*current_node)->left_node;
            }
        }
}


void check_SUB(node **current_node)
{
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == SUB_OPER)
        {
           //printf("sub\n");

            if ((*current_node)->left_node->type == CONST_TYPE && (*current_node)->right_node->type == CONST_TYPE)
            {
                change_node(current_node, CONST_TYPE, (*current_node)->left_node->value - (*current_node)->right_node->value);
            }

            else if ((*current_node)->left_node->value == 0 && (*current_node)->left_node->type == CONST_TYPE)
            {
                *current_node = (*current_node)->right_node;
                (*current_node)->value *= -1;
            }

            else if ((*current_node)->right_node->value == 0 && (*current_node)->right_node->type == CONST_TYPE)
            {
                *current_node = (*current_node)->left_node;
            }
        }
}


void check_MUL(node **current_node)
{
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == MUL_OPER)
        {   
           //printf("mul");

            if ((*current_node)->left_node->type == CONST_TYPE && (*current_node)->right_node->type == CONST_TYPE)
            {
                change_node(current_node, CONST_TYPE, (*current_node)->left_node->value * (*current_node)->right_node->value);
            }

            else if ((*current_node)->left_node->type == CONST_TYPE)
            {
                if ((*current_node)->left_node->value == 1)
                {
                    *current_node = (*current_node)->right_node;
                }

                else if ((*current_node)->left_node->value == 0)
                {
                    change_node(current_node, CONST_TYPE, 0);
                }
            }

            else if ((*current_node)->right_node->type == CONST_TYPE)
            {
                if ((*current_node)->right_node->value == 1)
                {
                    *current_node = (*current_node)->left_node;
                }

                else if ((*current_node)->right_node->value == 0)
                {
                    change_node(current_node, CONST_TYPE, 0);
                }
            }
        }
}


void check_DIV(node **current_node)
{
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == DIV_OPER)
        {   //printf("div\n");

            if ((*current_node)->left_node->value == 0 && (*current_node)->left_node->type == CONST_TYPE || 
               (*current_node)->right_node->value == 0 && (*current_node)->right_node->type == CONST_TYPE)
            {
                change_node(current_node, CONST_TYPE, 0);
            }

            else if ((*current_node)->right_node->value == 1 && (*current_node)->right_node->type == CONST_TYPE)
            {
                *current_node = (*current_node)->left_node;
            }
        }
}


void check_POW(node **current_node)
{
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == POW_OPER)
        {   //printf("pow\n");

            if ((*current_node)->left_node->value == 0 && (*current_node)->left_node->type == CONST_TYPE ||
               (*current_node)->right_node->value == 0 && (*current_node)->right_node->type == CONST_TYPE)
            {
                construct_node(current_node, CONST_TYPE, 1);
            }

            else if ((*current_node)->right_node->value == 1 && (*current_node)->right_node->type == CONST_TYPE)
            {
                *current_node = (*current_node)->left_node;
            }
        }
}


void check_LN(node **current_node)
{
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == LN_OPER)
        {   //printf("ln\n");

            if ((*current_node)->right_node->value == 1 && (*current_node)->right_node->type == CONST_TYPE)
            {
                change_node(current_node, CONST_TYPE, 0);
            }
        }
}


void check_LOG(node **current_node)
{
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == LOG_OPER)
        {   //printf("log\n");

            if ((*current_node)->right_node->type == CONST_TYPE && (*current_node)->left_node->type == CONST_TYPE)
            {
                if ((*current_node)->right_node->value == 1 && (*current_node)->right_node->type == CONST_TYPE)
                {
                    change_node(current_node, CONST_TYPE, 0);
                }
                
                else if ((*current_node)->right_node->value == (*current_node)->left_node->value && 
                         (*current_node)->right_node->type == CONST_TYPE && (*current_node)->left_node->type == CONST_TYPE)
                {
                    change_node(current_node, CONST_TYPE, 1);
                }
            }
        }
}


void check_SIN(node **current_node)
{
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == SIN_OPER)
        {   //printf("sin\n");

            if ((*current_node)->right_node->value == 0 && (*current_node)->right_node->type == CONST_TYPE)
            {
                change_node(current_node, CONST_TYPE, 0);
            }
        }
}


void check_COS(node **current_node)
{
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == COS_OPER)
        {   //printf("cos\n");

            if ((*current_node)->right_node->value == 0 && (*current_node)->right_node->type == CONST_TYPE)
            {
                change_node(current_node, CONST_TYPE, 1);
            }
        }
}


void check_TG(node **current_node)
{
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && (*current_node)->value == TG_OPER)
        {   //printf("tg\n");

            if ((*current_node)->right_node->value == 0 && (*current_node)->right_node->type == CONST_TYPE)
            {
                change_node(current_node, CONST_TYPE, 0);
            }
        }
}


void check_CTG(node **current_node)
{
    return;
}


#define DERIVATIVE(oper, symbols, level, code) \
    check_##oper(current_node);


void check_useless_operations(node **current_node)
{
    #include "derivative.h"
}


void make_simple(node **current_node)
{
    assert(*current_node != nullptr);

    check_node_nullptrs(current_node);

   //printf("nullptrs_was_checked\n");

    check_useless_operations(current_node);
}


void simplifier(node **current_node)
{    
    assert(*current_node != nullptr);

    make_simple(current_node);

    if ((*current_node)->left_node != nullptr)
    {  
        simplifier(&(*current_node)->left_node);

        make_simple(current_node);
    }

    if ((*current_node)->right_node != nullptr)
    {
        simplifier(&(*current_node)->right_node);

        make_simple(current_node);
    }
}