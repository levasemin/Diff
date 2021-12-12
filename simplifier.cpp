#include "differentiator.h"

void check_useless_operations(node **current_node);

void check_SUM(node **current_node);
void check_SUB(node **current_node);
void check_TG(node **current_node);
void check_CTG(node **current_node);
void check_COS(node **current_node);
void check_SIN(node **current_node);
void check_MUL(node **current_node);
void check_DIV(node **current_node);
void check_LN(node **current_node);
void check_LOG(node **current_node);
void check_POW(node **current_node, void (* simplify_exp)(node **) = nullptr);

void check_node_nullptrs(node **current_node);

void useless_constants(node **current_node, float type_oper, float (*operation)(float val1, float val2));

node *rec(node **main_node, node **current_node, float type_oper, float (*operation)(float val1, float val2));

float mul(float val1, float val2);
float sum(float val1, float val2);

node *rec(node **main_node, node **current_node, float type_oper, float (*operation)(float val1, float val2))
{
    if ((*current_node)->left_node->type == CONST_TYPE && (*current_node)->right_node->type == CONST_TYPE)
    {
        change_node(*current_node, CONST_TYPE, operation((*current_node)->left_node->value, (*current_node)->right_node->value));
        
        return *current_node;    
    }

    else if ((*current_node)->left_node->type == CONST_TYPE)
    {
                printf("!!L %p\n", *current_node);
                printf("!%p! %d\n", *current_node, (int)((*current_node)->left_node->type == CONST_TYPE));

        if ((*main_node)->right_node->type == CONST_TYPE)
        {
            printf("1\n");
            (*main_node)->right_node->value = operation((*main_node)->right_node->value, (*current_node)->left_node->value);
        }

        else if ((*main_node)->left_node->type == CONST_TYPE)
        {
                        printf("2\n");

            (*main_node)->left_node->value = operation((*main_node)->left_node->value, (*current_node)->left_node->value);
        }

        else
        {
            node *new_node = nullptr;

            if (*current_node != (*main_node)->right_node)
            {
                construct_node(&new_node, OPER_TYPE, type_oper, (*current_node)->right_node, (*main_node)->right_node);
            }

            else
            {
                construct_node(&new_node, OPER_TYPE, type_oper, (*current_node)->right_node, (*main_node)->left_node);
            }

            (*main_node)->left_node = (*current_node)->left_node;

            (*main_node)->right_node = new_node;
            printf("O %p\n", *main_node);
            DEBUG_GRAPHVIZ_NODE("graph.dot", (*main_node));

            return rec(main_node, &(*main_node)->right_node, type_oper, operation);
        }

        return (*current_node)->right_node;    
    }

    else if ((*current_node)->right_node->type == CONST_TYPE)
    {
                printf("R\n");

        if ((*main_node)->right_node->type == CONST_TYPE)
        {
            printf("1\n");
            (*main_node)->right_node->value = operation((*main_node)->right_node->value, (*current_node)->right_node->value);
        }

        else if ((*main_node)->left_node->type == CONST_TYPE)
        {
                        printf("2\n");

            (*main_node)->left_node->value = operation((*main_node)->left_node->value, (*current_node)->right_node->value);
        }

        else
        {
            node *new_node = nullptr;

            if (*current_node != (*main_node)->right_node)
            {
                construct_node(&new_node, OPER_TYPE, type_oper, (*current_node)->left_node, (*main_node)->right_node);
            }

            else
            {
                construct_node(&new_node, OPER_TYPE, type_oper, (*current_node)->left_node, (*main_node)->left_node);
            }

            (*main_node)->left_node = (*current_node)->right_node;

            (*main_node)->right_node = new_node;

            printf("O %p\n", *main_node);
            DEBUG_GRAPHVIZ_NODE("graph.dot", (*main_node));

            return rec(main_node, &(*main_node)->right_node, type_oper, operation);
        }

        return (*current_node)->left_node;    
    }

    if ((*current_node)->left_node->type == OPER_TYPE && compare_floats((*current_node)->left_node->value, type_oper) == 0)
    {
        node *result_node = rec(current_node, &(*current_node)->left_node, type_oper, operation);
        
        if (result_node != nullptr)
        {
                    printf("LEFT\n");

            printf("L %p\n", (*current_node)->left_node);
            (*current_node)->left_node = result_node; 
                        DEBUG_GRAPHVIZ_NODE("graph.dot", *main_node);

        }
    }
        
    if ((*current_node)->right_node->type == OPER_TYPE && compare_floats((*current_node)->right_node->value, type_oper) == 0)
    {
        printf("RIGHT\n");
        node *result_node = rec(current_node, &(*current_node)->right_node, type_oper, operation);
        
        if (result_node != nullptr)
        {
            printf("R %p\n", (*current_node)->right_node);

            (*current_node)->right_node = result_node; 
                        DEBUG_GRAPHVIZ_NODE("graph.dot", *main_node);

        }
    }

    return nullptr;
}

