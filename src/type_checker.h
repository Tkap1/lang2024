

struct s_type_check_context
{
	s_node* expected_return_type;
	s_node* expected_literal_type;
	s_node* member_access;
	s_node* wanted_type;
	int subscript_level;
	b8 inside_sizeof;
};

struct s_get_struct_member
{
	b8 is_imported;
	s_node* node;
	s_node* import_source;
};

struct s_op_table
{
	e_type left;
	e_type right;
	e_type result;
};

global constexpr s_op_table c_op_table[] = {
	{e_type_s32, e_type_f32, e_type_f32},
	{e_type_s32, e_type_s32, e_type_s32},
	{e_type_f32, e_type_f32, e_type_f32},
};

typedef s_sarray<s_scope, 1024> t_scope_arr;
typedef s_sarray<int, 64> t_scope_index_arr;
typedef s_sarray<s_node*, 128> t_get_imports;
typedef s_sarray<s_node*, 128> t_flat_struct_members;

func s_node* type_check_ast(s_node* ast, s_error_reporter* reporter, s_lin_arena* arena);
func b8 type_check_node(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr);
func b8 type_check_struct(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr);
func b8 type_check_struct_member(s_node* nstruct, s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr);
func char* node_to_str(s_node* node, s_lin_arena* arena);
func b8 type_check_func_decl(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr);
func b8 type_check_statement(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr);
func b8 type_check_expr(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr);
func s_maybe<s_node> get_compile_time_value(s_node* node, t_scope_index_arr* data, s_lin_arena* arena, t_scope_arr* scope_arr);
func void add_type_to_scope(t_scope_index_arr* data, s_node* type, s_lin_arena* arena, t_scope_arr* scope_arr);
func b8 add_var_to_scope(t_scope_index_arr* data, s_node* var, s_error_reporter* reporter, s_lin_arena* arena, t_scope_arr* scope_arr);
func s_node* get_struct_by_name_except(char* name, s_node* exclude, t_scope_index_arr* data, t_scope_arr* scope_arr);
func void add_struct_to_scope(t_scope_index_arr* data, s_node* nstruct, s_lin_arena* arena, t_scope_arr* scope_arr);
func int add_func_to_scope(t_scope_index_arr* data, s_node* nfunc, s_lin_arena* arena, t_scope_arr* scope_arr);
func s_node* get_type_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr);
func s_node* get_var_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr);
func b8 is_const(s_node* node, t_scope_index_arr* data);
func s_node* get_func_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr);
func s_node* get_latest_func(t_scope_index_arr* data, t_scope_arr* scope_arr);
func s_get_struct_member get_struct_member(char* name, s_node* nstruct, t_scope_index_arr* data);
func void maybe_fix_member_access(s_node* node, s_node* nstruct, t_scope_index_arr* data, s_lin_arena* arena);
func s64 get_size_in_bytes(s_node* node, t_scope_index_arr* data, s_lin_arena* arena, t_scope_arr* scope_arr);
func void add_enum_to_scope(t_scope_index_arr* data, s_node* nenum, s_lin_arena* arena, t_scope_arr* scope_arr);
func s_node* get_enum_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr);
func s_node* get_type_by_id(e_type id, t_scope_index_arr* data, t_scope_arr* scope_arr);
func b8 is_same_type(s_node* a, s_node* b);
func b8 type_check_arithmetic(s_node* node, s_error_reporter* reporter, t_scope_index_arr* data, s_lin_arena* arena, s_type_check_context context, t_scope_arr* scope_arr);
func b8 add_import_to_scope(t_scope_index_arr* data, s_node* import, s_error_reporter* reporter, s_lin_arena* arena, t_scope_arr* scope_arr);
func t_get_imports* get_imports(t_scope_index_arr* data, s_lin_arena* arena, t_scope_arr* scope_arr);
func b8 can_thing_be_added_to_scope(s_token name, t_scope_index_arr* data, s_error_reporter* reporter, s_lin_arena* arena, t_scope_arr* scope_arr);
func t_flat_struct_members get_flat_array_of_struct_members(s_node* nstruct);
func void get_flat_array_of_struct_members_(s_node* nstruct, t_flat_struct_members* result);
func s_node* get_data_enum_by_name(char* name, t_scope_index_arr* data, t_scope_arr* scope_arr);
func void add_data_enum_to_scope(t_scope_index_arr* data, s_node* data_enum, s_lin_arena* arena, t_scope_arr* scope_arr);
func b8 add_func_pointer_to_scope(t_scope_index_arr* data, s_node* var, s_error_reporter* reporter, s_lin_arena* arena, t_scope_arr* scope_arr);
func b8 can_type_a_be_converted_to_b(s_node* a, s_node* b);
func s_token make_identifier_token(char* str);
func s_node make_identifier_node(char* str);
func s_tokenizer quick_tokenizer(char* str);
func int get_func_argument_count(s_node* node);
func int make_scope(t_scope_arr* scope_arr);