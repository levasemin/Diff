#include "differentiator.h"

void fprintf_default_derivative(FILE *dump_file, node **default_node, node **derivative_node, int num_derivative)   
{
    printf("START BE SIMPLE DERIVATIVE\n");
    DEBUG_GRAPHVIZ_NODE("graph.dot", *derivative_node);
    make_simple_node(derivative_node);
    // printf("START BE SIMPLE DERFAULT\n");

    //make_simple_node(default_node);
    DEBUG_GRAPHVIZ_NODE("graph.dot", *derivative_node);

    fprintf(dump_file, "(");                                    
    write_graph(dump_file, *default_node);                        
    fprintf(dump_file, ")$^{(%d)}$ = ", num_derivative);                                   
                                                                       
    write_graph(dump_file, *derivative_node);                        
    fprintf(dump_file, "\\\\ \n");                               
}

node *differentiate_internal(node* calc_node, node *external_derivative, const char *dump_file_name);


void differentiate_graph(graph *diff_graph, int count_derivative, const char *dump_file_name, const char *graph_file_name, const char *png_file_name)
{
    
    FILE *dump_file = nullptr;

    if (dump_file_name != nullptr)
    {
        dump_file = open_file(dump_file_name, "ab");
    }
    
    graph default_graph = {};

    construct_graph(&default_graph);

    copy_node_with_childrens(&(default_graph.root_node), &diff_graph->root_node);

    for (int num_derivative; num_derivative < count_derivative; ++num_derivative)
    {
        if (dump_file_name != nullptr)
        {
            fprintf(dump_file, "Calculations %d derivative:\\\\ \n", num_derivative + 1);
        }
        
        diff_graph->root_node = differentiate_node(diff_graph->root_node, dump_file, 1);
        
        be_simple(diff_graph);
        
        if (graph_file_name != nullptr && png_file_name != nullptr)
        {
            dump_graph_graphviz(graph_file_name, diff_graph);
            create_png(graph_file_name, png_file_name);
        }

        if (dump_file_name != nullptr)
        {
            fprintf(dump_file, "%d Derivative : \\\\ \n", num_derivative + 1);
        }

        if (dump_file_name != nullptr)
        {
            fprintf_default_derivative(dump_file, &(default_graph.root_node), &(diff_graph->root_node), num_derivative + 1);        
        }

        fprintf(dump_file, "\\\\");
    }

    fclose(dump_file);
}


#define DERIVATIVE(oper, symbols, level, diff_code, oper_code)              \
    if (compare_floats(current_node->value, oper##_OPER) == 0)              \
        {                                                                   \
            diff_code                                                       \
        }                                                                   \
    else



node *differentiate_internal(node* calc_node, node *external_derivative, FILE *dump_file)
{
    node *current_node = nullptr;

    node* internal_derivative = differentiate_node(calc_node, dump_file);
    
    construct_node(&current_node, OPER_TYPE, MUL_OPER);

    copy_node_with_childrens(&current_node->left_node,  &external_derivative);

    copy_node_with_childrens(&current_node->right_node, &internal_derivative);
    
    return current_node;
}


node *differentiate_node(node *current_node, FILE *dump_file, int num_derivative)
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