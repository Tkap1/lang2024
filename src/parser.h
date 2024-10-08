

#define ft(token) (token).len, (token).at

#define for_node(iter, base) for(s_node* iter = base; iter; iter = iter->next)


enum e_type
{
	e_type_invalid,
	e_type_void,
	e_type_char,
	e_type_s8,
	e_type_s16,
	e_type_s32,
	e_type_s64,
	e_type_u8,
	e_type_u16,
	e_type_u32,
	e_type_u64,
	e_type_f32,
	e_type_f64,
	e_type_b8,
	e_type_b32,
	e_type_soa_view,
};


enum e_node
{
	e_node_invalid,
	e_node_struct,
	e_node_type,
	e_node_integer,
	e_node_float,
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
	e_node_identifier,
	e_node_func_call,
	e_node_string,
	e_node_while,
	e_node_logic_not,
	e_node_var_decl,
	e_node_member_access,
	e_node_assign,
	e_node_if,
	e_node_greater_than,
	e_node_greater_than_or_equal,
	e_node_less_than,
	e_node_less_than_or_equal,
	e_node_unary_minus,
	e_node_address_of,
	e_node_logic_or,
	e_node_logic_and,
	e_node_simple_for,
	e_node_range_for,
	e_node_struct_literal,
	e_node_comparison,
	e_node_not_equals,
	e_node_subscript,
	e_node_continue,
	e_node_break,
	e_node_enum,
	e_node_dereference,
	e_node_import,
	e_node_include,
	e_node_data_enum,
	e_node_data_enum_member,
	e_node_func_ptr,
	e_node_array_literal,
	e_node_var_args,
	e_node_auto_cast,
	e_node_iterator,
	e_node_yield,
	e_node_iterator_call,
	e_node_sizeof,
};

enum e_context
{
	e_context_struct = 1 << 0,
	e_context_statement = 1 << 1,
	e_context_global = 1 << 2,
};

struct s_parse_context
{
	int overload_id;
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
	{e_token_logic_not, e_node_logic_not, 15},
	{e_token_asterisk, e_node_dereference, 15},
	{e_token_address_of, e_node_address_of, 15},
	{e_token_dot, e_node_member_access, 16},
	{e_token_greater_than, e_node_greater_than, 9},
	{e_token_greater_than_or_equal, e_node_greater_than_or_equal, 9},
	{e_token_less_than_or_equal, e_node_less_than_or_equal, 9},
	{e_token_less_than, e_node_less_than, 9},
	{e_token_logic_or, e_node_logic_or, 3},
	{e_token_logic_and, e_node_logic_and, 4},
	{e_token_comparison, e_node_comparison, 8},
	{e_token_not_equals, e_node_not_equals, 8},
	{e_token_open_paren, e_node_func_call, 16},
	{e_token_open_bracket, e_node_subscript, 16},
	{e_token_auto_cast, e_node_auto_cast, 15},
};

struct s_node;

// @TODO(tkap, 28/09/2024): rename to _arr
struct s_scope
{
	s_dynamic_array<s_node*> structs;
	s_dynamic_array<s_node*> funcs;
	s_dynamic_array<s_node*> enums;
	s_dynamic_array<s_node*> data_enums;
	s_dynamic_array<s_node*> types;
	s_dynamic_array<s_node*> vars;
	s_dynamic_array<s_node*> imports;
	s_dynamic_array<s_node*> func_ptrs;
	s_dynamic_array<s_node*> iterator_arr;
};

struct s_node
{
	b8 type_checked;
	b8 is_data_enum_member;
	b8 is_data_enum_struct_access;
	e_node type;
	int line;
	s_token token;
	int pointer_level;
	b8 dont_generate;
	s64 size_in_bytes;
	s64 array_capacity; // @Note(tkap, 17/02/2024): For e_node_subscript
	int enum_value;
	s_node* temp_var_decl;
	s_node* operator_overload_func;
	b8 inside_sizeof;

