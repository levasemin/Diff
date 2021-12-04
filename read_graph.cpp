#include "differentiator.h"

void exponential_function(node **main_node);
bool get_add_sub(const char **s, node **main_node);
bool get_mul_div(const char **s, node **main_node);
bool get_func(const char **s, node **main_node);
bool get_pow(const char **s, node **main_node);
bool get_brackets(const char **s, node **main_node);
bool get_const_var(const char **s, node **main_node);


void Require(const char **s, const char expected)
{
    if (**s == expected)
    {
        *s += 1;
    }

    else
    {
        printf("expected %c\n", expected);
        printf("real %c\n", **s);

        assert(0);
    }
}

node *read_graph(graph *diff_graph, const char **current_el)
{
    bool is_const = false;
    
    is_const = get_add_sub(current_el, &diff_graph->root_node);

    Require(current_el, '$');

    return diff_graph->root_node;
}

#define DERIVATIVE(oper, symbols, level, code)                                      \
if (strncmp(*s, symbols, strlen(symbols)) == 0 && current_level == level)           \
{                                                                                   \
    node *old_node = *main_node;                                                    \
                                                                                    \
    construct_node(main_node, OPER_TYPE, oper##_OPER);                              \
                                                                                    \
    *s += strlen(symbols);                                                          \
                                                                                    \
    node *val2 = nullptr;                                                           \
    is_const = next_func(s, &val2);                                                 \
                                                                                    \
    if (strcmp(symbols, "log") == 0)                                                \
    {                                                                               \
        old_node = val2;                                                            \
        is_const = next_func(s, &val2);                                             \
    }                                                                               \
                                                                                    \
    (*main_node)->left_node = old_node;                                             \
    (*main_node)->right_node = val2;                                                \
}                                                                                   \
                                                                                    \
else                                                                                \


bool get_add_sub(const char **s, node **main_node)
{
    printf("start ge_\n");

    bool (*next_func)(const char **, node **) = get_mul_div;

    bool is_const = next_func(s, main_node);

    int current_level = 4;

    while(true)
    {
        #include "derivative.h"
    
        //else
        {   
            break;
        }
    }
    
    printf("end ge_\n");

    return is_const;
}


bool get_mul_div(const char **s, node **main_node)
{
    bool (*next_func)(const char **, node **) = get_pow;

    bool val = next_func(s, main_node);

    int current_level = 3;

    bool is_const = false;

    while(true)
    {
        #include "derivative.h"
    
        //else
        {
            break;
        }
    }

    return is_const;
}


void exponential_function(node **main_node)
{
    node *degree_node = nullptr;
            
    construct_node(&degree_node, OPER_TYPE, MUL_OPER);

    node *ln_node = nullptr;

    construct_node(&ln_node, OPER_TYPE, LN_OPER);

    copy_node_with_childrens(&ln_node->right_node, &(*main_node)->left_node);

    copy_node_with_childrens(&degree_node->left_node, &ln_node);

    copy_node_with_childrens(&degree_node->right_node, &(*main_node)->right_node);

    change_node(&(*main_node)->left_node, EXP_TYPE, E);

    (*main_node)->right_node = degree_node;
}


bool get_pow(const char **s, node **main_node)
{
    printf("pidor");
    bool (*next_func)(const char **, node **) = get_func;

    bool is_const = next_func(s, main_node);

    int current_level = 2;
    
    while(true)
    {   
        #include "derivative.h"
    
        //else
        {     
            break;
        }

        if (is_const == false)
        {   
            exponential_function(main_node);

            is_const = false;

        }
    }

    return is_const;
}


bool get_func(const char **s, node **main_node)
{        
    printf("loh");
    bool (*next_func)(const char **, node **) = get_brackets;

    *main_node = nullptr;
    
    int current_level = 1;

    bool is_const = false;

    while(true)
    {
        #include "derivative.h"
    
        //else
        {            
            break;
        }
    }

    if (*main_node == nullptr)
    {
        is_const = next_func(s, main_node);
    }

    return is_const;
}

bool get_brackets(const char **s, node **main_node)
{
    printf("get_brackets\n");
    if (**s == '(')
    {
        *s += 1;
        
        bool is_const = false;

        is_const = get_add_sub(s, main_node);

        Require(s, ')');

        printf("GG");
        
        return is_const;
    }

    else 
    {
        return get_const_var(s, main_node);
    }
}

bool get_const_var(const char **s, node **new_node)
{
    printf("LEF");
    int val = 0;

    const char *start = *s;

    *new_node = nullptr;

    bool is_const = false;

    if ('0' <= **s && **s <= '9')
    {        
        is_const = true;

        while ('0' <= **s && **s <= '9')
        {
            val = val * 10 + **s - '0';
            *s += 1;
        }
        
        construct_node(new_node, CONST_TYPE, val);
    }

    else if ('e' == **s)
    {
        construct_node(new_node, EXP_TYPE, E);

        *s += 1;
    }

    else if ('a' <= **s && **s <= 'z')
    {
        construct_node(new_node, VAR_TYPE, **s - 'a');
        
        *s += 1;
    }    

    if (start == *s)
    {
        assert(0);
    }
    
    return is_const;
}