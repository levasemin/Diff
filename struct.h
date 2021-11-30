enum types
{
    CONST_TYPE   = 1,
    VAR_TYPE     = 2,
    OPER_TYPE    = 3
};

enum operations
{
    COS_OPER = 1,
    SIN_OPER,
    TG_OPER,
    CTG_OPER,
    LN_OPER,
    LOG_OPER,


    POW_OPER,
    

    DIV_OPER,
    MUL_OPER,
    
    SUM_OPER,
    SUB_OPER,
};

struct node
{
    int type;
    int value;

    node *left_node;
    node *right_node;
};

struct graph
{
    int count_nodes;

    node *root_node;
};
