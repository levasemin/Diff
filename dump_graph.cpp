#include "differentiator.h"

void fprintf_expression_inside(FILE *dump_file, node *current_node);

void fprintf_double_arg(FILE *dump_file, node *current_node, const char *type);

void write_graph_rec(FILE *dump_file, node *current_node);

void write_graph_graphviz(FILE *file, graph *graph);

void fprintf_node_graphviz(FILE *dump_file, node *current_node, node *pre_node);

void write_graph_graphviz(const char *graphviz_name_file, graph *graph);

FILE *create_graphviz_file(const char *graphviz_file_name);


void fprintf_title_latex(const char *dump_file_name, const char *title)
{
    FILE *dump_file = open_file(dump_file_name, "wb");

    fprintf_title_latex(dump_file, title);

    fclose(dump_file);
}


void fprintf_title_latex(FILE *dump_file, const char *title)
{
    fprintf(dump_file, "\\documentclass[a4paper, 12pt]{article}\n");
    fprintf(dump_file, "\\DeclareUnicodeCharacter{2212}{-}\n");
    fprintf(dump_file, "\\usepackage[left=0.5cm,right=0.5cm,top=0.5cm,bottom=0.5cm,bindingoffset=0cm]{geometry}\n");
    fprintf(dump_file, "\\usepackage{amsmath, amsfonts, amssymb, amsthm, mathtools}\n");
    fprintf(dump_file, "\\title{%s}\n", title); 
    fprintf(dump_file, "\\begin{document}\n");                                                            
    fprintf(dump_file, "\\maketitle \n");
}


void fprintf_end_latex(const char *dump_file_name)
{
    FILE *dump_file = open_file(dump_file_name, "ab");

    fprintf_end_latex(dump_file);

    fclose(dump_file);    
}


void fprintf_end_latex(FILE *dump_file)
{
    fprintf(dump_file, "\n\\end{document}");

    fclose(dump_file);
}


void write_graph(const char *dump_file_name, node *current_node)
{
    FILE *dump_file = open_file(dump_file_name, "ab");

    write_graph(dump_file, current_node);

    fclose(dump_file);
}


void write_graph(FILE *dump_file, node *current_node)
{
    fprintf(dump_file, "$");

    node *copy_root_node = nullptr;

    copy_node_with_childrens(&copy_root_node, &current_node);

    write_graph_rec(dump_file, copy_root_node);
    
    fprintf(dump_file, "$");
}


void fprintf_expression_inside(FILE *dump_file, node *current_node)
{
    fprintf(dump_file, "{(");

    write_graph_rec(dump_file, current_node);
    
    fprintf(dump_file, ")}");
}


void fprintf_double_arg(FILE *dump_file, node *current_node, const char *type)
{
    fprintf(dump_file, "\\%s", type);
        
    fprintf_expression_inside(dump_file, current_node->left_node);        

    fprintf_expression_inside(dump_file, current_node->right_node);
}


void write_graph_rec(FILE *dump_file, node *current_node)
{    
    assert(dump_file != nullptr);
    assert(current_node != nullptr);
    
    int node_level = get_level(current_node);

    if (current_node->type == OPER_TYPE && compare_floats(current_node->value, LOG_OPER) == 0)
    {
        fprintf_double_arg(dump_file, current_node, "log");

        return;
    }
    
    if (current_node->type == OPER_TYPE && compare_floats(current_node->value, DIV_OPER) == 0)
    {
        fprintf_double_arg(dump_file, current_node, "frac");       

        return;
    }

    if (node_level == 1)
    {
        fprintf(dump_file, "\\");

        write_node_value(dump_file, current_node);
        
        fprintf_expression_inside(dump_file, current_node->right_node);
        
        return;
    }

    if (current_node->left_node != nullptr)    
    {
        if (current_node->left_node->type == OPER_TYPE && 
        (node_level < get_level(current_node->left_node)))
        {
            fprintf_expression_inside(dump_file, current_node->left_node);

            write_node_value(dump_file, current_node);
        }

        else
        {
            write_graph_rec(dump_file, current_node->left_node);

            write_node_value(dump_file, current_node);
        }
    } 

    if (current_node->right_node != nullptr)
    {
        if (current_node->right_node->type == OPER_TYPE &&
        (node_level < get_level(current_node->right_node)))
        {            
            fprintf_expression_inside(dump_file, current_node->right_node);
        }
           
        else
        {
            write_graph_rec(dump_file, current_node->right_node);
        }
    } 

    if (current_node->left_node == nullptr && current_node->right_node == nullptr)
    {
        write_node_value(dump_file, current_node);
    }

    return;
}


