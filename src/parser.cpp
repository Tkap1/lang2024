

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
		if(is_keyword(token)) { reporter->fatal(tokenizer.file, token.line, "Struct name cannot be a reversed keyword"); }
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
		s_parse_result pr = parse_expression(tokenizer, reporter, arena);
		if(!pr.success) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected expression inside []"); }
		tokenizer = pr.tokenizer;
		result.node.array.size_expr = alloc_node(pr.node, arena);

		if(!tokenizer.consume_token(e_token_close_bracket, reporter)) { reporter->fatal(tokenizer.file, tokenizer.line, "Expected ']'"); }

		result.success = true;
		result.tokenizer = tokenizer;
	}

	return result;
}

func s_parse_result parse_expression(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena)
{
	unreferenced(reporter);
	unreferenced(arena);
	s_parse_result result = zero;
	s_token token = zero;

	breakable_block {
		// @TODO(tkap, 10/02/2024):
		if(!tokenizer.consume_token(e_token_integer, &token, reporter)) { break; }
		result.node.type = e_node_integer;
		result.node.token = token;

		result.success = true;
		result.tokenizer = tokenizer;
	}

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
