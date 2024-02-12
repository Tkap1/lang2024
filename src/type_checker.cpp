

func b8 type_check_ast(s_node* ast, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_scope* base_scope = null;
	t_scope_arr* data = (t_scope_arr*)arena->alloc_zero(sizeof(t_scope_arr));
	data->add(&base_scope);

	{
		s_node node = zero;
		node.basic_type.name = "void";
		node.basic_type.size_in_bytes = 0;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.basic_type.name = "int";
		node.basic_type.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.basic_type.name = "s32";
		node.basic_type.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.basic_type.is_unsigned = true;
		node.basic_type.name = "u32";
		node.basic_type.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.basic_type.name = "b32";
		node.basic_type.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.basic_type.name = "s8";
		node.basic_type.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.basic_type.name = "u8";
		node.basic_type.is_unsigned = true;
		node.basic_type.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	{
		s_node node = zero;
		node.basic_type.name = "float";
		node.basic_type.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena);
	}

	while(true) {
		b8 successfully_typechecked_something = false;
		int not_type_checked_count = 0;
		for_node(node, ast) {
			if(node->type_checked) { continue; }
			b8 result = type_check_node(node, reporter, data, arena);
			if(result) {
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
				return false;
			}
			else {
				reporter->fatal(null, 0, "failed to typecheck a thing");
				return false;
			}
		}
	}

	s_scope* scope = *data->get(0);
	foreach_val(node_i, node, scope->structs) {

		s_node* dupe = get_struct_by_name_except(node->token.str(), node, data);
		if(dupe) {
			reporter->fatal(dupe->token.file, dupe->token.line, "Duplicate struct name '%s'", dupe->token.str());
			return false;
		}

		// @Note(tkap, 10/02/2024): Check duplicate struct members
		for_node(member, node->nstruct.members) {
			for_node(other_member, node->nstruct.members) {
				if(member == other_member) { continue; }
				if(member->var_decl.name.equals(other_member->var_decl.name)) {
					reporter->fatal(other_member->var_decl.name.file, other_member->var_decl.name.line, "Duplicate struct member name '%s' on struct '%s'", member->var_decl.name.str(), node->token.str());
					return false;
				}
			}
		}
	}

	return true;
}

func b8 type_check_node(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena)
{
	if(node->type_checked) { return true; }

	switch(node->type) {
		case e_node_struct: {
			return type_check_struct(node, reporter, data, arena);
		} break;

		case e_node_func_decl: {
			if(!type_check_func_decl(node, reporter, data, arena)) {
				return false;
			}
			node->var_type = node->func_decl.return_type;
			return true;
		} break;

		case e_node_array: {
			return type_check_expr(node, reporter, data, arena);
		} break;

		case e_node_var_decl: {
			return type_check_statement(node, reporter, data, arena);
		} break;

		case e_node_type: {
			return type_check_expr(node, reporter, data, arena);
		} break;


		invalid_default_case;
	}
	return false;
}

func b8 type_check_struct(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena)
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
		if(!type_check_struct_member(node, member, reporter, data, arena)) {
			result = false;
			break;
		}
	}
	node->type_checked = result;
	if(result) {
		// printf("Added struct '%s'\n", node->token.str());
		add_struct_to_scope(data, node, arena);
	}
	return result;
}

