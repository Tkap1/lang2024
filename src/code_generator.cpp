

func b8 generate_code(s_node* ast, s_lin_arena* arena)
{
	t_code_builder* builder = (t_code_builder*)arena->alloc_zero(sizeof(t_code_builder));
	builder->add_line("#include <stdint.h>");
	builder->add_line("#include <math.h>");
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
	builder->add_line("void* memcpy(void* restrict dest, const void* restrict src, size_t n);");
	s_code_gen_context context = zero;
	for_node(node, ast) {
		generate_node(node, builder, context);
	}

	write_file("output.c", builder->data, builder->len);

	return true;
}

func void generate_node(s_node* node, t_code_builder* builder, s_code_gen_context context)
{
	if(node->dont_generate) { return; }

	switch(node->type) {
		case e_node_func_decl: {
			node_to_c_str(node->func_decl.return_type, builder, context);
			builder->add(" %s(", node->func_decl.name.str());
			if(node->func_decl.argument_count <= 0) {
				builder->add("void");
			}
			else {
				for_node(arg, node->func_decl.arguments) {
					generate_func_decl_arg(arg, builder, node->func_decl.is_external);
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
				generate_statement(node->func_decl.body, builder);
			}
		} break;

		case e_node_struct: {
			builder->add_line_tabs("typedef struct %s", node->token.str());
			builder->push_scope();
			for_node(member, node->nstruct.members) {
				generate_struct_member(member, builder);
			}
			builder->pop_scope("%s", format_str(" %s;", node->token.str()));
		} break;

		case e_node_var_decl: {
			generate_statement(node, builder);
		} break;

		case e_node_enum: {
			builder->add_line("typedef enum %s", node->token.str());
			builder->push_scope();
			for_node(member, node->nenum.members) {
				builder->add_line_tabs("%s_%s,", node->token.str(), member->token.str());
			}
			builder->pop_scope(" %s;", node->token.str());
		} break;

		case e_node_data_enum: {
			builder->add_line("typedef enum %s", node->token.str());
			builder->push_scope();
			for_node(member, node->data_enum.members) {
				builder->add_line_tabs("%s_%s,", node->token.str(), member->token.str());
			}
			builder->pop_scope(" %s;", node->token.str());
			generate_node(node->data_enum.nstruct, builder, context);

			builder->add_line("static const %s %s_data[] = {", node->data_enum.nstruct->token.str(), node->token.str());
			builder->scope += 1;
			for_node(member, node->data_enum.members) {
				b8 is_count_member = member->next == null;
				if(is_count_member) { continue; }
				builder->add_tabs("{");
				for_node(field, member->data_enum_member.members) {
					builder->add(".%s = ", field->token.str());
					node_to_c_str(field->left, builder, context);
					if(field->next) {
						builder->add(", ");
					}
				}
				builder->add_line("},");
			}
			builder->add_line("};");
			builder->scope -= 1;

		} break;

		invalid_default_case;
	}
}

func void generate_func_decl_arg(s_node* node, t_code_builder* builder, b8 is_external)
{
	s_code_gen_context context = zero;
	context.is_func_arg = true;
	if(is_external) {
		node_to_c_str(node, builder, context);
		get_suffix_str(node, builder);
	}
	else {
		assert(node->type == e_node_var_decl);
		node_to_c_str(node->var_decl.type, builder, context);
		builder->add(" %s", node->var_decl.name.str());
		// get_suffix_str(node->var_decl.type, builder);
	}
}

func void generate_statement(s_node* node, t_code_builder* builder)
{
	if(node->dont_generate) { return; }

	s_code_gen_context context = zero;
	switch(node->type) {
		// case e_node_func_call: {
		// 	node_to_c_str(node->left, builder, context);
		// 	builder->add_tabs("(");
		// 	for_node(arg, node->func_call.arguments) {
		// 		node_to_c_str(arg, builder, context);
		// 		if(arg->next) {
		// 			builder->add(", ");
		// 		}
		// 	}
		// 	builder->add_line(");");
		// } break;

		case e_node_assign: {
			// @TODO(tkap, 17/02/2024): Massive copy paste from var_decl:

			if(node->left->var_type->type == e_node_array) {
				if(node->left->var_type->pointer_level <= 0) {
					builder->add_tabs("memcpy(");
					node_to_c_str(node->left, builder, context);
					builder->add(", ");
					node_to_c_str(node->right, builder, context);
					builder->add(", %i)", node->left->var_type->size_in_bytes);
					builder->add_line(";");
				}
				else {
					builder->add_tabs("");
					node_to_c_str(node->left, builder, context);
					builder->add(" = ");
					node_to_c_str(node->right, builder, context);
					builder->add_line(";");
					// builder->add_tabs("%s", get_name(node->var_decl.type));
					// for(int i = 0; i < node->var_decl.type->pointer_level; i++) {
					// 	builder->add("*");
					// }
					// builder->add(" %s = ", node->var_decl.name.str());
					// node_to_c_str(node->var_decl.value, builder, context);
					// builder->add_line(";");
				}
			}
			else {
				builder->add_tabs("");
				node_to_c_str(node->left, builder, context);
				builder->add(" = ");
				s_code_gen_context temp = context;
				temp.prefix_struct_literal = true;
				node_to_c_str(node->right, builder, temp);
				builder->add_line(";");
			}
		} break;

		case e_node_return: {
			builder->add_tabs("return");
			if(node->nreturn.expression) {
				builder->add(" ");
				node_to_c_str(node->nreturn.expression, builder, {.prefix_struct_literal = true});
			}
			builder->add_line(";");
		} break;

		case e_node_var_decl: {
			if(node->var_decl.type->type == e_node_array) {
				if(node->var_decl.type->pointer_level <= 0) {
					builder->add_tabs("");
					node_to_c_str(node->var_decl.type, builder, context);
					builder->add(" %s", node->var_decl.name.str());
					get_suffix_str(node->var_decl.type, builder);
					builder->add_line(";");
					if(node->var_decl.value) {
						builder->add_tabs("memcpy(%s, ", node->var_decl.name.str());
						node_to_c_str(node->var_decl.value, builder, context);
						builder->add_line(", %i);", node->var_type->size_in_bytes);
					}
				}
				else {
					builder->add_tabs("%s", get_name(node->var_decl.type));
					for(int i = 0; i < node->var_decl.type->pointer_level; i++) {
						builder->add("*");
					}
					builder->add(" %s = ", node->var_decl.name.str());
					node_to_c_str(node->var_decl.value, builder, context);
					builder->add_line(";");
				}
			}
			else {
				builder->add_tabs("");
				node_to_c_str(node->var_decl.type, builder, context);
				builder->add(" %s", node->var_decl.name.str());
				get_suffix_str(node->var_decl.type, builder);

				if(node->var_decl.value) {
					builder->add(" = ");
					node_to_c_str(node->var_decl.value, builder, context);
				}
				else {
					// @TODO(tkap, 10/02/2024): we need to know if this is a struct or not
					builder->add(" = {0}");
				}
				builder->add_line(";");
			}
		} break;

		case e_node_while: {
			builder->add_tabs("while(");
			if(!node->nwhile.condition) {
				builder->add("1");
			}
			else {
				node_to_c_str(node->nwhile.condition, builder, context);
			}
			builder->add_line(")");
			generate_statement(node->nwhile.body, builder);
		} break;

		case e_node_for: {
			char* iterator_name = node->nfor.iterator_index_name.str();
			builder->add_tabs("for(int %s = 0; %s < ", iterator_name, iterator_name);
			node_to_c_str(node->nfor.upper_bound, builder, context);
			builder->add_line("; %s += 1)", iterator_name);
			generate_statement(node->nfor.body, builder);
		} break;

		case e_node_continue: {
			builder->add_line_tabs("continue;");
		} break;

		case e_node_break: {
			builder->add_line_tabs("break;");
		} break;

		case e_node_if: {
			builder->add_tabs("if(");
			node_to_c_str(node->nwhile.condition, builder, context);
			builder->add_line(")");
			generate_statement(node->nif.body, builder);
			if(node->nif.nelse) {
				builder->add_line_tabs("else");
				generate_statement(node->nif.nelse, builder);
			}
		} break;

		case e_node_compound: {
			builder->push_scope();
			for_node(statement, node->compound.statements) {
				generate_statement(statement, builder);
			}
			builder->pop_scope();
		} break;

		default: {
			builder->add_tabs("");
			node_to_c_str(node, builder, context);
			builder->add_line(";");
		}
	}
}

func void generate_struct_member(s_node* node, t_code_builder* builder)
{
	s_code_gen_context context = zero;
	switch(node->type) {
		case e_node_var_decl: {
			builder->add_tabs("");
			node_to_c_str(node->var_decl.type, builder, context);
			builder->add(" %s", node->var_decl.name.str());
			get_suffix_str(node->var_decl.type, builder);
			builder->add_line(";");
		} break;
		invalid_default_case;
	}
}

func void node_to_c_str(s_node* node, t_code_builder* builder, s_code_gen_context context)
{
	if(node->dont_generate) { return; }

	switch(node->type) {

		case e_node_type: {
			builder->add("%s", node->token.str());
			for(int i = 0; i < node->pointer_level; i++) {
				builder->add("*");
			}
		} break;

		case e_node_integer: {
			builder->add("%i", node->integer.value);
		} break;

		case e_node_float: {
			builder->add("%ff", node->nfloat.value);
		} break;

		case e_node_identifier: {
			builder->add("%s", node->token.str());
			if(context.is_data_enum_struct_access) {
				builder->add("_data");
			}
		} break;

		// @TODO(tkap, 17/02/2024): I dont get this
		case e_node_array: {
			if(context.is_func_arg) {
				generate_array_base_type(node, builder);
			}
			else {
				node_to_c_str(node->left, builder, context);
			}
		} break;

		case e_node_string: {
			builder->add("\"%s\"", node->token.str());
		} break;

		case e_node_logic_not: {
			builder->add("!");
			node_to_c_str(node->left, builder, context);
		} break;

		case e_node_dereference: {
			builder->add("*");
			node_to_c_str(node->left, builder, context);
		} break;

		case e_node_unary_minus: {
			builder->add("(-");
			node_to_c_str(node->left, builder, context);
			builder->add(")");
		} break;

		case e_node_address_of: {
			if(node->left->var_type->type == e_node_array) {
				builder->add("(");
				generate_array_base_type(node->left, builder);
				builder->add(")");
				node_to_c_str(node->left, builder, context);
			}
			else {
				builder->add("(&");
				node_to_c_str(node->left, builder, context);
				builder->add(")");
			}
		} break;

		case e_node_member_access: {
			if(node->var_type->type == e_node_enum) {
				assert(node->right->type == e_node_identifier);
				// @TODO(tkap, 17/02/2024): We want the enum member somewhere
				builder->add("%i", node->right->temp_var_decl->enum_value);
			}
			else if(node->var_type->type == e_node_data_enum) {
				assert(node->right->type == e_node_identifier);
				// @TODO(tkap, 17/02/2024): We want the enum member somewhere
				builder->add("%i", node->right->temp_var_decl->enum_value);
			}
			else {
				if(node->var_type->is_data_enum_struct_access) {
					// @TODO(tkap, 21/02/2024): sus?
					builder->add(
						"%s_data[%s_%s].%s", node->left->var_type->token.str(), node->left->var_type->token.str(),
						node->left->right->token.str(), node->right->token.str()
					);

					// @TODO(tkap, 20/02/2024): This kind of thing feels like it should happen in the type checking stage!
					// s_node* idk = get_data_enum_member(node->left);
					// assert(idk->type == e_node_data_enum_member);
					// b8 found = false;
					// for_node(member, idk->data_enum_member.members) {
					// 	// @TODO(tkap, 20/02/2024): sus?
					// 	if(member->token.equals(node->right->token)) {
					// 		node_to_c_str(member->left, builder, context);
					// 		found = true;
					// 		break;
					// 	}
					// }
					// assert(found);
				}
				else {
					node_to_c_str(node->left, builder, context);
					if(node->left->temp_var_decl && node->left->temp_var_decl->pointer_level > 0) {
						builder->add("->");
					}
					else {
						builder->add(".");
					}
					node_to_c_str(node->right, builder, context);
				}
			}
		} break;

		case e_node_func_call: {
			node_to_c_str(node->left, builder, context);
			builder->add("(");
			for_node(arg, node->func_call.arguments) {
				node_to_c_str(arg, builder, context);
				if(arg->next) {
					builder->add(", ");
				}
			}
			builder->add(")");
		} break;

		case e_node_modulo: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" %% ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_add: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" + ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_divide: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" / ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_subtract: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" - ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_logic_or: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" || ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_logic_and: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" && ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_comparison: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" == ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_not_equals: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" != ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_multiply: {
			s_node* nfunc = node->operator_overload_func;
			if(nfunc) {
				builder->add("%s(", nfunc->func_decl.name.str());
				node_to_c_str(node->left, builder, context);
				builder->add(", ");
				node_to_c_str(node->right, builder, context);
				builder->add(")");
			}
			else {
				builder->add("(");
				node_to_c_str(node->left, builder, context);
				builder->add(" * ");
				node_to_c_str(node->right, builder, context);
				builder->add(")");
			}
		} break;

		case e_node_greater_than: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" > ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_greater_than_or_equal: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" >= ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_less_than_or_equal: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" <= ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_less_than: {
			builder->add("(");
			node_to_c_str(node->left, builder, context);
			builder->add(" < ");
			node_to_c_str(node->right, builder, context);
			builder->add(")");
		} break;

		case e_node_struct_literal: {
			if(context.prefix_struct_literal) {
				builder->add("(");
				node_to_c_str(node->var_type, builder, context);
				builder->add(")");
			}
			builder->add("{");
			for_node(expr, node->struct_literal.expressions) {
				node_to_c_str(expr, builder, context);
				if(expr->next) {
					builder->add(", ");
				}
			}
			builder->add("}");
		} break;

		case e_node_struct: {
			builder->add("%s", node->token.str());
		} break;

		case e_node_subscript: {
			builder->add("(*(");
			s_code_gen_context temp = context;
			temp.is_data_enum_struct_access = node->var_type->is_data_enum_struct_access;
			get_subscript_str(node, 0, builder, temp);
			builder->add("))");
		} break;

		invalid_default_case;
	}
}

