

func s_node* type_check_ast(s_node* ast, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_node* out_ast = null;
	s_node** target = &out_ast;
	s_scope* base_scope = null;
	t_scope_arr* data = (t_scope_arr*)arena->alloc_zero(sizeof(t_scope_arr));
	data->add(&base_scope);

	s_type_check_context context = zero;

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 4, .at = "void"};
		node.basic_type.name = "void";
		node.basic_type.id = e_type_void;
		node.size_in_bytes = 0;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "int"};
		node.basic_type.name = "int";
		node.basic_type.id = e_type_s32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "s32"};
		node.basic_type.name = "s32";
		node.basic_type.id = e_type_s32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "u32"};
		node.basic_type.is_unsigned = true;
		node.basic_type.name = "u32";
		node.basic_type.id = e_type_u32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "u64"};
		node.basic_type.is_unsigned = true;
		node.basic_type.name = "u64";
		node.basic_type.id = e_type_u64;
		node.size_in_bytes = 8;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "b32"};
		node.basic_type.name = "b32";
		node.basic_type.id = e_type_b32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 2, .at = "b8"};
		node.basic_type.name = "b8";
		node.basic_type.id = e_type_b8;
		node.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 2, .at = "s8"};
		node.basic_type.name = "s8";
		node.basic_type.id = e_type_s8;
		node.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 2, .at = "u8"};
		node.basic_type.name = "u8";
		node.basic_type.id = e_type_u8;
		node.basic_type.is_unsigned = true;
		node.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 5, .at = "float"};
		node.basic_type.name = "float";
		node.basic_type.id = e_type_f32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 6, .at = "double"};
		node.basic_type.name = "double";
		node.basic_type.id = e_type_f64;
		node.size_in_bytes = 8;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 4, .at = "char"};
		node.basic_type.name = "char";
		node.basic_type.id = e_type_char;
		node.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}


	{
		s_node node = zero;
		node.func_decl.is_external = true;
		node.func_decl.argument_count = 1;
		node.type = e_node_func_decl;
		node.func_decl.name = {.type = e_token_identifier, .len = 6, .at = "sizeof"};
		node.func_decl.return_type = get_type_by_id(e_type_u64, data);
		b8 result = type_check_func_decl(&node, reporter, data, arena, zero);
		assert(result);
		add_func_to_scope(data, alloc_node(node, arena), arena);
	}

	while(true) {
		reporter->error_level = e_error_level_none;
		b8 successfully_typechecked_something = false;
		int not_type_checked_count = 0;
		for_node(node, ast) {
			if(node->type_checked) { continue; }
			b8 result = type_check_node(node, reporter, data, arena, context);
			if(result) {
				target = advance_node_remove_next(target, *node, arena);
				successfully_typechecked_something = true;
			}
			else {
				not_type_checked_count += 1;
			}
		}
		if(not_type_checked_count == 0) { break; }
		if(!successfully_typechecked_something) {
			if(reporter->has_error) {
				reporter->fatal(null, 0, reporter->error_str);
				return null;
			}
			else {
				reporter->fatal(null, 0, "failed to typecheck a thing");
				return null;
			}
		}
	}

	s_scope* scope = *data->get(0);
	foreach_val(node_i, node, scope->structs) {

		s_node* dupe = get_struct_by_name_except(node->token.str(arena), node, data);
		if(dupe) {
			reporter->fatal(dupe->token.file, dupe->token.line, "Duplicate struct name '%s'", dupe->token.str(arena));
			return null;
		}

		// @Note(tkap, 10/02/2024): Check duplicate struct members
		for_node(member, node->nstruct.members) {
			for_node(other_member, node->nstruct.members) {
				if(member == other_member) { continue; }
				if(member->var_decl.name.equals(other_member->var_decl.name)) {
					reporter->fatal(other_member->var_decl.name.file, other_member->var_decl.name.line, "Duplicate struct member name '%s' on struct '%s'", member->var_decl.name.str(arena), node->token.str(arena));
					return null;
				}
			}
		}
	}

	return out_ast;
}

func b8 type_check_node(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context)
{
	if(node->type_checked) { return true; }

	switch(node->type) {
		case e_node_struct: {
			return type_check_struct(node, reporter, data, arena, context);
		} break;

		case e_node_enum: {
			node->type_checked = true;

			int member_index = 0;
			s_node* last_member = null;
			for_node(member, node->nenum.members) {
				member->enum_value = member_index;
				member_index += 1;
				last_member = member;
			}
			s_node count_member = zero;
			count_member.enum_value = member_index;
			count_member.token = {.type = e_token_identifier, .len = 5, .at = "count"};
			last_member->next = alloc_node(count_member, arena);

			add_enum_to_scope(data, node, arena);
			return true;
		} break;

		case e_node_data_enum: {

			int member_index = 0;
			s_node* last_member = null;
			for_node(member, node->data_enum.members) {
				member->enum_value = member_index;
				member_index += 1;
				last_member = member;
			}
			s_node count_member = zero;
			count_member.enum_value = member_index;
			count_member.token = {.type = e_token_identifier, .len = 5, .at = "count"};
			last_member->next = alloc_node(count_member, arena);

			if(!type_check_struct(node->data_enum.nstruct, reporter, data, arena, context)) {
				return false;
			}

			node->type_checked = true;
			add_data_enum_to_scope(data, node, arena);
			return true;
		} break;

		case e_node_func_decl: {
			if(!type_check_func_decl(node, reporter, data, arena, context)) {
				return false;
			}
			// node->var_type = node->func_decl.return_type;
			return true;
		} break;

		case e_node_array: {
			return type_check_expr(node, reporter, data, arena, context);
		} break;

		case e_node_var_decl: {
			return type_check_statement(node, reporter, data, arena, context);
		} break;

		case e_node_type: {
			return type_check_expr(node, reporter, data, arena, context);
		} break;


		invalid_default_case;
	}
	return false;
}

