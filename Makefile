CC=mpicc
CFLAGS=-Wall -Wextra -Wuninitialized -MMD -g -fdiagnostics-color=auto
LDFLAGS=
SRC=$(wildcard *.c)
OBJ=$(addprefix build/,$(SRC:.c=.o))
OBJ_PERF=$(addprefix build-perf/,$(SRC:.c=.o))
DEP=$(addprefix build/,$(SRC:.c=.d))
DEP_PERF=$(addprefix build-perf/,$(SRC:.c=.d))

all: tests perf

tests: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

perf: $(OBJ_PERF)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

build-perf/%.o: %.c
	@mkdir -p build-perf
	$(CC) -DPERFORMANCE_MEASURE $(CFLAGS) -o $@ -c $<

clean:
	rm -rf build

-include $(DEP)
-include $(DEP_PERF)
