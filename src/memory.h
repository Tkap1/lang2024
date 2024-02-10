

struct s_lin_arena
{
	int used;
	int capacity;
	u8* memory;

	u8* alloc(int in_size);
	u8* alloc_zero(int in_size);
};

func s_lin_arena alloc_arena(int in_capacity);