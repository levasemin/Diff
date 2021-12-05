enum types
{
    CONST_TYPE   = 1,
    VAR_TYPE     = 2,
    EXP_TYPE     = 3,
    OPER_TYPE    = 4
};

const float COS_OPER = 1;
const float SIN_OPER = 2;
const float TG_OPER  = 3;
const float CTG_OPER = 4;
const float LN_OPER  = 5;
const float LOG_OPER = 6;


const float POW_OPER = 7;
    

const float DIV_OPER = 8;
const float MUL_OPER = 9;
    
const float SUM_OPER = 10;
const float SUB_OPER = 11;

struct node
{
    int type;
    float value;

    node *left_node;
    node *right_node;
};

struct graph
{
    int count_nodes;

    node *root_node;
};
