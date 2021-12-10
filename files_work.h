FILE *open_file(const char *file_name, const char *key);

size_t get_file_size(FILE *file);

node *read_graph(graph *diff_graph, const char **current_el);

size_t read_file(const char *file_name, char **string);

size_t read_file(FILE *file, char **string);


void fprintf_float(FILE *dump_file, float number, int sign_count);

void sprintf_float(char *dump_string, float number, int sign_count);

void fprintf_title_latex(const char *dump_file_name, const char *title);

void fprintf_title_latex(FILE *dump_file, const char *title);

void fprintf_end_latex(const char *dump_file_name);

void fprintf_end_latex(FILE *dump_file);


void write_graph(const char *dump_file_name, node *current_node);

void write_graph(FILE *file, node *current_node);

void write_node_value(FILE *file, node *current_node);


void dump_graph_graphviz(const char* graph_file_name, graph *graph, Stack* characteristics_object = nullptr, char *color = nullptr);

void dump_graph_graphviz(FILE* graphviz_file, graph *graph, Stack* characteristics_object = nullptr, char *color = nullptr);

void write_node_graphviz(FILE *file, node *current_node);

void fill_object_graphviz(const char *graphviz_file_name, Stack *stack, char *color);

void fill_object_graphviz(FILE *file, Stack *stack, char *color);


void create_png(const char *graph_file_name, const char* png_file_name);

void create_png(FILE *graph_file, FILE *png_file);