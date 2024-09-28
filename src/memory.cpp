

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

template <typename T>
T& s_dynamic_array<T>::operator[](int index)
{
	assert(index >= 0);
	assert(index < count);
	return elements[index];
}

template <typename T>
int s_dynamic_array<T>::add(T new_element, s_lin_arena* arena)
{
	if(max_elements <= 0) {
		*this = make_dynamic_array<T>(16, arena);
	}

	assert(max_elements > 0);
	if(count == max_elements) {
		int old_max_elements = max_elements;
		int new_max_elements = max_elements * 2;
		T* temp = (T*)arena->alloc(sizeof(T) * new_max_elements);
		memcpy(temp, elements, old_max_elements * sizeof(T));
		elements = temp;
		max_elements = new_max_elements;
	}
	int index = count;
	elements[count] = new_element;
	count += 1;
	return index;
}

template <typename T>
func s_dynamic_array<T> make_dynamic_array(int initial_max_elements, s_lin_arena* arena)
{
	assert(initial_max_elements > 0);
	s_dynamic_array<T> result = zero;
	result.elements = (T*)arena->alloc(sizeof(T) * initial_max_elements);
	result.max_elements = initial_max_elements;
	return result;
}

template <typename T>
void s_dynamic_array<T>::remove_and_shift(int index)
{
	assert(index >= 0);
	assert(index < count);
	T* dst = &elements[index];
	T* src = &elements[index + 1];
	count -= 1;
	int to_copy = count - index;
	memmove(dst, src, to_copy * sizeof(T));
}

template <typename T>
T& s_dynamic_array<T>::get_last()
{
	assert(count > 0);
	return elements[count - 1];
}