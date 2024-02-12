

func b8 generate_code(s_node* ast, s_lin_arena* arena)
{
	t_code_builder* builder = (t_code_builder*)arena->alloc_zero(sizeof(t_code_builder));
	builder->add_line("#include <stdint.h>");
	builder->add_line("typedef int8_t s8;");
	builder->add_line("typedef int16_t s16;");
	builder->add_line("typedef int32_t s32;");
	builder->add_line("typedef int64_t s64;");
	builder->add_line("typedef uint8_t u8;");
	builder->add_line("typedef uint16_t u16;");
	builder->add_line("typedef uint32_t u32;");
	builder->add_line("typedef uint64_t u64;");
	builder->add_line("typedef uint8_t b8;");
	builder->add_line("typedef uint32_t b32;");
	for_node(node, ast) {
		generate_node(node, builder);
	}

	write_file("output.c", builder->data, builder->len);

	return true;
}

func void generate_node(s_node* node, t_code_builder* builder)
{
	switch(node->type) {
		case e_node_func_decl: {
			builder->add("%s ", node_to_c_str(node->func_decl.return_type));
			builder->add("%s(", node->func_decl.name.str());
			if(node->func_decl.argument_count <= 0) {
				builder->add("void");
			}
			else {
				for_node(arg, node->func_decl.arguments) {
					generate_node(arg, builder);
					// builder->add("%s", node_to_c_str(arg));
					// builder->add("%s ", node_to_c_str(arg->var_decl.type));
					// builder->add("%s", arg->var_decl.name.str());
					if(arg->next) {
						builder->add(", ");
					}
				}
			}
			if(node->func_decl.is_external) {
				builder->add_line(");");
			}
			else {
				builder->add_line(")");
				generate_node(node->func_decl.body, builder);
			}
		} break;

		case e_node_struct: {
			builder->add_line_tabs("typedef struct %s", node->token.str());
			builder->push_scope();
			for_node(member, node->nstruct.members) {
				generate_struct_member(member, builder);
			}
			builder->pop_scope(format_str(" %s;", node->token.str()));
		} break;

		case e_node_var_decl: {
			builder->add("%s %s%s", node_to_c_str(node->var_decl.type), node->var_decl.name.str(), get_suffix_str(node->var_decl.type));

			if(node->var_decl.value) {
				builder->add(" = %s", node_to_c_str(node->var_decl.value));
			}
			else {
				// @TODO(tkap, 10/02/2024): we need to know if this is a struct or not
				builder->add(" = {0}");
			}
			builder->add_line(";");
		} break;

		case e_node_compound: {
			builder->push_scope();
			for_node(statement, node->compound.statements) {
				generate_statement(statement, builder);
			}
			builder->pop_scope();
		} break;

		case e_node_type: {
			builder->add("%s", node_to_c_str(node));
		} break;

		invalid_default_case;
	}
}

