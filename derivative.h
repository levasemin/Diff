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

    construct_node(&expression, current_node->left_node->type, current_node->left_node->value, current_node->left_node->left_node, current_node->left_node->right_node);
    
    node *degree = nullptr;

    construct_node(&degree, current_node->right_node->type, current_node->right_node->value, current_node->right_node->left_node, current_node->right_node->right_node);

    node *external_derivative = nullptr;

    construct_node(&external_derivative, OPER_TYPE, MUL_OPER, expression, degree);
    
    if (degree->value - 1 != 1)
    {
        printf("%d", degree->value - 1);
        node *pow_node = nullptr;

        construct_node(&pow_node, OPER_TYPE, POW_OPER, expression);
        construct_node(&pow_node->right_node, CONST_TYPE, degree->value - 1);
        
        external_derivative->left_node = pow_node;
    }
    
    current_node = differentiate_internal(current_node, expression, external_derivative);
})

DERIVATIVE(DIV, "/", 1,
{
    node *first_sum = nullptr;

    first_sum = differentiate_internal(current_node, current_node->left_node, current_node->right_node);
        
    node *second_sum = nullptr;

    second_sum = differentiate_internal(current_node, current_node->right_node, current_node->left_node);
    
    node *numerator = nullptr;
    
    construct_node(&numerator, OPER_TYPE, SUB_OPER, first_sum, second_sum);

    node *denumerator = nullptr;

    construct_node(&denumerator, OPER_TYPE, POW_OPER, current_node->right_node);

    construct_node(&denumerator->right_node, CONST_TYPE, 2);

    construct_node(&current_node, OPER_TYPE, DIV_OPER, numerator, denumerator);
})

DERIVATIVE(MUL, "*", 1,
{
    node *first_factor = nullptr;

    construct_node(&first_factor, current_node->left_node->type, current_node->left_node->value, current_node->left_node->left_node, current_node->left_node->right_node);
    
    node *second_factor = nullptr;

    construct_node(&second_factor, current_node->right_node->type, current_node->right_node->value, current_node->right_node->left_node, current_node->right_node->right_node);


    node *first_sum = nullptr;

    first_sum = differentiate_internal(current_node, first_factor, second_factor);
        
    node *second_sum = nullptr;

    second_sum = differentiate_internal(current_node, second_factor, first_factor);


    construct_node(&current_node, OPER_TYPE, SUM_OPER, first_sum, second_sum);
}
)

DERIVATIVE(COS, "cos", 1,
{
    node *external_derivative = nullptr;

    construct_node(&external_derivative, OPER_TYPE, MUL_OPER);

    construct_node(&external_derivative->left_node, OPER_TYPE, SIN_OPER, nullptr, current_node->right_node);
    
    construct_node(&external_derivative->right_node, CONST_TYPE, -1);
    
    current_node = differentiate_internal(current_node, current_node->right_node, external_derivative);
})

DERIVATIVE(SIN, "sin", 1,
{
    node *external_derivative = nullptr;

    construct_node(&external_derivative, OPER_TYPE, COS_OPER, nullptr, current_node->right_node);
        
    current_node = differentiate_internal(external_derivative, current_node->right_node, external_derivative);
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

    current_node = differentiate_internal(current_node, current_node->right_node, external_derivative);
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

    current_node = differentiate_internal(current_node, current_node->right_node, external_derivative);
})

DERIVATIVE(LN, "ln", 1,
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

    current_node = differentiate_internal(current_node, current_node->right_node, external_derivative);
})  