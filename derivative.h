DERIVATIVE(SUM, "+", 4,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }

    construct_node(&current_node, OPER_TYPE, SUM_OPER, differentiate_node(current_node->left_node),
                                                       differentiate_node(current_node->right_node));
    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }
},
{
    result = val1 + val2;
})


DERIVATIVE(SUB, "-", 4,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }
    
    construct_node(&current_node, OPER_TYPE, SUB_OPER, differentiate_node(current_node->left_node), 
                                                       differentiate_node(current_node->right_node));

    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }
},
{
    result = val1 - val2;
})


DERIVATIVE(POW, "^", 2,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }


    node *expression = nullptr;

    copy_node_with_childrens(&expression, &current_node->left_node);
    
    node *degree = nullptr;

    copy_node_with_childrens(&degree, &current_node->right_node);


    node *external_derivative = nullptr;
    
    
    node *pow_node = nullptr;

    construct_node(&pow_node, OPER_TYPE, POW_OPER, expression);

    if (current_node->left_node->type == EXP_TYPE)
    {
        copy_node_with_childrens(&pow_node->right_node, &degree);

        current_node = differentiate_internal(degree, current_node, dump_file);
    }

    else
    {
        construct_node(&external_derivative, OPER_TYPE, MUL_OPER, expression, degree);

        construct_node(&pow_node->right_node, CONST_TYPE, degree->value - 1);

        external_derivative->left_node = pow_node;

        current_node = differentiate_internal(expression, external_derivative, dump_file);
    }
    
    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }

},
{
    result = pow(val1, val2);
})


DERIVATIVE(DIV, "/", 3,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }

    node *numerator = nullptr;
    
    node *denumerator = nullptr;
    
    node *first_sum = nullptr;
    
    node *second_sum = nullptr;

    node *default_numerator = nullptr;


    copy_node_with_childrens(&default_numerator, &current_node->left_node);
    
    node *default_denumerator = nullptr;
    
    copy_node_with_childrens(&default_denumerator, &current_node->right_node);


    first_sum = differentiate_internal(default_numerator, default_denumerator, dump_file);

    second_sum = differentiate_internal(default_denumerator, default_numerator, dump_file);
            

    construct_node(&numerator,   OPER_TYPE, SUB_OPER, first_sum, second_sum);


    construct_node(&denumerator, OPER_TYPE, POW_OPER, default_denumerator);

    construct_node(&denumerator->right_node, CONST_TYPE, 2);

    construct_node(&current_node, OPER_TYPE, DIV_OPER, numerator, denumerator);
    
    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }

},
{
    if (compare_floats(val2, 0) == 0)
    {
        *derivative_exist = false;
    }

    else
    {
        result = val1 / val2;
    }
})


DERIVATIVE(MUL, "*", 3,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }


    node *first_factor = nullptr;

    copy_node_with_childrens(&first_factor, &current_node->left_node);
    
    node *second_factor = nullptr;

    copy_node_with_childrens(&second_factor, &current_node->right_node);


    node *first_sum = nullptr;

    first_sum = differentiate_internal(first_factor, second_factor, dump_file);
        
    node *second_sum = nullptr;

    second_sum = differentiate_internal(second_factor, first_factor, dump_file);

    construct_node(&current_node, OPER_TYPE, SUM_OPER, first_sum, second_sum);

    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }

},
{
    result = val1 * val2;
})


DERIVATIVE(COS, "cos", 1,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }


    node *external_derivative = nullptr;

    construct_node(&external_derivative, OPER_TYPE, MUL_OPER);

    construct_node(&external_derivative->right_node, OPER_TYPE, SIN_OPER, nullptr, current_node->right_node);
    
    construct_node(&external_derivative->left_node, CONST_TYPE, -1);
    
    current_node = differentiate_internal(current_node->right_node, external_derivative, dump_file);

    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }

}, 
{
    result = cos(val2);
})


DERIVATIVE(SIN, "sin", 1,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }

    node *external_derivative = nullptr;

    construct_node(&external_derivative, OPER_TYPE, COS_OPER, nullptr, current_node->right_node);
        
    current_node = differentiate_internal(current_node->right_node, external_derivative, dump_file);

    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }

},
{
    result = sin(val2);
})


