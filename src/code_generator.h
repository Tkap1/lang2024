


struct s_code_gen_context
{
	b8 is_func_arg;
	b8 prefix_struct_literal;
	b8 is_data_enum_struct_access;
};

global constexpr int c_builder_size = 10 * c_mb;
typedef s_str_builder<c_builder_size> t_code_builder;

func void generate_node(s_node* node, t_code_builder* builder, s_code_gen_context context, s_lin_arena* arena);
func void node_to_c_str(s_node* node, t_code_builder* builder, s_code_gen_context context, s_lin_arena* arena);
func void get_suffix_str(s_node* node, t_code_builder* builder, s_lin_arena* arena);
func void generate_statement(s_node* node, t_code_builder* builder, s_lin_arena* arena);
func void generate_struct_member(s_node* node, t_code_builder* builder, s_lin_arena* arena);
func void generate_func_decl_arg(s_node* node, t_code_builder* builder, b8 is_external, s_lin_arena* arena);
func void generate_array_base_type(s_node* node, t_code_builder* builder, s_lin_arena* arena);
func void get_subscript_str(s_node* node, int level, t_code_builder* builder, s_code_gen_context context, s_lin_arena* arena);
func char* get_name(s_node* node, s_lin_arena* arena);
func s_node* get_data_enum_member(s_node* node);