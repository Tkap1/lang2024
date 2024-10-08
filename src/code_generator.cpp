

func b8 generate_code(s_node* ast, s_lin_arena* arena, char* c_file_name)
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
		generate_node(node, builder, context, arena);
	}

	write_file(c_file_name, builder->data, builder->len);

	return true;
}

func void generate_node(s_node* node, t_code_builder* builder, s_code_gen_context context, s_lin_arena* arena)
{
	if(node->dont_generate) { return; }

	switch(node->type) {
		case e_node_func_decl: {
			if(node->func_decl.is_dll_export) {
				builder->add("__declspec(dllexport) ");
			}
			node_to_c_str(node->func_decl.return_type, builder, context, arena);
			builder->add(" %s(", node->func_decl.name.str(arena));
			if(!node->func_decl.is_method && node->func_decl.argument_count <= 0) {
				builder->add("void");
			}
			else {
				if(node->func_decl.is_method) {
					builder->add("%s* this", node->func_decl.base_struct.str(arena));
					if(node->func_decl.argument_count > 0) {
						builder->add(", ");
					}
				}
				for_node(arg, node->func_decl.arguments) {
					generate_func_decl_arg(arg, builder, node->func_decl.is_external, arena);
					// builder->add("%s", node_to_c_str(arg));
					// builder->add("%s ", node_to_c_str(arg->var_decl.type));
					// builder->add("%s", arg->var_decl.name.str(arena));
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
				generate_statement(node->func_decl.body, builder, context, arena);
			}
		} break;

		case e_node_struct: {
			b8 is_packed = node->nstruct.is_packed;
			if(is_packed) {
				builder->add_line_tabs("#pragma pack(push, 1)");
			}
			builder->add_line_tabs("typedef struct %s", node->token.str(arena));
			builder->push_scope();
			for_node(member, node->nstruct.members) {
				generate_struct_member(member, builder, arena);
			}
			builder->pop_scope("%s", alloc_str(arena, " %s;", node->token.str(arena)));
			if(is_packed) {
				builder->add_line_tabs("#pragma pack(pop)");
			}
		} break;

		case e_node_var_decl: {
			generate_statement(node, builder, context, arena);
		} break;

		case e_node_enum: {
			builder->add_line("typedef enum %s", node->token.str(arena));
			builder->push_scope();
			for_node(member, node->nenum.members) {
				builder->add_line_tabs("%s_%s,", node->token.str(arena), member->token.str(arena));
			}
			builder->pop_scope(" %s;", node->token.str(arena));
		} break;

		case e_node_data_enum: {
			builder->add_line("typedef enum %s", node->token.str(arena));
			builder->push_scope();
			for_node(member, node->data_enum.members) {
				builder->add_line_tabs("%s_%s,", node->token.str(arena), member->token.str(arena));
			}
			builder->pop_scope(" %s;", node->token.str(arena));
			generate_node(node->data_enum.nstruct, builder, context, arena);

			builder->add_line("static const %s %s_data[] = {", node->data_enum.nstruct->token.str(arena), node->token.str(arena));
			builder->scope += 1;
			for_node(member, node->data_enum.members) {
				b8 is_count_member = member->next == null;
				if(is_count_member) { continue; }
				builder->add_tabs("{");
				for_node(field, member->data_enum_member.members) {
					builder->add(".%s = ", field->token.str(arena));
					node_to_c_str(field->left, builder, context, arena);
					if(field->next) {
						builder->add(", ");
					}
				}
				builder->add_line("},");
			}
			builder->add_line("};");
			builder->scope -= 1;

		} break;

		case e_node_iterator: {

		} break;

		invalid_default_case;
	}
}

func void generate_func_decl_arg(s_node* node, t_code_builder* builder, b8 is_external, s_lin_arena* arena)
{
	if(node->type == e_node_var_args) {
		builder->add("...");
		return;
	}

	s_code_gen_context context = zero;
	context.is_func_arg = true;
	if(is_external) {
		node_to_c_str(node, builder, context, arena);
		get_suffix_str(node, builder, arena);
	}
	else {
		assert(node->type == e_node_var_decl);
		node_to_c_str(node->var_decl.type, builder, context, arena);
		builder->add(" %s", node->var_decl.name.str(arena));
		// get_suffix_str(node->var_decl.type, builder);
	}
}

func void generate_statement(s_node* node, t_code_builder* builder, s_code_gen_context context, s_lin_arena* arena)
{
	if(node->dont_generate) { return; }

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
					node_to_c_str(node->left, builder, context, arena);
					builder->add(", ");
					node_to_c_str(node->right, builder, context, arena);
					builder->add(", %i)", node->left->var_type->size_in_bytes);
					builder->add_line(";");
				}
				else {
					builder->add_tabs("");
					node_to_c_str(node->left, builder, context, arena);
					builder->add(" = ");
					node_to_c_str(node->right, builder, context, arena);
					builder->add_line(";");
					// builder->add_tabs("%s", get_name(node->var_decl.type));
					// for(int i = 0; i < node->var_decl.type->pointer_level; i++) {
					// 	builder->add("*");
					// }
					// builder->add(" %s = ", node->var_decl.name.str(arena));
					// node_to_c_str(node->var_decl.value, builder, context);
					// builder->add_line(";");
				}
			}
			else {
				builder->add_tabs("");
				node_to_c_str(node->left, builder, context, arena);
				builder->add(" = ");
				s_code_gen_context temp = context;
				temp.prefix_struct_literal = true;
				node_to_c_str(node->right, builder, temp, arena);
				builder->add_line(";");
			}
		} break;

		case e_node_return: {
			builder->add_tabs("return");
			if(node->nreturn.expression) {
				builder->add(" ");
				node_to_c_str(node->nreturn.expression, builder, {.prefix_struct_literal = true}, arena);
			}
			builder->add_line(";");
		} break;

		case e_node_var_decl: {
			if(node->var_decl.type->type == e_node_array) {
				if(node->var_decl.type->pointer_level <= 0) {
					builder->add_tabs("");
					node_to_c_str(node->var_decl.type, builder, context, arena);
					builder->add(" %s", node->var_decl.name.str(arena));
					get_suffix_str(node->var_decl.type, builder, arena);
					if(node->var_decl.value) {
						if(node->var_decl.value->type == e_node_array_literal) {
							builder->add(" = ");
							node_to_c_str(node->var_decl.value, builder, context, arena);
						}
						else {
							builder->add_line(";");
							builder->add_tabs("memcpy(%s, ", node->var_decl.name.str(arena));
							node_to_c_str(node->var_decl.value, builder, context, arena);
							builder->add_line(", %i);", node->var_type->size_in_bytes);
						}
					}
					builder->add_line(";");
				}
				else {
					builder->add_tabs("%s", get_name(node->var_decl.type, arena));
					for(int i = 0; i < node->var_decl.type->pointer_level; i++) {
						builder->add("*");
					}
					builder->add(" %s = ", node->var_decl.name.str(arena));
					node_to_c_str(node->var_decl.value, builder, context, arena);
					builder->add_line(";");
				}
			}
			else {
				builder->add_tabs("");
				node_to_c_str(node->var_decl.type, builder, context, arena);

				s_token name_token = node->var_decl.name;
				if(context.identifier_replacement.active && context.identifier_replacement.to_be_replaced.equals(name_token)) {
					name_token = context.identifier_replacement.replacement;
				}
				builder->add(" %s", name_token.str(arena));
				get_suffix_str(node->var_decl.type, builder, arena);

				if(node->var_decl.value) {
					builder->add(" = ");
					node_to_c_str(node->var_decl.value, builder, context, arena);
				}
				else {
					// @TODO(tkap, 10/02/2024): we need to know if this is a struct or not
					builder->add(" = {0}");
				}
				builder->add_line(";");
			}
		} break;

		case e_node_func_ptr: {
			builder->add_tabs("typedef ");
			node_to_c_str(node->func_ptr.return_type, builder, context, arena);
			builder->add("(*%s)(", node->func_ptr.name.str(arena));
			if(node->func_ptr.argument_count <= 0) {
				builder->add("void");
			}
			for_node(arg, node->func_ptr.arguments) {
				node_to_c_str(arg, builder, context, arena);
			}
			builder->add_line(");");
		} break;

		case e_node_while: {
			builder->add_tabs("while(");
			if(!node->nwhile.condition) {
				builder->add("1");
			}
			else {
				node_to_c_str(node->nwhile.condition, builder, context, arena);
			}
			builder->add_line(")");
			generate_statement(node->nwhile.body, builder, context, arena);
		} break;

		case e_node_simple_for: {
			s_token iterator_name = node->simple_for.iterator_name;
			s_token iterator_index_name = node->simple_for.iterator_index_name;
			if(context.identifier_replacement.active && context.identifier_replacement.to_be_replaced.equals(iterator_name)) {
				iterator_name = context.identifier_replacement.replacement;
				if(iterator_index_name.len > 0) {
					iterator_index_name = make_identifier_token(alloc_str(arena, "%s_index", iterator_name.str(arena)));
				}
			}
			if(iterator_index_name.len <= 0) {
				iterator_index_name = iterator_name;
			}
			char* iterator_index_name_str = iterator_index_name.str(arena);

			builder->add_tabs("for(int %s = 0; ", iterator_index_name_str);
			builder->add("%s < ", iterator_index_name_str);
			s_code_gen_context temp_context = context;
			if(node->simple_for.expr->var_type->type == e_node_array) {
				node_to_c_str(node->simple_for.expr->var_type->array.size_expr, builder, context, arena);
				assert(!iterator_name.equals(iterator_index_name));
				temp_context.var_decl_to_add.name = iterator_name;
				temp_context.var_decl_to_add.index = iterator_index_name;
				temp_context.var_decl_to_add.node = node->simple_for.expr;
				temp_context.var_decl_to_add.is_ptr = node->simple_for.loop_by_ptr;
			}
			else {
				node_to_c_str(node->simple_for.expr, builder, context, arena);
			}
			builder->add_line("; %s += 1)", iterator_index_name_str);
			generate_statement(node->simple_for.body, builder, temp_context, arena);
		} break;

		case e_node_range_for: {
			s_token iterator_name = node->range_for.iterator_name;
			if(context.identifier_replacement.active && context.identifier_replacement.to_be_replaced.equals(iterator_name)) {
				iterator_name = context.identifier_replacement.replacement;
			}
			char* iterator_name_str = iterator_name.str(arena);

			builder->add_tabs("for(int %s = ", iterator_name_str);
			node_to_c_str(node->range_for.lower_bound, builder, context, arena);
			builder->add("; %s < ", iterator_name_str);
			node_to_c_str(node->range_for.upper_bound, builder, context, arena);
			builder->add_line("; %s += 1)", iterator_name_str);
			generate_statement(node->range_for.body, builder, context, arena);
		} break;

		case e_node_continue: {
			builder->add_line_tabs("continue;");
		} break;

		case e_node_break: {
			builder->add_line_tabs("break;");
		} break;

		case e_node_if: {
			builder->add_tabs("if(");
			node_to_c_str(node->nwhile.condition, builder, context, arena);
			builder->add_line(")");
			generate_statement(node->nif.body, builder, context, arena);
			if(node->nif.nelse) {
				builder->add_line_tabs("else");
				generate_statement(node->nif.nelse, builder, context, arena);
			}
		} break;

		case e_node_compound: {
			builder->push_scope();
			if(context.var_decl_to_add.name.len > 0) {
				s_code_gen_context temp_context = context;
				context.var_decl_to_add.name = zero;
				builder->add_tabs("%s", get_name(temp_context.var_decl_to_add.node->var_type, arena));
				if(temp_context.var_decl_to_add.is_ptr) {
					builder->add("*");
				}
				builder->add(" %s = ", temp_context.var_decl_to_add.name.str(arena));
				if(temp_context.var_decl_to_add.is_ptr) {
					builder->add("&");
				}
				node_to_c_str(temp_context.var_decl_to_add.node, builder, context, arena);
				builder->add_line("[%s];", context.var_decl_to_add.index.str(arena));
			}
			for_node(statement, node->compound.statements) {
				generate_statement(statement, builder, context, arena);
			}
			builder->pop_scope();
		} break;

		case e_node_yield: {
			generate_statement(context.yield_replacement, builder, context, arena);
		} break;

		default: {
			builder->add_tabs("");
			node_to_c_str(node, builder, context, arena);
			builder->add_line(";");
		}
	}
}