func b8 type_check_struct(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context)
{
	if(node->type_checked) { return true; }

	node->var_type = node;

	b8 result = true;
	// s_node* existing_struct = get_struct_by_name(data, token_to_str(node->token));
	// if(existing_struct) {
	// 	reporter->fatal("Struct '%s' already exists", token_to_str(node->token));
	// }
	for_node(member, node->nstruct.members) {
		if(member->type_checked) { continue; }
		if(!type_check_struct_member(node, member, reporter, data, arena, context)) {
			result = false;
			break;
		}
	}
	node->type_checked = result;
	if(result) {
		// printf("Added struct '%s'\n", node->token.str(arena));
		add_struct_to_scope(data, node, arena);
	}
	return result;
}

func b8 type_check_func_decl(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context)
{
	if(node->type_checked) { return true; }

	int scope_index = data->count - 1;
	int func_index;
	{
		s_scope* scope = *data->get_last();
		func_index = scope->funcs.count;
	}
	add_func_to_scope(data, node, arena);

	data->add(&node->func_decl.scope);

	if(!type_check_expr(node->func_decl.return_type, reporter, data, arena, context)) {
		reporter->recoverable_error(node->func_decl.name.file, node->func_decl.name.line, "Function '%s' has unknown return type '%s'", node->func_decl.name.str(arena), node_to_str(node->func_decl.return_type, arena));
		data->pop();
		s_scope* scope = *data->get(scope_index);
		scope->funcs.remove_and_shift(func_index);
		return false;
	}
	node->var_type = node->func_decl.return_type->var_type;

	// @TODO(tkap, 12/02/2024):
	#if 1
	// @TODO(tkap, 12/02/2024): Type check args
	if(!node->func_decl.is_external) {
		for_node(arg, node->func_decl.arguments) {
			assert(arg->type == e_node_var_decl);
			if(!type_check_statement(arg, reporter, data, arena, context)) {
				data->pop();
				s_scope* scope = *data->get(scope_index);
				scope->funcs.remove_and_shift(func_index);
				reporter->recoverable_error(arg->var_decl.name.file, arg->var_decl.name.line, "Function argument '%s' has unknown type '%s'", arg->var_decl.name.str(arena), node_to_str(arg->var_decl.type, arena));
				return false;
			}
		}
	}
	#endif

	if(!node->func_decl.is_external) {
		if(!type_check_statement(node->func_decl.body, reporter, data, arena, context)) {
			data->pop();
			s_scope* scope = *data->get(scope_index);
			scope->funcs.remove_and_shift(func_index);
			return false;
		}
	}

	node->type_checked = true;

	data->pop();

	// printf("Added function '%s'\n", node->func_decl.name.str(arena));
	return true;
}

