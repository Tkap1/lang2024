

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

func s_lin_arena alloc_arena(int in_capacity);