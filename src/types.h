

typedef uint8_t b8;
typedef uint8_t u8;
typedef uint64_t u64;

#define unreferenced(p) (void)p
#define zero {}
#define func static
#define global static
#define null NULL

global constexpr u64 c_kb = 1024;
global constexpr u64 c_mb = 1024 * c_kb;

#define assert(condition) if(!(condition)) { on_failed_assert(#condition, __FILE__, __LINE__); }

func void on_failed_assert(char* condition, char* file, int line)
{
	printf("failed assert %s at %s(%i)\n", condition, file, line);
	if(IsDebuggerPresent()) {
		__debugbreak();
	}
	exit(1);
}