DERIVATIVE(TG, "tan", 1,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }


    node *numerator = nullptr;

    construct_node(&numerator, CONST_TYPE, 1);
    
    node *cos_node = nullptr;

    construct_node(&cos_node, OPER_TYPE, COS_OPER, nullptr, current_node->right_node);
    
    node *denumerator = nullptr;
    
    construct_node(&denumerator, OPER_TYPE, POW_OPER, cos_node);
        
    construct_node(&denumerator->right_node, CONST_TYPE, 2);

    node *external_derivative = nullptr;
    
    construct_node(&external_derivative, OPER_TYPE, DIV_OPER, numerator, denumerator);

    current_node = differentiate_internal(current_node->right_node, external_derivative, dump_file);

    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }

},
{
    if (compare_floats(cos(val2), 0) == 0)
    {
        *derivative_exist = false;
    }
    else
    {
        result = sin(val2) / cos(val2);
    }
})


DERIVATIVE(CTG, "ctg", 1,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }


    node *numerator = nullptr;

    construct_node(&numerator, CONST_TYPE, -1);
    
    node *cos_node = nullptr;

    construct_node(&cos_node, OPER_TYPE, SIN_OPER, nullptr, current_node->right_node);
    
    node *denumerator = nullptr;
    
    construct_node(&denumerator, OPER_TYPE, POW_OPER, cos_node);
        
    construct_node(&denumerator->right_node, CONST_TYPE, 2);

    node *external_derivative = nullptr;
    
    construct_node(&external_derivative, OPER_TYPE, DIV_OPER, numerator, denumerator);

    current_node = differentiate_internal(current_node->right_node, external_derivative, dump_file);


    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }

},
{
    if (compare_floats(sin(val2), 0) == 0)
    {
        *derivative_exist = false;
    }

    else 
    {
        result = cos(val2) / sin(val2);
    }
})


DERIVATIVE(LN, "ln", 1,
{
    graph default_equation = {};

    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }


    node *numerator = nullptr;

    construct_node(&numerator, CONST_TYPE, 1);
    
    node *denumerator = nullptr;
    
    copy_node_with_childrens(&denumerator, &current_node->right_node);
        
    node *external_derivative = nullptr;
    
    construct_node(&external_derivative, OPER_TYPE, DIV_OPER, numerator, denumerator);

    current_node = differentiate_internal(current_node->right_node, external_derivative, dump_file);

    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }
},
{
    if (compare_floats(val2, 0) == 0)
    {
        *derivative_exist = false;
    }

    else
    {
        result = log(val2);
    }
})


DERIVATIVE(LOG, "log", 1,
{
    graph default_equation = {};
    
    if (dump_file != nullptr)
    {
        construct_graph(&default_equation);
    
        copy_node_with_childrens(&default_equation.root_node, &current_node);
    }


    node *numerator = nullptr;

    construct_node(&numerator, CONST_TYPE, 1);

    node *denumerator = nullptr;

    node *log_base = nullptr;
    
    copy_node_with_childrens(&log_base, &current_node->left_node);

    node *log_arg = nullptr;

    copy_node_with_childrens(&log_arg, &current_node->right_node);

    node *ln_node = nullptr;
    
    construct_node(&ln_node, OPER_TYPE, LN_OPER, nullptr, log_base);

    construct_node(&denumerator, OPER_TYPE, MUL_OPER, ln_node, log_arg);    

    node *external_derivative = nullptr;
    
    construct_node(&external_derivative, OPER_TYPE, DIV_OPER, numerator, denumerator);

    current_node = differentiate_internal(current_node->right_node, external_derivative, dump_file);

    if (dump_file != nullptr)
    {
        fprintf_default_derivative(dump_file, &(default_equation.root_node), &(current_node), num_derivative);
    
        destruct_graph(&default_equation);
    }

},
{
    if (compare_floats(val1, 0) == 0 || compare_floats(val2, 0) == 0)
    {
        *derivative_exist = false;
    }

    else if (compare_floats(log(val1), 0) == 0)
    {
        *derivative_exist = false;
    }

    else
    {
        result = log(val2) / log(val1);
    }
})