#pragma once

#define MS_B(x)  (x)
#define MS_KB(x) ((x) << 10)
#define MS_MB(x) ((x) << 20)
#define MS_GB(x) ((x) << 30)
#define MS_TB(x) (((uint64_t)x) << 40)

#define MS_ARRAY_SIZE(arr) sizeof(arr)/sizeof(arr[0])

/// N-bit signed integer maximum value
#define MS_INT_MAX_N(n) ((1 << (n - 1)) - 1)
/// N-bit signed integer minimum value
#define MS_INT_MIN_N(n) (-MS_INT_MAX_N(n) - 1)
#define MS_INT_IN_RANGE(v, nbits) ((v) >= MS_INT_MIN_N(nbits) && (v) <= MS_INT_MAX_N(nbits))
