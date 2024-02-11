



#define breakable_block for(int breakable_i_ = 0; breakable_i_ < 1; breakable_i_++)

#define foreach_ptr__(a, index_name, element_name, array) if(0) finished##a: ; else for(auto element_name = &(array).elements[0];;) if(1) goto body##a; else while(1) if(1) goto finished##a; else body##a: for(int index_name = 0; index_name < (array).count && (bool)(element_name = &(array)[index_name]); index_name++)
#define foreach_ptr_(a, index_name, element_name, array) foreach_ptr__(a, index_name, element_name, array)
#define foreach_ptr(index_name, element_name, array) foreach_ptr_(__LINE__, index_name, element_name, array)

#define foreach_val__(a, index_name, element_name, array) if(0) finished##a: ; else for(auto element_name = (array).elements[0];;) if(1) goto body##a; else while(1) if(1) goto finished##a; else body##a: for(int index_name = 0; index_name < (array).count && (void*)&(element_name = (array)[index_name]); index_name++)
#define foreach_val_(a, index_name, element_name, array) foreach_val__(a, index_name, element_name, array)
#define foreach_val(index_name, element_name, array) foreach_val_(__LINE__, index_name, element_name, array)

#define invalid_default_case default: assert(!"Invalid default case");


template <typename t, int n>
struct s_sarray
{
	t elements[n];
	int count;

	void add(t new_element);
	t& operator[](int index);
};

template <int n>
struct s_str_builder
{
	char data[n];
	int len;
	int scope;

	void add(char* str, ...);
	void add_tabs(char* str, ...);
	void add_line(char* str, ...);
	void add_line_tabs(char* str, ...);
	void push_scope();
	void pop_scope(char* str = "");
	void add_(char* str, b8 add_newline, b8 add_tabs, va_list args);
};

template <typename t>
struct s_maybe
{
	b8 valid;
	t value;
};

func char* read_file(char* path, s_lin_arena* arena);