#define DERIVATIVE(oper, symbols, level, diff_code, oper_code)      \
    if(compare_floats(current_node->value, oper##_OPER) == 0)       \
        {                                                           \
            fprintf(dump_file, symbols);                            \
        }                                                           \
                                                                    \
    else


void write_node_value(FILE *dump_file, node *current_node)
{
    if (current_node->type == CONST_TYPE)
    {                                                            
        fprintf(dump_file, "{%.*f}", SIGN_COUNT, current_node->value);           
    }                                                            

    else if(current_node->type == VAR_TYPE || current_node->type == EXP_TYPE)                      
    {
        fprintf(dump_file, "%c", (int)current_node->value + 'a'); 
    }                                                            

    else
    {
        #include "derivative.h"
        
        //else
        {
            assert(0);
        }
    }
}


void write_graph_graphviz(const char *graphviz_name_file, graph *graph)
{
    assert(graph != nullptr);
    
    FILE *graphviz_file = open_file(graphviz_name_file, "wb");

    write_graph_graphviz(graphviz_file, graph);

    fclose(graphviz_file);
}


void write_graph_graphviz(FILE *graphviz_file, graph *graph)
{
    assert(graphviz_file  != nullptr);
    assert(graph != nullptr);

    Stack stack = {};

    stack_constructor(&stack);

    push_stack(&stack, &graph->root_node);

    while (stack.size != 0)
    {
        node *current_node = pop_stack(&stack);
        
        write_node_graphviz(graphviz_file, current_node);

        if (current_node->right_node != nullptr)  
        {
            push_stack(&stack, &(current_node->right_node));
        }
        
        if (current_node->left_node != nullptr)
        {
            push_stack(&stack, &(current_node->left_node));
        }
    }
}

    
FILE *create_graphviz_file(const char *graphviz_file_name)
{
    FILE *graphviz_file = fopen(graphviz_file_name, "wb");

    fprintf(graphviz_file, "digraph G{\n}");

    return graphviz_file;
}


void fprintf_node_graphviz(FILE *dump_file, node *current_node, node *pre_node)
{
    fprintf(dump_file, "\"%p\n", current_node);                             
        
    write_node_value(dump_file, current_node);
    
    fprintf(dump_file, "\"->\"%p\n", pre_node);
    
    write_node_value(dump_file, pre_node);

    fprintf(dump_file, "\";\n");
}


void write_node_graphviz(FILE *dump_file, node *current_node)
{    
    assert(dump_file    != nullptr);
    assert(current_node != nullptr);
                
    fseek(dump_file, -1L, SEEK_END);

    if (current_node->left_node != nullptr)  
    {
        fprintf_node_graphviz(dump_file, current_node, current_node->left_node);
    }

    if (current_node->right_node != nullptr)
    {
        fprintf_node_graphviz(dump_file, current_node, current_node->right_node);
    }
    
    else
    {
        fprintf(dump_file, "\"%p\n", current_node);

        write_node_value(dump_file, current_node);
        
        fprintf(dump_file, "\";\n");    
    }
    
    fprintf(dump_file, "}");
}


void fill_object_graphviz(const char *graphviz_file_name, Stack *stack, char *color)
{
    assert(stack != nullptr);
    assert(color != nullptr);

    FILE *graphviz_file = open_file(graphviz_file_name, "rb+");

    fill_object_graphviz(graphviz_file, stack, color);

    fclose(graphviz_file);
}


void fill_object_graphviz(FILE *graphviz_file, Stack *stack, char *color)
{
    assert(graphviz_file  != nullptr);
    assert(stack != nullptr);
    assert(color != nullptr);
    
    fseek(graphviz_file, -1L, SEEK_END);

    while (0 < stack->size)
    {
        fprintf(graphviz_file, "\"%f\"[color=\"%s\" style=filled];\n", pop_stack(stack)->value, color);
    }

    fprintf(graphviz_file, "}");
}


void dump_graph_graphviz(const char* graphviz_file_name, graph *graph, Stack* characteristics_object, char *color)
{
    assert(graphviz_file_name != nullptr);
    assert(graph              != nullptr);

    FILE *graphviz_file = create_graphviz_file(graphviz_file_name);
    
    dump_graph_graphviz(graphviz_file, graph, characteristics_object, color);

    fclose(graphviz_file);
}


void dump_graph_graphviz(FILE* graphviz_file, graph *graph, Stack* characteristics_object, char *color)
{
    assert(graphviz_file != nullptr);
    assert(graph         != nullptr);

    write_graph_graphviz(graphviz_file, graph);

    if (characteristics_object != nullptr)
    {
        fill_object_graphviz(graphviz_file, characteristics_object, color);
    }
}


void create_png(const char *graph_file_name, const char* png_file_name)
{
    assert(graph_file_name != nullptr);
    assert(png_file_name   != nullptr);

    //"dot -T png graph.dot > graph.png"
    char str[100] = "dot -T png ";
    
    strcat(str, graph_file_name);
    strcat(str, ">");
    strcat(str, png_file_name);
    
    system(str);
}