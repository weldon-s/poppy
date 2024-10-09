# Sample Programs

```
int main(){
    !! this is a comment
    println("Hello world!");
    !- this is a comment
     - that spans multiple lines -!
    hop 0;
}
```

```
int fib(int n){
    if (n <= 1){
        hop n;
    }

    hop fib(n - 1) + fib(n - 2);
}

int main(){
    print(fib(20));
    hop 0;
}
```