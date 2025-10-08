# Preprocessing

Poppy uses a preprocessing system to include modules in user code. This is done via the directive

```
!!munch module_name
```

Poppy also supports comments prefixed with `!!-`.

The only module supported right now is `poppyio`, which contains the functions below:
- `void print(int)`
- `void printc(char)`
- `void println()`
- `int scan()`
- `char scanc()`
