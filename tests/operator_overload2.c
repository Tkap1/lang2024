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
typedef struct s_v2
{
	float x;
	float y;
} s_v2;
s_v2 v2(void)
{
	return v2(1, 2);
}
s_v2 __overload0__(s_v2 a, float b)
{
	a.x = (a.x * b);
	a.y = (a.y * b);
	return a;
}
void foo(void)
{
	s_v2 a = {0};
	s_v2 b = __overload0__(v2(), 2.000000f);
}
