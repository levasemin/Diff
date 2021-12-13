#include "differentiator.h"

void derivative_teilor(node **current_node, float locality, bool *derivative_exist);

void do_operation(node **current_node, float val1, float val2, bool *derivative_exist);

void sprintf_teilor(char *dump_string, graph calc_graph, float locality, int term_count, int num_term);

#define DERIVATIVE(oper, symbols, level, diff_code, oper_code)          \
    if (compare_floats(((*current_node)->value), (oper##_OPER)) == 0)   \
        {                                                               \
            oper_code                                                   \
        }                                                               \
                                                                        \
    else


void derivative_teilor(node **current_node, float locality, bool *derivative_exist)
{
    if (*derivative_exist == false)
    {
        return; 
    }

    if ((*current_node)->left_node != nullptr)
    {
        derivative_teilor(&(*current_node)->left_node, locality, derivative_exist);
    }  

    if ((*current_node)->type == VAR_TYPE)
    {
        (*current_node)->value = locality;
        
        (*current_node)->type  = CONST_TYPE; 
        
        return;
    }

    if ((*current_node)->type == EXP_TYPE)
    {
        (*current_node)->value = 2.718;
        
        (*current_node)->type  = CONST_TYPE;
    }

    if ((*current_node)->right_node != nullptr)
    {
        derivative_teilor(&(*current_node)->right_node, locality, derivative_exist);

        if ((*current_node)->right_node->type == CONST_TYPE)
        {
            if ((*current_node)->left_node == nullptr)
            {
                float val2 = (*current_node)->right_node->value;

                do_operation(current_node, 0, val2, derivative_exist);
            }
            
            else if ((*current_node)->left_node->type == CONST_TYPE)
            {
                float val1 = (*current_node)->left_node->value;
                float val2 = (*current_node)->right_node->value;
                
                do_operation(current_node, val1, val2, derivative_exist);
            }
        }    
    }
}


void do_operation(node **current_node, float val1, float val2, bool *derivative_exist)
{
    float result = 0;

    #include "derivative.h"
    //else
    {
        assert(0);
    }

    (*current_node)->type  = CONST_TYPE;
    (*current_node)->value = result;
}


void fprintf_float(FILE *dump_file, float number, int sign_count)
{
    fprintf(dump_file, "{%.*f}", sign_count, number);
}


void sprintf_float(char *dump_string, float number, int sign_count)
{
    char str_number[10] = {};

    sprintf(str_number, "{%.*f}", sign_count, number);
    
    strcat(dump_string, str_number);
}


int factorial(int number)
{
    if (number == 0)
    {
        return 1;
    }
    
    else
    {
        return(number * factorial(number-1));
    }
}


void sprintf_teilor(char *dump_string, graph calc_graph, float locality, int term_count, int num_term)
{
        char str_locality[10] = {};
        calc_graph.root_node->value /= (float)factorial(num_term+1);

        if (calc_graph.root_node->value < 0)
        {
            strcat(dump_string, "-");
            sprintf_float(dump_string, calc_graph.root_node->value * -1, SIGN_COUNT);
            strcat(dump_string, "*");
        }

        else if (calc_graph.root_node->value > 0)
        {
            strcat(dump_string, "+");
            sprintf_float(dump_string, calc_graph.root_node->value, SIGN_COUNT);
            strcat(dump_string, "*");
        }

        if (locality > 0)
        {
            sprintf(str_locality, "(x - %f)", locality);
            strcat(dump_string, str_locality);
        }

        else if (locality < 0)
        {
            sprintf(str_locality, "(x %f)", locality);
            strcat(dump_string, str_locality);
        }

        else 
        {
            strcat(dump_string, "x");
        }

        if (num_term != 0)
        {
            sprintf(str_locality, "^%d", num_term + 1);
            strcat(dump_string, str_locality);
        }
}


void teilor(const char *name_dump_file, graph *diff_graph, float locality, int term_count)
{    
    FILE *dump_file = open_file(name_dump_file, "wb");

    fprintf_title_latex(dump_file, "Teilor");

    teilor(dump_file, diff_graph, locality, term_count);

    fprintf_end_latex(dump_file);
}


void teilor(FILE *dump_file, graph *diff_graph, float locality, int term_count)
{   
    fprintf(dump_file, "Default equation :");

    write_graph(dump_file, diff_graph->root_node);

    fprintf(dump_file, "\\\\");

    char equation_string[300] = {};

    bool derivative_exist = true;

    graph calc_graph = {};
    
    construct_graph(&calc_graph);

    copy_node_with_childrens(&(calc_graph.root_node), &diff_graph->root_node);
    
    be_simple(&calc_graph, simplify_exponential_function);

    fprintf(dump_file, "%d Derivative:", 0);
    
    be_simple(&calc_graph, simplify_exponential_function);

    write_graph(dump_file, calc_graph.root_node);

    fprintf(dump_file, "\\\\ \n");                               

    derivative_teilor(&calc_graph.root_node, locality, &derivative_exist);

    if (derivative_exist && compare_floats(calc_graph.root_node->value, 0) != 0)
    {
        sprintf_float(equation_string,  calc_graph.root_node->value, SIGN_COUNT);
    }

    for (int num_term = 0; num_term < term_count; ++num_term)
    {   
        
        derivative_exist = true;
        
        diff_graph->root_node = differentiate_node(diff_graph->root_node);
                        
        be_simple(diff_graph);

        copy_node_with_childrens(&(calc_graph.root_node), &diff_graph->root_node);
        
        fprintf(dump_file, "%d Derivative:", num_term + 1);

        write_graph(dump_file, calc_graph.root_node);

        fprintf(dump_file, "\\\\ \n");

        derivative_teilor(&calc_graph.root_node, locality, &derivative_exist);

        if (derivative_exist == false)
        {
            continue;
        }

        else if(compare_floats(calc_graph.root_node->value, 0) == 0) 
        {
            continue;
        }
        
        sprintf_teilor(equation_string, calc_graph, locality, term_count, num_term);

        destruct_graph(&calc_graph);
        construct_graph(&calc_graph);
    }

    fprintf(dump_file, "$%s", equation_string);

    if (compare_floats(locality, 0) == 0)
    {
        fprintf(dump_file, "+o(x^%d)$", term_count);
    }

    else if (locality < 0)
    {
        fprintf(dump_file, "+o((x + %f)^%d)$", locality * -1, term_count);
    }

    else 
    {
        fprintf(dump_file, "+o((x - %f)^%d)$", locality, term_count);
    }
    
    destruct_graph(&calc_graph);
}