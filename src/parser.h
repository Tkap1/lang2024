

#define ft(token) (token).len, (token).at

#define for_node(iter, base) for(s_node* iter = base; iter; iter = iter->next)

enum e_node
{
	e_node_invalid,
	e_node_struct,
	e_node_type,
	e_node_integer,
	e_node_array,
};

struct s_type
{
	char* name;
	int size_in_bytes;
};

struct s_node
{
	b8 type_checked;
	e_node type;
	int line;
	s_token token;
	s_node* left;
	s_node* next;
	s_node* var_type;
	s_type actual_type;

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
			s_node* size_expr;
		} array;
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
func s_parse_result parse_expression(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);