void useless_constants(node **current_node, float type_oper, float (*operation)(float val1, float val2))
{
    if ((*current_node)->left_node->type == CONST_TYPE && (*current_node)->right_node->type == CONST_TYPE)
    {
        change_node(*current_node, CONST_TYPE, operation((*current_node)->left_node->value, (*current_node)->right_node->value));
    }

    else if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, type_oper) == 0)
    {           
        if ((*current_node)->left_node->type  == OPER_TYPE && compare_floats((*current_node)->left_node->value, type_oper)  == 0 &&
            (*current_node)->right_node->type == OPER_TYPE && compare_floats((*current_node)->right_node->value, type_oper) == 0)
        {
            printf("all\n");
            rec(current_node, current_node, type_oper, operation);
        }
        
        else if ((*current_node)->left_node->type == OPER_TYPE && compare_floats((*current_node)->left_node->value, type_oper) == 0)
        {
                    printf("left\n");

        node *result_node = rec(current_node, &(*current_node)->left_node, type_oper, operation);
        
        if (result_node != nullptr)
        {
                    printf("LEFT\n");

            printf("LI %p\n", (*current_node)->left_node);
            (*current_node)->left_node = result_node; 
            DEBUG_GRAPHVIZ_NODE("graph.dot", *current_node);

        }
        }

        else if ((*current_node)->right_node->type == OPER_TYPE && compare_floats((*current_node)->right_node->value, type_oper) == 0)
        {
            printf("right\n");
            node *result_node = rec(current_node, &(*current_node)->right_node, type_oper, operation);
            printf("result %p\n", result_node);
            if (result_node != nullptr)
        {
            printf("RI %p\n", (*current_node)->right_node);

            (*current_node)->right_node = result_node; 
                        DEBUG_GRAPHVIZ_NODE("graph.dot", *current_node);

        }
        }
    }
}


float mul(float val1, float val2)
{
    return val1 * val2;
}


float sum(float val1, float val2)
{
    return val1 + val2;
}


void simplify_exponential_function(node **current_node)
{
    if ((*current_node)->left_node->type == EXP_TYPE && (*current_node)->right_node->type == OPER_TYPE &&
        compare_floats((*current_node)->right_node->value, LN_OPER) == 0)
    {
        *current_node = (*current_node)->right_node->right_node;
    }

    else if ((*current_node)->left_node->type  == EXP_TYPE && 
             (*current_node)->right_node->type == OPER_TYPE && compare_floats((*current_node)->right_node->value, MUL_OPER) == 0)
    {
        if ((*current_node)->right_node->right_node->type == OPER_TYPE && 
            compare_floats((*current_node)->right_node->right_node->value, LN_OPER) == 0)
        {
            node *ln_node = (*current_node)->right_node->right_node;
            
            (*current_node)->right_node = (*current_node)->right_node->left_node;
            (*current_node)->left_node  = ln_node->right_node;
        }
        
        else if ((*current_node)->right_node->left_node->type == OPER_TYPE && \ 
                compare_floats((*current_node)->right_node->left_node->value, LN_OPER) == 0)
        {
            node *ln_node = (*current_node)->right_node->left_node;

            (*current_node)->left_node  = ln_node->right_node;
            (*current_node)->right_node = (*current_node)->right_node->right_node;
        }
    }
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
    //printf("sum");
    assert(*current_node != nullptr);
    
    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, SUM_OPER) == 0)
    {
        //DEBUG_GRAPHVIZ_NODE("graph.dot", *current_node);
        if ((*current_node)->left_node->type == CONST_TYPE && (*current_node)->right_node->type == CONST_TYPE)
        {
            change_node(*current_node, CONST_TYPE, (*current_node)->left_node->value + (*current_node)->right_node->value);
        }

        else if (compare_floats((*current_node)->left_node->value, 0) == 0 && (*current_node)->left_node->type == CONST_TYPE)
        {
            *current_node = (*current_node)->right_node;
        }

        else if (compare_floats((*current_node)->right_node->value, 0) == 0 && (*current_node)->right_node->type == CONST_TYPE)
        {
            *current_node = (*current_node)->left_node;
        }

        else
        {
            useless_constants(current_node, SUM_OPER, sum);
        }
    }
}


