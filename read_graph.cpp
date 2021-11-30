#include "differentiator.h"

node *get_add_sub(const char **s);
node *get_mul_div(const char **s);
node *get_func(const char **s);
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
    #define DERIVATIVE(oper, symbols, level, code)                                         \
    if (strncmp(*s, symbols, strlen(symbols)) == 0 && current_level == level)                             \
    {      \
        node *old_node = main_node;\
        \
        construct_node(&main_node, OPER_TYPE, oper##_OPER, nullptr, old_node);  \
        *s += strlen(symbols);                                                  \
        node *val2 = get_brackets(s);\
\
        main_node->right_node = val2;\
    }                                                                           \
                                                                                \
    else                                                                        \


node *get_add_sub(const char **s)
{
    node* val = get_mul_div(s);

    node *main_node = val;

    int current_level = 4;

    while (**s == '+' || **s == '-')
    {        
        node *old_node = main_node;

        construct_node(&main_node, OPER_TYPE, initializate_function(*s), old_node, nullptr);

        *s += 1;

        node *val2 = get_mul_div(s);

        main_node->right_node = val2;
    }

    return main_node;
}

node *get_mul_div(const char **s)
{
    node* val = get_func(s);

    node *main_node = val;

    int current_level = 3;

    while (**s == '*' || **s == '/')
    {
        node *old_node = main_node;
        
        construct_node(&main_node, OPER_TYPE, initializate_function(*s), old_node, nullptr);

        *s += 1;

        node *val2 = get_func(s);

        main_node->right_node = val2;
    }

    return main_node;
}


node* get_func(const char **s)
{
    node* val = get_brackets(s);

    node *main_node = val;
    
    int current_level = 1;

    while(true)
    {
        #include "derivative.h"
    
        //else
        {
            val = get_brackets(s);
            break;
        }
    }
    return val;
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

    else
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