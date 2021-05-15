## Tiny Basic Interpreter
A tiny basic interpreter in C. Does not follow the exact language spec as I could not find a standard spec. Still a **WIP**. You can read about the intepreter [here](https://blog.trieoflogs.com/tiny-basic/)

- [x] Implement the grammar (some commands omitted)
- [x] Comments
- [ ] Error reporting

```sh
build/tbc examples/fibonacci.tb
```

### Example Programs  
**Fibonacci**
```basic
REM This code prints n fibonacci numbers
LET a = 0
LET b = 1
LET n = 10

REM This is how loops work for now
100 PRINT a
IF n == 0 THEN END
LET t = b
LET b = a + b
LET a = t
LET n = n - 1
GOTO 100
```
  
 **Gimme some stars**
 ```basic
 PRINT "How many stars do you want?"
LET stars = 0
INPUT stars
10 IF stars == 0 THEN END
PRINT "*"
LET stars = stars - 1
GOTO 10
```