func b8 type_check_statement(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context)
{
	if(node->type_checked) { return true; }
	switch(node->type) {

		case e_node_compound: {
			data->add(&node->compound.scope);
			for_node(statement, node->compound.statements) {
				if(!type_check_statement(statement, reporter, data, arena, context)) {
					data->pop();
					return false;
				}
			}
			data->pop();
			node->type_checked = true;
			return true;
		} break;

		case e_node_while: {
			// @TODO(tkap, 10/02/2024):
			if(node->nwhile.condition && !type_check_expr(node->nwhile.condition, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_statement(node->nwhile.body, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_for: {
			// @TODO(tkap, 12/02/2024):
			if(!type_check_expr(node->nfor.expr, reporter, data, arena, context)) {
				return false;
			}
			assert(node->nfor.expr->var_type);
			assert(node->nfor.body);

			if(!node->nfor.generated_iterators) {
				node->nfor.generated_iterators = true;
				if(node->nfor.expr->var_type->type == e_node_array) {
					if(node->nfor.iterator_name.len <= 0) {
						node->nfor.iterator_index_name = {.type = e_token_identifier, .len = 8, .at = "it_index"};
						node->nfor.iterator_name = {.type = e_token_identifier, .len = 2, .at = "it"};
					}
					else {
						char* str = alloc_str(arena, "%s_index", node->nfor.iterator_name.str(arena));
						node->nfor.iterator_index_name = {.type = e_token_identifier, .len = (int)strlen(str), .at = str};
					}
					s_node iterator_index = statement_str_to_node(alloc_str(arena, "int %s;", node->nfor.iterator_index_name.str(arena)), reporter, arena);
					iterator_index.dont_generate = true;
					iterator_index.var_decl.name.file = node->nfor.expr->token.file;
					iterator_index.var_decl.name.line = node->nfor.expr->token.line;
					iterator_index.token.file = node->nfor.expr->token.file;
					iterator_index.token.line = node->nfor.expr->token.line;
					node->nfor.upper_bound = node->nfor.expr->var_type->array.size_expr;

					s_node iterator = zero;
					s_node value = zero;
					s_node subscript_expr = zero;
					subscript_expr.type = e_node_identifier;
					subscript_expr.token = node->nfor.iterator_index_name;
					value.type = e_node_subscript;
					value.left = node->nfor.expr;
					value.right = alloc_node(subscript_expr, arena);
					iterator.type = e_node_var_decl;
					// @TODO(tkap, 20/02/2024): This only works for 1d arrays!
					iterator.var_decl.type = node->nfor.expr->var_type->left->var_type;
					iterator.var_decl.name = node->nfor.iterator_name;
					iterator.var_decl.value = alloc_node(value, arena);
					iterator.var_decl.name.file = node->nfor.expr->token.file;
					iterator.var_decl.name.line = node->nfor.expr->token.line;
					iterator.token.file = node->nfor.expr->token.file;
					iterator.token.line = node->nfor.expr->token.line;
					node->nfor.upper_bound = node->nfor.expr->var_type->array.size_expr;

					s_node* temp_statement = node->nfor.body->compound.statements;
					iterator.next = temp_statement;
					iterator_index.next = alloc_node(iterator, arena);
					node->nfor.body->compound.statements = alloc_node(iterator_index, arena);
				}
				else {
					if(node->nfor.iterator_name.len <= 0) {
						node->nfor.iterator_index_name = {.type = e_token_identifier, .len = 2, .at = "it"};
						node->nfor.iterator_name = {.type = e_token_identifier, .len = 2, .at = "it"};
					}
					else {
						node->nfor.iterator_index_name = node->nfor.iterator_name;
					}
					s_node iterator_index = statement_str_to_node(alloc_str(arena, "int %s;", node->nfor.iterator_index_name.str(arena)), reporter, arena);
					iterator_index.dont_generate = true;
					s_node* temp_statement = node->nfor.body->compound.statements;
					iterator_index.next = temp_statement;
					iterator_index.var_decl.name.file = node->nfor.expr->token.file;
					iterator_index.var_decl.name.line = node->nfor.expr->token.line;
					iterator_index.token.file = node->nfor.expr->token.file;
					iterator_index.token.line = node->nfor.expr->token.line;
					node->nfor.body->compound.statements = alloc_node(iterator_index, arena);
					node->nfor.upper_bound = node->nfor.expr;
				}
			}

			if(!type_check_statement(node->nfor.body, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_if: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->nif.condition, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_statement(node->nif.body, reporter, data, arena, context)) {
				return false;
			}
			if(node->nif.nelse) {
				if(!type_check_statement(node->nif.nelse, reporter, data, arena, context)) {
					return false;
				}
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_assign: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			s_type_check_context temp = context;

			// @TODO(tkap, 15/02/2024): We shouldnt need this first check. Everything should have a var_type. Fix
			if(node->left->var_type && node->left->var_type->type == e_node_struct) {
				temp.expected_literal_type = node->left->var_type;
			}
			if(!type_check_expr(node->right, reporter, data, arena, temp)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_var_decl: {
			// @TODO(tkap, 12/02/2024): if var is a struct, recursively set the var_type of all e_node_struct_literal to the struct
			// @TODO(tkap, 10/02/2024): check that variable name doesnt already exist in scope
			if(!type_check_node(node->var_decl.type, reporter, data, arena, context)) {
				return false;
			}
			if(node->var_decl.is_const && !node->var_decl.value) {
				reporter->fatal(node->token.file, node->token.line, "Constant variable declaration must have a value");
				return false;
			}
			node->var_type = node->var_decl.type->var_type;
			if(node->var_decl.value) {
				if(!type_check_expr(node->var_decl.value, reporter, data, arena, context)) {
					return false;
				}
			}
			// @Hack(tkap, 14/02/2024): we need a better system. if we find "float foo = 7;", we have to turn that 7 into a float, because it is an int
			// @TODO(tkap, 17/02/2024): DISASTER!
			if(node->var_type && node->var_type->type == e_node_type && strcmp(node->var_type->basic_type.name, "float") == 0 && node->var_decl.value && node->var_decl.value->type == e_node_integer) {
				node->var_decl.value->type = e_node_float;
				node->var_decl.value->nfloat.value = (float)node->var_decl.value->integer.value;
			}
			// @Fixme(tkap, 14/02/2024): we want this, but it removes arrays
			// if(node->var_decl.type->ntype.is_const) {
			// 	node->dont_generate = true;
			// }
			if(!add_var_to_scope(data, node, reporter, arena)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_return: {
			s_node* nfunc = get_latest_func(data);
			assert(nfunc);
			if(node->nreturn.expression) {
				// @Fixme(tkap, 13/02/2024): we cant use this last param for struct literals and return checking
				s_type_check_context temp = context;
				temp.expected_return_type = nfunc->var_type;
				if(nfunc->var_type->type == e_node_struct) {
					temp.expected_literal_type = nfunc->var_type;
				}
				if(!type_check_expr(node->nreturn.expression, reporter, data, arena, temp)) {
					return false;
				}
				node->var_type = node->nreturn.expression->var_type;
			}
			// @TODO(tkap, 12/02/2024): check that types are the same
			node->type_checked = true;
			return true;
		} break;

		case e_node_import: {
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(node->left->var_type->type == e_node_struct) {
				if(!add_import_to_scope(data, node->left, reporter, arena)) {
					return false;
				}
			}
			invalid_else;
			node->dont_generate = true;
			node->type_checked = true;
			return true;
		} break;

		case e_node_continue: {
			// @TODO(tkap, 13/02/2024): check that we are inside a loop
			node->type_checked = true;
			return true;
		} break;

		case e_node_break: {
			// @TODO(tkap, 13/02/2024): check that we are inside a loop
			node->type_checked = true;
			return true;
		} break;

		default: {
			return type_check_expr(node, reporter, data, arena, context);
		}
	}
}

func b8 type_check_expr(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context)
{
	if(node->type_checked) { return true; }
	switch(node->type) {
		case e_node_integer: {
			node->var_type = get_type_by_name("int", data);
			assert(node->var_type);
			node->type_checked = true;
			return true;
		} break;
		case e_node_float: {
			node->var_type = get_type_by_name("float", data);
			assert(node->var_type);
			node->type_checked = true;
			return true;
		} break;

		case e_node_identifier: {
			// @TODO(tkap, 12/02/2024):
			b8 success = false;
			if(context.member_access) {
				if(context.member_access->type == e_node_struct) {
					s_get_struct_member member = get_struct_member(node->token.str(arena), context.member_access, data);
					if(member.node) {
						node->var_type = member.node->var_type;
						success = true;
					}
				}
				else if(context.member_access->type == e_node_enum) {
					s_node* nenum = context.member_access;
					for_node(member, nenum->nenum.members) {
						if(member->token.equals(node->token)) {
							node->var_type = nenum;
							node->temp_var_decl = member;
							success = true;
							break;
						}
					}
				}
				else if(context.member_access->type == e_node_data_enum) {
					s_node* data_enum = context.member_access;
					if(data_enum->is_data_enum_member) {
						for_node(member, data_enum->data_enum.nstruct->nstruct.members) {
							if(member->var_decl.name.equals(node->token)) {
								node->var_type = alloc_node(*member->var_type, arena);
								node->var_type->is_data_enum_struct_access = true;
								node->temp_var_decl = member;
								success = true;
								break;
							}
						}
					}
					else {
						for_node(member, data_enum->data_enum.members) {
							if(member->token.equals(node->token)) {
								node->var_type = alloc_node(*data_enum, arena);
								node->var_type->is_data_enum_member = true;
								node->temp_var_decl = member;
								success = true;
								break;
							}
						}
					}
				}
				invalid_else;
			}
			else {
				breakable_block {

					if(context.inside_sizeof) {
						s_node* type = get_type_by_name(node->token.str(arena), data);
						if(type) {
							node->var_type = type;
							success = true;
							break;
						}

						s_node* nstruct = get_struct_by_name_except(node->token.str(arena), null, data);
						if(nstruct) {
							node->var_type = type;
							success = true;
							break;
						}
					}

					s_node* var = get_var_by_name(node->token.str(arena), data);
					if(var) {
						assert(var->var_type);
						node->temp_var_decl = var->var_decl.type;
						node->var_type = var->var_type;
						success = true;

						if(var->var_type->type == e_node_type && var->var_decl.is_const) {
							s_maybe<s_node> c = get_compile_time_value(var->var_decl.value, data, arena);
							assert(c.valid);
							node->type = c.value.type;
							if(c.value.type == e_node_integer) {
								node->integer = c.value.integer;
							}
							else if(c.value.type == e_node_float) {
								node->nfloat = c.value.nfloat;
							}
							invalid_else;
						}

						break;
					}
					s_node* nfunc = get_func_by_name(node->token.str(arena), data);
					if(nfunc) {
						assert(nfunc->var_type);
						node->var_type = nfunc->var_type;
						success = true;
						break;
					}

					s_node* nenum = get_enum_by_name(node->token.str(arena), data);
					if(nenum) {
						node->var_type = nenum;
						success = true;
						break;
					}

					s_node* data_enum = get_data_enum_by_name(node->token.str(arena), data);
					if(data_enum) {
						node->var_type = data_enum;
						success = true;
						break;
					}

					t_get_imports* imports = get_imports(data, arena);
					for(int import_i = 0; import_i < imports->count; import_i++) {
						s_node* import = imports->get(import_i);
						if(import->var_type->type == e_node_struct) {
							s_get_struct_member member = get_struct_member(node->token.str(arena), import->var_type, data);
							if(!member.node) { continue; }
							s_node temp = zero;
							temp.type = e_node_member_access;
							temp.next = node->next;
							node->next = null;
							temp.left = import;
							temp.right = alloc_node(*node, arena);
							*node = temp;
							if(!type_check_expr(node, reporter, data, arena, context)) {
								assert(false);
							}
							success = true;
							break;
						}
					}
				}
			}
			if(success) {
				node->type_checked = true;
				return true;
			}
			else {
				reporter->recoverable_error(node->token.file, node->token.line, "Identifier '%s' not found", node->token.str(arena));
				return false;
			}
		} break;

		case e_node_add:
		case e_node_divide:
		case e_node_subtract:
		case e_node_multiply:
		case e_node_modulo: {
			// @TODO(tkap, 11/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) { return false; }
			if(!type_check_expr(node->right, reporter, data, arena, context)) { return false; }
			if(!type_check_arithmetic(node, reporter, data, arena, context)) {
				reporter->recoverable_error(node->token.file, node->token.line, "Bad arithmetic TODO");
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		// case e_node_multiply: {
		// 	// @TODO(tkap, 11/02/2024):
		// 	if(!type_check_expr(node->left, reporter, data, arena, context)) { return false; }
		// 	if(!type_check_expr(node->right, reporter, data, arena, context)) { return false; }

		// 	b8 success = false;
		// 	if(node->left->var_type->type != e_node_type || node->right->var_type->type != e_node_type) {
		// 		// @Note(tkap, 18/02/2024): Look for operator overloads
		// 		for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		// 			s_scope** scope2 = data->get(scope2_i);
		// 			if(*scope2) {
		// 				s_scope* scope1 = *scope2;
		// 				foreach_val(nfunc_i, nfunc, scope1->funcs) {
		// 					assert(nfunc->type == e_node_func_decl);
		// 					if(nfunc->func_decl.argument_count != 2) { continue; }
		// 					if(nfunc->func_decl.is_external) { continue; }
		// 					s_node* first_arg = nfunc->func_decl.arguments;
		// 					s_node* second_arg = first_arg->next;
		// 					if(is_same_type(first_arg->var_type, node->left->var_type) && is_same_type(second_arg->var_type, node->right->var_type)) {
		// 						node->operator_overload_func = nfunc;
		// 						success = true;
		// 						break;
		// 					}
		// 				}
		// 			}
		// 		}
		// 	}
		// 	else {
		// 		e_type left = node->left->var_type->basic_type.id;
		// 		e_type right = node->right->var_type->basic_type.id;
		// 		for(int i = 0; i < array_count(c_op_table); i++) {
		// 			s_op_table table = c_op_table[i];
		// 			if(
		// 				(table.left == left && table.right == right) ||
		// 				(table.left == right && table.right == left)
		// 			) {
		// 				node->var_type = get_type_by_id(table.result, data);
		// 				success = true;
		// 				break;
		// 			}
		// 		}
		// 	}
		// 	if(!success) {
		// 		reporter->recoverable_error(node->token.file, node->token.line, "Bad multiply");
		// 		return false;
		// 	}
		// 	node->type_checked = true;
		// 	return true;
		// } break;

		case e_node_string: {
			node->type_checked = true;
			return true;
		} break;

		case e_node_struct_literal: {
			// @TODO(tkap, 12/02/2024):
			node->var_type = context.expected_literal_type;
			node->type_checked = true;
			return true;
		} break;

		case e_node_member_access: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!node->left->var_type || (node->left->var_type->type != e_node_struct && node->left->var_type->type != e_node_enum && node->left->var_type->type != e_node_data_enum)) {
				reporter->fatal(node->token.file, node->token.line, "todo bad member access");
				return false;
			}
			s_type_check_context temp = context;
			// temp.expected_literal_type = node->left->var_type; // @TODO(tkap, 13/02/2024): Do I need this?
			temp.member_access = node->left->var_type;
			if(!type_check_expr(node->right, reporter, data, arena, temp)) {
				reporter->fatal(node->token.file, node->token.line, "todo bad member access 2");
				return false;
			}
			node->var_type = node->right->var_type;
			node->temp_var_decl = node->var_type;
			node->type_checked = true;
			if(node->left->var_type->type == e_node_struct) {
				maybe_fix_member_access(node, node->left->var_type, data, arena);
			}
			return true;
		} break;

		case e_node_func_call: {
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			s_type_check_context temp = context;
			if(node->left->type == e_node_identifier && node->left->token.equals("sizeof")) {
				temp.inside_sizeof = true;
			}
			for_node(arg, node->func_call.arguments) {
				if(!type_check_expr(arg, reporter, data, arena, temp)) {
					return false;
				}
			}
			node->var_type = node->left->var_type;
			// @TODO(tkap, 10/02/2024): check that function exists
			node->type_checked = true;
			return true;
		} break;

		case e_node_subscript: {
			s_type_check_context temp = context;
			if(node->left->type == e_node_subscript) {
				temp.subscript_level += 1;
			}
			if(!type_check_expr(node->left, reporter, data, arena, temp)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, temp)) {
				return false;
			}
			node->temp_var_decl = node->left->temp_var_decl;
			if(node->left->var_type->type == e_node_data_enum) {
				node->var_type = alloc_node(*node->left->var_type->data_enum.nstruct, arena);
				node->array_capacity = 1;
				node->var_type->is_data_enum_struct_access = true;
			}
			else {
				if(context.inside_sizeof) {
					node->var_type = node->left->var_type;
					node->array_capacity = 1;
					node->inside_sizeof = true;
				}
				else {
					// @TODO(tkap, 14/02/2024): Not sure about this
					node->var_type = node->left->var_type->left->var_type;
					s_node* temp_array = node->temp_var_decl;
					for(int i = 0; i < context.subscript_level; i++) {
						temp_array = temp_array->left;
					}
					node->array_capacity = temp_array->array.size_expr->integer.value;
				}
			}
			// @TODO(tkap, 12/02/2024): check that left is array
			node->type_checked = true;
			return true;
		} break;

		case e_node_logic_not: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_dereference: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(node->left->var_type->pointer_level <= 0) {
				reporter->fatal(node->left->token.file, node->left->token.line, "Can't dereference a non-pointer");
				return false;
			}
			node->var_type = alloc_node(*node->left->var_type, arena);
			node->var_type->pointer_level -= 1;
			node->type_checked = true;
			return true;
		} break;

		case e_node_greater_than: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_less_than: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_less_than_or_equal: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_greater_than_or_equal: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		// case e_node_subtract: {
		// 	// @TODO(tkap, 10/02/2024):
		// 	if(!type_check_expr(node->left, reporter, data, arena, context)) {
		// 		return false;
		// 	}
		// 	if(!type_check_expr(node->right, reporter, data, arena, context)) {
		// 		return false;
		// 	}
		// 	if(!type_check_arithmetic(node, reporter, data, arena, context)) {
		// 		return false;
		// 	}
		// 	node->type_checked = true;
		// 	return true;
		// } break;

		case e_node_logic_or: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_logic_and: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_comparison: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_not_equals: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_array: {
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			if(!type_check_expr(node->array.size_expr, reporter, data, arena, context)) {
				return false;
			}
			s_maybe<s_node> c = get_compile_time_value(node->array.size_expr, data, arena);
			if(!c.valid) {
				reporter->fatal(node->array.size_expr->token.file, node->array.size_expr->token.line, "Array size is not constant");
				return false;
			}
			if(c.value.type != e_node_integer) {
				reporter->fatal(node->array.size_expr->token.file, node->array.size_expr->token.line, "Array size is not an integer");
				return false;
			}
			node->array.size_expr->type = e_node_integer;
			node->array.size_expr->integer.value = c.value.integer.value;
			node->var_type = node;
			node->var_type->size_in_bytes = node->left->var_type->size_in_bytes * node->array.size_expr->integer.value;
			node->type_checked = true;
			return true;
		} break;

		case e_node_type: {
			// @TODO(tkap, 11/02/2024):
			s_node* type = get_type_by_name(node->token.str(arena), data);
			if(type) {
				// @TODO(tkap, 18/02/2024): Questionable.
				node->var_type = alloc_node(*type, arena);
				node->var_type->pointer_level = node->pointer_level;
				node->type_checked = true;
				return true;
			}
			s_node* nstruct = get_struct_by_name_except(node->token.str(arena), null, data);
			if(nstruct) {
				node->var_type = nstruct;
				node->type_checked = true;
				return true;
			}
			reporter->recoverable_error(node->token.file, node->token.line, "Unkown type '%s'", node->token.str(arena));
			return false;
		} break;

		case e_node_unary_minus: {
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			node->var_type = node->left->var_type;
			node->type_checked = true;
			return true;
		} break;

		case e_node_address_of: {
			if(!type_check_expr(node->left, reporter, data, arena, context)) {
				return false;
			}
			// @Note(tkap, 15/02/2024): Can't really do that here. We want to say pointer_level += 1 somehow
			// node->var_type = node->left->var_type;
			node->type_checked = true;
			return true;
		} break;

		invalid_default_case;
	}
	return false;
}

func b8 type_check_struct_member(s_node* nstruct, s_node* member, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context)
{
	unreferenced(context);
	unreferenced(nstruct);
	unreferenced(arena);

	if(member->type_checked) { return true; }

	{
		s_token token = member->var_decl.name;
		s_get_struct_member possible_duplicate = get_struct_member(token.str(arena), nstruct, data);
		if(possible_duplicate.node && possible_duplicate.node != member) {
			reporter->fatal(token.file, token.line, "Duplicate struct member '%s'", token.str(arena));
			return false;
		}
	}

	// @TODO(tkap, 10/02/2024): Handle a member of type current_struct*
	if(!type_check_statement(member->var_decl.type, reporter, data, arena, context)) {
		reporter->recoverable_error(member->var_decl.type->token.file, member->var_decl.type->token.line, "Struct member '%s' has unknown type '%s'", member->var_decl.name.str(arena), node_to_str(member->var_decl.type, arena));
		return false;
	}
	member->var_type = member->var_decl.type->var_type;
	member->temp_var_decl = member->var_decl.type;

	member->type_checked = true;
	return true;
}

func char* node_to_str(s_node* node, s_lin_arena* arena)
{
	switch(node->type) {
		case e_node_type: {
			return node->token.str(arena);
		} break;

		case e_node_array: {
			char* str = node_to_str(node->left, arena);
			return alloc_str(arena, "%s[]", str);
		} break;

		invalid_default_case;
	}
	return null;
}

func s_maybe<s_node> get_compile_time_value(s_node* node, t_scope_arr* data, s_lin_arena* arena)
{
	switch(node->type) {

		case e_node_integer: {
			s_node result = zero;
			result.type = e_node_integer;
			result.integer.value = node->integer.value;
			return maybe(result);
		} break;

		case e_node_identifier: {
			s_node* var = get_var_by_name(node->token.str(arena), data);
			if(var) {
				if(!is_const(var->var_decl.type, data)) { return zero; }
				return get_compile_time_value(var->var_decl.value, data, arena);
			}
			assert(false);
		} break;

		case e_node_add: {
			s_maybe<s_node> left = get_compile_time_value(node->left, data, arena);
			if(!left.valid) { return zero; }
			s_maybe<s_node> right = get_compile_time_value(node->right, data, arena);
			if(!right.valid) { return zero; }

			f64 left_val;
			if(left.value.type == e_node_float) { left_val = left.value.nfloat.value; }
			else { left_val = left.value.integer.value; }

			f64 right_val;
			if(right.value.type == e_node_float) { right_val = right.value.nfloat.value; }
			else { right_val = right.value.integer.value; }

			f64 val_result = left_val + right_val;
			b8 is_int = left.value.type != e_node_float && right.value.type != e_node_float;
			s_node result = zero;
			if(is_int) {
				result.type = e_node_integer;
				result.integer.value = (int)val_result;
			}
			else {
				result.type = e_node_float;
				result.nfloat.value = (float)val_result;
			}
			return maybe(result);

		} break;

		case e_node_multiply: {
			s_maybe<s_node> left = get_compile_time_value(node->left, data, arena);
			if(!left.valid) { return zero; }
			s_maybe<s_node> right = get_compile_time_value(node->right, data, arena);
			if(!right.valid) { return zero; }
			f64 left_val;
			if(left.value.type == e_node_float) { left_val = left.value.nfloat.value; }
			else { left_val = left.value.integer.value; }

			f64 right_val;
			if(right.value.type == e_node_float) { right_val = right.value.nfloat.value; }
			else { right_val = right.value.integer.value; }

			f64 val_result = left_val * right_val;
			b8 is_int = left.value.type != e_node_float && right.value.type != e_node_float;
			s_node result = zero;
			if(is_int) {
				result.type = e_node_integer;
				result.integer.value = (int)val_result;
			}
			else {
				result.type = e_node_float;
				result.nfloat.value = (float)val_result;
			}
			return maybe(result);
		} break;

		case e_node_divide: {
			s_maybe<s_node> left = get_compile_time_value(node->left, data, arena);
			if(!left.valid) { return zero; }
			s_maybe<s_node> right = get_compile_time_value(node->right, data, arena);
			if(!right.valid) { return zero; }
			f64 left_val;
			if(left.value.type == e_node_float) { left_val = left.value.nfloat.value; }
			else { left_val = left.value.integer.value; }

			f64 right_val;
			if(right.value.type == e_node_float) { right_val = right.value.nfloat.value; }
			else { right_val = right.value.integer.value; }

			f64 val_result = left_val / right_val;
			b8 is_int = left.value.type != e_node_float && right.value.type != e_node_float;
			s_node result = zero;
			if(is_int) {
				result.type = e_node_integer;
				result.integer.value = (int)val_result;
			}
			else {
				result.type = e_node_float;
				result.nfloat.value = (float)val_result;
			}
			return maybe(result);
		} break;

		case e_node_float: {
			s_node result = zero;
			result.type = e_node_float;
			result.nfloat.value = node->nfloat.value;
			return maybe(result);
		} break;

		case e_node_member_access: {
			if(node->left->var_type->type == e_node_enum) {
				s_node result = zero;
				result.type = e_node_integer;
				result.integer.value = node->right->temp_var_decl->enum_value;
				return maybe(result);
			}
		} break;

		invalid_default_case;
	}
	return zero;
}

func b8 add_var_to_scope(t_scope_arr* data, s_node* var, s_error_reporter* reporter, s_lin_arena* arena)
{
	assert(var->type == e_node_var_decl);
	s_token token = var->var_decl.name;
	if(!can_thing_be_added_to_scope(token, data, reporter, arena)) {
		return false;
	}
	s_scope** scope2 = data->get(data->count - 1);
	if(!*scope2) {
		*scope2 = (s_scope*)arena->alloc_zero(sizeof(s_scope));
	}
	s_scope* scope1 = *scope2;
	scope1->vars.add(var);
	return true;
}

func b8 add_import_to_scope(t_scope_arr* data, s_node* import, s_error_reporter* reporter, s_lin_arena* arena)
{
	// @Hack(tkap, 20/02/2024): Also temporary
	assert(import->var_type->type == e_node_struct);

	t_flat_struct_members members = get_flat_array_of_struct_members(import->var_type);
	foreach_val(member_i, member, members) {
		if(!can_thing_be_added_to_scope(member->var_decl.name, data, reporter, arena)) {
			return false;
		}
	}
	s_scope** scope2 = data->get(data->count - 1);
	if(!*scope2) {
		*scope2 = (s_scope*)arena->alloc_zero(sizeof(s_scope));
	}
	s_scope* scope1 = *scope2;
	scope1->imports.add(import);
	return true;
}

func void add_struct_to_scope(t_scope_arr* data, s_node* nstruct, s_lin_arena* arena)
{
	s_scope** scope2 = data->get(data->count - 1);
	if(!*scope2) {
		*scope2 = (s_scope*)arena->alloc_zero(sizeof(s_scope));
	}
	s_scope* scope1 = *scope2;
	scope1->structs.add(nstruct);
}

func void add_enum_to_scope(t_scope_arr* data, s_node* nenum, s_lin_arena* arena)
{
	s_scope** scope2 = data->get(data->count - 1);
	if(!*scope2) {
		*scope2 = (s_scope*)arena->alloc_zero(sizeof(s_scope));
	}
	s_scope* scope1 = *scope2;
	scope1->enums.add(nenum);
}

func void add_data_enum_to_scope(t_scope_arr* data, s_node* data_enum, s_lin_arena* arena)
{
	s_scope** scope2 = data->get(data->count - 1);
	if(!*scope2) {
		*scope2 = (s_scope*)arena->alloc_zero(sizeof(s_scope));
	}
	s_scope* scope1 = *scope2;
	scope1->data_enums.add(data_enum);
}

func void add_func_to_scope(t_scope_arr* data, s_node* nfunc, s_lin_arena* arena)
{
	s_scope** scope2 = data->get(data->count - 1);
	if(!*scope2) {
		*scope2 = (s_scope*)arena->alloc_zero(sizeof(s_scope));
	}
	s_scope* scope1 = *scope2;
	scope1->funcs.add(nfunc);
}

func void add_type_to_scope(t_scope_arr* data, s_node* type, s_lin_arena* arena)
{
	s_scope** scope2 = data->get(data->count - 1);
	if(!*scope2) {
		*scope2 = (s_scope*)arena->alloc_zero(sizeof(s_scope));
	}
	s_scope* scope1 = *scope2;
	scope1->types.add(type);
}

func s_node* get_struct_by_name_except(char* name, s_node* exclude, t_scope_arr* data)
{
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		if(*scope2) {
			s_scope* scope1 = *scope2;
			foreach_val(nstruct_i, nstruct, scope1->structs) {
				if(nstruct != exclude && nstruct->token.equals(name)) {
					return nstruct;
				}
			}
		}
	}
	return null;
}

// @TODO(tkap, 18/02/2024): Delete? just query by id, it's faster
func s_node* get_type_by_name(char* name, t_scope_arr* data)
{
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		if(*scope2) {
			s_scope* scope1 = *scope2;
			foreach_val(type_i, type, scope1->types) {
				if(strcmp(type->basic_type.name, name) == 0) {
					return type;
				}
			}
		}
	}
	return null;
}

func s_node* get_type_by_id(e_type id, t_scope_arr* data)
{
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		if(*scope2) {
			s_scope* scope1 = *scope2;
			foreach_val(type_i, type, scope1->types) {
				if(type->basic_type.id == id) {
					return type;
				}
			}
		}
	}
	return null;
}

func s_node* get_var_by_name(char* name, t_scope_arr* data)
{
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		if(*scope2) {
			s_scope* scope1 = *scope2;
			foreach_val(var_i, var, scope1->vars) {
				assert(var->type == e_node_var_decl);
				if(var->var_decl.name.equals(name)) {
					return var;
				}
			}
		}
	}
	return null;
}

func t_get_imports* get_imports(t_scope_arr* data, s_lin_arena* arena)
{
	t_get_imports* result = (t_get_imports*)arena->alloc_zero(sizeof(t_get_imports));
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		if(*scope2) {
			s_scope* scope1 = *scope2;
			foreach_val(import_i, import, scope1->imports) {
				result->add(import);
			}
		}
	}
	return result;
}

func s_node* get_func_by_name(char* name, t_scope_arr* data)
{
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		if(*scope2) {
			s_scope* scope1 = *scope2;
			foreach_val(nfunc_i, nfunc, scope1->funcs) {
				assert(nfunc->type == e_node_func_decl);
				if(nfunc->func_decl.name.equals(name)) {
					return nfunc;
				}
			}
		}
	}
	return null;
}

func s_node* get_enum_by_name(char* name, t_scope_arr* data)
{
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		if(*scope2) {
			s_scope* scope1 = *scope2;
			foreach_val(nenum_i, nenum, scope1->enums) {
				assert(nenum->type == e_node_enum);
				if(nenum->token.equals(name)) {
					return nenum;
				}
			}
		}
	}
	return null;
}

func s_node* get_data_enum_by_name(char* name, t_scope_arr* data)
{
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		if(*scope2) {
			s_scope* scope1 = *scope2;
			foreach_val(nenum_i, nenum, scope1->data_enums) {
				assert(nenum->type == e_node_data_enum);
				if(nenum->token.equals(name)) {
					return nenum;
				}
			}
		}
	}
	return null;
}

