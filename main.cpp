#include "differentiator.h"

const char *default_expression_file_name = "database_files/expression.txt";
const char *default_save_file_name       = "database_files/result.tex";
const char *default_graph_file_name      = "database_files/derivative.dot";
const char *default_png_file_name        = "database_files/diff_graph.png";

int main(int argc, const char *argv[])
{
    const char *expression_file_name = default_expression_file_name;
    const char *save_file_name       = default_save_file_name;
    const char *graph_file_name      = default_graph_file_name;
    const char *png_file_name        = default_png_file_name;    
    
    graph graph = {};

    construct_graph(&graph);
    
    if (argc >= 3)
    {

        if (strcmp(argv[1], "derivative") == 0)
        {
            switch(argc)
            {
                case 4:
                    expression_file_name = argv[3];
                case 5:
                    save_file_name       = argv[4];
                case 6:
                    graph_file_name      = argv[5];
                case 7:   
                    graph_file_name      = argv[6];
                default:
                    break;
            }

            get_graph(&graph, expression_file_name);
            
            dump_graph_graphviz("database_files/read.dot", &graph);
            create_png("database_files/read.dot", "database_files/read.png");

            be_simple(&graph);
            
            dump_graph_graphviz("database_files/read_s.dot", &graph);
            create_png("database_files/read_s.dot", "database_files/read_s.png");

            fprintf_title_latex(save_file_name, "Derivative");

            differentiate_graph(&graph, atoi(argv[2]), save_file_name);

            fprintf_end_latex(save_file_name);

            be_simple(&graph, simplify_exponential_function);

            dump_graph_graphviz(graph_file_name, &graph);
            
            create_png(graph_file_name, png_file_name);            
        }

        else if (strcmp(argv[1], "teilor") == 0)
        {
            switch(argc)
            {
                case 5:
                    expression_file_name = argv[4];
                case 6:
                    save_file_name       = argv[5];
                case 7:
                    graph_file_name      = argv[6];
                case 8:   
                    graph_file_name      = argv[7];
                default:
                    break;
            }

            get_graph(&graph, expression_file_name);

            be_simple(&graph);

            float locality = (float)atoi(argv[2]);
            int term       = atoi(argv[3]);

            teilor(save_file_name, &graph, locality, term);
            
            be_simple(&graph, simplify_exponential_function);

            dump_graph_graphviz(graph_file_name, &graph);
            create_png(graph_file_name, png_file_name);
        }
    }
    
    system("pdflatex --interaction=nonstopmode database_files/result.tex");

}