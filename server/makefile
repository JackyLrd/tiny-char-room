SRC = ./src
INC = ./include
BUILD = ./build
BIN = ./bin

SOURCE = $(wildcard ${SRC}/*.cpp)  
OBJECT = $(patsubst %.cpp, ${BUILD}/%.o, $(notdir ${SOURCE}))  

TARGET = main
BIN_TARGET = ${BIN}/${TARGET}

CC = clang++
CFLAGS = -D REENTRANT -std=c++11

${BIN_TARGET}:${OBJECT}
	$(CC) -o $@ -lpthread ${OBJECT}

${BUILD}/%.o:${SRC}/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY:clean
clean:
	find $(BUILD) -name *.o -exec rm -rf {} \;
	rm -rf $(BIN_TARGET)