# Preprocessing

Poppy uses a preprocessing system to include modules in user code. This is done via the directive

```
!!munch module_name
```

The only module supported right now is `poppyio`, which contains the functions `print(int)`, `printc(char)`, and `println()`. 