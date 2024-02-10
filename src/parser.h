

#define ft(token) (token).len, (token).at

#define for_node(iter, base) for(s_node* iter = base; iter; iter = iter->next)

enum e_node
{
	e_node_invalid,
	e_node_struct,
	e_node_type,
	e_node_integer,
	e_node_array,
	e_node_basic_type,
	e_node_add,
	e_node_subtract,
	e_node_multiply,
	e_node_divide,
	e_node_modulo,
	e_node_compound,
	e_node_func_decl,
	e_node_return,
	e_node_var_decl,
};

struct s_operator_data
{
	e_token token_type;
	e_node node_type;
	int precedence;
};

global constexpr s_operator_data c_operator_data[] = {
	{e_token_plus, e_node_add, 12},
	{e_token_minus, e_node_subtract, 12},
	{e_token_asterisk, e_node_multiply, 13},
	{e_token_forward_slash, e_node_divide, 13},
	{e_token_percent, e_node_modulo, 13},
};

struct s_node
{
	b8 type_checked;
	e_node type;
	int line;
	s_token token;

	// @TODO(tkap, 10/02/2024): probably dont want this
	s_node* left;
	s_node* right;

	s_node* next;
	s_node* var_type;

	union
	{
		struct
		{
			int member_count;
			s_node* members;
		} nstruct;

		struct
		{
			s_node* type;
			s_token name;
		} var_decl;

		struct
		{
			int argument_count;
			s_node* return_type;
			s_token name;
			s_node* arguments;
			s_node* body;
		} func_decl;

		struct
		{
			s_node* size_expr;
		} array;

		struct
		{
			int size_in_bytes;
			char* name;
		} basic_type;

		struct
		{
			s_node* statements;
		} compound;

		struct
		{
			s_node* expression;
		} nreturn;
	};
};


struct s_parse_result
{
	b8 success;
	s_tokenizer tokenizer;
	s_node node;
};

func s_node* parse(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_struct(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_node** advance_node(s_node** a2, s_node node, s_lin_arena* arena);
func s_node* alloc_node(s_node node, s_lin_arena* arena);
func b8 is_keyword(s_token token);
func s_parse_result parse_type(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_array(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_expression(s_tokenizer tokenizer, s_error_reporter* reporter, int in_operator_level, s_lin_arena* arena);
func int get_operator_precedence(e_token type);
func void print_expression(s_node* node);
func s_parse_result parse_function(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_statement(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);