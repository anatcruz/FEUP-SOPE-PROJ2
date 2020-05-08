CC = gcc
CFLAGS = -Wall -pthread
DEPS = src/utils.h src/args.h
OBJ = src/utils.o src/args.o
TARGETS = src/U2 src/Q2

all: src/U src/Q

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

src/U: $(OBJ)
	@$(CC) $(CFLAGS) -o $@2 $@.c $(OBJ)
	@echo $@2

src/Q: $(OBJ)
	@$(CC) $(CFLAGS) -o $@2 $@.c $(OBJ)
	@echo $@2

clean:
	@rm $(OBJ) $(TARGETS)