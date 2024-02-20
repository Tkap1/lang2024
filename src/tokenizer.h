
enum e_token
{
	e_token_invalid,
	e_token_eof,
	e_token_identifier,
	e_token_integer,
	e_token_float,
	e_token_open_brace,
	e_token_close_brace,
	e_token_semicolon,
	e_token_comma,
	e_token_open_bracket,
	e_token_close_bracket,
	e_token_open_paren,
	e_token_close_paren,
	e_token_plus,
	e_token_minus,
	e_token_asterisk,
	e_token_forward_slash,
	e_token_percent,
	e_token_string,
	e_token_logic_not,
	e_token_dot,
	e_token_assign,
	e_token_greater_than,
	e_token_greater_than_or_equal,
	e_token_less_than,
	e_token_less_than_or_equal,
	e_token_logic_or,
	e_token_logic_and,
	e_token_plus_equals,
	e_token_minus_equals,
	e_token_times_equals,
	e_token_divide_equals,
	e_token_comparison,
	e_token_not_equals,
	e_token_address_of,
	e_token_pound,
	e_token_colon,
};

struct s_token
{
	e_token type;
	int line;
	int len;
	char* at;
	char* file;

	b8 equals(char* str);
	b8 equals(s_token token);
	char* str();
};

struct s_tokenizer
{
	int line;
	char* at;
	char* file;

	s_token peek(s_error_reporter* reporter, int offset = 0);
	b8 peek_token(e_token type, s_error_reporter* reporter, int offset = 0);
	s_token next_token(s_error_reporter* reporter);
	b8 consume_token(char* str, s_error_reporter* reporter);
	b8 consume_token(e_token type, s_error_reporter* reporter);
	b8 consume_token(e_token type, s_token* out_token, s_error_reporter* reporter);
	b8 eat_whitespace();
	b8 eat_comment();
};

func char* token_to_str(s_token token);