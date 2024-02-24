

func s_lin_arena alloc_arena(int in_capacity)
{
	assert(in_capacity > 0);
	s_lin_arena result = zero;
	int capacity = align_to_8(in_capacity);
	result.capacity = capacity;
	result.memory = (u8*)malloc(capacity);
	return result;
}

u8* s_lin_arena::alloc(int in_size)
{
	assert(in_size > 0);
	int size = align_to_8(in_size);
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

void s_lin_arena::push()
{
	push_arr.add(used);
}

void s_lin_arena::pop()
{
	used = push_arr.pop();
}