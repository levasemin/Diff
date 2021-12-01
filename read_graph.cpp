#include "differentiator.h"

node *get_add_sub(const char **s);
node *get_mul_div(const char **s);
node *get_func(const char **s);
node *get_pow(const char **s);
node *get_brackets(const char**s);
node *get_const_var(const char **s);


void Require(const char **s, const char expected)
{
    if (**s == expected)
    {
        *s += 1;
    }

    else
    {
        printf("expected %c\n", expected);
        printf("real %s\n", *s);
        assert(0);
    }
}

node *read_graph(graph *diff_graph, const char **current_el)
{
    diff_graph->root_node = get_add_sub(current_el);

    Require(current_el, '$');

    return diff_graph->root_node;
}

#define DERIVATIVE(oper, symbols, level, code)                                      \
if (strncmp(*s, symbols, strlen(symbols)) == 0 && current_level == level)           \
{                                                                                   \
    node *old_node = main_node;                                                     \
                                                                                    \
    construct_node(&main_node, OPER_TYPE, oper##_OPER);                             \
                                                                                    \
    *s += strlen(symbols);                                                          \
                                                                                    \
    node *val2 = next_func(s);                                                      \
                                                                                    \
    if (strcmp(symbols, "log") == 0)                                                \
    {                                                                               \
        old_node = val2;                                                            \
        val2 = next_func(s);                                                        \
    }                                                                               \
                                                                                    \
    main_node->left_node = old_node;                                                \
    main_node->right_node = val2;                                                   \
                                                                                    \
    break;                                                                          \
}                                                                                   \
                                                                                    \
else                                                                                \


node *get_add_sub(const char **s)
{
    node *(*next_func)(const char **) = get_mul_div;

    node* val = next_func(s);

    node *main_node = val;

    int current_level = 4;

    while(true)
    {
        #include "derivative.h"
    
        //else
        {   
            break;
        }
    }

    return main_node;
}

node *get_mul_div(const char **s)
{
    node *(*next_func)(const char **) = get_pow;

    node* val = next_func(s);

    node *main_node = val;

    int current_level = 3;

    while(true)
    {
        #include "derivative.h"
    
        //else
        {
            break;
        }
    }
    return main_node;
}

node *get_pow(const char **s)
{
    node *(*next_func)(const char **) = get_func;

    node* val = next_func(s);

    node *main_node = val;

    int current_level = 2;

    while(true)
    {
        #include "derivative.h"
    
        //else
        {
            break;
        }
    }
    return main_node;
}


node* get_func(const char **s)
{        
    node *(*next_func)(const char **) = get_brackets;

    node *main_node = nullptr;
    
    int current_level = 1;

    while(true)
    {
        #include "derivative.h"
    
        //else
        {
            main_node = next_func(s);
            break;
        }
    }

    return main_node;
}

node *get_brackets(const char **s)
{

    if (**s == '(')
    {
        *s += 1;

        node *val = get_add_sub(s);
        Require(s, ')');

        return val;
    }

    else 
    {
        return get_const_var(s);
    }
}

node *get_const_var(const char **s)
{
    int val = 0;

    const char *start = *s;

    node *new_node = nullptr;
    
    if ('0' <= **s && **s <= '9')
    {        
        while ('0' <= **s && **s <= '9')
        {
            val = val * 10 + **s - '0';
            *s += 1;
        }
        
        construct_node(&new_node, CONST_TYPE, val);
    }

    else if ('a' <= **s && **s <= 'z')
    {
        construct_node(&new_node, VAR_TYPE, **s - 'a');

        *s += 1;
    }    

    if (start == *s)
    {
        assert(0);
    }
    
    return new_node;
}