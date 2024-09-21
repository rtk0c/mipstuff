## `as`
A basic assembler targetting MIPS32R6 only

## Notes on semantics
- `make_*()` has no side effects other than allocating
- `init_*()` may perform both allocating and other side effects
- `destroy_*()` has no side effects other than freeing
- `finish_*()` may perform both freeing and other side effects
