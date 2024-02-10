

func s_lin_arena alloc_arena(int in_capacity)
{
	assert(in_capacity > 0);
	s_lin_arena result = zero;
	int capacity = (in_capacity + 7) & ~7;
	result.capacity = capacity;
	result.memory = (u8*)malloc(capacity);
	return result;
}

u8* s_lin_arena::alloc(int in_size)
{
	assert(in_size > 0);
	int size = (in_size + 7) & ~7;
	assert(used + size <= capacity);
	u8* result = memory + used;
	used += size;
	return result;
}

u8* s_lin_arena::alloc_zero(int in_size)
{
	u8* result = alloc(in_size);
	assert(result);
	memset(result, 0, in_size);
	return result;
}