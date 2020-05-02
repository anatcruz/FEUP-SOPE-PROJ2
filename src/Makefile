CC = gcc
CFLAGS = -Wall -pthread
DEPS = utils.h args.h
OBJ = utils.o args.o
TARGETS = U1 Q1

all: U Q

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

U: $(OBJ)
	@$(CC) $(CFLAGS) -o $@1 $@.c $(OBJ)
	@echo $@1

Q: $(OBJ)
	@$(CC) $(CFLAGS) -o $@1 $@.c $(OBJ)
	@echo $@1

clean:
	@rm *.o $(TARGETS)