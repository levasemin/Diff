if ((*main_node)->right_node->type == CONST_TYPE)
        {
            (*main_node)->right_node->value = operation((*main_node)->right_node->value, (*current_node)->right_node->value);
        }

        else if ((*main_node)->left_node->type == CONST_TYPE)
        {
            (*main_node)->left_node->value = operation((*main_node)->left_node->value, (*current_node)->right_node->value);
        }

        else
        {
            node *new_node = nullptr;

            if (*current_node != (*main_node)->right_node)
            {
                construct_node(&new_node, OPER_TYPE, type_oper, (*current_node)->left_node, (*main_node)->right_node);
            }

            else
            {
                construct_node(&new_node, OPER_TYPE, type_oper, (*current_node)->left_node, (*main_node)->left_node);
            }

            (*main_node)->left_node = (*current_node)->right_node;

            (*main_node)->right_node = new_node;

            return rec(main_node, &(*main_node)->right_node, type_oper, operation);
        }

        return (*current_node)->left_node; 