void check_SUB(node **current_node)
{
    //printf("sub");

    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, SUB_OPER) == 0)
    {
        if ((*current_node)->left_node->type == CONST_TYPE && (*current_node)->right_node->type == CONST_TYPE)
        {
            change_node(*current_node, CONST_TYPE, (*current_node)->left_node->value - (*current_node)->right_node->value);
        }

        else if (compare_floats((*current_node)->right_node->value, 0) == 0 && (*current_node)->right_node->type == CONST_TYPE)
        {
            *current_node = (*current_node)->left_node;
        }

        else if (compare_floats((*current_node)->left_node->value, 0) == 0 && (*current_node)->left_node->type == CONST_TYPE)
        {
            node *minus_one = nullptr;

            construct_node(&minus_one, CONST_TYPE, -1);
            
            change_node(*current_node, OPER_TYPE, MUL_OPER, minus_one, (*current_node)->right_node);
        }
    }
}


void check_MUL(node **current_node)
{
    assert(*current_node != nullptr);
    
    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, MUL_OPER) == 0)
    {   
        if ((*current_node)->left_node->type == OPER_TYPE && compare_floats((*current_node)->left_node->value, DIV_OPER) == 0 &&
            compare_floats((*current_node)->left_node->left_node->value, 1) == 0)
        {
            //DEBUG_GRAPHVIZ_NODE("graph.dot", *current_node);
            change_node(*current_node, OPER_TYPE, DIV_OPER, (*current_node)->right_node, (*current_node)->left_node->right_node);
            //DEBUG_GRAPHVIZ_NODE("graph.dot", *current_node);

        }

        else if ((*current_node)->right_node->type == OPER_TYPE && compare_floats((*current_node)->right_node->value, DIV_OPER) == 0 &&
            compare_floats((*current_node)->right_node->left_node->value, 1) == 0)
        {
            change_node(*current_node, OPER_TYPE, DIV_OPER, (*current_node)->left_node, (*current_node)->right_node->right_node);
        }
        
        else if ((*current_node)->left_node->type == CONST_TYPE)
        {
            if (compare_floats((*current_node)->left_node->value, 1) == 0)
            {
                *current_node = (*current_node)->right_node;
            }

            else if (compare_floats((*current_node)->left_node->value, 0) == 0)
            {
                change_node(*current_node, CONST_TYPE, 0);
            }
            
            else
            {
                useless_constants(current_node, MUL_OPER, mul);
            }
        }

        else if ((*current_node)->right_node->type == CONST_TYPE)
        {
            if (compare_floats((*current_node)->right_node->value, 1) == 0)
            {
                *current_node = (*current_node)->left_node;
            }

            else if (compare_floats((*current_node)->right_node->value, 0) == 0)
            {
                change_node(*current_node, CONST_TYPE, 0);
            }

            else
            {
                useless_constants(current_node, MUL_OPER, mul);
            }
        }
        
        else
        {
            useless_constants(current_node, MUL_OPER, mul);
        }
    }
}


void check_DIV(node **current_node)
{
    //printf("div");

    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, DIV_OPER) == 0)
    {   
        if (compare_floats((*current_node)->left_node->value, 0) == 0 && (*current_node)->left_node->type  == CONST_TYPE)        
        {
            change_node(*current_node, CONST_TYPE, 0);
        }

        else if (compare_floats((*current_node)->right_node->value, 1) == 0 && (*current_node)->right_node->type == CONST_TYPE)
        {
            *current_node = (*current_node)->left_node;
        }

        else if ((*current_node)->left_node->type == CONST_TYPE && (*current_node)->right_node->type == CONST_TYPE)
        {
            change_node(*current_node, CONST_TYPE, (*current_node)->left_node->value / (*current_node)->right_node->value);
        }
    }
}


void check_POW(node **current_node, void (* simplify_exp)(node **))
{    
    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, POW_OPER) == 0)
    {
        if (simplify_exp != nullptr)
        {
            simplify_exp(current_node);
        }

        else if ((compare_floats((*current_node)->left_node->value, 0) == 0 && (*current_node)->left_node->type == CONST_TYPE) ||
            (compare_floats((*current_node)->right_node->value, 0) == 0 && (*current_node)->right_node->type == CONST_TYPE))
        {
            construct_node(current_node, CONST_TYPE, 1);
        }

        else if (compare_floats((*current_node)->right_node->value, 1) == 0 && (*current_node)->right_node->type == CONST_TYPE)
        {
            *current_node = (*current_node)->left_node;
        }

        else if ((*current_node)->left_node->type == OPER_TYPE && compare_floats((*current_node)->left_node->value, POW_OPER) == 0)
        {
            node *mul_degrees = nullptr;
            
            construct_node(&mul_degrees, OPER_TYPE, MUL_OPER, (*current_node)->left_node->right_node, (*current_node)->right_node);
            change_node(*current_node, OPER_TYPE, POW_OPER, (*current_node)->left_node->left_node, mul_degrees);
        }
    }
}


