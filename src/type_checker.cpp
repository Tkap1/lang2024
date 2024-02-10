

func void type_check_ast(s_node* ast, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_type_check* data = (s_type_check*)arena->alloc_zero(sizeof(s_type_check));

	data->types.add({.name = "int", .size_in_bytes = 4});

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
	data->structs.add(node);
	return result;
}

func b8 type_check_struct_member(s_node* nstruct, s_node* member, s_error_reporter* reporter, s_type_check* data, s_lin_arena* arena)
{
	unreferenced(nstruct);
	unreferenced(arena);

	assert(!member->type_checked);
	b8 result = true;

	// @TODO(tkap, 10/02/2024): Handle a member of type current_struct*
	// @TODO(tkap, 10/02/2024): check that array size makes sense
	s_type* type = node_to_s_type(member->var_decl.type, data);
	if(!type) {
		result = false;
		reporter->recoverable_error(member->var_decl.type->token.file, member->var_decl.type->token.line, "Struct member '%s' has unknown type '%s'", member->var_decl.name.str(), node_to_str(member->var_decl.type));
	}

	member->type_checked = result;
	if(result) {
		member->var_type = member->var_decl.type;
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

func s_node* type_to_node(s_type type, s_lin_arena* arena)
{
	s_node node = zero;
	node.type = e_node_type;
	node.actual_type = type;
	return alloc_node(node, arena);
}

// func b8 compare_s_type(s_type a, s_type b)
// {
// 	return strcmp(a.name, b.name) == 0;
// }

func s_type* node_to_s_type(s_node* node, s_type_check* data)
{
	switch(node->type) {
		case e_node_type: {
			foreach_ptr(type_i, type, data->types) {
				if(node->token.equals(type->name)) {
					return type;
				}
			}
		} break;

		case e_node_array: {
			return node_to_s_type(node->left, data);
		} break;

		invalid_default_case;
	}
	return zero;
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