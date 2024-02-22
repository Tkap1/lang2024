
// @TODO(tkap, 18/02/2024): Global alert
global int overload_id = 0;

func s_node* parse(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	overload_id = 0;
	s_node* ast = null;
	s_node** current = &ast;
	while(true) {
		s_token token = tokenizer.peek(reporter);
		if(token.type == e_token_eof) { break; }
		s_parse_result pr = parse_struct(tokenizer, reporter, arena);
		if(pr.success) {
			tokenizer = pr.tokenizer;
			current = advance_node(current, pr.node, arena);
			continue;
		}

		pr = parse_enum(tokenizer, reporter, arena);
		if(pr.success) {
			tokenizer = pr.tokenizer;
			current = advance_node(current, pr.node, arena);
			continue;
		}

		pr = parse_data_enum(tokenizer, reporter, arena);
		if(pr.success) {
			tokenizer = pr.tokenizer;
			current = advance_node(current, pr.node, arena);
			continue;
		}

		pr = parse_func_decl(tokenizer, reporter, arena);
		if(pr.success) {
			tokenizer = pr.tokenizer;
			current = advance_node(current, pr.node, arena);
			continue;
		}

		pr = parse_include(tokenizer, reporter, arena);
		if(pr.success) {
			tokenizer = pr.tokenizer;
			s_node* node = parse_step(pr.node.token.str(arena), reporter, arena, reporter->ignore_errors);
			if(!node) { continue; }
			*current = node;
			while(*current) {
				s_node* temp = *current;
				current = &temp->next;
			}
			continue;
		}

		pr = parse_external_func_decl(tokenizer, reporter, arena);
		if(pr.success) {
			tokenizer = pr.tokenizer;
			current = advance_node(current, pr.node, arena);
			continue;
		}

		pr = parse_var_decl(tokenizer, reporter, e_context_global, arena);
		if(pr.success) {
			tokenizer = pr.tokenizer;
			current = advance_node(current, pr.node, arena);
			continue;
		}

		reporter->fatal(token.file, token.line, "Failed to parse something starting at:\nvvvvvvvvvv\n%.32s\n^^^^^^^^^^\n", token.at);

		if(reporter->error_level >= e_error_level_fatal) {
			return null;
		}
	}

	// for_node(node, ast->nstruct.members) {
	// 	printf("%.*s %.*s,\n", ft(node->token), ft(node->left->token));
	// }

	return ast;
}

func s_parse_result parse_struct(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {
		s_parse_result pr = parse_pack(tokenizer, reporter);
		b8 is_packed = false;
		if(pr.success) {
			tokenizer = pr.tokenizer;
			is_packed = true;
		}
		if(!tokenizer.consume_token("struct", reporter)) { break; }
		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line); }
		if(is_keyword(token)) { reporter->fatal(token.file, token.line, "Struct name cannot be a reserved keyword"); }
		result.node.token = token;
		result.node.type = e_node_struct;
		result.node.nstruct.is_packed = is_packed;
		if(!tokenizer.consume_token(e_token_open_brace, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line); }

		s_node** curr_struct_member = &result.node.nstruct.members;
		while(true) {
			s_node member = zero;
			if(tokenizer.consume_token("import", reporter)) {
				member.var_decl.is_import = true;
			}
			pr = parse_type(tokenizer, reporter, arena);
			if(!pr.success) { break; }
			tokenizer = pr.tokenizer;
			if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Expected a name after struct member type");
			}
			member.type = e_node_var_decl;
			member.var_decl.name = token;
			member.var_decl.type = alloc_node(pr.node, arena);
			curr_struct_member = advance_node(curr_struct_member, member, arena);
			if(!tokenizer.consume_token(e_token_comma, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected ','"); return result; }
			result.node.nstruct.member_count += 1;
		}
		if(result.node.nstruct.member_count <= 0) {
			reporter->fatal(tokenizer.file, result.node.token.line, "Structs must have at least 1 member");
		}

		if(!tokenizer.consume_token(e_token_close_brace, reporter)) {
			reporter->fatal(tokenizer.file, result.node.token.line, "Struct '%s' missing closing brace", result.node.token.str(arena));
		}

		result.tokenizer = tokenizer;
		result.success = true;
	}

	return result;
}

