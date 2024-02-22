

typedef uint8_t b8;
typedef uint8_t u8;
typedef uint64_t u64;
typedef double f64;
typedef b8 u8;

#define unreferenced(p) (void)p
#define zero {}
#define func static
#define global static
#define null NULL

global constexpr u64 c_kb = 1024;
global constexpr u64 c_mb = 1024 * c_kb;
global constexpr u64 c_gb = 1024 * c_mb;

#ifdef __clang__
#define printf_warnings(a, b) __attribute__((format(printf, (a), (b))))
#else // __clang__
#define printf_warnings(a, b)
#endif // __clang__


#define assert(condition) if(!(condition)) { on_failed_assert(#condition, __FILE__, __LINE__); }

#define array_count(arr) (sizeof((arr)) / sizeof((arr)[0]))

func void on_failed_assert(char* condition, char* file, int line)
{
	printf("failed assert %s at %s(%i)\n", condition, file, line);
	if(IsDebuggerPresent()) {
		__debugbreak();
	}
	exit(1);
}

template <typename t, int n>
struct s_sarray
{
	t elements[n];
	int count = 0;

	void add(t new_element);
	t pop();
	t& get(int index);
	t& operator[](int index);
	t& get_last();
	void remove_and_shift(int index);
};
