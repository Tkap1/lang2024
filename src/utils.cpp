
func char* read_file(char* path, s_lin_arena* arena)
{
	FILE* file = fopen(path, "rb");
	if(!file) { return null; }
	fseek(file, 0, SEEK_END);
	int file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* result = (char*)arena->alloc_zero(file_size + 1);
	fread(result, 1, file_size, file);
	result[file_size] = 0;
	fclose(file);
	return result;
}

func b8 write_file(char* path, char* data, int data_size)
{
	FILE* file = fopen(path, "wb");
	if(!file) { return false; }
	fwrite(data, 1, data_size, file);
	fclose(file);
	return true;
}

func int ptrs_to_int(void* a, void* b)
{
	return (int)((u8*)b - (u8*)a);
}

template <typename t, int n>
void s_sarray<t, n>::add(t new_element)
{
	assert(count < n);
	elements[count] = new_element;
	count += 1;
}

template <typename t, int n>
t s_sarray<t, n>::pop()
{
	assert(count > 0);
	count -= 1;
	return elements[count];
}

template <typename t, int n>
t& s_sarray<t, n>::get_last()
{
	assert(count > 0);
	return elements[count - 1];
}

template <typename t, int n>
void s_sarray<t, n>::remove_and_shift(int index)
{
	assert(index < count);
	count -= 1;
	int to_copy = count - index;
	if(to_copy < 0) {
		memcpy(&elements[index], &elements[index + 1], sizeof(t) * to_copy);
	}
}

template <typename t, int n>
t& s_sarray<t, n>::operator[](int index)
{
	assert(index >= 0 && index < count);
	return elements[index];
}

template <typename t, int n>
t& s_sarray<t, n>::get(int index)
{
	assert(index >= 0 && index < count);
	return elements[index];
}

printf_warnings(2, 3)
func char* alloc_str(s_lin_arena* arena, char* str, ...)
{
	char* buffer = (char*)arena->alloc(1024);
	va_list args;
	va_start(args, str);
	int written = vsnprintf(buffer, 1024, str, args);
	va_end(args);
	assert(written >= 0);
	arena->used -= 1024 - (written + 1);
	return buffer;
}

template <int n>
void s_str_builder<n>::add_(char* str, b8 add_newline, b8 add_tabs, va_list args)
{
	if(add_tabs) {
		for(int i = 0; i < scope; i++) {
			data[len++] = '\t';
			assert(len < n);
		}
	}
	char* where_to_write = &data[len];
	int written = vsnprintf(where_to_write, n - len, str, args);
	len += written;
	assert(len < n);

	if(add_newline) {
		data[len] = '\n';
		len += 1;
	}
	data[len] = 0;
	assert(len < n);
}

template <int n>
printf_warnings(2, 3)
void s_str_builder<n>::add(char* str, ...)
{
	va_list args;
	va_start(args, str);
	add_(str, false, false, args);
	va_end(args);
}

template <int n>
printf_warnings(2, 3)
void s_str_builder<n>::add_tabs(char* str, ...)
{
	va_list args;
	va_start(args, str);
	add_(str, false, true, args);
	va_end(args);
}

template <int n>
printf_warnings(2, 3)
void s_str_builder<n>::add_line(char* str, ...)
{
	va_list args;
	va_start(args, str);
	add_(str, true, false, args);
	va_end(args);
}

template <int n>
printf_warnings(2, 3)
void s_str_builder<n>::add_line_tabs(char* str, ...)
{
	va_list args;
	va_start(args, str);
	add_(str, true, true, args);
	va_end(args);
}

template <int n>
void s_str_builder<n>::push_scope()
{
	add_line_tabs("{");
	scope += 1;
}

template <int n>
printf_warnings(2, 3)
void s_str_builder<n>::pop_scope(char* str, ...)
{
	scope -= 1;
	add_tabs("}");
	va_list args;
	va_start(args, str);
	add_(str, true, true, args);
	va_end(args);
}

template <typename t>
func s_maybe<t> maybe(t value)
{
	return {.valid = true, .value = value};
}

template <typename t>
func s_maybe<t> maybe()
{
	return {.valid = false};
}