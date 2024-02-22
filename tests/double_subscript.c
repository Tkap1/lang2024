#include <stdint.h>
#include <math.h>
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t b8;
typedef uint32_t b32;
void* memcpy(void* restrict dest, const void* restrict src, size_t n);
typedef struct s_foo
{
	int foo[32];
} s_foo;
typedef struct s_bar
{
	s_foo foos[64];
} s_bar;
void baz(void)
{
	s_bar bar[128];
	(*((*(bar + 4)).foos + 9));
}
