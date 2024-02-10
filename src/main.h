

struct s_error_reporter
{
	char error_str[1024];
	b8 has_error;
	void fatal(char* file, int line, char* str = null, ...);
	void recoverable_error(char* file, int line, char* str = null, ...);
};