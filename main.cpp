#include "differentiator.h"

const char *file_name = "database_files/expression.txt";
const char *default_database_file_name   = "database_files/tree.database";
const char *default_save_file_name       = "database_files/save.database";
const char *default_graph_file_name      = "database_files/graph.dot";
const char *default_png_file_name        = "database_files/graph.png";

int main()
{
    const char *database_file_name = default_database_file_name;
    const char *save_file_name     = default_save_file_name;
    const char *graph_file_name    = default_graph_file_name;
    const char *png_file_name      = default_png_file_name;

    graph diff_graph = {};

    construct_graph(&diff_graph);

    get_graph(&diff_graph, file_name);
    
    dump_graph_graphviz("database_files/read_graph.dot", &diff_graph);
    create_png("database_files/read_graph.dot", "database_files/read_graph.png");

    differentiate_graph(&diff_graph);

    dump_graph_graphviz("database_files/diff_graph.dot", &diff_graph);
    create_png("database_files/diff_graph.dot", "database_files/dif_graph.png");

    be_simple(&diff_graph);
    
    dump_graph_graphviz("database_files/simple_graph.dot", &diff_graph);
    create_png("database_files/simple_graph.dot", "database_files/simple_graph.png");
    
    dump_graph(save_file_name, &diff_graph);

    //DEBUG_GRAPHVIZ("graph.dot", &diff_graph);
}