func b8 type_check_func_decl(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena)
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

	if(!type_check_expr(node->func_decl.return_type, reporter, data, arena)) {
		reporter->recoverable_error(node->func_decl.name.file, node->func_decl.name.line, "Function '%s' has unknown return type '%s'", node->func_decl.name.str(), node_to_str(node->func_decl.return_type));
		data->pop();
		s_scope* scope = *data->get(scope_index);
		scope->funcs.remove_and_shift(func_index);
		return false;
	}
	node->var_type = node->func_decl.return_type->var_type;

	// @TODO(tkap, 12/02/2024):
	#if 0
	// @TODO(tkap, 12/02/2024): Type check args
	if(!node->func_decl.is_external) {
		for_node(arg, node->func_decl.arguments) {
			if(arg->type_checked) { continue; }
			type = node_to_basic_type(arg->var_decl.type, data);
			if(type) {
				arg->type_checked = true;
			}
			else {
				reporter->recoverable_error(arg->var_decl.name.file, arg->var_decl.name.line, "Function argument '%s' has unknown type '%s'", arg->var_decl.name.str(), node_to_str(arg->var_decl.type));
				data->pop();
				s_scope* scope = *data->get(scope_index);
				scope->funcs.remove_and_shift(func_index);
				return false;
			}
		}
	}
	#endif

	if(!node->func_decl.is_external) {
		if(!type_check_statement(node->func_decl.body, reporter, data, arena)) {
			data->pop();
			s_scope* scope = *data->get(scope_index);
			scope->funcs.remove_and_shift(func_index);
			return false;
		}
	}

	node->type_checked = true;

	data->pop();

	// printf("Added function '%s'\n", node->func_decl.name.str());
	return true;
}

