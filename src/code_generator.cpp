

func void generate_code(s_node* ast, s_lin_arena* arena)
{
	t_code_builder* builder = (t_code_builder*)arena->alloc_zero(sizeof(t_code_builder));
	for_node(node, ast) {
		generate_node(node, builder);
	}

	write_file("output.c", builder->data, builder->len);
}

func void generate_node(s_node* node, t_code_builder* builder)
{
	switch(node->type) {
		case e_node_func_decl: {
			builder->add("%s ", node_to_c_str(node->func_decl.return_type));
			builder->add("%s(", node->func_decl.name.str());
			for_node(arg, node->func_decl.arguments) {
				builder->add("%s", node_to_c_str(arg));
				// builder->add("%s ", node_to_c_str(arg->var_decl.type));
				// builder->add("%s", arg->var_decl.name.str());
				if(arg->next) {
					builder->add(", ");
				}
			}
			builder->add_line(")");
			generate_node(node->func_decl.body, builder);
		} break;

		case e_node_compound: {
			builder->push_scope();
			for_node(statement, node->compound.statements) {
				generate_node(statement, builder);
			}
			builder->pop_scope();
		} break;

		case e_node_return: {
			builder->add_line("return %s;", node_to_c_str(node->nreturn.expression));
		} break;

		invalid_default_case;
	}
}

func char* node_to_c_str(s_node* node)
{
	switch(node->type) {
		case e_node_type: {
			return node->token.str();
		} break;

		case e_node_integer: {
			return node->token.str();
		} break;

		case e_node_var_decl: {
			char* str = node_to_c_str(node->var_decl.type);
			str = format_str("%s %s%s", str, node->var_decl.name.str(), get_suffix_str(node->var_decl.type));
			return str;
		} break;

		case e_node_array: {
			return node_to_c_str(node->left);
		} break;

		invalid_default_case;
	}
	return null;
}

func char* get_suffix_str(s_node* node)
{
	switch(node->type) {
		case e_node_array: {
			return format_str("[%s]", node_to_c_str(node->array.size_expr));
		} break;
		default: return "";
	}
}