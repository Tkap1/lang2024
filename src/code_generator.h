


global constexpr int c_builder_size = 10 * c_mb;
typedef s_str_builder<c_builder_size> t_code_builder;

func void generate_node(s_node* node, t_code_builder* builder);
func char* node_to_c_str(s_node* node);
func char* get_suffix_str(s_node* node);