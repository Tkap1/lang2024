#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "custom_win32.h"


#include "types.h"
#include "memory.h"
#include "utils.h"
#include "main.h"
#include "tokenizer.h"
#include "parser.h"
#include "type_checker.h"
#include "code_generator.h"

#include "memory.cpp"
#include "utils.cpp"
#include "tokenizer.cpp"
#include "parser.cpp"
#include "type_checker.cpp"
#include "code_generator.cpp"

int main(int argc, char** argv)
{
	unreferenced(argc);
	unreferenced(argv);

	s_lin_arena arena = alloc_arena(100 * c_mb);

	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "test") == 0) {
			run_tests(&arena);
			return 0;
		}
	}

	char* file_name = "test.tk";
	s_error_reporter reporter = zero;
	if(compile(file_name, &arena, false, &reporter)) {
		printf("Success!\n");
	}
}

void s_error_reporter::fatal(char* file, int line, char* str, ...)
{
	if(error_level >= e_error_level_fatal) { return; }
	error_level = e_error_level_fatal;

	if(!str) {
		str = "fatal!";
	}

	va_list args;
	va_start(args, str);
	char buffer[1024] = zero;
	vsnprintf(buffer, 1024, str, args);
	va_end(args);
	if(file && line > 0) {
		sprintf(error_str, "%s(%i): %s\n", file, line, buffer);
	}
	else if(file) {
		sprintf(error_str, "%s: %s\n", file, buffer);
	}
	else if(line > 0) {
		sprintf(error_str, "(%i): %s\n", line, buffer);
	}
	else {
		sprintf(error_str, "%s\n", buffer);
	}

	if(IsDebuggerPresent()) {
		__debugbreak();
	}

	if(ignore_errors) { return; }

	printf("%s\n", error_str);

	exit(1);
}

void s_error_reporter::recoverable_error(char* file, int line, char* str, ...)
{
	if(error_level > e_error_level_recoverable) { return; }
	error_level = e_error_level_recoverable;

	if(!str) {
		str = "recoverable error";
	}
	has_error = true;

	va_list args;
	va_start(args, str);
	char buffer[1024] = zero;
	vsnprintf(buffer, 1024, str, args);
	va_end(args);
	if(file && line > 0) {
		sprintf(error_str, "%s(%i): %s\n", file, line, buffer);
	}
	else if(file) {
		sprintf(error_str, "%s: %s\n", file, buffer);
	}
	else if(line > 0) {
		sprintf(error_str, "(%i): %s\n", line, buffer);
	}
	else {
		sprintf(error_str, "%s\n", buffer);
	}
}

func b8 compile(char* file_path, s_lin_arena* arena, b8 ignore_errors, s_error_reporter* reporter)
{
	reporter->ignore_errors = ignore_errors;
	s_tokenizer tokenizer = zero;
	char* file_content = read_file(file_path, arena);
	if(!file_content) {
		reporter->fatal(null, 0, "Failed to read '%s'", file_path);
	}
	tokenizer.at = file_content;
	tokenizer.line = 1;
	tokenizer.file = file_path;

	s_node* ast = parse(tokenizer, reporter, arena);
	if(!ast) {
		if(!ignore_errors) {
			printf("An empty program is not valid!\n");
		}
		return false;
	}
	ast = type_check_ast(ast, reporter, arena);
	if(!ast) {
		return false;
	}

	return generate_code(ast, arena);
}

func void run_tests(s_lin_arena* arena)
{
	struct s_test
	{
		char* file_path;
		b8 should_compile;
	};

	constexpr s_test test_data[] = {
		{"tests/foo.tk", false},
		{"tests/bar.tk", true},
		{"tests/subscript0.tk", true},
		{"tests/semicolon_instead_of_comma.tk", false},
		{"tests/two_member_access.tk", true},
		{"tests/for_int.tk", true},
		{"tests/for_int_array.tk", true},
		{"tests/for_struct_array.tk", true},
		{"tests/struct_var_decl.tk", true},
		{"tests/function.tk", true},
		{"tests/return_struct_literal.tk", true},
		{"tests/func_arg.tk", true},
		{"tests/func_arg2.tk", true},
		{"tests/member_access_subscript.tk", true},
		{"tests/no_forward_decl.tk", true},
	};

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	s_error_reporter reporter = zero;
	for(int test_i = 0; test_i < array_count(test_data); test_i++) {
		arena->used = 0;
		s_test test = test_data[test_i];
		b8 success = compile(test.file_path, arena, true, &reporter);
		if(success == test.should_compile) {
			SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
			printf("%s ", test.file_path);
			printf("PASSED!\n");
			SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else {
			SetConsoleTextAttribute(hstdout, FOREGROUND_RED);
			printf("%s ", test.file_path);
			printf("FAILED!\n");
			printf("\t%s\n", reporter.error_str);
			SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		reporter = zero;
	}
}