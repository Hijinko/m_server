CMD = gcc 
CFLAGS = -Wall -Wextra -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal
LNK = -lcheck -lm -pthread -lrt -lsubunit
SRC = ./src/
BIN = ./bin/
INC = ./include/
TST = ./test/
TSTSRC = ./test/src/
TSTBIN = ./test/bin/
TSTINC = ./test/include
CMD += $(LNK)
CMD += $(CFLAGS)
CMD += -I $(INC)
BINS = server
OBJS = $(BIN)server.o $(BIN)cserver.o

all: $(BINS)

################
# main targets #
################
server: $(OBJS)
	$(CMD) $^ -o $@
$(BIN)%.o: $(SRC)%.c
	$(CMD) -c $^ -o $@

clean:
	rm -rf bin/*
	rm -rf test/bin/*
	rm -rf check_check
	rm -rf server
debug: CMD += -g
debug: clean all
check: CMD += -I $(TSTINC)
check: check_check
valgrind: CMD += -I $(TSTINC)
valgrind: debug check_check
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./check_check
