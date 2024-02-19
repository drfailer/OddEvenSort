CC=mpicc
CFLAGS=-Wall -Wextra -Wuninitialized -MMD -g -fdiagnostics-color=auto
LDFLAGS=
SRC=$(wildcard *.c)
OBJ=$(addprefix build/,$(SRC:.c=.o))
DEP=$(addprefix build/,$(SRC:.c=.d))

prog: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf build

-include $(DEP)
