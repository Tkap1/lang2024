

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

func void run_tests(s_lin_arena* arena);
func b8 compile(char* file_path, s_lin_arena* arena, b8 ignore_errors, s_error_reporter* reporter);