OBJS = lexer.o parser.o environment.o evaluator.o scanner.o types.o pretty.o
OOPTS = -Wall -Wextra -g -c

#I don't get a compiler errors with or without -lm so
#hopefully this is where it goes
all	:	$(OBJS)
		gcc -Wall -Wextra -g -o Fractus $(OBJS) -lm

test	: all
			dpl test.spqr

#Shows missing semicolon
error1	: error1.spqr
				cat error1.spqr
error1x	: all error1.spqr
				dpl error1.spqr
#Shows missing closing paren
error2	: error2.spqr
				cat error2.spqr
error2x	: all error2.spqr
				dpl error2.spqr
#Shows keyword RUN without a VARIABLE or LAMBDA
error3	: error3.spqr
				cat error3.spqr
error3x	: all error3.spqr
				dpl error3.spqr
#Shows attempting to create an array with size as a given STRING
error4	: error4.spqr
				cat error4.spqr
error4x	: all error4.spqr
				dpl error4.spqr
#Shows attempting to apply an operator to two different types, currently unsupported.
#Who am I kidding, it will never be supported
error5	: error5.spqr
				cat error5.spqr
error5x	: all error5.spqr
				dpl error5.spqr
#Shows use of arrays
arrays	: array.spqr
				cat array.spqr
arraysx	: all array.spqr
				dpl array.spqr
#Shows extensive use of if/elseif/else
conditionals	: cond.spqr
							cat cond.spqr
conditionalsx	: all cond.spqr
							dpl cond.spqr
#Shows basic recursion
recursion	: recur.spqr
							cat recur.spqr
recursionx	: all recur.spqr
							dpl recur.spqr
#Shows use of while loops
iteration	: iter.spqr
							cat iter.spqr
iterationx	: all iter.spqr
							dpl iter.spqr
#Shows functions as first class objects
functions	: func.spqr
							cat func.spqr
functionsx	: all func.spqr
							dpl func.spqr
#Returning a lambda from a function and calling that
lambda	: lambda.spqr
							cat lambda.spqr
lambdax	: all lambda.spqr
							dpl lambda.spqr
#UNIMPLEMENTED
dictionary	:	dict.spqr
							cat dict.spqr
dictionaryx	:	all dict.spqr
							dpl dict.spqr
#UNIMPLEMENTED
problem		:	sieve.spqr
						cat sieve.spqr
problemx	: all sieve.spqr
						dpl sieve.spqr

#Helper Rules
lexer.o	: lexer.c lexer.h
				gcc $(OOPTS) lexer.c
parser.o	:	parser.c parser.h
					gcc $(OOPTS) parser.c
environment.o	:	environment.c environment.h
					gcc $(OOPTS) environment.c
evaluator.o	:	evaluator.c evaluator.h
					gcc $(OOPTS) evaluator.c
scanner.o	: scanner.c
					gcc $(OOPTS) scanner.c
types.o	:	types.h types.c
				gcc $(OOPTS) types.c
pretty.o	:	pretty.c pretty.h
					gcc $(OOPTS) pretty.c
clean	:
			rm -f *.o Fractus
