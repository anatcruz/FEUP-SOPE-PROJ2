CC = gcc
CFLAGS = -Wall -pthread
OBJ = src/utils.o src/args.o
UOBJ = src/U.o
QOBJ = src/Q.o src/queue.o
TARGETS = U2 Q2

all: U2 Q2

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

U2: $(UOBJ) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(UOBJ) -o $@
	@echo $@

Q2: $(QOBJ) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(QOBJ) -o $@
	@echo $@

clean:
	@rm $(OBJ) $(UOBJ) $(QOBJ) $(TARGETS)