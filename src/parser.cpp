

func s_node* parse(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
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

		pr = parse_function(tokenizer, reporter, arena);
		if(pr.success) {
			tokenizer = pr.tokenizer;
			current = advance_node(current, pr.node, arena);
			continue;
		}

		reporter->fatal(token.file, token.line, "Failed to parse something starting at:\nvvvvvvvvvv\n%.32s\n^^^^^^^^^^\n", token.at);
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
		if(!tokenizer.consume_token("struct", reporter)) { break; }
		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line); }
		if(is_keyword(token)) { reporter->fatal(token.file, token.line, "Struct name cannot be a reserved keyword"); }
		result.node.token = token;
		result.node.type = e_node_struct;
		if(!tokenizer.consume_token(e_token_open_brace, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line); }

		s_node** curr_struct_member = &result.node.nstruct.members;
		while(true) {
			s_parse_result pr = parse_type(tokenizer, reporter, arena);
			if(!pr.success) { break; }
			tokenizer = pr.tokenizer;
			if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) {
				reporter->fatal(tokenizer.file, tokenizer.line, "Expected a name after struct member type");
			}
			s_node member = zero;
			member.var_decl.name = token;
			member.var_decl.type = alloc_node(pr.node, arena);
			curr_struct_member = advance_node(curr_struct_member, member, arena);
			if(!tokenizer.consume_token(e_token_comma, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected ','"); }
			result.node.nstruct.member_count += 1;
		}
		if(result.node.nstruct.member_count <= 0) {
			reporter->fatal(tokenizer.file, result.node.token.line, "Structs must have at least 1 member");
		}

		if(!tokenizer.consume_token(e_token_close_brace, reporter)) {
			reporter->fatal(tokenizer.file, result.node.token.line, "Struct '%s' missing closing brace", result.node.token.str());
		}

		result.tokenizer = tokenizer;
		result.success = true;
	}

	return result;
}

func s_parse_result parse_function(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {
		if(!tokenizer.consume_token("func", reporter)) { break; }
		result.node.type = e_node_func_decl;
		s_parse_result pr = parse_type(tokenizer, reporter, arena);
		if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected a type after 'func'"); }
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

		if(!tokenizer.consume_token(e_token_close_paren, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected ')'"); }

		pr = parse_statement(tokenizer, reporter, arena);
		if(!pr.success || pr.node.type != e_node_compound) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected '{'");
		}
		tokenizer = pr.tokenizer;
		result.node.func_decl.body = alloc_node(pr.node, arena);

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
		if(!tokenizer.consume_token(e_token_identifier, &token, reporter)) { break; }
		result.node.token = token;
		result.node.type = e_node_type;

		while(true) {
			s_parse_result pr = parse_array(tokenizer, reporter, arena);
			if(!pr.success) { break; }
			tokenizer = pr.tokenizer;
			pr.node.left = alloc_node(result.node, arena);
			result.node = pr.node;
		}
		result.success = true;
		result.tokenizer = tokenizer;
	}

	return result;
}

func s_parse_result parse_array(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

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

	breakable_block {

		if(!tokenizer.consume_token(e_token_integer, &token, reporter)) { break; }
		result.node.type = e_node_integer;
		result.node.token = token;

		result.success = true;
		result.tokenizer = tokenizer;
	}

	return result;
}

func s_parse_result parse_expression(s_tokenizer tokenizer, s_error_reporter* reporter, int in_operator_level, s_lin_arena* arena)
{
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {

		s_parse_result pr = parse_sub_expression(tokenizer, reporter, arena);
		if(!pr.success) { break; }
		tokenizer = pr.tokenizer;
		result.node = pr.node;

		while(true) {
			s_parse_result operator_pr = parse_operator(tokenizer, reporter, arena);
			if(!operator_pr.success) { break; }
			int level = get_operator_precedence(operator_pr.node.token.type);
			if(level <= in_operator_level) { break; }

			tokenizer = operator_pr.tokenizer;
			pr = parse_expression(tokenizer, reporter, level, arena);
			if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression"); }
			tokenizer = pr.tokenizer;
			operator_pr.node.left = alloc_node(result.node, arena);
			operator_pr.node.right = alloc_node(pr.node, arena);
			result.node = operator_pr.node;
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
		result.success = true;
	}

	else if(tokenizer.consume_token("return", reporter)) {

		result.node.type = e_node_return;
		s_parse_result pr = parse_expression(tokenizer, reporter, 0, arena);
		if(pr.success) {
			result.node.nreturn.expression = alloc_node(pr.node, arena);
			tokenizer = pr.tokenizer;
		}

		if(!tokenizer.consume_token(e_token_semicolon, reporter)) {
			reporter->fatal(tokenizer.file, tokenizer.line, "Expected ';'");
		}

		result.success = true;
	}

	result.tokenizer = tokenizer;

	return result;
}

func s_node** advance_node(s_node** a2, s_node node, s_lin_arena* arena)
{
	assert(!*a2);
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
	return token.equals("if");
}

func int get_operator_precedence(e_token type)
{
	// @TODO(tkap, 10/02/2024): Cringe that we set up the array every call. Unless the compiler is smart...
	int arr[e_token_count] = zero;
	arr[e_token_plus] = 12;
	arr[e_token_minus] = 12;
	arr[e_token_asterisk] = 13;
	arr[e_token_forward_slash] = 13;
	arr[e_token_percent] = 13;

	return arr[type];
}

func void print_expression(s_node* node)
{
	switch(node->type) {
		case e_node_add: {
			printf("(");
			print_expression(node->left);
			printf("+");
			print_expression(node->right);
			printf(")");
		} break;
		case e_node_multiply: {
			printf("(");
			print_expression(node->left);
			printf("*");
			print_expression(node->right);
			printf(")");
		} break;
		case e_node_integer: {
			printf("%s", node->token.str());
		} break;

		invalid_default_case;
	}
}