#include "differentiator.h"

void write_graph_graphviz(FILE *file, graph *graph);

void dump_graph(const char* database_file_name, graph *graph)
{
    assert(graph != nullptr);
    
    FILE *database_file = open_file(database_file_name, "wb");

    dump_graph(database_file, graph);

    fclose(database_file);
}

void dump_graph(FILE *database_file, graph *graph)
{
    assert(database_file  != nullptr);
    assert(graph != nullptr);
    
    write_graph(database_file, graph->root_node);
}

void write_graph(FILE *database_file, node *current_node)
{    
    assert(database_file != nullptr);
    assert(current_node != nullptr);
    
    if (current_node->left_node == nullptr && current_node->right_node != nullptr)
    {
        write_node(database_file, current_node);
    }

    if (current_node->left_node != nullptr)    
    {
        fprintf(database_file, "(");
        write_graph(database_file, current_node->left_node);
        fprintf(database_file, ")");
    } 

    if (current_node->left_node != nullptr && current_node->right_node != nullptr)
    {
        write_node(database_file, current_node);
    }

    if (current_node->right_node != nullptr)
    {        
        fprintf(database_file, "(");
        write_graph(database_file, current_node->right_node);
        fprintf(database_file, ")");
    }

    if (current_node->left_node == nullptr && current_node->right_node == nullptr)
    {
        write_node(database_file, current_node);
    }
    
    return;
}

void write_node(FILE *database_file, node *current_node)
{       
    if (current_node->type == CONST_TYPE)
    {
        fprintf(database_file, "%d", current_node->value);
    }

    else if(current_node->type == VAR_TYPE)
    {
        fprintf(database_file, "%c", current_node->value + 'a');
    }

    else
    {
        switch(current_node->value)
        {
            case SUM_OPER:
                fprintf(database_file, "+");
                break;
            case SUB_OPER:
                fprintf(database_file, "-");
                break;
            case DIV_OPER:
                fprintf(database_file, "/");
                break;
            case MUL_OPER:
                fprintf(database_file, "*");
                break;
            case POW_OPER:
                fprintf(database_file, "^");
                break;
            case COS_OPER:
                fprintf(database_file, "cos");
                break;
            case SIN_OPER:
                fprintf(database_file, "sin");
                break;
            case TG_OPER:
                fprintf(database_file, "tg");
                break;
            case CTG_OPER:
                fprintf(database_file, "ctg");
                break;
            default:
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


#define FWRITE_CHILD(child_type)                                                                     \
    fprintf(graphviz_file, "\"%x \n %d\"->\"%x \n %d\";\n", current_node, current_node->value, current_node->child_type##_node, current_node->child_type##_node->value);


FILE *create_graphviz_file(const char *graphviz_file_name)
{
    FILE *graphviz_file = fopen(graphviz_file_name, "wb");

    fprintf(graphviz_file, "digraph G{\n}");

    return graphviz_file;
}

void write_node_graphviz(FILE *graphviz_file, node *current_node)
{    
    assert(graphviz_file         != nullptr);
    assert(current_node != nullptr);
    
    size_t file_size = 0;
            
    fseek(graphviz_file, -1L, SEEK_END);

    if (current_node->left_node != nullptr)  
    {
        FWRITE_CHILD(left)                
    }

    if (current_node->right_node != nullptr)
    {
        FWRITE_CHILD(right)
    }

    else
    {
        fprintf(graphviz_file, "\"%x \n %d\";\n", current_node, current_node->value);
    }

    fprintf(graphviz_file, "}");
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