// @TODO(tkap, 18/02/2024): What to do about this?
func b8 is_const(s_node* node, t_scope_arr* data)
{
	unreferenced(node);
	unreferenced(data);
	assert(false);
	// switch(node->type) {
		// case e_node_type: {
		// 	return node->ntype.is_const;
		// } break;
		// invalid_default_case;
	// }
	return false;
}


func s_node* get_latest_func(t_scope_arr* data)
{
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		s_scope* scope1 = *scope2;
		if(!scope1) { continue; }
		if(scope1->funcs.count <= 0) { continue; }
		return scope1->funcs.get_last();
	}
	return null;
}

func s_get_struct_member get_struct_member(char* name, s_node* nstruct, t_scope_arr* data)
{
	s_get_struct_member result = zero;
	assert(nstruct->type == e_node_struct);
	for_node(member, nstruct->nstruct.members) {
		if(member->var_decl.name.equals(name)) {
			result.node = member;
			break;
		}
		if(member->var_decl.is_import) {
			s_get_struct_member temp = get_struct_member(name, member->var_type, data);
			if(temp.node) {
				temp.is_imported = true;
				temp.import_source = member;
				result = temp;
				break;
			}
		}
	}
	return result;
}


func void maybe_fix_member_access(s_node* node, s_node* nstruct, t_scope_arr* data, s_lin_arena* arena)
{
	assert(nstruct->type == e_node_struct);
	assert(node->type == e_node_member_access);
	assert(node->type_checked);

	// @TODO(tkap, 15/02/2024): Do we need this???
	// if(node->left->type == e_node_member_access) {
	// 	maybe_fix_member_access(node->left, node->left->left->var_type, data, arena);
	// }
	s_get_struct_member member = get_struct_member(node->right->token.str(arena), nstruct, data);
	assert(member.node);
	if(member.is_imported) {
		s_node new_right = zero;
		s_node* old_right = node->right;
		s_node* old_next = node->next;
		new_right.type = e_node_identifier;
		new_right.token = member.import_source->var_decl.name;
		node->right = alloc_node(new_right, arena);

		s_node temp = zero;
		temp.type_checked = true;
		temp.type = e_node_member_access;
		temp.left = alloc_node(*node, arena);
		temp.right = old_right;
		temp.temp_var_decl = member.node->temp_var_decl;
		temp.var_type = member.node->var_type;
		temp.next = old_next;
		*node = temp;
		maybe_fix_member_access(node, member.import_source->var_type, data, arena);
	}
}


