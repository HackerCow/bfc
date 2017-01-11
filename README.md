# Brainfuck Compiler

## Features
* Transpile Brainfuck code to multiple targets (only C supported so far)

## Installation
```
$ make
```
Done.

## Usage Example
```
$ cat hello
++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
$ ./bfc hello > hello.c
$ gcc -o hello.out hello.c
$ ./hello.out
Hello, World!
```

`bfc` outputs the transpiled code to stdout, so by saving that to a file we can compile it using a regular C compiler. We then get an executable that has equivalent functionality to our Brainfuck script.