func s_parse_result parse_enum(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {
		if(!tokenizer.consume_token("enum", reporter)) { break; }
		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Enum missing name"); }
		if(is_keyword(token)) {reporter->fatal(tokenizer.file, tokenizer.line, "Enum name cannot be a reserved keyword"); }
		if(!tokenizer.consume_token(e_token_open_brace, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected '{' after 'enum'"); }

		result.node.token = token;
		result.node.type = e_node_enum;
		s_node** curr_member = &result.node.nenum.members;

		while(true) {
			if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { break; }
			s_node member = zero;
			member.token = token;
			if(is_keyword(token)) {reporter->fatal(tokenizer.file, tokenizer.line, "Enum member name cannot be a reserved keyword"); }
			curr_member = advance_node(curr_member, member, arena);
			result.node.nenum.member_count += 1;
			if(!tokenizer.consume_token(e_token_comma, &token, reporter)) { break; }
		}
		if(result.node.nenum.member_count <= 0) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Enums must have at least 1 member");
		}

		if(!tokenizer.consume_token(e_token_close_brace, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected '}' after 'enum'"); }

		result.tokenizer = tokenizer;
		result.success = true;
	}

	return result;
}

func s_parse_result parse_data_enum(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {
		if(!tokenizer.consume_token("data_enum", reporter)) { break; }
		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "data_enum missing name");
			return result;
		}
		if(is_keyword(token)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "data_enum name cannot be a reserved keyword");
			return result;
		}
		if(!tokenizer.consume_token(e_token_open_brace, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected '{' after 'data_enum'");
			return result;
		}

		result.node.token = token;
		result.node.type = e_node_data_enum;

		s_parse_result pr = parse_struct(tokenizer, reporter, arena);
		if(!pr.success) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected a struct inside data_enum");
			return result;
		}
		tokenizer = pr.tokenizer;
		result.node.data_enum.nstruct = alloc_node(pr.node, arena);

		s_node** curr_member = &result.node.data_enum.members;

		// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		members start		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
		while(true) {
			if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { break; }
			s_node member = zero;
			member.type = e_node_data_enum_member;
			member.token = token;
			if(is_keyword(token)) {
				reporter->fatal(tokenizer.file, tokenizer.line, "data_enum member name cannot be a reserved keyword");
				return result;
			}

			if(!tokenizer.consume_token(e_token_open_brace, reporter)) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Expected '{'");
				return result;
			}

			// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		fields start		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
			s_node** curr_field = &member.data_enum_member.members;
			while(true) {
				s_node field = zero;
				if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) {
					break;
				}
				field.token = token;
				if(!tokenizer.consume_token(e_token_assign, reporter)) {
					reporter->fatal(tokenizer.file, tokenizer.line, "Expected '='");
					return result;
				}
				pr = parse_expression(tokenizer, reporter, 0, arena);
				if(!pr.success) {
					reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression");
					return result;
				}
				tokenizer = pr.tokenizer;
				field.left = alloc_node(pr.node, arena);
				member.data_enum_member.member_count += 1;
				curr_field = advance_node(curr_field, field, arena);
				if(!tokenizer.consume_token(e_token_comma, &token, reporter)) { break; }
			}
			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		fields end		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

			if(member.data_enum_member.member_count <= 0) {
				reporter->fatal(tokenizer.file, tokenizer.line, "data_enum member must have at least 1 field");
			}

			if(!tokenizer.consume_token(e_token_close_brace, reporter)) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Expected '}'");
				return result;
			}

			curr_member = advance_node(curr_member, member, arena);
			result.node.data_enum.member_count += 1;
		}
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		members end		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		if(result.node.data_enum.member_count <= 0) {
			reporter->fatal(tokenizer.file, tokenizer.line, "data_enums must have at least 1 member");
		}

		if(!tokenizer.consume_token(e_token_close_brace, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected '}' after 'enum'"); }

		result.tokenizer = tokenizer;
		result.success = true;
	}

	return result;
}

