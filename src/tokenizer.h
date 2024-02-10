
enum e_token
{
	e_token_invalid,
	e_token_eof,
	e_token_identifier,
	e_token_integer,
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
	e_token_count,
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

	s_token peek(s_error_reporter* reporter);
	s_token next_token(s_error_reporter* reporter);
	b8 consume_token(char* str, s_error_reporter* reporter);
	b8 consume_token(e_token type, s_error_reporter* reporter);
	b8 consume_token(e_token type, s_token* out_token, s_error_reporter* reporter);
	void eat_whitespace();
};

func char* token_to_str(s_token token);