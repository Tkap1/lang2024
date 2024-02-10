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

	char* file_name = "test.tk";
	s_error_reporter reporter = zero;
	s_tokenizer tokenizer = zero;
	char* file_content = read_file(file_name, &arena);
	if(!file_content) {
		reporter.fatal(null, 0, "Failed to read '%s'", file_name);
	}
	tokenizer.at = file_content;
	tokenizer.line = 1;
	tokenizer.file = file_name;


	s_node* ast = parse(tokenizer, &reporter, &arena);
	if(!ast) {
		reporter.fatal(null, 0, "An empty program is not valid");
	}
	type_check_ast(ast, &reporter, &arena);
	printf("Success!\n");

}

void s_error_reporter::fatal(char* file, int line, char* str, ...)
{
	if(!str) {
		str = "fatal!";
	}

	va_list args;
	va_start(args, str);
	char buffer[1024] = zero;
	vsnprintf(buffer, 1024, str, args);
	va_end(args);
	if(file && line > 0) {
		printf("%s(%i): %s\n", file, line, buffer);
	}
	else if(file) {
		printf("%s: %s\n", file, buffer);
	}
	else if(line > 0) {
		printf("(%i): %s\n", line, buffer);
	}
	else {
		printf("%s\n", buffer);
	}

	if(IsDebuggerPresent()) {
		__debugbreak();
	}
	exit(1);
}

void s_error_reporter::recoverable_error(char* file, int line, char* str, ...)
{
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