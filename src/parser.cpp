

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

		pr = parse_var_decl(tokenizer, reporter, e_context_global, arena);
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
			member.type = e_node_var_decl;
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

		if(tokenizer.consume_token("const", reporter)) {
			result.node.ntype.is_const = true;
		}

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

	if(tokenizer.consume_token(e_token_integer, &token, reporter)) {
		result.node.type = e_node_integer;
		result.node.token = token;
		goto success;
	}
	if(tokenizer.consume_token(e_token_float, &token, reporter)) {
		result.node.type = e_node_float;
		result.node.token = token;
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
		}
		else if(pr.operator_data.node_type == e_node_subtract) {
			node_type = e_node_unary_minus;
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
	s_token token = zero;

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
				{e_token_times_equals, e_node_multiply},
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