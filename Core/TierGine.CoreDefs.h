#include <memory>

#define interface struct

#define BIT(n) n == 0 ? 0 : 1 << (n-1)

typedef int TG_Status;

#define TG_Ok 0
#define TG_Critical 1
