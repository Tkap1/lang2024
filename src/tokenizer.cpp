

func b8 is_newline(char c)
{
	return c == '\n';
}

func b8 is_whitespace(char c)
{
	return c == ' ' || c == '\t' || c == '\r' || is_newline(c);
}

func b8 is_number(char c)
{
	return c >= '0' && c <= '9';
}

func b8 is_alpha(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

func b8 is_alphanum(char c)
{
	return is_number(c) || is_alpha(c);
}

func b8 begins_identifier(char c)
{
	return c == '_' || is_alpha(c);
}

func b8 continues_identifier(char c)
{
	return c == '_' || is_alphanum(c);
}

s_token s_tokenizer::peek(s_error_reporter* reporter, int offset)
{
	s_tokenizer copy = *this;
	for(int i = 0; i < offset; i++) {
		copy.next_token(reporter);
	}
	return copy.next_token(reporter);
}

s_token s_tokenizer::next_token(s_error_reporter* reporter)
{
	s_token token = zero;
	while(true) {
		b8 ate_something = eat_whitespace();
		ate_something = eat_comment() || ate_something;
		if(!ate_something) { break; }
	}
	token.line = line;
	token.file = file;

	if(*at == 0) {
		token.type = e_token_eof;
	}

	// @Note(tkap, 24/02/2024): auto cast
	else if(*at == 'x' && at[1] == 'x' && !continues_identifier(at[2])) {
		token.type = e_token_auto_cast;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(begins_identifier(*at)) {
		char* start = at;
		at += 1;
		while(continues_identifier(*at)) {
			at += 1;
		}

		token.type = e_token_identifier;
		token.at = start;
		token.len = ptrs_to_int(start, at);
	}

	else if(is_number(*at)) {
		e_token token_type = e_token_integer;
		char* start = at;
		at += 1;
		while(true) {
			while(is_number(*at)) {
				at += 1;
			}
			if(*at == '.' && at[1] == '.') {
				break;
			}
			else if(*at == '.') {
				token_type = e_token_float;
				at += 1;
				continue;
			}
			break;
		}

		token.type = token_type;
		token.at = start;
		token.len = ptrs_to_int(start, at);
	}

	else if(*at == '"') {
		at += 1;
		char* start = at;
		while(!(*at == '"' && at[-1] != '\\')) {
			at += 1;
		}

		token.type = e_token_string;
		token.at = start;
		token.len = ptrs_to_int(start, at);
		at += 1;
	}

	else if(*at == '.' && at[1] == '.' && at[2] == '.') {
		token.type = e_token_var_args;
		token.at = at;
		token.len = 3;
		at += 3;
	}

	else if(*at == '|' && at[1] == '|') {
		token.type = e_token_logic_or;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '=' && at[1] == '=') {
		token.type = e_token_comparison;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '!' && at[1] == '=') {
		token.type = e_token_not_equals;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '&' && at[1] == '&') {
		token.type = e_token_logic_and;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '*' && at[1] == '=') {
		token.type = e_token_times_equals;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '/' && at[1] == '=') {
		token.type = e_token_divide_equals;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '+' && at[1] == '=') {
		token.type = e_token_plus_equals;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '-' && at[1] == '=') {
		token.type = e_token_minus_equals;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '>' && at[1] == '=') {
		token.type = e_token_greater_than_or_equal;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '<' && at[1] == '=') {
		token.type = e_token_less_than_or_equal;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '.' && at[1] == '.') {
		token.type = e_token_dot_dot;
		token.at = at;
		token.len = 2;
		at += 2;
	}

	else if(*at == '{') {
		token.type = e_token_open_brace;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '}') {
		token.type = e_token_close_brace;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '[') {
		token.type = e_token_open_bracket;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == ']') {
		token.type = e_token_close_bracket;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '(') {
		token.type = e_token_open_paren;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == ')') {
		token.type = e_token_close_paren;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == ';') {
		token.type = e_token_semicolon;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == ':') {
		token.type = e_token_colon;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == ',') {
		token.type = e_token_comma;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '!') {
		token.type = e_token_logic_not;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '&') {
		token.type = e_token_address_of;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '#') {
		token.type = e_token_pound;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '+') {
		token.type = e_token_plus;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '.') {
		token.type = e_token_dot;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '=') {
		token.type = e_token_assign;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '-') {
		token.type = e_token_minus;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '*') {
		token.type = e_token_asterisk;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '/') {
		token.type = e_token_forward_slash;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '%') {
		token.type = e_token_percent;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '>') {
		token.type = e_token_greater_than;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	else if(*at == '<') {
		token.type = e_token_less_than;
		token.at = at;
		token.len = 1;
		at += 1;
	}

	if(token.type == e_token_invalid) {
		reporter->fatal(file, token.line, "Bad token: %c", *at);
	}
	return token;
}

b8 s_tokenizer::eat_whitespace()
{
	b8 result = false;
	while(true) {
		if(is_newline(*at)) {
			result = true;
			at += 1;
			line += 1;
			continue;
		}
		if(is_whitespace(*at)) {
			result = true;
			at += 1;
			continue;
		}
		break;
	}
	return result;
}

b8 s_tokenizer::eat_comment()
{
	b8 result = false;
	if(*at == '/' && at[1] == '/') {
		result = true;
		at += 2;
		while(true) {
			if(*at == 0) { break; }
			else if(*at == '\n') {
				at += 1;
				line += 1;
				break;
			}
			at += 1;
		}
	}
	return result;
}

b8 s_tokenizer::consume_token(char* str, s_error_reporter* reporter)
{
	int len = (int)strlen(str);
	assert(len > 0);

	s_tokenizer copy = *this;
	s_token token = copy.next_token(reporter);
	if(token.len != len) { return false; }
	if(strncmp(token.at, str, len) == 0) {
		*this = copy;
		return true;
	}
	return false;
}

b8 s_tokenizer::consume_token(e_token type, s_error_reporter* reporter)
{
	assert(type > e_token_invalid);

	s_tokenizer copy = *this;
	s_token token = copy.next_token(reporter);
	if(token.type == type) {
		*this = copy;
		return true;
	}
	return false;
}

b8 s_tokenizer::peek_token(e_token type, s_error_reporter* reporter, int offset)
{
	assert(type > e_token_invalid);
	s_token token = peek(reporter, offset);
	if(token.type == type) { return true; }
	return false;
}

b8 s_tokenizer::consume_token(e_token type, s_token* out_token, s_error_reporter* reporter)
{
	assert(type > e_token_invalid);
	assert(out_token);

	s_tokenizer copy = *this;
	s_token token = copy.next_token(reporter);
	if(token.type == type) {
		*this = copy;
		*out_token = token;
		return true;
	}
	return false;
}

b8 s_token::equals(char* in_str)
{
	int str_len = (int)strlen(in_str);
	if(len != str_len) { return false; }
	return strncmp(at, in_str, len) == 0;
}

b8 s_token::equals(s_token token)
{
	if(len != token.len) { return false; }
	if(type != token.type) { return false; }
	return strncmp(at, token.at, len) == 0;
}

char* s_token::str(s_lin_arena* arena)
{
	assert(type > e_token_invalid);
	return token_to_str(*this, arena);
}

func char* token_to_str(s_token token, s_lin_arena* arena)
{
	assert(token.type > e_token_invalid);
	if(token.type == e_token_eof) { return "EOF"; }
	char* buffer = (char*)arena->alloc(1024);
	int written = sprintf(buffer, "%.*s", ft(token));
	assert(written >= 0);
	arena->used -= 1024 - (align_to_8(written + 1));
	return buffer;
}