func b8 type_check_statement(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena)
{
	if(node->type_checked) { return true; }
	switch(node->type) {

		case e_node_compound: {
			data->add(&node->compound.scope);
			for_node(statement, node->compound.statements) {
				if(!type_check_statement(statement, reporter, data, arena)) {
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
			if(node->nwhile.condition && !type_check_expr(node->nwhile.condition, reporter, data, arena)) {
				return false;
			}
			if(!type_check_statement(node->nwhile.body, reporter, data, arena)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_for: {
			// @TODO(tkap, 12/02/2024):
			if(!type_check_expr(node->nfor.expr, reporter, data, arena)) {
				return false;
			}
			// @TODO(tkap, 12/02/2024): This is fucked. we are going to add the "it" variable to the scope the for loop is in, rather than inside the for loop
			// @TODO(tkap, 12/02/2024): We are going to add this multiple times!!!
			s_node temp = statement_str_to_node(format_str("int %s = 0;", node->nfor.iterator_name.str()), reporter, arena);
			b8 success = type_check_statement(&temp, reporter, data, arena);
			assert(success);
			add_var_to_scope(data, alloc_node(temp, arena), arena);
			if(!type_check_statement(node->nfor.body, reporter, data, arena)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_if: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->nif.condition, reporter, data, arena)) {
				return false;
			}
			if(!type_check_statement(node->nif.body, reporter, data, arena)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_assign: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_var_decl: {
			// @TODO(tkap, 12/02/2024): if var is a struct, recursively set the var_type of all e_node_struct_literal to the struct
			// @TODO(tkap, 10/02/2024): check that variable name doesnt already exist in scope
			// s_node* type = node_to_basic_type(node->var_decl.type, data);
			// if(!type) {
			// 	reporter->recoverable_error(node->var_decl.type->token.file, node->var_decl.type->token.line, "Variable '%s' has unknown type '%s'", node->var_decl.name.str(), node_to_str(node->var_decl.type));
			// 	return false;
			// }
			if(!type_check_node(node->var_decl.type, reporter, data, arena)) {
				return false;
			}
			node->var_type = node->var_decl.type->var_type;
			if(node->var_decl.value) {
				if(!type_check_expr(node->var_decl.value, reporter, data, arena, node->var_decl.type)) {
					return false;
				}
			}
			add_var_to_scope(data, node, arena);
			node->type_checked = true;
			return true;
		} break;

		case e_node_return: {
			s_node* nfunc = get_latest_func(data);
			assert(nfunc);
			if(node->nreturn.expression) {
				if(!type_check_expr(node->nreturn.expression, reporter, data, arena, nfunc->var_type)) {
					return false;
				}
			}
			// @TODO(tkap, 12/02/2024): check that types are the same
			node->var_type = node->nreturn.expression->var_type;
			node->type_checked = true;
			return true;
		} break;

		default: {
			return type_check_expr(node, reporter, data, arena);
		}
	}
}

func b8 type_check_expr(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_node* expected_type)
{
	if(node->type_checked) { return true; }
	switch(node->type) {
		case e_node_integer: {
			node->type_checked = true;
			return true;
		} break;

		case e_node_identifier: {
			// @TODO(tkap, 12/02/2024):
			b8 success = false;
			if(expected_type) {
				assert(expected_type->type == e_node_struct);
				for_node(member, expected_type->nstruct.members) {
					if(node->token.equals(member->var_decl.name)) {
						node->var_type = member->var_decl.type->var_type;
						success = true;
						break;
					}
				}
			}
			else {
				breakable_block {
					s_node* var = get_var_by_name(node->token.str(), data);
					if(var) {
						assert(var->var_type);
						node->var_type = var->var_type;
						success = true;
						break;
					}
					s_node* nfunc = get_func_by_name(node->token.str(), data);
					if(nfunc) {
						assert(nfunc->var_type);
						node->var_type = nfunc->var_type;
						success = true;
						break;
					}
				}
			}
			if(success) {
				node->type_checked = true;
				return true;
			}
			else {
				reporter->recoverable_error(node->token.file, node->token.line, "Identifier '%s' not found", node->token.str());
				return false;
			}
		} break;

		case e_node_add:
		case e_node_multiply: {
			// @TODO(tkap, 11/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena)) { return false; }
			if(!type_check_expr(node->right, reporter, data, arena)) { return false; }
			node->type_checked = true;
			return true;
		} break;

		case e_node_string: {
			node->type_checked = true;
			return true;
		} break;

		case e_node_struct_literal: {
			// @TODO(tkap, 12/02/2024):
			node->var_type = expected_type;
			node->type_checked = true;
			return true;
		} break;

		case e_node_member_access: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, expected_type)) {
				return false;
			}
			if(!node->left->var_type || node->left->var_type->type != e_node_struct) {
				reporter->fatal(node->token.file, node->token.line, "todo bad member access");
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, node->left->var_type)) {
				reporter->fatal(node->token.file, node->token.line, "todo bad member access 2");
				return false;
			}
			node->var_type = node->right->var_type;
			node->type_checked = true;
			return true;
		} break;

		case e_node_func_call: {
			if(!type_check_expr(node->left, reporter, data, arena)) {
				return false;
			}
			for_node(arg, node->func_call.arguments) {
				if(!type_check_expr(arg, reporter, data, arena)) {
					return false;
				}
			}
			// @TODO(tkap, 10/02/2024): check that function exists
			node->type_checked = true;
			return true;
		} break;


		case e_node_logic_not: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_greater_than: {
			// @TODO(tkap, 10/02/2024):
			node->type_checked = true;
			return true;
		} break;

		case e_node_less_than: {
			// @TODO(tkap, 10/02/2024):
			node->type_checked = true;
			return true;
		} break;

		case e_node_subtract: {
			// @TODO(tkap, 10/02/2024):
			node->type_checked = true;
			return true;
		} break;

		case e_node_logic_or: {
			// @TODO(tkap, 10/02/2024):
			node->type_checked = true;
			return true;
		} break;

		case e_node_logic_and: {
			// @TODO(tkap, 10/02/2024):
			node->type_checked = true;
			return true;
		} break;

		case e_node_array: {
			if(!type_check_expr(node->left, reporter, data, arena)) {
				return false;
			}
			if(!type_check_expr(node->array.size_expr, reporter, data, arena)) {
				return false;
			}
			s_maybe<int> size = get_compile_time_value(node->array.size_expr, data);
			if(!size.valid) {
				reporter->fatal(node->array.size_expr->token.file, node->array.size_expr->token.line, "Array size is not constant");
				return false;
			}
			node->array.size_expr->type = e_node_integer;
			node->array.size_expr->integer.value = size.value;
			// @Fixme(tkap, 12/02/2024):
			node->type_checked = true;
			return true;
		} break;

		case e_node_type: {
			// @TODO(tkap, 11/02/2024):
			s_node* type = get_type_by_name(node->token.str(), data);
			if(type) {
				node->var_type = type;
				node->type_checked = true;
				return true;
			}
			s_node* nstruct = get_struct_by_name_except(node->token.str(), null, data);
			if(nstruct) {
				node->var_type = nstruct;
				node->type_checked = true;
				return true;
			}
			reporter->recoverable_error(node->token.file, node->token.line, "Unkown type '%s'", node->token.str());
			return false;
		} break;

		invalid_default_case;
	}
	return false;
}

func b8 type_check_struct_member(s_node* nstruct, s_node* member, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena)
{
	unreferenced(nstruct);
	unreferenced(arena);

	if(member->type_checked) { return true; }
	b8 result = true;

	// @TODO(tkap, 10/02/2024): Handle a member of type current_struct*
	// @TODO(tkap, 10/02/2024): check that array size makes sense
	s_node* type = node_to_basic_type(member->var_decl.type, data);
	if(!type) {
		result = false;
		reporter->recoverable_error(member->var_decl.type->token.file, member->var_decl.type->token.line, "Struct member '%s' has unknown type '%s'", member->var_decl.name.str(), node_to_str(member->var_decl.type));
	}

	member->type_checked = result;
	if(result) {
		member->var_type = type;
		// printf("type checked %s %s\n", node_to_str(member->var_decl.type), member->var_decl.name.str());
	}
	return result;
}

// func b8 compare_s_type(s_type a, s_type b)
// {
// 	return strcmp(a.name, b.name) == 0;
// }

func s_node* node_to_basic_type(s_node* node, t_scope_arr* data)
{
	switch(node->type) {
		case e_node_type: {
			s_node* type = get_type_by_name(node->token.str(), data);
			if(type) {
				return type;
			}
			type = get_struct_by_name_except(node->token.str(), null, data);
			if(type) {
				return type;
			}
		} break;

		case e_node_array: {
			return node_to_basic_type(node->left, data);
		} break;

		invalid_default_case;
	}
	return null;
}

func char* node_to_str(s_node* node)
{
	switch(node->type) {
		case e_node_type: {
			return node->token.str();
		} break;

		case e_node_array: {
			char* str = node_to_str(node->left);
			return format_str("%s[]", str);
		} break;

		invalid_default_case;
	}
	return null;
}

func s_maybe<int> get_compile_time_value(s_node* node, t_scope_arr* data)
{
	switch(node->type) {

		case e_node_integer: {
			return maybe(node->integer.value);
		} break;

		case e_node_identifier: {
			s_node* var = get_var_by_name(node->token.str(), data);
			if(var) {
				if(!is_const(var->var_decl.type, data)) { return zero; }
				return get_compile_time_value(var->var_decl.value, data);
			}
			assert(false);
		} break;

		case e_node_add: {
			s_maybe<int> left = get_compile_time_value(node->left, data);
			if(!left.valid) { return zero; }
			s_maybe<int> right = get_compile_time_value(node->right, data);
			if(!right.valid) { return zero; }
			return maybe(left.value + right.value);
		} break;

		case e_node_multiply: {
			s_maybe<int> left = get_compile_time_value(node->left, data);
			if(!left.valid) { return zero; }
			s_maybe<int> right = get_compile_time_value(node->right, data);
			if(!right.valid) { return zero; }
			return maybe(left.value * right.value);
		} break;

		invalid_default_case;
	}
	return zero;
}

func void add_var_to_scope(t_scope_arr* data, s_node* var, s_lin_arena* arena)
{
	s_scope** scope2 = data->get(data->count - 1);
	if(!*scope2) {
		*scope2 = (s_scope*)arena->alloc_zero(sizeof(s_scope));
	}
	s_scope* scope1 = *scope2;
	scope1->vars.add(var);
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

func b8 is_const(s_node* node, t_scope_arr* data)
{
	unreferenced(data);
	switch(node->type) {
		case e_node_type: {
			return node->ntype.is_const;
		} break;
		invalid_default_case;
	}
	return false;
}


func s_node* get_latest_func(t_scope_arr* data)
{
	for(int scope2_i = data->count - 1; scope2_i >= 0; scope2_i -= 1) {
		s_scope** scope2 = data->get(scope2_i);
		s_scope* scope1 = *scope2;
		if(!scope1) { continue; }
		return scope1->funcs.get_last();
	}
	return null;
}
