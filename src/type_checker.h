


struct s_type_check
{
	s_sarray<s_node*, 1024> structs;
	s_sarray<s_node*, 1024> types;
};

func void type_check_ast(s_node* ast, s_error_reporter* reporter, s_lin_arena* arena);
func b8 type_check_node(s_node* node, s_error_reporter* reporter, s_type_check* data, s_lin_arena* arena);
func b8 type_check_struct(s_node* node, s_error_reporter* reporter, s_type_check* data, s_lin_arena* arena);
func b8 type_check_struct_member(s_node* nstruct, s_node* node, s_error_reporter* reporter, s_type_check* data, s_lin_arena* arena);
// func b8 compare_s_type(s_type a, s_type b);
func s_node* node_to_basic_type(s_node* node, s_type_check* data);
func char* node_to_str(s_node* node);