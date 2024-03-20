#pragam once

#define MS_B(x)  (x)
#define MS_KB(x) ((x) << 10)
#define MS_MB(x) ((x) << 20)
#define MS_GB(x) ((x) << 30)
#define MS_TB(x) (((uint64_t)x) << 40)

#define MS_ARRAY_SIZE(arr) sizeof(arr)/sizeof(arr[0])
