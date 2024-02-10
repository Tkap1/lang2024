

func void type_check_ast(s_node* ast, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_type_check* data = (s_type_check*)arena->alloc_zero(sizeof(s_type_check));

	{
		s_node node = zero;
		node.basic_type.name = "int";
		node.basic_type.size_in_bytes = 4;
		data->types.add(alloc_node(node, arena));
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
			}
			else {
				reporter->fatal(null, 0, "failed to typecheck a thing");
			}
		}
	}

	foreach_val(node_i, node, data->structs) {

		// @Note(tkap, 10/02/2024): Check duplicate struct names
		foreach_val(node_j, other, data->structs) {
			if(node_i == node_j) { continue; }
			if(node->token.equals(other->token)) {
				reporter->fatal(other->token.file, other->token.line, "Duplicate struct name '%s'", node->token.str());
			}
		}

		// @Note(tkap, 10/02/2024): Check duplicate struct members
		for_node(member, node->nstruct.members) {
			for_node(other_member, node->nstruct.members) {
				if(member == other_member) { continue; }
				if(member->var_decl.name.equals(other_member->var_decl.name)) {
					reporter->fatal(other_member->var_decl.name.file, other_member->var_decl.name.line, "Duplicate struct member name '%s' on struct '%s'", member->var_decl.name.str(), node->token.str());
				}
			}
		}
	}

}

func b8 type_check_node(s_node* node, s_error_reporter* reporter, s_type_check* data, s_lin_arena* arena)
{
	assert(!node->type_checked);

	switch(node->type) {
		case e_node_struct: {
			return type_check_struct(node, reporter, data, arena);
		} break;

		case e_node_func_decl: {
			return type_check_func_decl(node, reporter, data, arena);
		} break;

		invalid_default_case;
	}
	return false;
}

func b8 type_check_struct(s_node* node, s_error_reporter* reporter, s_type_check* data, s_lin_arena* arena)
{
	assert(!node->type_checked);

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
		printf("Added struct '%s'\n", node->token.str());
		data->structs.add(node);
	}
	return result;
}

func b8 type_check_func_decl(s_node* node, s_error_reporter* reporter, s_type_check* data, s_lin_arena* arena)
{
	assert(!node->type_checked);

	s_node* type = node_to_basic_type(node->func_decl.return_type, data);
	if(!type) {
		reporter->recoverable_error(node->func_decl.name.file, node->func_decl.name.line, "Function '%s' has unknown return type '%s'", node->func_decl.name.str(), node_to_str(node->func_decl.return_type));
		return false;
	}

	for_node(arg, node->func_decl.arguments) {
		if(arg->type_checked) { continue; }
		type = node_to_basic_type(arg->var_decl.type, data);
		if(type) {
			arg->type_checked = true;
		}
		else {
			reporter->recoverable_error(arg->var_decl.name.file, arg->var_decl.name.line, "Function argument '%s' has unknown type '%s'", arg->var_decl.name.str(), node_to_str(arg->var_decl.type));
			return false;
		}
	}

	node->type_checked = true;

	printf("Added function '%s'\n", node->func_decl.name.str());
	data->funcs.add(node);
	return true;
}

func b8 type_check_struct_member(s_node* nstruct, s_node* member, s_error_reporter* reporter, s_type_check* data, s_lin_arena* arena)
{
	unreferenced(nstruct);
	unreferenced(arena);

	assert(!member->type_checked);
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
		printf("type checked %s %s\n", node_to_str(member->var_decl.type), member->var_decl.name.str());
	}
	return result;
}

func s_node* get_struct_by_name(s_type_check* data, char* str)
{
	foreach_val(node_i, node, data->structs) {
		assert(node->type == e_node_struct);
		if(node->token.equals(str)) {
			return node;
		}
	}
	return null;
}


// func b8 compare_s_type(s_type a, s_type b)
// {
// 	return strcmp(a.name, b.name) == 0;
// }

func s_node* node_to_basic_type(s_node* node, s_type_check* data)
{
	switch(node->type) {
		case e_node_type: {
			foreach_val(type_i, type, data->types) {
				if(node->token.equals(type->basic_type.name)) {
					return type;
				}
			}
			foreach_val(nstruct_i, nstruct, data->structs) {
				// @TODO(tkap, 10/02/2024): is node->token always fine? seems sus
				if(node->token.equals(nstruct->token)) {
					return nstruct;
				}
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