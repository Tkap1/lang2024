

struct s_lin_arena
{
	int used;
	int capacity;
	u8* memory;
	s_sarray<int, 16> push_arr;

	u8* alloc(int in_size);
	u8* alloc_zero(int in_size);
	void push();
	void pop();
};

template <typename T>
struct s_dynamic_array
{
	int count;
	int max_elements;
	T* elements;

	T& operator[](int index);
	int add(T new_element, s_lin_arena* arena);
	void remove_and_shift(int index);
	T& get_last();
};


func s_lin_arena alloc_arena(int in_capacity);

template <typename T>
func s_dynamic_array<T> make_dynamic_array(int initial_max_elements, s_lin_arena* arena);