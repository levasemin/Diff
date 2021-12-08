#include "differentiator.h"

#define EQUATION(expression_file, current_node)     \
    fprintf(expression_file, "$(");                  \
    write_graph(expression_file, current_node);      \
    fprintf(expression_file, ") =");                \


#define DER_EQUATION(expression_file, current_node)       \
    write_graph(expression_file, current_node);      \
    fprintf(expression_file, "$ \\ \n");             \


node *differentiate_internal(node* calc_node, node *external_derivative, const char *expression_file_name);


void differentiate_graph(graph *diff_graph, int num, const char *expression_file_name, const char *graph_file_name, const char *png_file_name)
{
    FILE *expression_file = nullptr;

    if (expression_file_name != nullptr)
    {
        expression_file = open_file(expression_file_name, "wb");
    }

    for (int i; i < num; ++i)
    {
        diff_graph->root_node = differentiate_node(diff_graph->root_node, expression_file);
        
        be_simple(diff_graph);
        
        if (graph_file_name != nullptr && png_file_name != nullptr)
        {
            dump_graph_graphviz(graph_file_name, diff_graph);
            create_png(graph_file_name, png_file_name);
        }
    }
}


#define DERIVATIVE(oper, symbols, level, diff_code, oper_code)              \
    if (compare_floats(current_node->value, oper##_OPER) == 0)              \
        {                                                                   \
            diff_code                                                       \
        }                                                                   \
    else



node *differentiate_internal(node* calc_node, node *external_derivative, FILE *expression_file)
{
    node *current_node = nullptr;

    node* internal_derivative = differentiate_node(calc_node, expression_file);
    
    construct_node(&current_node, OPER_TYPE, MUL_OPER);

    copy_node_with_childrens(&current_node->left_node,  &external_derivative);

    copy_node_with_childrens(&current_node->right_node, &internal_derivative);
    
    return current_node;
}


node *differentiate_node(node *current_node, FILE *expression_file)
{
    assert(current_node != nullptr);
    
    
    Stack func_stack = {};

    stack_constructor(&func_stack);

    
    Stack div_func_stack = {};

    stack_constructor(&div_func_stack);


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