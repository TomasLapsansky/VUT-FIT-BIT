.PHONY: all main

all: main
	./main && echo pass || echo fail

main:
	make -C .. main
	cp ../main .
