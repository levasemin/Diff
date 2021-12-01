#include "differentiator.h"

const char *file_name = "expression.txt";
const char *default_database_file_name   = "database_files/tree.database";
const char *default_save_file_name       = "database_files/save.database";
const char *default_graph_file_name      = "graph.dot";
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
    
    dump_graph_graphviz("read_graph.dot", &diff_graph);
    create_png("read_graph.dot", "read_graph.png");

    differentiate_graph(&diff_graph);

    dump_graph_graphviz("diff_graph.dot", &diff_graph);
    create_png("diff_graph.dot", "dif_graph.png");

    be_simple(&diff_graph);

    dump_graph_graphviz("simple_graph.dot", &diff_graph);
    create_png("simple_graph.dot", "simple_graph.png");
    
    dump_graph(save_file_name, &diff_graph);

    //DEBUG_GRAPHVIZ("graph.dot", &diff_graph);
}