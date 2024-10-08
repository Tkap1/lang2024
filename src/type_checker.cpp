

func s_node* type_check_ast(s_node* ast, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_node* out_ast = null;
	s_node** target = &out_ast;
	t_scope_index_arr* data = (t_scope_index_arr*)arena->alloc_zero(sizeof(t_scope_index_arr));
	s_type_check_context context = zero;
	t_scope_arr scope_arr;
	data->add(make_scope(&scope_arr));


	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 4, .at = "void"};
		node.basic_type.name = "void";
		node.basic_type.id = e_type_void;
		node.size_in_bytes = 0;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "int"};
		node.basic_type.name = "int";
		node.basic_type.id = e_type_s32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "s32"};
		node.basic_type.name = "s32";
		node.basic_type.id = e_type_s32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "u32"};
		node.basic_type.is_unsigned = true;
		node.basic_type.name = "u32";
		node.basic_type.id = e_type_u32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "u64"};
		node.basic_type.is_unsigned = true;
		node.basic_type.name = "u64";
		node.basic_type.id = e_type_u64;
		node.size_in_bytes = 8;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 3, .at = "b32"};
		node.basic_type.name = "b32";
		node.basic_type.id = e_type_b32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 2, .at = "b8"};
		node.basic_type.name = "b8";
		node.basic_type.id = e_type_b8;
		node.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 2, .at = "s8"};
		node.basic_type.name = "s8";
		node.basic_type.id = e_type_s8;
		node.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 2, .at = "u8"};
		node.basic_type.name = "u8";
		node.basic_type.id = e_type_u8;
		node.basic_type.is_unsigned = true;
		node.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 5, .at = "float"};
		node.basic_type.name = "float";
		node.basic_type.id = e_type_f32;
		node.size_in_bytes = 4;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 6, .at = "double"};
		node.basic_type.name = "double";
		node.basic_type.id = e_type_f64;
		node.size_in_bytes = 8;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 4, .at = "char"};
		node.basic_type.name = "char";
		node.basic_type.id = e_type_char;
		node.size_in_bytes = 1;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	{
		s_node node = zero;
		node.type = e_node_type;
		node.token = {.type = e_token_identifier, .len = 8, .at = "soa_view"};
		node.basic_type.name = "soa_view";
		node.basic_type.id = e_type_soa_view;
		node.size_in_bytes = 0;
		add_type_to_scope(data, alloc_node(node, arena), arena, &scope_arr);
	}

	while(true) {
		reporter->error_level = e_error_level_none;
		b8 successfully_typechecked_something = false;
		int not_type_checked_count = 0;
		for_node(node, ast) {
			if(node->type_checked) { continue; }
			b8 result = type_check_node(node, reporter, data, arena, context, &scope_arr);
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

	s_scope scope = scope_arr[0];
	if(scope.structs.count > 0) {
		foreach_val(node_i, node, scope.structs) {

			s_node* dupe = get_struct_by_name_except(node->token.str(arena), node, data, &scope_arr);
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
	}

	return out_ast;
}

func b8 type_check_node(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr)
{
	if(node->type_checked) { return true; }

	switch(node->type) {
		case e_node_struct: {
			return type_check_struct(node, reporter, data, arena, context, scope_arr);
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

			add_enum_to_scope(data, node, arena, scope_arr);
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

			if(!type_check_struct(node->data_enum.nstruct, reporter, data, arena, context, scope_arr)) {
				return false;
			}

			node->type_checked = true;
			add_data_enum_to_scope(data, node, arena, scope_arr);
			return true;
		} break;

		case e_node_func_decl: {
			if(!type_check_func_decl(node, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			// node->var_type = node->func_decl.return_type;
			return true;
		} break;

		case e_node_array: {
			return type_check_expr(node, reporter, data, arena, context, scope_arr);
		} break;

		case e_node_var_decl: {
			return type_check_statement(node, reporter, data, arena, context, scope_arr);
		} break;

		case e_node_type: {
			return type_check_expr(node, reporter, data, arena, context, scope_arr);
		} break;

		case e_node_iterator: {
			// @TODO(tkap, 28/09/2024): make sure that we found a "yield" inside the body!
			// @TODO(tkap, 28/09/2024):
			// @TODO(tkap, 28/09/2024): prevent name collision etc
			// @TODO(tkap, 28/09/2024): Make sure that the iterator variable is the first argument
			auto iterator = &node->iterator;

			if(iterator->argument_count <= 0) {
				reporter->fatal(iterator->name.file, iterator->name.line, "Iterators need at least 1 parameter");
				return false;
			}

			if(iterator->scope_index <= 0) {
				iterator->scope_index = make_scope(scope_arr);
			}
			s_scope* scope_before = &scope_arr->get(data->get_last());
			int iterator_index = scope_before->iterator_arr.add(node, arena);
			data->add(iterator->scope_index);

			// @Note(tkap, 28/09/2024): ->next because we never want to type-check the first argument, because that is just the name of the iterator variable
			for_node(argument, iterator->arguments->next) {
				if(!type_check_statement(argument, reporter, data, arena, context, scope_arr)) {
					reporter->recoverable_error(
						iterator->name.file, iterator->name.line, "Argument '%s' on iterator '%s' has unknown type '%s'",
						token_to_str(argument->var_decl.name, arena), iterator->name.str(arena), "TODO"
					);
					return false;
				}
			}

			s_type_check_context temp_context = context;
			temp_context.inside_iterator = true;
			if(!type_check_statement(node->iterator.body, reporter, data, arena, temp_context, scope_arr)) {
				scope_before->iterator_arr.remove_and_shift(iterator_index);
				data->pop();
				return false;
			}

			data->pop();
			node->type_checked = true;
			return true;

		} break;


		invalid_default_case;
	}
	return false;
}

func b8 type_check_struct(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr)
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
		if(!type_check_struct_member(node, member, reporter, data, arena, context, scope_arr)) {
			result = false;
			break;
		}
	}
	node->type_checked = result;
	if(result) {
		// printf("Added struct '%s'\n", node->token.str(arena));
		add_struct_to_scope(data, node, arena, scope_arr);
	}
	return result;
}

func b8 type_check_func_decl(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr)
{
	if(node->type_checked) { return true; }

	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		return type start		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	if(!type_check_expr(node->func_decl.return_type, reporter, data, arena, context, scope_arr)) {
		reporter->recoverable_error(node->func_decl.name.file, node->func_decl.name.line, "Function '%s' has unknown return type '%s'", node->func_decl.name.str(arena), node_to_str(node->func_decl.return_type, arena));
		return false;
	}
	node->var_type = node->func_decl.return_type->var_type;
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		return type end		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	int func_index = add_func_to_scope(data, node, arena, scope_arr);
	if(node->func_decl.scope_index <= 0) {
		node->func_decl.scope_index = make_scope(scope_arr);
	}
	data->add(node->func_decl.scope_index);

	for_node(arg, node->func_decl.arguments) {
		if(arg->type == e_node_var_args) {
			assert(!arg->next);
			break;
		}
		if(node->func_decl.is_external) {
			if(!type_check_expr(arg, reporter, data, arena, context, scope_arr)) {
				s_scope* scope = &scope_arr->get(data->get(data->count - 2));
				scope->funcs.remove_and_shift(func_index);
				data->pop();
				reporter->recoverable_error(arg->var_decl.name.file, arg->var_decl.name.line, "Function argument '%s' has unknown type '%s'", arg->token.str(arena), node_to_str(arg, arena));
				return false;
			}
		}
		else {
			assert(arg->type == e_node_var_decl);
			if(!type_check_statement(arg, reporter, data, arena, context, scope_arr)) {
				s_scope* scope = &scope_arr->get(data->get(data->count - 2));
				scope->funcs.remove_and_shift(func_index);
				data->pop();
				reporter->recoverable_error(arg->var_decl.name.file, arg->var_decl.name.line, "Function argument '%s' has unknown type '%s'", arg->var_decl.name.str(arena), node_to_str(arg->var_decl.type, arena));
				return false;
			}
		}
	}

	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		handle methods start		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	if(node->func_decl.is_method) {
		s_node* nstruct = get_struct_by_name_except(node->func_decl.base_struct.str(arena), null, data, scope_arr);
		if(!nstruct) {
			reporter->recoverable_error(node->func_decl.name.file, node->func_decl.name.line, "TODO");
			s_scope* scope = &scope_arr->get(data->get(data->count - 2));
			scope->funcs.remove_and_shift(func_index);
			data->pop();
			return false;
		}

		// @TODO(tkap, 25/02/2024): Turn this into an if check after we fail it once.
		assert(!node->func_decl.method_import_added_to_scope);
		{
			s_node var_decl = zero;
			var_decl.type = e_node_var_decl;
			{
				s_tokenizer tokenizer = quick_tokenizer(alloc_str(arena, "%s*", nstruct->token.str(arena)));
				var_decl.var_decl.type = alloc_node(parse_type(tokenizer, reporter, arena).node, arena);
			}
			var_decl.var_decl.name = make_identifier_token("this");
			if(!type_check_statement(alloc_node(var_decl, arena), reporter, data, arena, context, scope_arr)) { assert(false); }
		}

		{
			s_node import = zero;
			import.type = e_node_import;
			{
				s_node left = zero;
				left.type = e_node_identifier;
				left.token = make_identifier_token("this");
				import.left = alloc_node(left, arena);
			}
			if(!type_check_statement(alloc_node(import, arena), reporter, data, arena, context, scope_arr)) { assert(false); }
		}
		node->func_decl.method_import_added_to_scope = true;

		{
			// s_node left = zero;
			// left.type = e_node_identifier;
			// left.var_type = nstruct;
			// left.token = {.type = e_token_identifier, .len = 8, .at = "__this__"};
			// import->left = alloc_node(left, arena);
		}
		// if(!add_import_to_scope(data, import, reporter, arena)) {
		// 	node->func_decl.method_import_added_to_scope = true;
		// 	reporter->fatal(node->func_decl.name.file, node->func_decl.name.line, "TODO");
		// 	data->pop();
		// 	s_scope* scope = *data->get(scope_index);
		// 	scope->funcs.remove_and_shift(func_index);
		// 	return false;
		// }
	}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		handle methods end		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	if(!node->func_decl.is_external) {
		if(!type_check_statement(node->func_decl.body, reporter, data, arena, context, scope_arr)) {
			s_scope* scope = &scope_arr->get(data->get(data->count - 2));
			scope->funcs.remove_and_shift(func_index);
			data->pop();
			return false;
		}
	}

	node->type_checked = true;

	data->pop();

	// printf("Added function '%s'\n", node->func_decl.name.str(arena));
	return true;
}

func b8 type_check_statement(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr)
{
	if(node->type_checked) { return true; }
	switch(node->type) {

		case e_node_compound: {
			if(node->compound.scope_index <= 0) {
				node->compound.scope_index = make_scope(scope_arr);
			}
			data->add(node->compound.scope_index);
			for_node(statement, node->compound.statements) {
				if(!type_check_statement(statement, reporter, data, arena, context, scope_arr)) {
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
			if(node->nwhile.condition && !type_check_expr(node->nwhile.condition, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_statement(node->nwhile.body, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_simple_for: {
			if(!type_check_expr(node->simple_for.expr, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(node->simple_for.expr->var_type->type != e_node_array) {
				node->simple_for.iterator_index_name = zero;
			}
			if(!can_thing_be_added_to_scope(node->simple_for.iterator_name, data, reporter, context, arena, scope_arr)) {
				return false;
			}
			if(node->simple_for.iterator_index_name.len > 0 && !can_thing_be_added_to_scope(node->simple_for.iterator_index_name, data, reporter, context, arena, scope_arr)) {
				return false;
			}

			context.for_loop_arr.add(node);
			if(!type_check_statement(node->simple_for.body, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_range_for: {
			// @TODO(tkap, 29/09/2024): Make sure that this is a number and that it is the same? as upper bound
			if(!type_check_expr(node->range_for.lower_bound, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			// @TODO(tkap, 29/09/2024): Make sure that this is a number
			if(!type_check_expr(node->range_for.upper_bound, reporter, data, arena, context, scope_arr)) {
				return false;
			}

			if(!can_thing_be_added_to_scope(node->range_for.iterator_name, data, reporter, context, arena, scope_arr)) {
				return false;
			}

			context.for_loop_arr.add(node);
			if(!type_check_statement(node->range_for.body, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_if: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->nif.condition, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_statement(node->nif.body, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(node->nif.nelse) {
				if(!type_check_statement(node->nif.nelse, reporter, data, arena, context, scope_arr)) {
					return false;
				}
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_assign: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			s_type_check_context temp = context;

			// @TODO(tkap, 15/02/2024): We shouldnt need this first check. Everything should have a var_type. Fix
			if(node->left->var_type && node->left->var_type->type == e_node_struct) {
				temp.expected_literal_type = node->left->var_type;
			}
			if(!type_check_expr(node->right, reporter, data, arena, temp, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_var_decl: {
			// @TODO(tkap, 12/02/2024): if var is a struct, recursively set the var_type of all e_node_struct_literal to the struct
			// @TODO(tkap, 10/02/2024): check that variable name doesnt already exist in scope
			if(!type_check_node(node->var_decl.type, reporter, data, arena, context, scope_arr)) {
				return false;
			}

			if(node->var_decl.is_const && !node->var_decl.value) {
				reporter->fatal(node->token.file, node->token.line, "Constant variable declaration must have a value");
				return false;
			}
			node->var_type = node->var_decl.type->var_type;
			if(node->var_decl.value) {
				s_type_check_context temp = context;
				temp.wanted_type = node->var_decl.type->var_type;
				temp.expected_literal_type = node->var_decl.type->var_type;
				if(!type_check_expr(node->var_decl.value, reporter, data, arena, temp, scope_arr)) {
					return false;
				}
			}

			// @TODO(tkap, 25/02/2024): check that left type and right type are compatible
			if(node->var_decl.value) {
				if(!can_type_a_be_converted_to_b(node->var_decl.type->var_type, node->var_decl.value->var_type)) {
					reporter->fatal(
						node->token.file, node->token.line, "Can't assign '%s' of type '%s' to '%s' of type '%s'",
						node_to_str(node->var_decl.value, arena),
						node_to_str(node->var_decl.value->var_type, arena),
						node->var_decl.name.str(arena),
						node_to_str(node->var_decl.type, arena)
					);
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
			if(!add_var_to_scope(data, node, reporter, context, arena, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_return: {
			s_node* nfunc = get_latest_func(data, scope_arr);
			assert(nfunc);
			if(node->nreturn.expression) {
				// @Fixme(tkap, 13/02/2024): we cant use this last param for struct literals and return checking
				s_type_check_context temp = context;
				temp.expected_return_type = nfunc->var_type;
				if(nfunc->var_type->type == e_node_struct) {
					temp.expected_literal_type = nfunc->var_type;
				}
				if(!type_check_expr(node->nreturn.expression, reporter, data, arena, temp, scope_arr)) {
					return false;
				}
				node->var_type = node->nreturn.expression->var_type;
			}
			// @TODO(tkap, 12/02/2024): check that types are the same
			node->type_checked = true;
			return true;
		} break;

		case e_node_func_ptr: {
			// @TODO(tkap, 22/02/2024): type check return type and parameters
			if(!type_check_expr(node->func_ptr.return_type, reporter, data, arena, context, scope_arr)) {
				reporter->recoverable_error(node->token.file, node->token.line, "bad return type TODO");
				return false;
			}
			for_node(arg, node->func_ptr.arguments) {
				if(!type_check_expr(arg, reporter, data, arena, context, scope_arr)) {
					reporter->recoverable_error(node->token.file, node->token.line, "bad arg type TODO");
					return false;
				}
			}
			if(!add_func_pointer_to_scope(data, node, reporter, context ,arena, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_import: {
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(node->left->var_type->type == e_node_struct) {
				if(!add_import_to_scope(data, node->left, reporter, context, arena, scope_arr)) {
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
			return type_check_expr(node, reporter, data, arena, context, scope_arr);
		}
	}
}

func b8 type_check_expr(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr)
{
	if(node->type_checked) { return true; }
	switch(node->type) {
		case e_node_integer: {
			node->var_type = get_type_by_name("int", data, scope_arr);
			assert(node->var_type);
			node->type_checked = true;
			return true;
		} break;
		case e_node_float: {
			node->var_type = get_type_by_name("float", data, scope_arr);
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
						node->temp_var_decl = member.node->var_type;
						success = true;
					}
					if(!success) {
						s_node* nfunc = get_func_by_name(node->token.str(arena), data, scope_arr);
						if(nfunc && nfunc->func_decl.is_method && nfunc->func_decl.base_struct.equals(context.member_access->token)) {
							node->var_type = nfunc->func_decl.return_type->var_type;
							node->temp_var_decl = nfunc;
							success = true;
						}
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
				else if(context.member_access->type == e_node_array) {
					assert(node->token.equals("size"));
					node->var_type = get_type_by_id(e_type_s32, data, scope_arr);
					success = true;
				}
				invalid_else;
			}
			else {
				breakable_block {

					if(context.inside_sizeof) {
						s_node* type = get_type_by_name(node->token.str(arena), data, scope_arr);
						if(type) {
							node->var_type = type;
							success = true;
							break;
						}

						s_node* nstruct = get_struct_by_name_except(node->token.str(arena), null, data, scope_arr);
						if(nstruct) {
							node->var_type = type;
							success = true;
							break;
						}
					}

					s_node* var = get_var_by_name(node->token.str(arena), data, scope_arr);
					if(var) {
						assert(var->var_type);
						node->temp_var_decl = var->var_decl.type;
						node->var_type = var->var_type;
						success = true;

						if(var->var_type->type == e_node_type && var->var_decl.is_const) {
							s_maybe<s_node> c = get_compile_time_value(var->var_decl.value, data, arena, scope_arr);
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

					{
						s_node* for_var = get_for_loop_var_by_name(node->token.str(arena), data, context, scope_arr, arena);
						if(for_var) {
							node->var_type = for_var;
							success = true;
							break;
						}
					}

					s_node* nfunc = get_func_by_name(node->token.str(arena), data, scope_arr);
					if(nfunc) {
						assert(nfunc->var_type);
						node->var_type = nfunc->var_type;
						node->temp_var_decl = nfunc;
						success = true;
						break;
					}

					s_node* iterator = get_iterator_by_name(node->token.str(arena), data, scope_arr);
					if(iterator) {
						success = true;
						node->var_type = iterator;
						break;
					}

					s_node* nenum = get_enum_by_name(node->token.str(arena), data, scope_arr);
					if(nenum) {
						node->var_type = nenum;
						success = true;
						break;
					}

					s_node* data_enum = get_data_enum_by_name(node->token.str(arena), data, scope_arr);
					if(data_enum) {
						node->var_type = data_enum;
						success = true;
						break;
					}

					t_get_imports* imports = get_imports(data, arena, scope_arr);
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
							if(!type_check_expr(node, reporter, data, arena, context, scope_arr)) {
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
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) { return false; }
			if(!type_check_expr(node->right, reporter, data, arena, context, scope_arr)) { return false; }
			if(!type_check_arithmetic(node, reporter, data, arena, context, scope_arr)) {
				reporter->recoverable_error(node->token.file, node->token.line, "Bad arithmetic TODO");
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_array_literal: {
			// @TODO(tkap, 23/02/2024): check that the type of each expression is the same as the type of the array
			for_node(expr, node->array_literal.expressions) {
				if(!type_check_expr(expr, reporter, data, arena, context, scope_arr)) {
					return false;
				}
			}
			node->var_type = context.wanted_type;
			node->type_checked = true;
			return true;
		} break;

		case e_node_string: {
			// @TODO(tkap, 24/02/2024): This is kinda cringe, lots of allocating
			node->var_type = alloc_node(*get_type_by_id(e_type_char, data, scope_arr), arena);
			node->var_type->pointer_level = 1;
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
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}

			if(!node->left->var_type) {
				reporter->fatal(node->token.file, node->token.line, "todo bad member access");
				return false;
			}
			breakable_block {
				if(node->left->var_type->type == e_node_array && node->right->token.equals("size")) {
					break;
				}
				if(node->left->var_type->type == e_node_struct) { break; }
				if(node->left->var_type->type == e_node_enum) { break; }
				if(node->left->var_type->type == e_node_data_enum) { break; }
				reporter->fatal(node->token.file, node->token.line, "todo bad member access");
				return false;
			}

			s_type_check_context temp = context;
			// temp.expected_literal_type = node->left->var_type; // @TODO(tkap, 13/02/2024): Do I need this?
			temp.member_access = node->left->var_type;
			if(!type_check_expr(node->right, reporter, data, arena, temp, scope_arr)) {
				reporter->recoverable_error(node->token.file, node->token.line, "todo bad member access 2");
				return false;
			}
			node->var_type = node->right->var_type;
			// @TODO(tkap, 25/02/2024): Questionble
			node->temp_var_decl = node->right->temp_var_decl;
			node->type_checked = true;
			if(node->left->var_type->type == e_node_struct) {
				maybe_fix_member_access(node, node->left->var_type, data, arena);
			}
			return true;
		} break;

		case e_node_func_call: {
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			b8 is_iterator = node->left->var_type->type == e_node_iterator;
			if(!is_iterator) {
				if(node->left->temp_var_decl->func_decl.is_method && node->left->type != e_node_member_access) {
					reporter->fatal(
						node->token.file, node->token.line, "bad func call TODO"
					);
					return false;
				}
			}
			int decl_arg_count = get_func_argument_count(node->left);
			if(node->func_call.argument_count != decl_arg_count) {
				int call_count = node->func_call.argument_count;
				reporter->fatal(
					node->token.file, node->token.line, "Function %s wants %i argument%s but %i %s given",
					node_to_str(node->left, arena), decl_arg_count, decl_arg_count == 1 ? "": "s", call_count, call_count == 1 ? "was" : "were"
				);
				return false;
			}
			s_type_check_context temp = context;
			s_node* decl_arg = null;
			if(is_iterator) {
				decl_arg = node->left->var_type->iterator.arguments;
			}
			else {
				if(node->left->temp_var_decl->type == e_node_func_decl) {
					decl_arg = node->left->temp_var_decl->func_decl.arguments;
				}
				else if(node->left->temp_var_decl->var_type->type == e_node_func_ptr) {
					decl_arg = node->left->temp_var_decl->var_type->func_ptr.arguments;
				}
				invalid_else;
			}

			b8 found_var_args = false;
			for_node(call_arg, node->func_call.arguments) {
				if(decl_arg->type == e_node_var_args) {
					found_var_args = true;
				}

				s_type_check_context temp2 = temp;
				if(!found_var_args) {
					// @Note(tkap, 28/09/2024): Skip the first argument if this is an iterator
					if(!is_iterator || call_arg != node->func_call.arguments) {
						assert(decl_arg->var_type);
						temp2.wanted_type = decl_arg->var_type;
						if(decl_arg->var_type->type == e_node_struct) {
							temp2.expected_literal_type = decl_arg->var_type;
						}
					}
				}

				// @TODO(tkap, 28/09/2024): We actually just want to ignore the ones with "declare" in the argument, not all arguments
				if(!is_iterator) {
					if(!type_check_expr(call_arg, reporter, data, arena, temp2, scope_arr)) {
						return false;
					}

					// @Note(tkap, 24/02/2024): If we haven't found var args, then we should check that the call argument and the decl argument types are compatible
					if(!found_var_args) {
						if(!can_type_a_be_converted_to_b(call_arg->var_type, decl_arg->var_type)) {
							reporter->fatal(call_arg->token.file, call_arg->token.line, "bad type TODO");
							return false;
						}
					}
				}
				decl_arg = decl_arg->next;
			}

			if(is_iterator) {
				s_node* next = node->next;
				s_node* next2 = node->next->next;
				node->func_call.body = next;
				next->next = null;
				node->next = next2;
				node->type = e_node_iterator_call;
			}

			node->var_type = node->left->var_type;
			// @TODO(tkap, 10/02/2024): check that function exists
			node->type_checked = true;
			return true;
		} break;

		case e_node_sizeof: {
			assert(node->func_call.argument_count == 1);

			s_type_check_context temp_context = context;
			temp_context.inside_sizeof = true;
			if(!type_check_expr(node->func_call.arguments, reporter, data, arena, temp_context, scope_arr)) {
				return false;
			}

			// @TODO(tkap, 30/09/2024): maybe not the best type
			node->var_type = get_type_by_id(e_type_s32, data, scope_arr);

			node->type_checked = true;
			return true;
		} break;

		case e_node_subscript: {
			s_type_check_context temp = context;
			if(node->left->type == e_node_subscript) {
				temp.subscript_level += 1;
			}
			if(!type_check_expr(node->left, reporter, data, arena, temp, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, temp, scope_arr)) {
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
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_auto_cast: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			// @Fixme(tkap, 24/02/2024): check that auto cast makes sense
			if(!context.wanted_type) {
				node->var_type = node->left->var_type;
				// @TODO(tkap, 24/02/2024): Remove this assert eventually... maybe
				// Unless we want to allow this kind of statement: "xx 5.0;"
				assert(false);
			}
			else {
				node->var_type = context.wanted_type;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_dereference: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
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
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_less_than: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_less_than_or_equal: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_greater_than_or_equal: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context, scope_arr)) {
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
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_logic_and: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			// @TODO(tkap, 25/02/2024): proper bool
			node->var_type = get_type_by_id(e_type_s32, data, scope_arr);
			node->type_checked = true;
			return true;
		} break;

		case e_node_comparison: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		case e_node_not_equals: {
			// @TODO(tkap, 10/02/2024):
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->right, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			// @TODO(tkap, 25/02/2024): We want a proper bool
			node->var_type = get_type_by_id(e_type_s32, data, scope_arr);
			node->type_checked = true;
			return true;
		} break;

		case e_node_array: {
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			if(!type_check_expr(node->array.size_expr, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			s_maybe<s_node> c = get_compile_time_value(node->array.size_expr, data, arena, scope_arr);
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
			s_node* type = get_type_by_name(node->token.str(arena), data, scope_arr);
			if(type) {
				// @TODO(tkap, 18/02/2024): Questionable.
				node->var_type = alloc_node(*type, arena);
				node->var_type->pointer_level = node->pointer_level;
				node->type_checked = true;
				return true;
			}
			s_node* nstruct = get_struct_by_name_except(node->token.str(arena), null, data, scope_arr);
			if(nstruct) {
				node->var_type = alloc_node(*nstruct, arena);
				node->var_type->pointer_level = node->pointer_level;
				node->type_checked = true;
				return true;
			}
			reporter->recoverable_error(node->token.file, node->token.line, "Unkown type '%s'", node->token.str(arena));
			return false;
		} break;

		case e_node_unary_minus: {
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			node->var_type = node->left->var_type;
			node->type_checked = true;
			return true;
		} break;

		case e_node_address_of: {
			if(!type_check_expr(node->left, reporter, data, arena, context, scope_arr)) {
				return false;
			}
			// @TODO(tkap, 24/02/2024): Check that getting the address of this makes sense. We shouldn't allow &5
			node->var_type = alloc_node(*node->left->var_type, arena);
			node->var_type->pointer_level += 1;
			// node->var_type = node->left->var_type;
			node->type_checked = true;
			return true;
		} break;

		case e_node_yield: {
			if(!context.inside_iterator) {
				reporter->fatal(node->token.file, node->token.line, "Can't use 'yield' outside iterators");
				return false;
			}
			node->type_checked = true;
			return true;
		} break;

		invalid_default_case;
	}
	return false;
}

func b8 type_check_struct_member(s_node* nstruct, s_node* member, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr)
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
	if(!type_check_statement(member->var_decl.type, reporter, data, arena, context, scope_arr)) {
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

		case e_node_integer: {
			return alloc_str(arena, "%lli", node->integer.value);
		} break;

		case e_node_add: {
			return alloc_str(arena, "%s + %s", node_to_str(node->left, arena), node_to_str(node->right, arena));
		} break;

		case e_node_multiply: {
			return alloc_str(arena, "%s * %s", node_to_str(node->left, arena), node_to_str(node->right, arena));
		} break;

		case e_node_identifier: {
			return node->token.str(arena);
		} break;

		invalid_default_case;
	}
	return null;
}

func s_maybe<s_node> get_compile_time_value(s_node* node, t_scope_index_arr* data, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	switch(node->type) {

		case e_node_integer: {
			s_node result = zero;
			result.type = e_node_integer;
			result.integer.value = node->integer.value;
			return maybe(result);
		} break;

		case e_node_identifier: {
			s_node* var = get_var_by_name(node->token.str(arena), data, scope_arr);
			if(var) {
				if(!is_const(var->var_decl.type, data)) { return zero; }
				return get_compile_time_value(var->var_decl.value, data, arena, scope_arr);
			}
			assert(false);
		} break;

		case e_node_add: {
			s_maybe<s_node> left = get_compile_time_value(node->left, data, arena, scope_arr);
			if(!left.valid) { return zero; }
			s_maybe<s_node> right = get_compile_time_value(node->right, data, arena, scope_arr);
			if(!right.valid) { return zero; }

			f64 left_val;
			if(left.value.type == e_node_float) { left_val = left.value.nfloat.value; }
			else { left_val = (f64)left.value.integer.value; }

			f64 right_val;
			if(right.value.type == e_node_float) { right_val = right.value.nfloat.value; }
			else { right_val = (f64)right.value.integer.value; }

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
			s_maybe<s_node> left = get_compile_time_value(node->left, data, arena, scope_arr);
			if(!left.valid) { return zero; }
			s_maybe<s_node> right = get_compile_time_value(node->right, data, arena, scope_arr);
			if(!right.valid) { return zero; }
			f64 left_val;
			if(left.value.type == e_node_float) { left_val = left.value.nfloat.value; }
			else { left_val = (f64)left.value.integer.value; }

			f64 right_val;
			if(right.value.type == e_node_float) { right_val = right.value.nfloat.value; }
			else { right_val = (f64)right.value.integer.value; }

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
			s_maybe<s_node> left = get_compile_time_value(node->left, data, arena, scope_arr);
			if(!left.valid) { return zero; }
			s_maybe<s_node> right = get_compile_time_value(node->right, data, arena, scope_arr);
			if(!right.valid) { return zero; }
			f64 left_val;
			if(left.value.type == e_node_float) { left_val = left.value.nfloat.value; }
			else { left_val = (f64)left.value.integer.value; }

			f64 right_val;
			if(right.value.type == e_node_float) { right_val = right.value.nfloat.value; }
			else { right_val = (f64)right.value.integer.value; }

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

func b8 add_var_to_scope(t_scope_index_arr* data, s_node* var, s_error_reporter* reporter, s_type_check_context context, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	assert(var->type == e_node_var_decl);
	s_token token = var->var_decl.name;
	if(!can_thing_be_added_to_scope(token, data, reporter, context, arena, scope_arr)) {
		return false;
	}
	s_scope* scope = &scope_arr->get(data->get_last());
	scope->vars.add(var, arena);
	return true;
}

func b8 add_func_pointer_to_scope(t_scope_index_arr* data, s_node* func_ptr, s_error_reporter* reporter, s_type_check_context context, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	assert(func_ptr->type == e_node_func_ptr);
	s_token token = func_ptr->func_ptr.name;
	if(!can_thing_be_added_to_scope(token, data, reporter, context, arena, scope_arr)) {
		return false;
	}
	s_scope* scope = &scope_arr->get(data->get_last());
	scope->func_ptrs.add(func_ptr, arena);
	return true;
}

func b8 add_import_to_scope(t_scope_index_arr* data, s_node* import, s_error_reporter* reporter, s_type_check_context context, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	// @Hack(tkap, 20/02/2024): Also temporary
	assert(import->var_type->type == e_node_struct);

	t_flat_struct_members members = get_flat_array_of_struct_members(import->var_type);
	foreach_val(member_i, member, members) {
		if(!can_thing_be_added_to_scope(member->var_decl.name, data, reporter, context, arena, scope_arr)) {
			return false;
		}
	}
	s_scope* scope = &scope_arr->get(data->get_last());
	scope->imports.add(import, arena);
	return true;
}

func void add_struct_to_scope(t_scope_index_arr* data, s_node* nstruct, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	s_scope* scope = &scope_arr->get(data->get_last());
	scope->structs.add(nstruct, arena);
}

func void add_enum_to_scope(t_scope_index_arr* data, s_node* nenum, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	s_scope* scope = &scope_arr->get(data->get_last());
	scope->enums.add(nenum, arena);
}

func void add_data_enum_to_scope(t_scope_index_arr* data, s_node* data_enum, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	s_scope* scope = &scope_arr->get(data->get_last());
	scope->data_enums.add(data_enum, arena);
}

func int add_func_to_scope(t_scope_index_arr* data, s_node* nfunc, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	s_scope* scope = &scope_arr->get(data->get_last());
	int index = scope->funcs.add(nfunc, arena);
	return index;
}

func void add_type_to_scope(t_scope_index_arr* data, s_node* type, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	s_scope* scope = &scope_arr->get(data->get_last());
	scope->types.add(type, arena);
}

func s_node* get_struct_by_name_except(char* name, s_node* exclude, t_scope_index_arr* data, t_scope_arr* scope_arr)
{
	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		for(int nstruct_i = 0; nstruct_i < scope->structs.count; nstruct_i += 1) {
			s_node* nstruct = scope->structs[nstruct_i];
			if(nstruct != exclude && nstruct->token.equals(name)) {
				return nstruct;
			}
		}
	}
	return null;
}

// @TODO(tkap, 18/02/2024): Delete? just query by id, it's faster
func s_node* get_type_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr)
{
	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		for(int type_i = 0; type_i < scope->types.count; type_i += 1) {
			s_node* type = scope->types[type_i];
			if(strcmp(type->basic_type.name, name) == 0) {
				return type;
			}
		}
		for(int type_i = 0; type_i < scope->func_ptrs.count; type_i += 1) {
			s_node* type = scope->func_ptrs[type_i];
			if(type->func_ptr.name.equals(name)) {
				return type;
			}
		}
	}
	return null;
}

func s_node* get_type_by_id(e_type id, t_scope_index_arr* data, t_scope_arr* scope_arr)
{
	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		for(int type_i = 0; type_i < scope->types.count; type_i += 1) {
			s_node* type = scope->types[type_i];
			if(type->basic_type.id == id) {
				return type;
			}
		}
	}

	return null;
}

func s_node* get_var_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr)
{
	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		for(int var_i = 0; var_i < scope->vars.count; var_i += 1) {
			s_node* var = scope->vars[var_i];
			assert(var->type == e_node_var_decl);
			if(var->var_decl.name.equals(name)) {
				return var;
			}
		}
	}

	return null;
}

func s_node* get_for_loop_var_by_name(char* name, t_scope_index_arr* data, s_type_check_context context, t_scope_arr* scope_arr, s_lin_arena* arena)
{
	for(int for_i = context.for_loop_arr.count - 1; for_i >= 0; for_i -= 1) {
		s_node* nfor = context.for_loop_arr[for_i];
		switch(nfor->type) {
			case e_node_simple_for: {
				if(nfor->simple_for.iterator_name.equals(name)) {
					if(nfor->simple_for.expr->var_type->type == e_node_array) {
						if(nfor->simple_for.loop_by_ptr) {
							s_node* temp = alloc_node(*nfor->simple_for.expr->var_type->left->var_type, arena);
							temp->pointer_level += 1;
							return temp;
						}
						else {
						return nfor->simple_for.expr->var_type->left->var_type;
						}
					}
					else {
						assert(!nfor->simple_for.loop_by_ptr);
						return nfor->simple_for.expr->var_type;
					}
				}
				else if(nfor->simple_for.iterator_index_name.equals(name)) {
					return get_type_by_id(e_type_s32, data, scope_arr);
					break;
				}
			} break;
			case e_node_range_for: {
				if(nfor->range_for.iterator_name.equals(name)) {
					return nfor->range_for.lower_bound->var_type;
				}
			} break;
			invalid_default_case;
		}
	}

	return null;
}

func t_get_imports* get_imports(t_scope_index_arr* data, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	t_get_imports* result = (t_get_imports*)arena->alloc_zero(sizeof(t_get_imports));

	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		for(int import_i = 0; import_i < scope->imports.count; import_i += 1) {
			s_node* import = scope->imports[import_i];
			result->add(import);
		}
	}

	return result;
}

func s_node* get_func_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr)
{
	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		for(int nfunc_i = 0; nfunc_i < scope->funcs.count; nfunc_i += 1) {
			s_node* nfunc = scope->funcs[nfunc_i];
			if(nfunc->func_decl.name.equals(name)) {
				return nfunc;
			}
		}
	}
	return null;
}

func s_node* get_iterator_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr)
{
	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		for(int iterator_i = 0; iterator_i < scope->iterator_arr.count; iterator_i += 1) {
			s_node* iterator = scope->iterator_arr[iterator_i];
			if(iterator->iterator.name.equals(name)) {
				return iterator;
			}
		}
	}
	return null;
}

func s_node* get_enum_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr)
{
	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		for(int nenum_i = 0; nenum_i < scope->enums.count; nenum_i += 1) {
			s_node* nenum = scope->enums[nenum_i];
			if(nenum->token.equals(name)) {
				return nenum;
			}
		}
	}
	return null;
}

func s_node* get_data_enum_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr)
{
	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		for(int nenum_i = 0; nenum_i < scope->data_enums.count; nenum_i += 1) {
			s_node* nenum = scope->data_enums[nenum_i];
			if(nenum->token.equals(name)) {
				return nenum;
			}
		}
	}
	return null;
}

// @TODO(tkap, 18/02/2024): What to do about this?
func b8 is_const(s_node* node, t_scope_index_arr* data)
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


func s_node* get_latest_func(t_scope_index_arr* data, t_scope_arr* scope_arr)
{

	for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
		s_scope* scope = &scope_arr->get(data->get(index_i));
		if(scope->funcs.count > 0) {
			return scope->funcs.get_last();
		}
	}

	return null;
}

func s_get_struct_member get_struct_member(char* name, s_node* nstruct, t_scope_index_arr* data)
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

func void maybe_fix_member_access(s_node* node, s_node* nstruct, t_scope_index_arr* data, s_lin_arena* arena)
{
	assert(nstruct->type == e_node_struct);
	assert(node->type == e_node_member_access);
	assert(node->type_checked);

	// @TODO(tkap, 25/02/2024): May not work with imports... should it?
	if(node->right->temp_var_decl && node->right->temp_var_decl->type == e_node_func_decl) {
		auto decl = node->right->temp_var_decl->func_decl;
		assert(decl.is_method);
		assert(decl.base_struct.equals(nstruct->token));
		return;
	}

	s_get_struct_member member = get_struct_member(node->right->token.str(arena), nstruct, data);
	assert(member.node);
	if(member.is_imported) {
		s_node new_right = zero;
		s_node* old_right = node->right;
		s_node* old_next = node->next;
		new_right.type = e_node_identifier;
		new_right.token = member.import_source->var_decl.name;
		// @TODO(tkap, 24/02/2024): I feel like this wont work recursively? maybe it does, because the deepest nodes are the one that reach this function first
		new_right.var_type = member.import_source->var_type;
		node->right = alloc_node(new_right, arena);
		node->var_type = member.import_source->var_type;

		s_node temp = zero;
		temp.type_checked = true;
		temp.type = e_node_member_access;
		temp.left = alloc_node(*node, arena);
		temp.right = old_right;
		temp.temp_var_decl = member.node->temp_var_decl;
		temp.var_type = old_right->var_type;
		temp.next = old_next;
		*node = temp;
		maybe_fix_member_access(node, member.import_source->var_type, data, arena);
	}
}


func s64 get_size_in_bytes(s_node* node, t_scope_index_arr* data, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	assert(node->type_checked);
	s64 result = 0;
	switch(node->type) {
		case e_node_array: {
			assert(node->array.size_expr->type == e_node_integer);
			result = get_size_in_bytes(node->left, data, arena, scope_arr) * node->array.size_expr->integer.value;
		} break;

		case e_node_type: {
			s_node* type = get_type_by_name(node->token.str(arena), data, scope_arr);
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

func b8 type_check_arithmetic(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr)
{
	unreferenced(reporter);
	unreferenced(arena);
	unreferenced(context);
	b8 success = false;
	if(node->left->var_type->type != e_node_type || node->right->var_type->type != e_node_type) {
		// @Note(tkap, 18/02/2024): Look for operator overloads

		for(int index_i = data->count - 1; index_i >= 0; index_i -= 1) {
			s_scope* scope = &scope_arr->get(data->get(index_i));
			for(int nfunc_i = 0; nfunc_i < scope->funcs.count; nfunc_i += 1) {
				s_node* nfunc = scope->funcs[nfunc_i];
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
						node->var_type = nfunc->func_decl.return_type->var_type;
						node->operator_overload_func = nfunc;
						success = true;
						break;
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
				node->var_type = get_type_by_id(table.result, data, scope_arr);
				success = true;
				break;
			}
		}
	}
	return success;
}

// @TODO(tkap, 20/02/2024): Better errors. This assumes that we only call this from add_var_to_scope
func b8 can_thing_be_added_to_scope(s_token name, t_scope_index_arr* data, s_error_reporter* reporter, s_type_check_context context, s_lin_arena* arena, t_scope_arr* scope_arr)
{
	if(get_var_by_name(name.str(arena), data, scope_arr)) {
		reporter->fatal(name.file, name.line, "Duplicate variable name '%s'", name.str(arena));
		return false;
	}
	if(get_struct_by_name_except(name.str(arena), null, data, scope_arr)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because a struct with that name already exists", name.str(arena));
		return false;
	}
	if(get_func_by_name(name.str(arena), data, scope_arr)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because a function with that name already exists", name.str(arena));
		return false;
	}
	if(get_enum_by_name(name.str(arena), data, scope_arr)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because an enum with that name already exists", name.str(arena));
		return false;
	}
	if(get_data_enum_by_name(name.str(arena), data, scope_arr)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because a data_enum with that name already exists", name.str(arena));
		return false;
	}
	if(get_type_by_name(name.str(arena), data, scope_arr)) {
		reporter->fatal(name.file, name.line, "Cannot declare variable '%s' because a type with that name already exists", name.str(arena));
		return false;
	}
	if(get_for_loop_var_by_name(name.str(arena), data, context, scope_arr, arena)) {
		reporter->fatal(name.file, name.line, "Duplicate variable name '%s'", name.str(arena));
		return false;
	}

	arena->push();
	t_get_imports* imports = get_imports(data, arena, scope_arr);
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

func b8 can_type_a_be_converted_to_b(s_node* a, s_node* b)
{
	assert(a);
	assert(b);

	// @Note(tkap, 24/02/2024): Allow any pointer to work as void*
	if(b->type == e_node_type && b->basic_type.id == e_type_void && b->pointer_level >= 1 && a->pointer_level >= 1) { return true; }
	if(a->type == e_node_type && a->basic_type.id == e_type_void && a->pointer_level >= 1 && b->pointer_level >= 1) { return true; }

	if(a->type == e_node_struct && b->type != e_node_struct) { return false; }

	return true;
}

func s_token make_identifier_token(char* str)
{
	return {.type = e_token_identifier, .len = (int)strlen(str), .at = str};
}

func s_node make_identifier_node(char* str)
{
	s_node node = zero;
	node.type = e_node_identifier;
	node.token = make_identifier_token(str);
	return node;
}

func s_tokenizer quick_tokenizer(char* str)
{
	s_tokenizer tokenizer = zero;
	tokenizer.at = str;
	return tokenizer;
}

func int get_func_argument_count(s_node* node)
{
	if(node->var_type->type == e_node_iterator) {
		return node->var_type->iterator.argument_count;
	}
	else if(node->var_type->type == e_node_func_ptr) {
		return node->var_type->func_ptr.argument_count;
	}
	else if(node->temp_var_decl->type == e_node_func_decl) {
		return node->temp_var_decl->func_decl.argument_count;
	}
	invalid_else;
	return 0;
}

func int make_scope(t_scope_arr* scope_arr)
{
	int index = scope_arr->add(zero);
	return index;
}
