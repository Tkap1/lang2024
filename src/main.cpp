#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "custom_win32.h"

#ifdef __SANITIZE_ADDRESS__
#include <sanitizer/asan_interface.h>
#endif


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

	b8 build_dll = false;
	char output_name[1024] = zero;
	char input_name[1024] = zero;
	b8 should_run_tests = false;
	b8 run_only_last_test = false;
	b8 multi_thread_tests = true;
	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "test") == 0) {
			should_run_tests = true;
		}
		else if(strcmp(argv[i], "dll") == 0) {
			build_dll = true;
		}
		else if(strncmp(argv[i], "in:", 3) == 0) {
			strcpy(input_name, argv[i] + 3);
		}
		else if(strncmp(argv[i], "out:", 4) == 0) {
			strcpy(output_name, argv[i] + 4);
		}
		else if(strncmp(argv[i], "last_test_only", 14) == 0) {
			run_only_last_test = true;
		}
		else if(strncmp(argv[i], "no_multi_thread", 18) == 0) {
			multi_thread_tests = false;
		}
	}
	if(should_run_tests) {
		run_tests(run_only_last_test, multi_thread_tests);
		return 0;
	}
	if(!input_name[0]) {
		strcpy(input_name, "main.tk");
	}
	if(!output_name[0]) {
		strcpy(output_name, "output.exe");
	}

	s_error_reporter reporter = zero;
	if(compile(input_name, &arena, false, &reporter, {.build_dll = build_dll, .output_name = output_name})) {
		printf("Success!\n");
		u64 data[] = {c_gb, c_mb, c_kb, 1};
		char* data_str[] = {"gb", "mb", "kb", "b"};
		u64 used = arena.used;
		for(int i = 0; i < array_count(data); i++) {
			u64 val = used / data[i];
			if(val > 0) {
				printf("%llu%s ", val, data_str[i]);
				used -= val * data[i];
			}
		}
		printf("\n");
	}
}

printf_warnings(4, 5)
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

printf_warnings(4, 5)
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

func s_node* parse_step(char* file_path, s_error_reporter* reporter, s_lin_arena* arena, b8 ignore_errors)
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
	return ast;
}

func b8 compile(char* file_path, s_lin_arena* arena, b8 ignore_errors, s_error_reporter* reporter, s_compile_options options)
{
	s_node* ast = parse_step(file_path, reporter, arena, ignore_errors);
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

	char c_file_name[1024] = zero;
	int c_file_name_index = 0;
	char* c = file_path;
	while(*c) {
		if(*c == '.') { break; }
		c_file_name[c_file_name_index++] = *c;
		c += 1;
	}
	c_file_name[c_file_name_index++] = '.';
	c_file_name[c_file_name_index++] = 'c';
	c_file_name[c_file_name_index++] = 0;

	b8 result = generate_code(ast, arena, c_file_name);
	s_str_builder<1024> builder;
	if(options.compile_c_code) {
		builder.add("pushd build && ");
		builder.add("cl ..\\%s -Fe:%s -W4 -wd 4244 -Zi -Od -nologo ", c_file_name, options.output_name);
		if(options.build_dll) {
			builder.add("-LD ");
		}
		builder.add("-link ..\\raylib.lib");
		system(builder.data);
		system(alloc_str(arena, "copy build\\%s %s > NUL", options.output_name, options.output_name));
	}
	return result;
}

global volatile int g_tests_done = 0;
global volatile int g_success_count = 0;
global volatile int g_fail_count = 0;
global volatile HANDLE g_mutex;
global volatile HANDLE g_hstdout;
func void run_tests(b8 run_only_last_test, b8 multi_thread_tests)
{

	g_mutex = CreateMutex(null, false, null);
	g_hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	assert(g_mutex);

	constexpr s_test test_data[] = {
		{"foo", false},
		{"bar", true},
		{"subscript0", true},
		{"subscript_2d", true},
		{"comma_instead_of_semicolon", false},
		{"two_member_access", true},
		{"for_int", true},
		{"for_int_array", true},
		{"for_bad", false},
		{"for_struct_array", true},
		{"struct_var_decl", true},
		{"function", true},
		{"return_struct_literal", true},
		{"func_arg", true},
		{"func_arg2", true},
		{"member_access_subscript", true},
		{"no_forward_decl", true},
		{"import", true},
		{"import2", true},
		{"import3", false},
		{"array_in_struct", true},
		{"import_nest", true},
		{"assign_array", true},
		{"assign_array_ptr", true},
		{"address_of_array_in_struct", true},
		{"assign_to_array_element", true},
		{"enum", true},
		{"for_member_access", true},
		{"for_enum_member_access", true},
		{"double_subscript", true},
		{"operator_overload", true},
		{"operator_overload2", true, 2},
		{"operator_overload3", true},
		{"dereference", true},
		{"dereference2", true},
		{"dereference3", true},
		{"import4", true},
		{"import_struct_statement", true},
		{"import_shadow", false},
		{"nested_for_loops", false},
		{"nonsense", false},
		{"const_without_value", false},
		{"data_enum", true},
		{"data_enum2", true},
		{"data_enum3", true},
		{"data_enum4", true},
		{"import5", true},
		{"import6", true},
		{"sizeof", true, 4},
		{"sizeof2", true, 4},
		{"sizeof3", true, 128},
		{"sizeof4", true, 12},
		{"sizeof5", true, 12},
		{"sizeof6", true, 4},
		{"sizeof7", true, 128},
		{"operator_overload4", true},
		{"dereference4", true},
		{"array_literal", true},
		{"for_starting_index", true},
		{"array_size", true},
		{"bad_function_call", false},
		{"auto_cast", true},
		{"unknown_arg_type_in_external_func", false},
		{"unknown_return_type_type_in_external_func", false},
		{"func_ptr_call", true},
		{"struct_literal_in_func_arguments", true},
		{"struct_set_to_int", false},
		{"method", true},
		{"method2", false},
		{"method3", false},
		{"iterator0", true},
		{"iterator1", true},
		{"for_member_access2", true},
		{"for_loop_array_by_ptr", true},
		{"for_loop_array_by_ptr2", true},
		{"for_loop_array_by_ptr3", true},
		{"aos_access_on_soa", true},
	};

	for(int test_i = 0; test_i < array_count(test_data); test_i++) {
		for(int other_i = 0; other_i < array_count(test_data); other_i++) {
			if(test_i == other_i) { continue; }
			if(strcmp(test_data[test_i].file_path, test_data[other_i].file_path) == 0) {
				printf("Test '%s' is duplicated!\n", test_data[test_i].file_path);
				exit(1);
			}
		}
	}

	s_thread_data thread_data[array_count(test_data)] = zero;

	int test_start_index = 0;
	int target_test_completed_count = array_count(test_data);
	if(run_only_last_test) {
		test_start_index = array_count(test_data) - 1;
		target_test_completed_count = 1;
	}

	for(int test_i = test_start_index; test_i < array_count(test_data); test_i++) {
		thread_data[test_i].arena = alloc_arena(20 * c_mb);
		thread_data[test_i].test = test_data[test_i];
		if(multi_thread_tests) {
			CreateThread(null, 0, run_a_test, (void*)&thread_data[test_i], 0, null);
		}
		else {
			run_a_test(&thread_data[test_i]);
		}
	}

	while(g_tests_done != target_test_completed_count) { Sleep(10); }

	for(int test_i = 0; test_i < array_count(test_data); test_i++) {
		s_test test = test_data[test_i];
		// DeleteFileA(format_str("%s.exe", test.file_path));
		DeleteFileA(format_str("%s.obj", test.file_path));
	}

	printf("Passed %i/%i\n", g_success_count, g_success_count + g_fail_count);

}

