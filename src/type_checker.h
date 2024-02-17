

struct s_type_check_context
{
	s_node* expected_return_type;
	s_node* expected_literal_type;
	s_node* member_access;
	int subscript_level;
};

struct s_get_struct_member
{
	b8 is_imported;
	s_node* node;
	s_node* import_source;
};


typedef s_sarray<s_scope**, 64> t_scope_arr;

func s_node* type_check_ast(s_node* ast, s_error_reporter* reporter, s_lin_arena* arena);
func b8 type_check_node(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context);
func b8 type_check_struct(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context);
func b8 type_check_struct_member(s_node* nstruct, s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context);
func char* node_to_str(s_node* node);
func b8 type_check_func_decl(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context);
func b8 type_check_statement(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context);
func b8 type_check_expr(s_node* node, s_error_reporter* reporter, t_scope_arr* data, s_lin_arena* arena, s_type_check_context context);
func s_maybe<s_node> get_compile_time_value(s_node* node, t_scope_arr* data);
func void add_type_to_scope(t_scope_arr* data, s_node* type, s_lin_arena* arena);
func void add_var_to_scope(t_scope_arr* data, s_node* var, s_lin_arena* arena);
func s_node* get_struct_by_name_except(char* name, s_node* exclude, t_scope_arr* data);
func void add_struct_to_scope(t_scope_arr* data, s_node* nstruct, s_lin_arena* arena);
func void add_func_to_scope(t_scope_arr* data, s_node* nfunc, s_lin_arena* arena);
func s_node* get_type_by_name(char* name, t_scope_arr* data);
func s_node* get_var_by_name(char* name, t_scope_arr* data);
func b8 is_const(s_node* node, t_scope_arr* data);
func s_node* get_func_by_name(char* name, t_scope_arr* data);
func s_node* get_latest_func(t_scope_arr* data);
func s_get_struct_member get_struct_member(char* name, s_node* nstruct, t_scope_arr* data);
func void maybe_fix_member_access(s_node* node, s_node* nstruct, t_scope_arr* data, s_lin_arena* arena);
func int get_size_in_bytes(s_node* node, t_scope_arr* data);
func void add_enum_to_scope(t_scope_arr* data, s_node* nenum, s_lin_arena* arena);
func s_node* get_enum_by_name(char* name, t_scope_arr* data);