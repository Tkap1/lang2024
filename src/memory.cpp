

func s_lin_arena alloc_arena(int in_capacity)
{
	assert(in_capacity > 0);
	s_lin_arena result = zero;
	int capacity = align_to_8(in_capacity);
	result.capacity = capacity;
	result.memory = (u8*)malloc(capacity);
	poison_memory(result.memory, capacity);
	return result;
}

u8* s_lin_arena::alloc(int in_size)
{
	assert(in_size > 0);
	int size = align_to_8(in_size);
	assert(used + size <= capacity);
	u8* result = memory + used;
	used += size;
	unpoison_memory(result, in_size);
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
	int before = used;
	used = push_arr.pop();
	assert(before >= used);
	poison_memory(memory + used, before - used);
}