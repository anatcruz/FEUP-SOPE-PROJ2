CC = gcc
CFLAGS = -Wall -pthread
DEPS = src/utils.h src/args.h
OBJ = src/utils.o src/args.o
TARGETS = U2 Q2

all: U2 Q2

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

U2: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ src/U.c $(OBJ)
	@echo $@

Q2: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ src/Q.c $(OBJ)
	@echo $@

clean:
	@rm $(OBJ) $(TARGETS)