func s_parse_result parse_func_decl(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {
		result.node.func_decl.is_dll_export = tokenizer.consume_token("dll_export", reporter);
		// @TODO(tkap, 22/02/2024): We should probably fail if we find "dll_export" but then don't find "func"
		if(!tokenizer.consume_token("func", reporter)) { break; }
		result.node.type = e_node_func_decl;
		s_parse_result pr = parse_type(tokenizer, reporter, arena);
		if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected a type after 'func'"); }
		tokenizer = pr.tokenizer;
		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected function name"); return result; }
		result.node.func_decl.name = token;
		result.node.func_decl.return_type = alloc_node(pr.node, arena);

		if(token.equals("operator")) {
			result.node.func_decl.is_operator_overload = true;
			pr = parse_operator(tokenizer, reporter, arena);
			if(!pr.success) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Expected operator");
				return result;
			}
			tokenizer = pr.tokenizer;
			result.node.func_decl.operator_overload = pr.operator_data.node_type;
		}
		else if(is_keyword(token)) {
			reporter->fatal(token.file, token.line, "Function name cannot be a reserved keyword");
			return result;
		}

		if(!tokenizer.consume_token(e_token_open_paren, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected '('"); return result; }

		s_node** curr_argument = &result.node.func_decl.arguments;
		while(true) {
			pr = parse_type(tokenizer, reporter, arena);
			if(!pr.success) { break; }
			tokenizer = pr.tokenizer;
			if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected argument name"); }
			if(is_keyword(token)) { reporter->fatal(token.file, token.line, "Argument name cannot be a reserved keyword"); }
			s_node node = zero;
			node.type = e_node_var_decl;
			node.var_decl.type = alloc_node(pr.node, arena);
			node.var_decl.name = token;
			curr_argument = advance_node(curr_argument, node, arena);
			result.node.func_decl.argument_count += 1;

			if(!tokenizer.consume_token(e_token_comma, reporter)) { break; }
		}

		if(result.node.func_decl.is_operator_overload && result.node.func_decl.argument_count != 2) {
			reporter->fatal(tokenizer.file, tokenizer.line, "An operator overload must have exactly 2 arguments");
			return result;
		}

		if(!tokenizer.consume_token(e_token_close_paren, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected ')'"); }

		pr = parse_statement(tokenizer, reporter, arena);
		if(!pr.success || pr.node.type != e_node_compound) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected '{'");
		}
		tokenizer = pr.tokenizer;
		result.node.func_decl.body = alloc_node(pr.node, arena);

		if(result.node.func_decl.is_operator_overload) {
			char* name = alloc_str(arena, "__overload%i__", overload_id++);
			result.node.func_decl.name = {.type = e_token_identifier, .len = (int)strlen(name), .at = name};
		}

		result.tokenizer = tokenizer;
		result.success = true;
	}

	return result;
}

func s_parse_result parse_include(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	unreferenced(arena);
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {
		result.node.type = e_node_include;
		if(!tokenizer.consume_token(e_token_pound, reporter)) { break; }
		if(!tokenizer.consume_token("include", reporter)) { break; }
		if(!tokenizer.consume_token(e_token_string, &token, reporter)) {
			reporter->fatal(token.file, token.line, "Expected string after '#include'");
			return result;
		}
		result.node.token = token;
		result.tokenizer = tokenizer;
		result.success = true;
	}

	return result;
}

func s_parse_result parse_pack(s_tokenizer tokenizer, s_error_reporter* reporter)
{
	s_parse_result result = zero;
	breakable_block {
		if(!tokenizer.consume_token(e_token_pound, reporter)) { break; }
		if(!tokenizer.consume_token("pack", reporter)) { break; }
		result.tokenizer = tokenizer;
		result.success = true;
	}

	return result;
}

func s_parse_result parse_external_func_decl(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {
		if(!tokenizer.consume_token("external_func", reporter)) { break; }
		result.node.type = e_node_func_decl;
		result.node.func_decl.is_external = true;
		s_parse_result pr = parse_type(tokenizer, reporter, arena);
		if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected a type after 'external_func'"); }
		tokenizer = pr.tokenizer;
		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected function name"); }
		result.node.func_decl.name = token;
		result.node.func_decl.return_type = alloc_node(pr.node, arena);
		if(is_keyword(token)) { reporter->fatal(token.file, token.line, "Function name cannot be a reserved keyword"); }

		if(!tokenizer.consume_token(e_token_open_paren, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected '('"); }

		s_node** curr_argument = &result.node.func_decl.arguments;
		while(true) {
			pr = parse_type(tokenizer, reporter, arena);
			if(!pr.success) { break; }
			tokenizer = pr.tokenizer;
			curr_argument = advance_node(curr_argument, pr.node, arena);
			result.node.func_decl.argument_count += 1;
			if(!tokenizer.consume_token(e_token_comma, reporter)) { break; }
		}

		if(!tokenizer.consume_token(e_token_close_paren, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected ')'"); }

		if(!tokenizer.consume_token(e_token_semicolon, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ';'");
		}

		result.tokenizer = tokenizer;
		result.success = true;
	}

	return result;
}

func s_parse_result parse_type(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {

		if(tokenizer.consume_token("const", reporter)) {
			result.is_const = true;
		}

		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { break; }
		result.node.token = token;
		result.node.type = e_node_type;
		if(is_keyword(token)) {
			reporter->fatal(token.file, token.line, "Variable type cannot be a reserved keyword");
			return result;
		}

		while(true) {
			s_parse_result pr = parse_array(tokenizer, reporter, arena);
			if(!pr.success) { break; }
			tokenizer = pr.tokenizer;
			pr.node.left = alloc_node(result.node, arena);
			result.node = pr.node;
		}

		while(tokenizer.consume_token(e_token_asterisk, reporter)) {
			result.node.pointer_level += 1;
		}

		result.success = true;
		result.tokenizer = tokenizer;
	}

	return result;
}

func s_parse_result parse_array(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;

	breakable_block {

		if(!tokenizer.consume_token(e_token_open_bracket, reporter)) { break; }
		result.node.type = e_node_array;
		s_parse_result pr = parse_expression(tokenizer, reporter, 0, arena);
		if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression inside []"); }
		tokenizer = pr.tokenizer;
		result.node.array.size_expr = alloc_node(pr.node, arena);

		if(!tokenizer.consume_token(e_token_close_bracket, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected ']'"); }

		result.success = true;
		result.tokenizer = tokenizer;
	}

	return result;
}

func s_parse_result parse_operator(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	unreferenced(arena);
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {

		b8 parsed_something = false;
		for(int operator_i = 0; operator_i < array_count(c_operator_data); operator_i++) {
			s_operator_data data = c_operator_data[operator_i];
			if(tokenizer.consume_token(data.token_type, &token, reporter)) {
				parsed_something = true;
				// @TODO(tkap, 10/02/2024): do we want to set node.type to something?
				result.node.token = token;
				result.node.type = data.node_type;
				result.operator_data = data;
				break;
			}
		}
		if(!parsed_something) { break; }

		result.success = true;
		result.tokenizer = tokenizer;
	}

	return result;
}

func s_parse_result parse_sub_expression(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	unreferenced(arena);
	s_parse_result result = zero;
	s_token token = zero;


	if(tokenizer.consume_token(e_token_open_paren, reporter)) {

		s_parse_result pr = parse_expression(tokenizer, reporter, 0, arena);
		if(!pr.success) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression");
		}
		tokenizer = pr.tokenizer;
		result.node = pr.node;

		if(!tokenizer.consume_token(e_token_close_paren, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ')'");
		}
		goto success;
	}

	if(tokenizer.consume_token(e_token_open_brace, reporter)) {
		result.node.type = e_node_struct_literal;
		s_node** curr_expr = &result.node.struct_literal.expressions;
		while(true) {
			s_parse_result pr = parse_expression(tokenizer, reporter, 0, arena);
			if(!pr.success) { break; }
			tokenizer = pr.tokenizer;
			curr_expr = advance_node(curr_expr, pr.node, arena);
			if(!tokenizer.consume_token(e_token_comma, reporter)) { break; }
		}

		if(!tokenizer.consume_token(e_token_close_brace, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected '}' to end struct literal");
		}

		goto success;
	}

	if(tokenizer.consume_token(e_token_integer, &token, reporter)) {
		result.node.type = e_node_integer;
		result.node.token = token;
		result.node.integer.value = atoi(token.str(arena));
		goto success;
	}
	if(tokenizer.consume_token(e_token_float, &token, reporter)) {
		result.node.type = e_node_float;
		result.node.token = token;
		result.node.nfloat.value = (float)atof(token.str(arena));
		goto success;
	}
	if(tokenizer.consume_token(e_token_identifier, &token, reporter)) {
		result.node.type = e_node_identifier;
		result.node.token = token;
		goto success;
	}
	if(tokenizer.consume_token(e_token_string, &token, reporter)) {
		result.node.type = e_node_string;
		result.node.token = token;
		goto success;
	}

	s_parse_result pr = parse_operator(tokenizer, reporter, arena);
	if(pr.success) {
		e_node node_type = e_node_invalid;
		// @TODO(tkap, 10/02/2024): we can make this into a for loop. this is cringe
		if(pr.operator_data.node_type == e_node_logic_not) {
			node_type = e_node_logic_not;
			pr.operator_data.precedence = 15;
		}
		else if(pr.operator_data.node_type == e_node_subtract) {
			node_type = e_node_unary_minus;
			pr.operator_data.precedence = 15;
		}
		else if(pr.operator_data.node_type == e_node_address_of) {
			node_type = e_node_address_of;
			pr.operator_data.precedence = 15;
		}
		else if(pr.operator_data.node_type == e_node_multiply) {
			node_type = e_node_dereference;
			pr.operator_data.precedence = 15;
		}
		else {
			goto end;
		}
		tokenizer = pr.tokenizer;
		pr = parse_expression(tokenizer, reporter, pr.operator_data.precedence, arena);
		if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression"); }
		tokenizer = pr.tokenizer;
		result.node.type = node_type;
		result.node.left = alloc_node(pr.node, arena);
		goto success;
	}

	goto end;

	success:
	result.success = true;
	result.tokenizer = tokenizer;

	end:

	return result;
}

func s_parse_result parse_expression(s_tokenizer tokenizer, s_error_reporter* reporter, int in_operator_level, s_lin_arena* arena)
{
	s_parse_result result = zero;

	breakable_block {

		s_parse_result pr = parse_sub_expression(tokenizer, reporter, arena);
		if(!pr.success) { break; }
		tokenizer = pr.tokenizer;
		result.node = pr.node;

		while(true) {
			s_parse_result operator_pr = parse_operator(tokenizer, reporter, arena);
			if(!operator_pr.success) { break; }
			int level = operator_pr.operator_data.precedence;
			if(level <= in_operator_level) { break; }
			tokenizer = operator_pr.tokenizer;

			if(operator_pr.operator_data.node_type == e_node_func_call) {
				s_node node = zero;
				node.type = e_node_func_call;
				node.left = alloc_node(result.node, arena);
				result.node = node;

				s_node** curr_arg = &result.node.func_call.arguments;
				while(true) {
					pr = parse_expression(tokenizer, reporter, 0, arena);
					if(!pr.success) { break; }
					tokenizer = pr.tokenizer;
					curr_arg = advance_node(curr_arg, pr.node, arena);

					if(!tokenizer.consume_token(e_token_comma, reporter)) { break; }
				}

				if(!tokenizer.consume_token(e_token_close_paren, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected ')'"); }
			}

			else if(operator_pr.operator_data.node_type == e_node_subscript) {
				s_node node = zero;
				node.type = e_node_subscript;
				node.left = alloc_node(result.node, arena);

				pr = parse_expression(tokenizer, reporter, 0, arena);
				if(!pr.success) {
					reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression inside '[]'");
				}
				tokenizer = pr.tokenizer;
				node.right = alloc_node(pr.node, arena);

				if(!tokenizer.consume_token(e_token_close_bracket, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected ']'"); }
				result.node = node;
			}
			else {
				pr = parse_expression(tokenizer, reporter, level, arena);
				if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression"); }
				tokenizer = pr.tokenizer;

				operator_pr.node.left = alloc_node(result.node, arena);
				operator_pr.node.right = alloc_node(pr.node, arena);
				result.node = operator_pr.node;
			}
		}

		result.success = true;
		result.tokenizer = tokenizer;
	}

	return result;
}

func s_parse_result parse_var_decl(s_tokenizer tokenizer, s_error_reporter* reporter, int context, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {

		s_parse_result pr = parse_type(tokenizer, reporter, arena);
		if(!pr.success) { break; }
		result.node.var_decl.is_const = pr.is_const;
		tokenizer = pr.tokenizer;
		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { break; }

		result.node.type = e_node_var_decl;
		result.node.var_decl.type = alloc_node(pr.node, arena);
		result.node.var_decl.name = token;

		if(tokenizer.peek_token(e_token_assign, reporter)) {
			if(context & e_context_struct) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Unexpected '='");
			}
			tokenizer.next_token(reporter);

			pr = parse_expression(tokenizer, reporter, 0, arena);
			if(!pr.success) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression after '='");
			}
			tokenizer = pr.tokenizer;
			result.node.var_decl.value = alloc_node(pr.node, arena);
		}

		if(context & e_context_global || context & e_context_statement) {
			if(!tokenizer.consume_token(e_token_semicolon, reporter)) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Expected ';'");
			}
		}

		result.success = true;
		result.tokenizer = tokenizer;
	}

	return result;
}

func s_parse_result parse_statement(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	if(tokenizer.consume_token(e_token_open_brace, reporter)) {

		result.node.type = e_node_compound;

		s_node** curr_statement = &result.node.compound.statements;
		while(true) {
			s_parse_result pr = parse_statement(tokenizer, reporter, arena);
			if(!pr.success) { break; }
			tokenizer = pr.tokenizer;
			curr_statement = advance_node(curr_statement, pr.node, arena);
		}

		if(!tokenizer.consume_token(e_token_close_brace, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected '}'");
		}

		goto success;
	}

	if(tokenizer.consume_token("import", reporter)) {

		result.node.type = e_node_import;
		s_parse_result pr = parse_expression(tokenizer, reporter, 0, arena);
		if(pr.success) {
			result.node.left = alloc_node(pr.node, arena);
			tokenizer = pr.tokenizer;
		}

		if(!tokenizer.consume_token(e_token_semicolon, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ';'");
		}

		goto success;
	}

	if(tokenizer.consume_token("func_ptr", reporter)) {

		result.node.type = e_node_func_ptr;
		s_parse_result pr = parse_type(tokenizer, reporter, arena);
		if(!pr.success) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected a type after 'func_ptr'");
			return result;
		}
		result.node.func_ptr.type = alloc_node(pr.node, arena);
		tokenizer = pr.tokenizer;

		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected identifier");
			return result;
		}
		result.node.func_ptr.name = token;

		if(!tokenizer.consume_token(e_token_open_paren, &token, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected '('");
			return result;
		}

		s_node** curr_arg = &result.node.func_ptr.arguments;
		while(true) {
			pr = parse_type(tokenizer, reporter, arena);
			if(!pr.success) { break; }
			tokenizer = pr.tokenizer;
			curr_arg = advance_node(curr_arg, pr.node, arena);
			result.node.func_ptr.argument_count += 1;
			if(!tokenizer.consume_token(e_token_comma, reporter)) {
				break;
			}
		}

		if(!tokenizer.consume_token(e_token_close_paren, &token, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ')'");
			return result;
		}

		if(!tokenizer.consume_token(e_token_semicolon, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ';'");
			return result;
		}

		goto success;
	}

	if(tokenizer.consume_token("return", reporter)) {

		result.node.type = e_node_return;
		s_parse_result pr = parse_expression(tokenizer, reporter, 0, arena);
		if(pr.success) {
			result.node.nreturn.expression = alloc_node(pr.node, arena);
			tokenizer = pr.tokenizer;
		}

		if(!tokenizer.consume_token(e_token_semicolon, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ';'");
		}

		goto success;
	}

	if(tokenizer.consume_token("continue", reporter)) {
		result.node.type = e_node_continue;
		if(!tokenizer.consume_token(e_token_semicolon, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ';' after 'continue'");
		}
		goto success;
	}

	if(tokenizer.consume_token("break", reporter)) {
		result.node.type = e_node_break;
		if(!tokenizer.consume_token(e_token_semicolon, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ';' after 'continue'");
		}
		goto success;
	}

	if(tokenizer.consume_token("while", reporter)) {

		result.node.type = e_node_while;
		s_parse_result pr = parse_expression(tokenizer, reporter, 0, arena);
		if(pr.success) {
			result.node.nwhile.condition = alloc_node(pr.node, arena);
			tokenizer = pr.tokenizer;
		}

		pr = parse_statement(tokenizer, reporter, arena);
		if(!pr.success || pr.node.type != e_node_compound) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected '{' after 'while'");
		}
		tokenizer = pr.tokenizer;
		result.node.nwhile.body = alloc_node(pr.node, arena);

		goto success;
	}

	if(tokenizer.consume_token("for", reporter)) {

		result.node.type = e_node_for;
		s_tokenizer copy = tokenizer;
		if(copy.consume_token(e_token_identifier, &token, reporter) && copy.consume_token(e_token_colon, reporter)) {
			tokenizer = copy;
			result.node.nfor.iterator_name = token;
		}

		s_parse_result pr = parse_expression(tokenizer, reporter, 0, arena);
		if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression after 'for'"); }
		result.node.nfor.expr = alloc_node(pr.node, arena);
		tokenizer = pr.tokenizer;

		pr = parse_statement(tokenizer, reporter, arena);
		if(!pr.success || pr.node.type != e_node_compound) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected '{' after 'for'");
		}
		tokenizer = pr.tokenizer;
		result.node.nfor.body = alloc_node(pr.node, arena);

		goto success;
	}

	if(tokenizer.consume_token("if", reporter)) {

		result.node.type = e_node_if;
		s_parse_result pr = parse_expression(tokenizer, reporter, 0, arena);
		if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression after 'if'"); }
		result.node.nif.condition = alloc_node(pr.node, arena);
		tokenizer = pr.tokenizer;

		pr = parse_statement(tokenizer, reporter, arena);
		if(!pr.success || pr.node.type != e_node_compound) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected '{' after 'if'");
		}
		tokenizer = pr.tokenizer;
		result.node.nif.body = alloc_node(pr.node, arena);

		if(tokenizer.consume_token("else", reporter)) {
			pr = parse_statement(tokenizer, reporter, arena);
			if(!pr.success || pr.node.type != e_node_compound) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Expected '{' after 'else'");
			}
			tokenizer = pr.tokenizer;
			result.node.nif.nelse = alloc_node(pr.node, arena);
		}

		goto success;
	}


	s_parse_result pr = parse_var_decl(tokenizer, reporter, e_context_statement, arena);
	if(pr.success) {
		tokenizer = pr.tokenizer;
		result.node = pr.node;
		goto success;
	}

	pr = parse_expression(tokenizer, reporter, 0, arena);
	if(pr.success) {
		tokenizer = pr.tokenizer;
		result.node = pr.node;

		if(tokenizer.consume_token(e_token_assign, reporter)) {
			s_node node = zero;
			node.type = e_node_assign;
			node.left = alloc_node(pr.node, arena);

			pr = parse_expression(tokenizer, reporter, 0, arena);
			if(!pr.success) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression after '='");
			}
			tokenizer = pr.tokenizer;
			node.right = alloc_node(pr.node, arena);
			result.node = node;
		}
		else {

			struct s_temp
			{
				e_token token_type;
				e_node node_type;
			};
			constexpr s_temp c_assignments[] = {
				{e_token_plus_equals, e_node_add},
				{e_token_minus_equals, e_node_subtract},
				{e_token_times_equals, e_node_multiply},
				{e_token_divide_equals, e_node_divide},
			};

			// @Note(tkap, 11/02/2024): We are turning "a *= b" into "a = a * (b)"
			for(int assign_i = 0; assign_i < array_count(c_assignments); assign_i++) {
				s_temp data = c_assignments[assign_i];
				if(tokenizer.consume_token(data.token_type, reporter)) {
					s_node node = zero;
					node.type = e_node_assign;
					node.left = alloc_node(pr.node, arena);

					s_node right = zero;
					right.type = data.node_type;
					right.left = alloc_node(pr.node, arena);

					pr = parse_expression(tokenizer, reporter, 0, arena);
					if(!pr.success) {
						reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression after");
					}
					tokenizer = pr.tokenizer;

					right.right = alloc_node(pr.node, arena);
					node.right = alloc_node(right, arena);
					result.node = node;
					break;
				}
			}
		}

		if(!tokenizer.consume_token(e_token_semicolon, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ';'");
		}
		goto success;
	}

	goto end;

	success:
	result.tokenizer = tokenizer;
	result.success = true;

	end:

	return result;
}

func s_node** advance_node(s_node** a2, s_node node, s_lin_arena* arena)
{
	assert(!*a2);
	*a2 = alloc_node(node, arena);
	s_node* a1 = *a2;
	return &a1->next;
}

func s_node** advance_node_remove_next(s_node** a2, s_node node, s_lin_arena* arena)
{
	node.next = null;
	*a2 = alloc_node(node, arena);
	s_node* a1 = *a2;
	return &a1->next;
}

func s_node* alloc_node(s_node node, s_lin_arena* arena)
{
	s_node* new_node = (s_node*)arena->alloc(sizeof(s_node));
	*new_node = node;
	return new_node;
}

func b8 is_keyword(s_token token)
{
	// @TODO(tkap, 10/02/2024):
	constexpr char* c_keywords[] = {
		"if", "struct", "for", "while", "enum", "else", "import", "operator", "data_enum",
	};
	for(int keyword_i = 0; keyword_i < array_count(c_keywords); keyword_i++) {
		if(token.equals(c_keywords[keyword_i])) { return true; }
	}
	return false;
}

func void print_expression(s_node* node, s_lin_arena* arena)
{
	switch(node->type) {
		case e_node_add: {
			printf("(");
			print_expression(node->left, arena);
			printf("+");
			print_expression(node->right, arena);
			printf(")");
		} break;
		case e_node_multiply: {
			printf("(");
			print_expression(node->left, arena);
			printf("*");
			print_expression(node->right, arena);
			printf(")");
		} break;
		case e_node_integer: {
			printf("%s", node->token.str(arena));
		} break;

		invalid_default_case;
	}
}

func s_node statement_str_to_node(char* str, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_tokenizer tokenizer = zero;
	tokenizer.at = str;
	s_parse_result pr = parse_statement(tokenizer, reporter, arena);
	assert(pr.success);
	return pr.node;
}