
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
	return result;
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
t& s_sarray<t, n>::operator[](int index)
{
	assert(index >= 0 && index < count);
	return elements[index];
}

func char* format_str(char* str, ...)
{
	static int index = 0;
	static char buffers[16][1024];
	char* buffer = buffers[index];
	index = (index + 1) % 16;
	va_list args;
	va_start(args, str);
	vsnprintf(buffer, 1024, str, args);
	va_end(args);
	return buffer;
}