#include "differentiator.h"

void Require(const char **string, const char expected);

void exponential_function(node **main_node);

bool get_add_sub(const char **string, node **main_node);

bool get_mul_div(const char **string, node **main_node);

bool get_func(const char **string, node **main_node);

bool get_pow(const char **string, node **main_node);

bool get_brackets(const char **string, node **main_node);

bool get_const_var(const char **string, node **main_node);


void Require(const char **string, const char expected)
{
    if (**string == expected)
    {
        *string += 1;
    }

    else
    {
        printf("expected %c\n", expected);
        printf("real %c\n", **string);

        assert(0);
    }
}

node *read_graph(graph *diff_graph, const char **current_el)
{    
    bool is_const = get_add_sub(current_el, &diff_graph->root_node);

    Require(current_el, '$');

    return diff_graph->root_node;
}

#define DERIVATIVE(oper, symbols, level, oper_code, diff_code)                      \
if (strncmp(*string, symbols, strlen(symbols)) == 0 && current_level == level)      \
{                                                                                   \
    node *old_node = *main_node;                                                    \
                                                                                    \
    construct_node(main_node, OPER_TYPE, oper##_OPER);                              \
                                                                                    \
    *string += strlen(symbols);                                                     \
                                                                                    \
    node *val2 = nullptr;                                                           \
                                                                                    \
    is_const *= next_func(string, &val2);                                           \
                                                                                    \
    if (strcmp(symbols, "log") == 0)                                                \
    {                                                                               \
        old_node = val2;                                                            \
        is_const *= next_func(string, &val2);                                       \
    }                                                                               \
                                                                                    \
    (*main_node)->left_node = old_node;                                             \
    (*main_node)->right_node = val2;                                                \
}                                                                                   \
                                                                                    \
else                                                                                \

void skip_spaces(const char **string)
{
    while (**string == ' ')
    {
        *string += 1;
    }
}

bool get_add_sub(const char **string, node **main_node)
{
    bool (*next_func)(const char **, node **) = get_mul_div;

    bool is_const = next_func(string, main_node);

    int current_level = 4;

    while(true)
    {
        skip_spaces(string);

        #include "derivative.h"

        //else
        {   
            break;
        }
    }

    printf("sum %d\n", is_const);
    
    return is_const;
}


bool get_mul_div(const char **string, node **main_node)
{
    bool (*next_func)(const char **, node **) = get_pow;

    bool is_const = next_func(string, main_node);

    int current_level = 3;

    while(true)
    {
        skip_spaces(string);

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


bool get_pow(const char **string, node **main_node)
{
    bool (*next_func)(const char **, node **) = get_func;

    bool is_const = next_func(string, main_node);

    int current_level = 2;
    
    while(true)
    {   
        printf("pow %d ||| %s\n", is_const, *string);

        skip_spaces(string);

        if (**string == '^')
        {
            is_const = true;

            #include "derivative.h"
            {}
        }

        else
        {     
            printf("BREAK\n");
            break;
        }
        
        if (is_const == false)
        {   printf("exponential function %s\n", *string);
            exponential_function(main_node);
        }
    }

    return is_const;
}


bool get_func(const char **string, node **main_node)
{        
    bool (*next_func)(const char **, node **) = get_brackets;

    *main_node = nullptr;
    
    int current_level = 1;

    bool is_const = true;

    while(true)
    {
        skip_spaces(string);

        #include "derivative.h"
    
        //else
        {            
            break;
        }
    }

    if (*main_node == nullptr)
    {
        is_const = next_func(string, main_node);
    }
    printf("func %d\n", is_const);
    return is_const;
}

bool get_brackets(const char **string, node **main_node)
{
    bool is_const = true;

    if (**string == '(')
    {
        *string += 1;
        
        is_const = get_add_sub(string, main_node);

        Require(string, ')');
        
        printf("brackets %d\n", is_const);

        return is_const;
    }

    else 
    {
        is_const = get_const_var(string, main_node);
        printf("no brackets %d\n", is_const);
        return is_const;
    }
}

bool get_const_var(const char **string, node **new_node)
{
    float val = 0;

    const char *start = *string;

    *new_node = nullptr;

    bool is_const = false;

    skip_spaces(string);

    if ('0' <= **string && **string <= '9')
    {        
        is_const = true;

        while ('0' <= **string && **string <= '9')
        {
            val = val * 10 + **string - '0';
            *string += 1;
        }
        
        if (**string == '.' || **string == ',')
        {
            *string += 1;

            float fractional_val = 0;

            while ('0' <= **string && **string <= '9')
            {
                fractional_val = fractional_val * 0.1 + (**string - '0')* 0.1;
                *string += 1;
            }

            val += fractional_val; 
        }

        construct_node(new_node, CONST_TYPE, val);
    }

    else if ('e' == **string)
    {
        is_const = true;

        construct_node(new_node, EXP_TYPE, E);

        *string += 1;
    }

    else if ('a' <= **string && **string <= 'z')
    {
        construct_node(new_node, VAR_TYPE, (float)(**string - 'a'));
        
        *string += 1;
    }    

    if (start == *string)
    {
        assert(0);
    }
    printf("-%d-\n", is_const);
    return is_const;
}