func int compile_c_code(char* file_path, s_lin_arena* arena)
{
	STARTUPINFO startup = zero;
	PROCESS_INFORMATION info = zero;
	char* compiler_path = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.41.34120\\bin\\Hostx64\\x64\\cl.exe -nologo -Od";
	constexpr int CREATE_NO_WINDOW = 134217728;
	BOOL success = CreateProcessA(null, alloc_str(arena, "%s %s", compiler_path, file_path), null, null, false, CREATE_NO_WINDOW, null, null, &startup, &info);
	assert(success);

	WaitForSingleObject(info.hProcess, INFINITE);
	DWORD exit_code = 0;
	GetExitCodeProcess(info.hProcess, &exit_code);
	CloseHandle(info.hProcess);
	CloseHandle(info.hThread);
	return exit_code;
}

func int run_c_program(char* file_path)
{
	STARTUPINFO startup = zero;
	PROCESS_INFORMATION info = zero;
	constexpr int CREATE_NO_WINDOW = 134217728;
	BOOL success = CreateProcessA(null, file_path, null, null, false, CREATE_NO_WINDOW, null, null, &startup, &info);
	assert(success);

	WaitForSingleObject(info.hProcess, INFINITE);
	DWORD exit_code = 0;
	GetExitCodeProcess(info.hProcess, &exit_code);
	CloseHandle(info.hProcess);
	CloseHandle(info.hThread);
	return exit_code;
}

DWORD WINAPI run_a_test(void* arg)
{
	s_thread_data data = *(s_thread_data*)arg;
	s_error_reporter reporter = zero;

	b8 our_success = compile(alloc_str(&data.arena, "tests/%s.tk", data.test.file_path), &data.arena, true, &reporter, {.compile_c_code = false});
	int c_success = 0;
	if(!data.test.should_compile && our_success) {
		goto fail;
	}
	if(!our_success && data.test.should_compile) {
		goto fail;
	}
	if(our_success) {
		c_success = compile_c_code(alloc_str(&data.arena, "tests/%s.c", data.test.file_path), &data.arena);
		if(data.test.should_compile && c_success != 0) {
			goto fail;
		}
		int exit_code = run_c_program(alloc_str(&data.arena, "%s.exe", data.test.file_path));
		if(exit_code != data.test.expected_exit_code) { goto fail; }
	}

	goto success;

	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		fail start		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	fail:
	InterlockedIncrement((LONG*)&g_fail_count);
	WaitForSingleObject(g_mutex, INFINITE);
	SetConsoleTextAttribute(g_hstdout, FOREGROUND_RED);
	printf("%s ", data.test.file_path);
	printf("FAILED!\n");
	if(reporter.error_level > e_error_level_none) {
		printf("\t%s\n", reporter.error_str);
	}
	SetConsoleTextAttribute(g_hstdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	ReleaseMutex(g_mutex);
	goto end;
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		fail end		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		success start		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	success:
	InterlockedIncrement((LONG*)&g_success_count);
	WaitForSingleObject(g_mutex, INFINITE);
	SetConsoleTextAttribute(g_hstdout, FOREGROUND_GREEN);
	printf("%s ", data.test.file_path);
	printf("PASSED!\n");
	SetConsoleTextAttribute(g_hstdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	ReleaseMutex(g_mutex);
	goto end;
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		success end		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	end:
	InterlockedIncrement((LONG*)&g_tests_done);

	return 0;
}