func void generate_statement(s_node* node, t_code_builder* builder)
{
	switch(node->type) {
		case e_node_func_call: {
			builder->add_tabs("%s(", node_to_c_str(node->left));
			for_node(arg, node->func_call.arguments) {
				builder->add("%s", node_to_c_str(arg));
				if(arg->next) {
					builder->add(", ");
				}
			}
			builder->add_line(");");
		} break;

		case e_node_assign: {
			builder->add_line_tabs("%s = %s;", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_return: {
			builder->add_line_tabs("return %s;", node_to_c_str(node->nreturn.expression));
		} break;

		case e_node_var_decl: {
			builder->add_tabs("%s %s%s", node_to_c_str(node->var_decl.type), node->var_decl.name.str(), get_suffix_str(node->var_decl.type));

			if(node->var_decl.value) {
				builder->add(" = %s", node_to_c_str(node->var_decl.value));
			}
			else {
				// @TODO(tkap, 10/02/2024): we need to know if this is a struct or not
				builder->add(" = {0}");
			}
			builder->add_line(";");
		} break;

		case e_node_while: {
			builder->add_tabs("while(");
			if(!node->nwhile.condition) {
				builder->add("1");
			}
			else {
				builder->add("%s", node_to_c_str(node->nwhile.condition));
			}
			builder->add_line(")");
			generate_node(node->nwhile.body, builder);
		} break;

		case e_node_for: {
			builder->add_line_tabs("for(int it = 0; it < %s; it += 1)", node_to_c_str(node->nfor.expr));
			generate_statement(node->nfor.body, builder);
		} break;

		case e_node_if: {
			builder->add_line_tabs("if(%s)", node_to_c_str(node->nwhile.condition));
			generate_node(node->nif.body, builder);
		} break;

		case e_node_compound: {
			builder->push_scope();
			for_node(statement, node->compound.statements) {
				generate_statement(statement, builder);
			}
			builder->pop_scope();
		} break;

		invalid_default_case;
	}
}

func void generate_struct_member(s_node* node, t_code_builder* builder)
{
	switch(node->type) {
		case e_node_var_decl: {
			builder->add_line_tabs("%s %s%s;", node_to_c_str(node->var_decl.type), node->var_decl.name.str(), get_suffix_str(node->var_decl.type));
		} break;
		invalid_default_case;
	}
}

func char* node_to_c_str(s_node* node)
{
	switch(node->type) {
		case e_node_type: {
			s_str_builder<1024> builder;
			builder.add("%s", node->token.str());
			for(int i = 0; i < node->pointer_level; i++) {
				builder.add("*");
			}
			return format_str(builder.data);
		} break;

		case e_node_integer: {
			return format_str("%i", node->integer.value);
		} break;

		case e_node_float: {
			return format_str("%sf", node->token.str());
		} break;

		case e_node_identifier: {
			return node->token.str();
		} break;

		case e_node_array: {
			return node_to_c_str(node->left);
		} break;

		case e_node_string: {
			return format_str("\"%s\"", node->token.str());
		} break;

		case e_node_logic_not: {
			char* str = node_to_c_str(node->left);
			return format_str("!%s", str);
		} break;

		case e_node_unary_minus: {
			char* str = node_to_c_str(node->left);
			return format_str("-%s", str);
		} break;

		case e_node_member_access: {
			return format_str("%s.%s", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_func_call: {
			s_str_builder<1024> builder = zero;
			builder.add("%s(", node_to_c_str(node->left));
			for_node(arg, node->func_call.arguments) {
				builder.add("%s", node_to_c_str(arg));
				if(arg->next) {
					builder.add(", ");
				}
			}
			builder.add(")", node_to_c_str(node->left));
			return format_str("%s", builder.data);
		} break;

		case e_node_modulo: {
			return format_str("(%s %% %s)", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_add: {
			return format_str("(%s + %s)", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_subtract: {
			return format_str("(%s - %s)", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_logic_or: {
			return format_str("(%s || %s)", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_logic_and: {
			return format_str("(%s && %s)", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_multiply: {
			return format_str("(%s * %s)", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_greater_than: {
			return format_str("(%s > %s)", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_less_than: {
			return format_str("(%s < %s)", node_to_c_str(node->left), node_to_c_str(node->right));
		} break;

		case e_node_struct_literal: {
			s_str_builder<1024> builder;
			// builder.add("(%s){", node_to_c_str(node->var_type));
			builder.add("{");
			for_node(expr, node->struct_literal.expressions) {
				builder.add("%s", node_to_c_str(expr));
				if(expr->next) {
					builder.add(", ");
				}
			}
			builder.add("}");
			return format_str("%s", builder.data);
		} break;

		invalid_default_case;
	}
	return null;
}

func char* get_suffix_str(s_node* node)
{
	switch(node->type) {
		case e_node_array: {
			return format_str("%s[%s]", get_suffix_str(node->left), node_to_c_str(node->array.size_expr));
		} break;
		default: return "";
	}
}