void check_LN(node **current_node)
{    
    //printf("ln");

    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, LN_OPER) == 0)
    { 
        if (compare_floats((*current_node)->right_node->value, 1) == 0 && (*current_node)->right_node->type == CONST_TYPE)
        {
            change_node(*current_node, CONST_TYPE, 0);
        }

        else if (compare_floats((*current_node)->right_node->value, (float)E) == 0 && (*current_node)->right_node->type == EXP_TYPE)
        {
            change_node(*current_node, CONST_TYPE, 1);
        }
    }
}


void check_LOG(node **current_node)
{    
    //printf("log");

    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, LOG_OPER) == 0)
    {   
        if ((*current_node)->right_node->type == CONST_TYPE && (*current_node)->left_node->type == CONST_TYPE)
        {
            if (compare_floats((*current_node)->right_node->value, 1) == 0 && (*current_node)->right_node->type == CONST_TYPE)
            {
                change_node(*current_node, CONST_TYPE, 0);
            }
            
            else if (compare_floats((*current_node)->right_node->value, (*current_node)->left_node->value) == 0 && 
                    (*current_node)->right_node->type == CONST_TYPE && (*current_node)->left_node->type == CONST_TYPE)
            {
                change_node(*current_node, CONST_TYPE, 1);
            }
        }
    }
}


void check_SIN(node **current_node)
{    
    //printf("sin");

    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, SIN_OPER) == 0)
    {   
        if ((*current_node)->right_node->type == CONST_TYPE)
        {
            if (compare_floats(sin((*current_node)->right_node->value), 1) == 0)
            {
                change_node(*current_node, CONST_TYPE, 1);
            }
            
            else if (compare_floats(sin((*current_node)->right_node->value), 0) == 0)
            {
                change_node(*current_node, CONST_TYPE, 0);
            }
        }
    }
}


void check_COS(node **current_node)
{    
    //printf("cos");

    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, COS_OPER) == 0)
    {   
        if ((*current_node)->right_node->type == CONST_TYPE)
        {
            if (compare_floats(cos((*current_node)->right_node->value), 1) == 0)
            {
                change_node(*current_node, CONST_TYPE, 1);
            }

            else if (compare_floats(cos((*current_node)->right_node->value), 0) == 0)
            {
                change_node(*current_node, CONST_TYPE, 0);
            }
        }
    }
}


void check_TG(node **current_node)
{    
    //printf("tg");

    assert(*current_node != nullptr);

    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, TG_OPER) == 0)
    {   
        float tg_val = sin((*current_node)->right_node->value) / cos((*current_node)->right_node->value);

        if ((*current_node)->right_node->type == CONST_TYPE)
        {
            if (compare_floats(tg_val, 1) == 0)
            {
                change_node(*current_node, CONST_TYPE, 1);
            }

            else if (compare_floats(tg_val, 0) == 0)
            {
                change_node(*current_node, CONST_TYPE, 0);
            }
        }
    }
}


void check_CTG(node **current_node)
{    
    //printf("ctg");

    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value, CTG_OPER) == 0)
    {   
        if ((*current_node)->right_node->type == CONST_TYPE)
        {
            float ctg_val = cos((*current_node)->right_node->value) / sin((*current_node)->right_node->value);
            
            if (compare_floats(ctg_val, 0) == 0)
            {
                change_node(*current_node, CONST_TYPE, 0);
            }
            
            else if (compare_floats(ctg_val, 1) == 0)
            {
                change_node(*current_node, CONST_TYPE, 1);
            }
        }
    }
}


#define DERIVATIVE(oper, symbols, level, diff_code, oper_code)  \
    check_##oper(current_node);


void check_useless_operations(node **current_node, void (* simplify_exp)(node **))
{
    if ((*current_node)->type == OPER_TYPE && compare_floats((*current_node)->value,  POW_OPER) == 0)
    {
        check_POW(current_node, simplify_exp);
    }

    else
    {
        #include "derivative.h"
    }
}


void be_simple(graph *graph, void (* simplify_exp)(node **))
{
    assert(graph != nullptr);

    make_simple_node(&graph->root_node, simplify_exp);
}


void make_simple(node **current_node, void (* simplify_exp)(node **))
{
    assert(*current_node != nullptr);

    check_node_nullptrs(current_node);

    check_useless_operations(current_node, simplify_exp);
}


void make_simple_node(node **current_node, void (* simplify_exp)(node **))
{    
    assert(*current_node != nullptr);

    make_simple(current_node, simplify_exp);

    if ((*current_node)->left_node != nullptr)
    {  
        make_simple_node(&(*current_node)->left_node, simplify_exp);

        make_simple(current_node, simplify_exp);
    }

    if ((*current_node)->right_node != nullptr)
    {
        make_simple_node(&(*current_node)->right_node, simplify_exp);

        make_simple(current_node, simplify_exp);
    }
}