CMD = gcc
CFLAGS = -Wall -Wextra -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal
LNK = -lcheck -lm -pthread -lrt -lsubunit
SRC = ./src/
BIN = ./bin/
INC = ./include/
TST = ./test/
TSTSRC = ./test/src/
TSTBIN = ./test/bin/
TSTINC = ./test/include/
CMD += $(LNK)
CMD += $(CFLAGS)
CMD += -I $(INC)
TARGETS = server
TEST_TARGETS = check_check
OBJS = $(BIN)server.o $(BIN)cserver.o $(BIN)list.o $(BIN)queue.o
TSTOBJS = $(TSTBIN)check_check.o $(TSTBIN)test_list.o $(TSTBIN)test_queue.o $(BIN)list.o $(BIN)queue.o
all: $(TARGETS)

################
# main targets #
################
$(TARGETS): $(OBJS)
	$(CMD) $^ -o $@
$(BIN)%.o: $(SRC)%.c
	$(CMD) -c $^ -o $@
################
# test targets #
################
$(TEST_TARGETS): $(TSTOBJS)
	$(CMD) $^ $(LNK) -o $@
$(TSTBIN)%.o: $(TSTSRC)%.c
	$(CMD) -c $^ -o $@
#######################
# maintenance targets #
#######################

clean:
	rm -rf bin/*
	rm -rf test/bin/*
	rm -rf check_check
	rm -rf server
debug: CMD += -g
debug: clean all
check: CMD += -g -I $(TSTINC)
check: check_check
valgrind: debug check
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./check_check
vmain: debug check
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./server
