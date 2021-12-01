#include "differentiator.h"

void write_graph_graphviz(FILE *file, graph *graph);

void dump_graph(const char* dump_file_name, graph *graph)
{
    assert(graph != nullptr);
    
    FILE *dump_file = open_file(dump_file_name, "wb");

    dump_graph(dump_file, graph);

    fclose(dump_file);
}

void dump_graph(FILE *dump_file, graph *graph)
{
    assert(dump_file  != nullptr);
    assert(graph != nullptr);
    
    write_graph(dump_file, graph->root_node);
}

void write_graph(FILE *dump_file, node *current_node)
{    
    assert(dump_file != nullptr);
    assert(current_node != nullptr);
    
    if (current_node->left_node == nullptr && current_node->right_node != nullptr)
    {
        write_node(dump_file, current_node);
        fprintf(dump_file, "(");
    }

    if (current_node->left_node != nullptr)    
    {
        write_graph(dump_file, current_node->left_node);
    } 

    if (current_node->left_node != nullptr && current_node->right_node != nullptr)
    {
        write_node(dump_file, current_node);
        fprintf(dump_file, "(");
    }

    if (current_node->right_node != nullptr)
    {        
        write_graph(dump_file, current_node->right_node);
        fprintf(dump_file, ")");
    }

    if (current_node->left_node == nullptr && current_node->right_node == nullptr)
    {
        write_node(dump_file, current_node);
    }
    
    return;
}

#define DERIVATIVE(oper, symbols, level, code) \
    case oper##_OPER:                          \
        fprintf(dump_file, symbols);           \  
        break;                                 \


#define TRY_PRINT_VAR_CONST(current_node)                        \
    if (current_node->type == CONST_TYPE)                        \
    {                                                            \
        fprintf(dump_file, "%d", current_node->value);           \
    }                                                            \
                                                                 \
    else if(current_node->type == VAR_TYPE)                      \
    {                                                            \
        fprintf(dump_file, "%c", current_node->value + 'a');     \
    }                                                            \


void write_node(FILE *dump_file, node *current_node)
{       
    TRY_PRINT_VAR_CONST(current_node)

    else
    {
        switch(current_node->value)
        {
            #include "derivative.h"
            default:
                printf("%c", current_node->value + 'a');
                break;
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


void fprintf_node(FILE *dump_file, node *current_node, node *pre_node)
{
    fprintf(dump_file, "\"%x\n", current_node);                             
        
    TRY_PRINT_VAR_CONST(current_node)

    else
    {
        switch(current_node->value)
        {
            #include "derivative.h"
            default:
                break;
        }
    }        
    
    fprintf(dump_file, "\"->\"%x\n", pre_node);
    
    TRY_PRINT_VAR_CONST(pre_node)

    else
    {
        switch(pre_node->value)
        {
            #include "derivative.h"
            default:
                break;
        }
    } 

    fprintf(dump_file, "\";\n");
}

void write_node_graphviz(FILE *dump_file, node *current_node)
{    
    assert(dump_file         != nullptr);
    assert(current_node != nullptr);
    
    size_t file_size = 0;
            
    fseek(dump_file, -1L, SEEK_END);

    if (current_node->left_node != nullptr)  
    {
        fprintf_node(dump_file, current_node, current_node->left_node);
    }

    if (current_node->right_node != nullptr)
    {
        fprintf_node(dump_file, current_node, current_node->right_node);
    }
    else
    {
        fprintf(dump_file, "\"%x\n", current_node);

        TRY_PRINT_VAR_CONST(current_node)

        else
        {
            switch(current_node->value)
            {
                #include "derivative.h"
                default:
                    break;
            }
        }

        fprintf(dump_file, "\";\n");    
    }
    fprintf(dump_file, "}");
}

void fill_object_graphviz(const char *graphviz_file_name, Stack *stack, char *color)
{
    assert(stack != nullptr);
    assert(color != nullptr);

    FILE *file = open_file(graphviz_file_name, "rb+");

    fill_object_graphviz(file, stack, color);

    fclose(file);
}

void fill_object_graphviz(FILE *graphviz_file, Stack *stack, char *color)
{
    assert(graphviz_file  != nullptr);
    assert(stack != nullptr);
    assert(color != nullptr);
    
    fseek(graphviz_file, -1L, SEEK_END);

    while (0 < stack->size)
    {
        fprintf(graphviz_file, "\"%d\"[color=\"%d\" style=filled];\n", pop_stack(stack)->value, color);
    }

    fprintf(graphviz_file, "}");
    
    fclose(graphviz_file);
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