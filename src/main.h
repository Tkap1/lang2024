

// @Fixme(tkap, 21/02/2024): delete
global constexpr int c_static_buffers = 1024;
global constexpr int c_static_buffer_len = 1024;

enum e_error_level
{
	e_error_level_none,
	e_error_level_recoverable,
	e_error_level_fatal,
};

struct s_error_reporter
{
	e_error_level error_level;
	b8 ignore_errors;
	char error_str[1024];
	b8 has_error;
	void fatal(char* file, int line, char* str = null, ...);
	void recoverable_error(char* file, int line, char* str = null, ...);
};

struct s_node;

func void run_tests(s_lin_arena* arena);
func b8 compile(char* file_path, s_lin_arena* arena, b8 ignore_errors, s_error_reporter* reporter);
func s_node* parse_step(char* file_path, s_error_reporter* reporter, s_lin_arena* arena, b8 ignore_errors);