# Poppy 1.0

This is the first version of Poppy. It supports 64-bit integer variables, basic arithmetic operations and control flow constructs in the form of conditional statements, loops, and functions.

Entry is via a `main` function that takes no arguments and does not return anything.

## I/O
Currently, Poppy supports two functions for output, both included in the `poppyio` module. 

- `print` takes a single integer parameter and prints it
- `println` advances the print cursor to the next line

Reading input is not currently possible.

## Sample Program
This program calculates 5 to the 10th power.

```
munch poppyio

int exp(int x, int n){
    if (n == 0){
        hop 1;
    }

    let int y = 1;
    while (n > 1){
        if (n % 2 == 1){
            y = x * y;
            n = n - 1;
        }

        x = x * x;
        n = n / 2;
    }
    hop x * y;
}

void main(){
    print(exp(5, 10));
}
```