	// @TODO(tkap, 10/02/2024): probably dont want this
	s_node* left;
	s_node* right;

	s_node* next;
	s_node* var_type;

	union
	{
		struct
		{
			b8 is_packed;
			int member_count;
			s_node* members;
		} nstruct;

		struct
		{
			b8 is_const;
			b8 is_import; // @Note(tkap, 14/02/2024): Used by struct members
			s_node* type;
			s_node* value;
			s_token name;
		} var_decl;

		struct
		{
			b8 is_external;
			b8 is_dll_export;
			b8 is_operator_overload;
			b8 is_method;
			b8 method_import_added_to_scope;
			e_node operator_overload;
			int argument_count;
			s_node* return_type;
			s_token name;
			s_node* arguments;
			s_node* body;
			int scope_index;
			s_token base_struct;
		} func_decl;

		struct
		{
			int argument_count;
			s_token name;
			s_node* arguments;
			s_node* body;
			int scope_index;
		} iterator;

		struct
		{
			s_node* size_expr;
		} array;

		struct
		{
			e_type id;
			b8 is_unsigned;
			char* name;
		} basic_type;

		struct
		{
			s_node* statements;
			int scope_index;
		} compound;

		struct
		{
			s_node* expression;
		} nreturn;

		struct
		{
			int argument_count;
			s_node* arguments;
			s_node* body; // @Note(tkap, 28/09/2024): For calls to iterators
		} func_call;

		struct
		{
			s_node* condition;
			s_node* body;
		} nwhile;

		struct
		{
			s_node* condition;
			s_node* body;
			s_node* nelse;
		} nif;

		struct
		{
			s64 value;
		} integer;

		struct
		{
			float value;
		} nfloat;

		struct
		{
			b8 loop_by_ptr;
			s_token iterator_index_name;
			s_token iterator_name;
			s_node* expr;
			s_node* body;
		} simple_for;

		struct
		{
			s_token iterator_name;
			s_node* body;
			s_node* lower_bound;
			s_node* upper_bound;
		} range_for;

		struct
		{
			s_node* expressions;
		} struct_literal;

		struct
		{
			int member_count;
			s_node* members;
		} nenum;

		struct
		{
			s_node* nstruct;
			int member_count;
			s_node* members;
		} data_enum;

		struct
		{
			int member_count;
			s_node* members;
		} data_enum_member;

		struct
		{
			s_node* return_type;
			s_token name;
			int argument_count;
			s_node* arguments;
		} func_ptr;

		struct
		{
			int expression_count;
			s_node* expressions;
		} array_literal;
	};
};


struct s_parse_result
{
	b8 success;
	b8 is_const; // @TODO(tkap, 18/02/2024): parse_type() sets this. We don't use it everywhere where we should currently...
	s_tokenizer tokenizer;
	s_node node;

	// @Note(tkap, 10/02/2024): Used by parse_operator
	s_operator_data operator_data;
};

func s_node* parse(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_struct(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_node** advance_node(s_node** a2, s_node node, s_lin_arena* arena);
func s_node* alloc_node(s_node node, s_lin_arena* arena);
func b8 is_keyword(s_token token);
func s_parse_result parse_type(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_array(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_expression(s_tokenizer tokenizer, s_error_reporter* reporter, int in_operator_level, s_lin_arena* arena);
func void print_expression(s_node* node, s_lin_arena* arena);
func s_parse_result parse_func_decl(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena, s_parse_context* context);
func s_parse_result parse_external_func_decl(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_statement(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_var_decl(s_tokenizer tokenizer, s_error_reporter* reporter, int context, s_lin_arena* arena);
func s_node statement_str_to_node(char* str, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_enum(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_operator(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_include(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_data_enum(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);
func s_parse_result parse_pack(s_tokenizer tokenizer, s_error_reporter* reporter);
func s_parse_result parse_iterator(s_tokenizer tokenizer, s_error_reporter* reporter, s_lin_arena* arena);