// @TODO(tkap, 17/02/2024): bad name
func void get_suffix_str(s_node* node, t_code_builder* builder)
{
	s_code_gen_context context = zero;
	switch(node->type) {
		case e_node_array: {
			get_suffix_str(node->left, builder);
			builder->add("[");
			node_to_c_str(node->array.size_expr, builder, context);
			builder->add("]");
		} break;
		default: return;
	}
}

func void generate_array_base_type(s_node* node, t_code_builder* builder)
{
	switch(node->type) {
		case e_node_array: {
			generate_array_base_type(node->left, builder);
		} break;

		case e_node_type: {
			builder->add("%s*", node->token.str());
		} break;

		// @TODO(tkap, 17/02/2024): Questionable that this is here given the function name
		case e_node_identifier: {
			generate_array_base_type(node->var_type, builder);
		} break;

		case e_node_member_access: {
			generate_array_base_type(node->var_type, builder);
		} break;

		invalid_default_case;
	}
}

func void get_subscript_str(s_node* node, int level, t_code_builder* builder, s_code_gen_context context)
{
	assert(node->type == e_node_subscript);

	if(level > 0 && node->left->type == e_node_subscript) {
		get_subscript_str(node->left, level + 1, builder, context);
		builder->add(" + (");
		node_to_c_str(node->right, builder, context);
		builder->add(" * %i)", node->array_capacity);
		return;
	}
	if(level > 0 && node->left->type != e_node_subscript) {
		builder->add("(");
		generate_array_base_type(node->left, builder);
		builder->add(")");
		node_to_c_str(node->left, builder, context);
		builder->add(" + (");
		node_to_c_str(node->right, builder, context);
		builder->add(" * %i)", node->array_capacity);
		return;
	}
	if(node->left->type == e_node_subscript) {
		get_subscript_str(node->left, level + 1, builder, context);
		builder->add(" + ");
		node_to_c_str(node->right, builder, context);
		return;
	}
	if(node->left->type != e_node_subscript) {
		node_to_c_str(node->left, builder, context);
		builder->add(" + ");
		node_to_c_str(node->right, builder, context);
		return;
	}

	// char* result = node_to_c_str(node->right, builder, context);

	// if(level > 0) {
	// 	result = format_str("(%s*%i)", result, node->array_capacity);
	// }
	// if(node->left->type == e_node_subscript) {
	// 	result = format_str("%s+%s", get_subscript_str(node->left, level + 1, context), result);
	// }
	// else {
	// 	result = format_str("%s+%s", node_to_c_str(node->left, context), result);
	// }
	// return result;
}

func char* get_name(s_node* node)
{
	switch(node->type) {
		case e_node_array: {
			return get_name(node->left);
		} break;

		case e_node_type: {
			return node->token.str();
		} break;

		invalid_default_case;
	}
	return null;
}

func s_node* get_data_enum_member(s_node* node)
{
	assert(node->type == e_node_member_access);
	assert(node->left->type == e_node_identifier);
	assert(node->right->type == e_node_identifier);
	return node->right->temp_var_decl;

}