func int get_size_in_bytes(s_node* node, t_scope_arr* data, s_lin_arena* arena)
{
	assert(node->type_checked);
	int result = 0;
	switch(node->type) {
		case e_node_array: {
			assert(node->array.size_expr->type == e_node_integer);
			result = get_size_in_bytes(node->left, data, arena) * node->array.size_expr->integer.value;
		} break;

		case e_node_type: {
			s_node* type = get_type_by_name(node->token.str(arena), data);
			assert(type);
			return type->var_type->size_in_bytes;
		} break;
		invalid_default_case;
	}
	return result;
}

func b8 is_same_type(s_node* a, s_node* b)
{
	if(a->type != b->type) { return false; }
	if(a->pointer_level != b->pointer_level) { return false; }
	if(a->type == e_node_struct) {
		if(!a->token.equals(b->token)) { return false; }
	}
	if(a->type == e_node_type) {
		if(a->basic_type.id != b->basic_type.id) { return false; }
	}
	return true;
}

// @TODO(tkap, 18/02/2024): Dogshit?
func b8 is_compatible_type(s_node* a, s_node* b, e_type target_type)
{
	if(a->type != b->type) { return false; }
	if(a->pointer_level != b->pointer_level) { return false; }
	if(a->type == e_node_struct) {
		if(!a->token.equals(b->token)) { return false; }
	}
	if(a->type == e_node_type) {
		// @TODO(tkap, 18/02/2024): Copy paste
		e_type left = a->basic_type.id;
		e_type right = b->basic_type.id;
		for(int i = 0; i < array_count(c_op_table); i++) {
			s_op_table table = c_op_table[i];
			if(
				(table.left == left && table.right == right) ||
				(table.left == right && table.right == left)
			) {
				if(target_type == table.result) { return true; }
			}
		}
		return false;
	}
	return true;
}

