# Fractus; A Romance (Programming) Language

----
### What is Fractus?

A Latin-themed programming language.
It is dynamically typed and, as the name implies, fundamentally broken.  
Source code files should end in .spqr



## Format

#### Translations
Fractus would hardly be a Latin-themed language if it were not written in Latin.  Translations were chosen by pasting the language's keywords into Google Translate and choosing the first result.  

* if -> *si* (if)
* elseif -> *aliud* (otherwise),
* else -> *tandem* (at last)
* while -> *dum* (while)
* define -> *praenomino* (title, to give a name)
* lambda -> *incognita* (unknown)
* run -> *exequor* (to execute)

#### Comments
A single exclamation point symbolizes the Boolean NOT.  Two exclamation points is a double negative which confuses the interpreter.  It chooses to ignore what it does not understand and thus comments are created.

    !!  This is a comment

#### Statements
StatementLists make up a Fractus program.  A statement can be a function call, variable assignment, expression, if statement, while loop, lambda, or variable definition.  Semicolons are the chosen statement terminator.

#### Functions
All functions are anonymous, in order to save them for later use they must be asssigned to a variable.  Lambdas can be run immediately after being defined.  Function calls require the keyword *exequor* before the variable name to be executed.

For example:  

    !! This is a function definition
      praenomino foo lambda(){"Salve Mundi!"}

    !! This is how you call it
      exequor foo();

    !! Alternatively, call the lambda immediately by appending  
    !! its arguments to the end of the definition
      exequor lambda(){"Salve Mundi!"}()

#### Definitions
Definition is done using the keyword _praenomino_.  Variables can be defined as integers, strings, arrays or functions.  Variables that are defined but not explicitly given a value will be instantiated as NULL.  

#### Operators
Operators are prefix style, take exactly two expressions, and are right associative. Precedence is given to the deepest nested operator.  
For example:

    foo = * + 5 - 2 1 ^ 4 2;

Is equivalent to:

    foo = (*  (+  5
                  (- 2 1))
              (^ 4 2));

which is also allowed and preferred for the sake of readability.

Provided operators are:
* (+) PLUS
* (-) MINUS
* (*) MULTIPLY
* (/) DIVIDE
* (^) EXPONENTIATE
* (%) MODULO
* (<) LESS THAN
* (>) GREATER THAN
* (<=) LESS THAN OR EQUAL
* (>=) GREATER THAN OR EQUAL
* (==) EQUAL
* (!=) NOT EQUAL
* (&) AND
* (|) OR
* (!) NOT

#### If statement
If statements follow the format:  
The else and elseif blocks are optional.

    si (expression) {
      !! if true do this
    } aliud (expression) {
      !! else if true do this
    } tandem {
      !! else do this
    }


#### While statement
While statements follow the format:

    dum (expression) {
      !! do this while true
    }

---
## Why You Should Use Fractus
1. It's unique.  
There is only one other Latin programming language ever written, [Lusus](https://en.wikipedia.org/wiki/Lusus_(programming_language), and there is more documentation on wikipedia than the language's website.
2. You want to learn exactly seven words of Latin: no more, no less, and proper grammar does not concern you.
3. You believe that no language that is easy to use is worth using.

### Quotes about Fractus

> "Who wrote this crap?" - Me   
> "I know I fixed *that*!" - Also me  
> "I thought code was ones and zeros" - a friend  
> "How is it a language if you can't speak it?" - another friend  
> "Looks great sweetie" - My mother


---
### Notes

Many things are currently unsupported, such as types other than ints and strings, negative numbers, for loops, do while loops, switch cases, the NOT operator, and lists. In the case of the NOT operator, it will return the value of itself applied to the first expression and ignore the second. Provide a dummy expression if you want to use it. Negative numbers exist but a variable cannot be assigned to one. In order to use a variable with a negative value, simply assign to it the expression (- 0 [desired value])

### Usage

    dpl filename.spqr

Enjoy

---
##### Coming Soon
Development has not finished, there are many things yet to be added, such as built-in functions with appropriate Latin names, improvements on operators, negative numbers, reals, the DOT operator, lists, streams, and general grammar changes.
