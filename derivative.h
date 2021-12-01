DERIVATIVE(SUM, "+", 4,
{
    construct_node(&current_node, OPER_TYPE, SUM_OPER, differentiate_node(current_node->left_node), differentiate_node(current_node->right_node));
})

DERIVATIVE(SUB, "-", 4,
{
    construct_node(&current_node, OPER_TYPE, SUB_OPER, differentiate_node(current_node->left_node), differentiate_node(current_node->right_node));
})

DERIVATIVE(POW, "^", 2,
{
    node *expression = nullptr;

    copy_node_with_childrens(&expression, &current_node->left_node);
    
    node *degree = nullptr;

    copy_node_with_childrens(&degree, &current_node->right_node);

    node *external_derivative = nullptr;

    construct_node(&external_derivative, OPER_TYPE, MUL_OPER, expression, degree);

    if (degree->value - 1 != 1)
    {
        node *pow_node = nullptr;

        construct_node(&pow_node, OPER_TYPE, POW_OPER, expression);
        
        construct_node(&pow_node->right_node, CONST_TYPE, degree->value - 1);
        
        external_derivative->left_node = pow_node;
    }
    
    current_node = differentiate_internal(expression, external_derivative);
})

DERIVATIVE(DIV, "/", 3,
{
    node *numerator = nullptr;
    
    node *denumerator = nullptr;
    
    node *first_sum = nullptr;
    
    node *second_sum = nullptr;

    node *default_numerator = nullptr;

    copy_node_with_childrens(&default_numerator, &current_node->left_node);
    
    node *default_denumerator = nullptr;
    
    copy_node_with_childrens(&default_denumerator, &current_node->right_node);

    if ( false)//current_node->left_node->type == VAR_TYPE && current_node->right_node->type == CONST_TYPE)
    {
        node *default_node = nullptr;

        construct_node(&default_node, CONST_TYPE, 1);

        numerator = differentiate_internal(default_numerator, default_node);

        denumerator = default_denumerator;
    }

    else
    {
        first_sum = differentiate_internal(default_numerator,    default_denumerator);

        second_sum = differentiate_internal(default_denumerator, default_numerator);
            

        construct_node(&numerator, OPER_TYPE, SUB_OPER, first_sum, second_sum);

        construct_node(&denumerator, OPER_TYPE, POW_OPER, default_denumerator);

        construct_node(&denumerator->right_node, CONST_TYPE, 2);
    }

    construct_node(&current_node, OPER_TYPE, DIV_OPER, numerator, denumerator);
})

DERIVATIVE(MUL, "*", 3,
{

    node *first_factor = nullptr;

    copy_node_with_childrens(&first_factor, &current_node->left_node);
    
    node *second_factor = nullptr;

    copy_node_with_childrens(&second_factor, &current_node->right_node);


    node *first_sum = nullptr;

    first_sum = differentiate_internal(first_factor, second_factor);
        
    node *second_sum = nullptr;

    second_sum = differentiate_internal(second_factor, first_factor);


    construct_node(&current_node, OPER_TYPE, SUM_OPER, first_sum, second_sum);

    construct_node(&current_node, OPER_TYPE, SUM_OPER, first_sum, second_sum);


    make_simple(&current_node);
}
)

DERIVATIVE(COS, "cos", 1,
{
    node *external_derivative = nullptr;

    construct_node(&external_derivative, OPER_TYPE, MUL_OPER);

    construct_node(&external_derivative->right_node, OPER_TYPE, SIN_OPER, current_node->left_node, nullptr);
    
    construct_node(&external_derivative->right_node, CONST_TYPE, -1);
    
    current_node = differentiate_internal(current_node->left_node, external_derivative);
})

DERIVATIVE(SIN, "sin", 1,
{
    node *external_derivative = nullptr;

    construct_node(&external_derivative, OPER_TYPE, COS_OPER, current_node->left_node, nullptr);
        
    current_node = differentiate_internal(current_node->left_node, external_derivative);
})

DERIVATIVE(TG, "tg", 1,
{
    node *numerator = nullptr;

    construct_node(&numerator, CONST_TYPE, 1);
    
    node *cos_node = nullptr;

    construct_node(&cos_node, OPER_TYPE, COS_OPER, nullptr, current_node->right_node);
    
    node *denumerator = nullptr;
    
    construct_node(&denumerator, OPER_TYPE, POW_OPER, cos_node);
        
    construct_node(&denumerator->right_node, CONST_TYPE, 2);

    node *external_derivative = nullptr;
    
    construct_node(&external_derivative, OPER_TYPE, DIV_OPER, numerator, denumerator);

    current_node = differentiate_internal(current_node->right_node, external_derivative);
})

DERIVATIVE(CTG, "ctg", 1,
{
    node *numerator = nullptr;

    construct_node(&numerator, CONST_TYPE, -1);
    
    node *cos_node = nullptr;

    construct_node(&cos_node, OPER_TYPE, SIN_OPER, nullptr, current_node->right_node);
    
    node *denumerator = nullptr;
    
    construct_node(&denumerator, OPER_TYPE, POW_OPER, cos_node);
        
    construct_node(&denumerator->right_node, CONST_TYPE, 2);

    node *external_derivative = nullptr;
    
    construct_node(&external_derivative, OPER_TYPE, DIV_OPER, numerator, denumerator);

    current_node = differentiate_internal(current_node->right_node, external_derivative);
})

DERIVATIVE(LN, "ln", 1,
{
    node *numerator = nullptr;

    construct_node(&numerator, CONST_TYPE, 1);
    
    node *denumerator = nullptr;
    
    construct_node(&denumerator, OPER_TYPE, current_node->right_node->type, current_node->right_node);
        
    node *external_derivative = nullptr;
    
    construct_node(&external_derivative, OPER_TYPE, DIV_OPER, numerator, denumerator);

    current_node = differentiate_internal(current_node->right_node, external_derivative);
})

DERIVATIVE(LOG, "log", 1,
{
    node *numerator = nullptr;

    construct_node(&numerator, CONST_TYPE, 1);

    node *denumerator = nullptr;

    node *log_arg = nullptr;

    copy_node_with_childrens(&log_arg, &current_node->right_node);
    
    node *ln_node = nullptr;
    
    construct_node(&ln_node, OPER_TYPE, LN_OPER, nullptr, log_arg);

    construct_node(&denumerator, OPER_TYPE, MUL_OPER, ln_node, log_arg);    

    node *external_derivative = nullptr;
    
    construct_node(&external_derivative, OPER_TYPE, DIV_OPER, numerator, denumerator);

    current_node = differentiate_internal(current_node->right_node, external_derivative);
})