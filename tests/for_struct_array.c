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
typedef struct s_v2i
{
	int x;
	int y;
} s_v2i;
int main(void)
{
	s_v2i foo[32];
	for(int it_index = 0; it_index < 32; it_index += 1)
	{
		s_v2i it = (*(foo + it_index));
	}	
}
