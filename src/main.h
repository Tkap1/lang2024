

enum e_error_level
{
	e_error_level_none,
	e_error_level_recoverable,
	e_error_level_fatal,
};

struct s_compile_options
{
	b8 compile_c_code = true;
	b8 build_dll = false;
	char* output_name = "output.exe";
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

struct s_test
{
	char* file_path;
	b8 should_compile;
	int expected_exit_code = 42;
};

struct s_thread_data
{
	s_test test;
	s_lin_arena arena;
};

struct s_node;

func void run_tests(b8 run_only_last_test, b8 multi_thread_tests);
func b8 compile(char* file_path, s_lin_arena* arena, b8 ignore_errors, s_error_reporter* reporter, s_compile_options options = {});
func s_node* parse_step(char* file_path, s_error_reporter* reporter, s_lin_arena* arena, b8 ignore_errors);
func int compile_c_code(char* file_path, s_lin_arena* arena);
func int run_c_program(char* file_path);
DWORD WINAPI run_a_test(void* arg);