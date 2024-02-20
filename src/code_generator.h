


struct s_code_gen_context
{
	b8 is_func_arg;
	b8 prefix_struct_literal;
};

global constexpr int c_builder_size = 10 * c_mb;
typedef s_str_builder<c_builder_size> t_code_builder;

func void generate_node(s_node* node, t_code_builder* builder, s_code_gen_context context);
func void node_to_c_str(s_node* node, t_code_builder* builder, s_code_gen_context context);
func void get_suffix_str(s_node* node, t_code_builder* builder);
func void generate_statement(s_node* node, t_code_builder* builder);
func void generate_struct_member(s_node* node, t_code_builder* builder);
func void generate_func_decl_arg(s_node* node, t_code_builder* builder, b8 is_external);
func void generate_array_base_type(s_node* node, t_code_builder* builder);
func void get_subscript_str(s_node* node, int level, t_code_builder* builder, s_code_gen_context context);
func char* get_name(s_node* node);