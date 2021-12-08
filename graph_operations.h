void construct_graph(graph *graph);

bool construct_node(node **new_node, int type, float value, node *left_node = nullptr, node* right_node = nullptr);


bool change_node(node **new_node, int type, float value, node *left_node = nullptr, node* right_node = nullptr);

void change_node(node *current_node, int type, float value, node *left_node, node *right_node);


void copy_node_with_childrens(node **new_node, node **old_node);

void copy_node(node **new_node, node **old_node);


float initializate_function(const char *value);

int get_count_params(node *current_node);

int compare_floats(float val1, float val2);

int get_level(node *current_node);
