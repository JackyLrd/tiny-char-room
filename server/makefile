SRC = ./src
INC = ./include
BUILD = ./build
BIN = ./bin

SOURCE = $(wildcard ${SRC}/*.c)
OBJECT = $(patsubst %.c, ${BUILD}/%.o, $(notdir ${SOURCE}))

TARGET = main
BIN_TARGET = ${BIN}/${TARGET}

CC = clang
CFLAGS = -D REENTRANT

${BIN_TARGET}:${OBJECT}
	$(CC) -o $@ -lpthread ${OBJECT}

${BUILD}/%.o:${SRC}/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY:clean
clean:
	rm $(BUILD)/*.o