func void generate_struct_member(s_node* node, t_code_builder* builder, s_lin_arena* arena)
{
	s_code_gen_context context = zero;
	switch(node->type) {
		case e_node_var_decl: {
			builder->add_tabs("");
			node_to_c_str(node->var_decl.type, builder, context, arena);
			builder->add(" %s", node->var_decl.name.str(arena));
			get_suffix_str(node->var_decl.type, builder, arena);
			builder->add_line(";");
		} break;
		invalid_default_case;
	}
}

func void node_to_c_str(s_node* node, t_code_builder* builder, s_code_gen_context context, s_lin_arena* arena)
{
	if(node->dont_generate) { return; }

	char* arithmetic_symbol = null;

	switch(node->type) {

		case e_node_type: {
			builder->add("%s", node->token.str(arena));
			for(int i = 0; i < node->pointer_level; i++) {
				builder->add("*");
			}
		} break;

		case e_node_integer: {
			builder->add("%lli", node->integer.value);
		} break;

		case e_node_float: {
			builder->add("%ff", node->nfloat.value);
		} break;

		case e_node_identifier: {
			s_token token = node->token;
			if(context.identifier_replacement.active && context.identifier_replacement.to_be_replaced.equals(token)) {
				token = context.identifier_replacement.replacement;
			}

			builder->add("%s", token.str(arena));
			if(context.is_data_enum_struct_access) {
				builder->add("_data");
			}
		} break;

		// @TODO(tkap, 17/02/2024): I dont get this
		case e_node_array: {
			if(context.is_func_arg) {
				generate_array_base_type(node, builder, arena);
			}
			else {
				node_to_c_str(node->left, builder, context, arena);
			}
		} break;

		case e_node_string: {
			builder->add("\"%s\"", node->token.str(arena));
		} break;

		case e_node_logic_not: {
			builder->add("!");
			node_to_c_str(node->left, builder, context, arena);
		} break;

		case e_node_dereference: {
			builder->add("*");
			node_to_c_str(node->left, builder, context, arena);
		} break;

		case e_node_unary_minus: {
			builder->add("(-");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_address_of: {
			if(node->left->var_type->type == e_node_array) {
				builder->add("(");
				generate_array_base_type(node->left, builder, arena);
				builder->add(")");
				node_to_c_str(node->left, builder, context, arena);
			}
			else {
				builder->add("(&");
				node_to_c_str(node->left, builder, context, arena);
				builder->add(")");
			}
		} break;

		case e_node_array_literal: {
			builder->add("{");
			for_node(expr, node->array_literal.expressions) {
				node_to_c_str(expr, builder, context, arena);
				if(expr->next) {
					builder->add(", ");
				}
			}
			builder->add("}");
		} break;

		case e_node_member_access: {
			if(node->var_type->type == e_node_enum) {
				assert(node->right->type == e_node_identifier);
				// @TODO(tkap, 17/02/2024): We want the enum member somewhere
				builder->add("%i", node->right->temp_var_decl->enum_value);
			}
			else if(node->left->var_type->type == e_node_array) {
				assert(node->right->token.equals("size"));
				node_to_c_str(node->left->var_type->array.size_expr, builder, context, arena);
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
						"%s_data[%s_%s].%s", node->left->var_type->token.str(arena), node->left->var_type->token.str(arena),
						node->left->right->token.str(arena), node->right->token.str(arena)
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
					node_to_c_str(node->left, builder, context, arena);
					// @TODO(tkap, 29/09/2024): I commented this out, it could totally break something
					// if(node->left->temp_var_decl && node->left->temp_var_decl->pointer_level > 0) {
					if(node->left->var_type->pointer_level > 0) {
						builder->add("->");
					}
					else {
						builder->add(".");
					}
					node_to_c_str(node->right, builder, context, arena);
				}
			}
		} break;

		case e_node_auto_cast: {
			// @TODO(tkap, 24/02/2024): Need to handle pointers, arrays, etc... ?
			builder->add("(");
			node_to_c_str(node->var_type, builder, context, arena);
			builder->add(")");
			node_to_c_str(node->left, builder, context, arena);

		} break;

		case e_node_iterator_call: {
			s_code_gen_context temp_context = context;
			temp_context.yield_replacement = node->func_call.body;
			temp_context.identifier_replacement.active = true;
			// @TODO(tkap, 28/09/2024): assert?? this wont work for multiple arguments
			temp_context.identifier_replacement.to_be_replaced = node->var_type->iterator.arguments->var_decl.name;
			temp_context.identifier_replacement.replacement = node->func_call.arguments->token;
			generate_statement(node->var_type->iterator.body, builder, temp_context, arena);
		} break;

		case e_node_func_call: {
			if(node->left->temp_var_decl && node->left->temp_var_decl->type == e_node_func_decl && node->left->temp_var_decl->func_decl.is_method) {
				s_code_gen_context temp = context;
				temp.prefix_struct_literal = true;
				node_to_c_str(node->left->right, builder, context, arena);
				builder->add("(");
				if(node->left->left->var_type->pointer_level <= 0) {
					builder->add("&");
				}
				node_to_c_str(node->left->left, builder, context, arena);
				if(node->func_call.arguments) {
					builder->add(", ");
				}
				for_node(arg, node->func_call.arguments) {
					node_to_c_str(arg, builder, temp, arena);
					if(arg->next) {
						builder->add(", ");
					}
				}
				builder->add(")");
			}
			else {
				node_to_c_str(node->left, builder, context, arena);
				builder->add("(");
				s_code_gen_context temp = context;
				temp.prefix_struct_literal = true;
				for_node(arg, node->func_call.arguments) {
					node_to_c_str(arg, builder, temp, arena);
					if(arg->next) {
						builder->add(", ");
					}
				}
				builder->add(")");
			}
		} break;

		case e_node_sizeof: {
			builder->add("sizeof(");
			node_to_c_str(node->func_call.arguments, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_modulo: {
			builder->add("(");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(" %% ");
			node_to_c_str(node->right, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_logic_or: {
			builder->add("(");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(" || ");
			node_to_c_str(node->right, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_logic_and: {
			builder->add("(");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(" && ");
			node_to_c_str(node->right, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_comparison: {
			builder->add("(");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(" == ");
			node_to_c_str(node->right, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_not_equals: {
			builder->add("(");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(" != ");
			node_to_c_str(node->right, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_add:
			arithmetic_symbol = "+";
			goto arithmethic;
		case e_node_subtract:
			arithmetic_symbol = "-";
			goto arithmethic;
		case e_node_multiply:
			arithmetic_symbol = "*";
			goto arithmethic;
		case e_node_divide:
			arithmetic_symbol = "/";
			goto arithmethic;
		{
			arithmethic:
			s_node* nfunc = node->operator_overload_func;
			if(nfunc) {
				builder->add("%s(", nfunc->func_decl.name.str(arena));
				node_to_c_str(node->left, builder, context, arena);
				builder->add(", ");
				node_to_c_str(node->right, builder, context, arena);
				builder->add(")");
			}
			else {
				builder->add("(");
				node_to_c_str(node->left, builder, context, arena);
				builder->add(" %s ", arithmetic_symbol);
				node_to_c_str(node->right, builder, context, arena);
				builder->add(")");
			}
		} break;

		case e_node_greater_than: {
			builder->add("(");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(" > ");
			node_to_c_str(node->right, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_greater_than_or_equal: {
			builder->add("(");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(" >= ");
			node_to_c_str(node->right, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_less_than_or_equal: {
			builder->add("(");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(" <= ");
			node_to_c_str(node->right, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_less_than: {
			builder->add("(");
			node_to_c_str(node->left, builder, context, arena);
			builder->add(" < ");
			node_to_c_str(node->right, builder, context, arena);
			builder->add(")");
		} break;

		case e_node_struct_literal: {
			if(context.prefix_struct_literal) {
				builder->add("(");
				node_to_c_str(node->var_type, builder, context, arena);
				builder->add(")");
			}
			builder->add("{");
			for_node(expr, node->struct_literal.expressions) {
				node_to_c_str(expr, builder, context, arena);
				if(expr->next) {
					builder->add(", ");
				}
			}
			builder->add("}");
		} break;

		case e_node_struct: {
			builder->add("%s", node->token.str(arena));
		} break;

		case e_node_subscript: {
			if(node->inside_sizeof) {
				node_to_c_str(node->left, builder, context, arena);
				builder->add("[");
				node_to_c_str(node->right, builder, context, arena);
				builder->add("]");
			}
			else {
				builder->add("(*(");
				s_code_gen_context temp = context;
				temp.is_data_enum_struct_access = node->var_type->is_data_enum_struct_access;
				get_subscript_str(node, 0, builder, temp, arena);
				builder->add("))");
			}
		} break;

		// case e_node_yield: {
		// 	generate_statement(context.yield_replacement, builder, arena);
		// } break;

		invalid_default_case;
	}
}

// @TODO(tkap, 17/02/2024): bad name
func void get_suffix_str(s_node* node, t_code_builder* builder, s_lin_arena* arena)
{
	s_code_gen_context context = zero;
	switch(node->type) {
		case e_node_array: {
			get_suffix_str(node->left, builder, arena);
			builder->add("[");
			node_to_c_str(node->array.size_expr, builder, context, arena);
			builder->add("]");
		} break;
		default: return;
	}
}

func void generate_array_base_type(s_node* node, t_code_builder* builder, s_lin_arena* arena)
{
	switch(node->type) {
		case e_node_array: {
			generate_array_base_type(node->left, builder, arena);
		} break;

		case e_node_type: {
			builder->add("%s*", node->token.str(arena));
		} break;

		// @TODO(tkap, 17/02/2024): Questionable that this is here given the function name
		case e_node_identifier: {
			generate_array_base_type(node->var_type, builder, arena);
		} break;

		case e_node_member_access: {
			generate_array_base_type(node->var_type, builder, arena);
		} break;

		invalid_default_case;
	}
}

func void get_subscript_str(s_node* node, int level, t_code_builder* builder, s_code_gen_context context, s_lin_arena* arena)
{
	assert(node->type == e_node_subscript);

	if(level > 0 && node->left->type == e_node_subscript) {
		get_subscript_str(node->left, level + 1, builder, context, arena);
		builder->add(" + (");
		node_to_c_str(node->right, builder, context, arena);
		builder->add(" * %i)", node->array_capacity);
		return;
	}
	if(level > 0 && node->left->type != e_node_subscript) {
		builder->add("(");
		generate_array_base_type(node->left, builder, arena);
		builder->add(")");
		node_to_c_str(node->left, builder, context, arena);
		builder->add(" + (");
		node_to_c_str(node->right, builder, context, arena);
		builder->add(" * %i)", node->array_capacity);
		return;
	}
	if(node->left->type == e_node_subscript) {
		get_subscript_str(node->left, level + 1, builder, context, arena);
		builder->add(" + ");
		node_to_c_str(node->right, builder, context, arena);
		return;
	}
	if(node->left->type != e_node_subscript) {
		node_to_c_str(node->left, builder, context, arena);
		builder->add(" + ");
		node_to_c_str(node->right, builder, context, arena);
		return;
	}

	// char* result = node_to_c_str(node->right, builder, context);

	// if(level > 0) {
	// 	result = alloc_str("(%s*%i)", result, node->array_capacity);
	// }
	// if(node->left->type == e_node_subscript) {
	// 	result = alloc_str("%s+%s", get_subscript_str(node->left, level + 1, context), result);
	// }
	// else {
	// 	result = alloc_str("%s+%s", node_to_c_str(node->left, context), result);
	// }
	// return result;
}

func char* get_name(s_node* node, s_lin_arena* arena)
{
	switch(node->type) {
		case e_node_array: {
			return get_name(node->left, arena);
		} break;

		case e_node_type: {
			return node->token.str(arena);
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