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
typedef struct s_a
{
	int a;
} s_a;
typedef struct s_b
{
	s_a b;
} s_b;
typedef struct s_c
{
	s_b c;
	int kiwi;
} s_c;
void f(void)
{
	s_c d = {0};
	d.c.b.a = 1;
}