func b8 type_check_arithmetic(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context)
{
	unreferenced(reporter);
	unreferenced(arena);
	unreferenced(context);
	b8 success = false;
	if(node->left->var_type->type != e_node_type || node->right->var_type->type != e_node_type) {
		// @Note(tkap, 18/02/2024): Look for operator overloads
		for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
			s_scope** scope2 = data->get(scope2_i);
			if(*scope2) {
				s_scope* scope1 = *scope2;
				foreach_val(nfunc_i, nfunc, scope1->funcs) {
					assert(nfunc->type == e_node_func_decl);
					if(nfunc->func_decl.is_external) { continue; }
					if(nfunc->func_decl.argument_count != 2) { continue; }
					if(!nfunc->func_decl.is_operator_overload) { continue; }
					if(nfunc->func_decl.operator_overload != node->type) { continue; }
					s_node* first_arg = nfunc->func_decl.arguments;
					s_node* second_arg = first_arg->next;

					// @TODO(tkap, 18/02/2024): TERRIBLE! copy paste, not complete
					// @TODO(tkap, 18/02/2024): We actually want something like can_type_be_converted_to_whatever()
					if(second_arg->var_type->type == e_node_type) {
						if(is_same_type(first_arg->var_type, node->left->var_type) && is_compatible_type(second_arg->var_type, node->right->var_type, second_arg->var_type->basic_type.id)) {
							node->var_type = nfunc->func_decl.return_type->var_type;
							node->operator_overload_func = nfunc;
							success = true;
							break;
						}
					}
					else {
						if(is_same_type(first_arg->var_type, node->left->var_type) && is_same_type(second_arg->var_type, node->right->var_type)) {
							// @TODO(tkap, 18/02/2024): Or do we want return_type->var_type?
							node->var_type = nfunc->func_decl.return_type;
							node->operator_overload_func = nfunc;
							success = true;
							break;
						}
					}
				}
			}
		}
	}
	else {
		e_type left = node->left->var_type->basic_type.id;
		e_type right = node->right->var_type->basic_type.id;
		for(int i = 0; i < array_count(c_op_table); i++) {
			s_op_table table = c_op_table[i];
			if(
				(table.left == left && table.right == right) ||
				(table.left == right && table.right == left)
			) {
				node->var_type = get_type_by_id(table.result, data);
				success = true;
				break;
			}
		}
	}
	return success;
}

