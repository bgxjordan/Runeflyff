// Fails to compiler under 64 bit mode

#include <climits>

#define EXPECTED_CHAR_BIT 8
#define EXPECTED_FLOAT_SIZE 4
#define EXPECTED_INT_SIZE 4
#define EXPECTED_LONG_SIZE 4
#define EXPECTED_LONG_LONG_SIZE 8
#define EXPECTED_PTR_SIZE 4
#define EXPECTED_SHORT_SIZE 2

#define DECL_SIZE_TEST(name, expectedSize) \
template<unsigned long Size> struct Test ## name ## Size; \
template<> struct Test ## name ## Size<expectedSize> {};

#define SIZE_TEST(name, actualSize) Test ## name ## Size<actualSize>();

DECL_SIZE_TEST(char_bits, EXPECTED_CHAR_BIT)
DECL_SIZE_TEST(float, EXPECTED_FLOAT_SIZE)
DECL_SIZE_TEST(int, EXPECTED_INT_SIZE)
DECL_SIZE_TEST(long, EXPECTED_LONG_SIZE)
DECL_SIZE_TEST(long_long, EXPECTED_LONG_LONG_SIZE)
DECL_SIZE_TEST(pointer, EXPECTED_PTR_SIZE)
DECL_SIZE_TEST(short, EXPECTED_SHORT_SIZE)

int main()
{
    SIZE_TEST(char_bits, CHAR_BIT)
    SIZE_TEST(float, sizeof(float))
    SIZE_TEST(int, sizeof(int))
    SIZE_TEST(long, sizeof(long))
    SIZE_TEST(long_long, sizeof(long long))
    SIZE_TEST(pointer, sizeof(void*))
    SIZE_TEST(short, sizeof(short))
    return 0;
}