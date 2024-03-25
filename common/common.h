#pragma once

#define MS_B(x) (x)
#define MS_KB(x) ((x) << 10)
#define MS_MB(x) ((x) << 20)
#define MS_GB(x) ((x) << 30)
#define MS_TB(x) (((uint64_t)x) << 40)

#define MS_ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])

#if __STDC_VERSION__ >= 202311L || defined(__GNUC__) || defined(__clang__)
#	define MS_SWAP_VARS(a, b) \
		{                      \
			typeof(a) tmp = a; \
			a = b;             \
			b = tmp;           \
		}
#else
// https://stackoverflow.com/a/3982430
#	define MS_SWAP_VARS(a, b)                                                        \
		do {                                                                          \
			unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
			memcpy(swap_temp, &y, sizeof(x));                                         \
			memcpy(&y, &x, sizeof(x));                                                \
			memcpy(&x, swap_temp, sizeof(x));                                         \
		} while (0)
#endif

/// N-bit signed integer maximum value
#define MS_INT_MAX_N(n) ((1 << (n - 1)) - 1)
/// N-bit signed integer minimum value
#define MS_INT_MIN_N(n) (-MS_INT_MAX_N(n) - 1)
#define MS_INT_IN_RANGE(v, nbits) ((v) >= MS_INT_MIN_N(nbits) && (v) <= MS_INT_MAX_N(nbits))
