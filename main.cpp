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
                default:
                    break;
            }

            get_graph(&graph, expression_file_name);
            //DEBUG_GRAPHVIZ_GRAPH("graph.dot", &graph);
            be_simple(&graph);

            fprintf_title_latex(save_file_name, "Derivative");

            differentiate_graph(&graph, atoi(argv[2]), save_file_name);

            fprintf_end_latex(save_file_name);
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
                default:
                    break;
            }

            get_graph(&graph, expression_file_name);

            be_simple(&graph);

            float locality = (float)atoi(argv[2]);
            int term       = atoi(argv[3]);

            teilor(save_file_name, &graph, locality, term);
        }
    }
    system("pdflatex --interaction=nonstopmode database_files/result.tex");

}