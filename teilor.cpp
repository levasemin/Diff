#include "differentiator.h"

void do_operation(node **current_node, float val1, float val2, bool *derivative_exist);
void derivative_teilor(node **current_node, float locality, bool *derivative_exist);
void do_operation(node **current_node, float val1, float val2, bool *derivative_exist);

#define DERIVATIVE(oper, symbols, level, diff_code, oper_code)      \
    if(((*current_node)->value) == (oper##_OPER))                   \
        {                                                           \
            oper_code                                               \
        }                                                           \
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

    printf("%d %d ", val1, val2);

    #include "derivative.h"
    //else
    {
        assert(0);
    }

    printf("%d %d\n", result, (*current_node)->value);

    (*current_node)->type  = CONST_TYPE;
    (*current_node)->value = result;
}

void teilor(const char *name_dump_file, graph *diff_graph, float locality, int term_count)
{
    FILE *dump_file = open_file(name_dump_file, "wb");
    
    teilor(dump_file, diff_graph, locality, term_count);
}

void fprintf_float(FILE *dump_file, float number, int sign_count)
{
    fprintf(dump_file, "%.*f", sign_count, number);
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

void teilor(FILE *dump_file, graph *diff_graph, float locality, int term_count)
{   
    bool derivative_exist = true;

    graph calc_graph = {};
    
    construct_graph(&calc_graph);

    copy_node_with_childrens(&calc_graph.root_node, &diff_graph->root_node);
    
    derivative_teilor(&calc_graph.root_node, locality, &derivative_exist);
    
    if (derivative_exist && calc_graph.root_node->value != 0)
    {
        fprintf_float(dump_file,  calc_graph.root_node->value, SIGN_COUNT);
    }

    //DEBUG_GRAPHVIZ("graph.dot", diff_graph) 
    //DEBUG_GRAPHVIZ("graph.dot", &calc_graph) 

    
    for (int i = 0; i < term_count; ++i)
    {
        derivative_exist = true;

        differentiate_graph(diff_graph);
    
        be_simple(diff_graph);

        //DEBUG_GRAPHVIZ("graph.dot", diff_graph) 

        copy_node_with_childrens(&calc_graph.root_node, &diff_graph->root_node);
        
        derivative_teilor(&calc_graph.root_node, locality, &derivative_exist);

        //DEBUG_GRAPHVIZ("graph.dot", &calc_graph) 

        if (derivative_exist == false || calc_graph.root_node->value == 0)
        {
            printf("derivaitive does not exist\n");
            continue;
        }

        calc_graph.root_node->value /= factorial(i+1);

        if (compare_floats(calc_graph.root_node->value, 1))
        {
            fprintf(dump_file, "+", locality);
        }
        
        else if (calc_graph.root_node->value < 0)
        {
            fprintf(dump_file, "-", locality);
            fprintf_float(dump_file, calc_graph.root_node->value * -1, SIGN_COUNT);
            fprintf(dump_file, "*");
        }

        else if (calc_graph.root_node->value > 0)
        {
            fprintf(dump_file, "+", locality);
            fprintf_float(dump_file, calc_graph.root_node->value, SIGN_COUNT);
            fprintf(dump_file, "*");
        }

        if (locality > 0)
        {
            fprintf(dump_file, "(x - %f)", locality);
        }

        else if (locality < 0)
        {
            fprintf(dump_file, "(x %f)", locality);
        }

        else 
        {
            fprintf(dump_file, "x");
        }

        if (i != 0)
        {
            fprintf(dump_file, "^%d", i + 1);
        }
    }

    if (compare_floats(locality, 0))
    {
        fprintf(dump_file, "+o(x^%d)", term_count);
    }

    else if (locality < 0)
    {
        fprintf(dump_file, "+o((x + %d)^%d)", locality * -1, term_count);
    }

    else 
    {
        fprintf(dump_file, "+o((x - )^%d)", locality, term_count);
    }
}