// @TODO(tkap, 20/02/2024): Better errors. This assumes that we only call this from add_var_to_scope
func b8 can_thing_be_added_to_scope(s_token name, t_scope_arr* data, s_error_reporter* reporter, s_lin_arena* arena)
{
	if(get_var_by_name(name.str(arena), data)) {
		reporter->fatal(name.file, name.line, "Duplicate variable name '%s'", name.str(arena));
		return false;
	}
	if(get_struct_by_name_except(name.str(arena), null, data)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because a struct with that name already exists", name.str(arena));
		return false;
	}
	if(get_func_by_name(name.str(arena), data)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because a function with that name already exists", name.str(arena));
		return false;
	}
	if(get_enum_by_name(name.str(arena), data)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because an enum with that name already exists", name.str(arena));
		return false;
	}
	if(get_data_enum_by_name(name.str(arena), data)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because a data_enum with that name already exists", name.str(arena));
		return false;
	}
	if(get_type_by_name(name.str(arena), data)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because a type with that name already exists", name.str(arena));
		return false;
	}
	arena->push();
	t_get_imports* imports = get_imports(data, arena);
	for(int import_i = 0; import_i < imports->count; import_i++) {
		s_node* import = imports->get(import_i);
		if(import->var_type->type == e_node_struct) {
			s_get_struct_member member = get_struct_member(name.str(arena), import->var_type, data);
			if(member.node) {
				reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because a variable coming from an imported struct already has that name", name.str(arena));
				return false;
			}
		}
	}
	arena->pop();
	return true;
}

func t_flat_struct_members get_flat_array_of_struct_members(s_node* nstruct)
{
	assert(nstruct->type == e_node_struct);
	t_flat_struct_members result;
	get_flat_array_of_struct_members_(nstruct, &result);
	return result;
}

func void get_flat_array_of_struct_members_(s_node* nstruct, t_flat_struct_members* result)
{
	assert(nstruct->type == e_node_struct);
	for_node(member, nstruct->nstruct.members) {
		if(member->var_decl.is_import) {
			get_flat_array_of_struct_members_(member->var_type, result);
		}
		else {
			result->add(member);
		}
	}
}