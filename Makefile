CC := g++

CFLAGS := -std=c++11 

SRCS := scrypt.cpp userpass.cpp

OBJS := ${SRCS:.cpp=.o} pass_server.o userpass.o

PROGS := ${SRCS:.cpp=} 

.PHONY: all

all: ${PROGS}

scrypt: scrypt.o 
	${CC} ${CFLAGS} $^ -o $@ -lcrypt

userpass: userpass.o pass_server.o
	${CC} ${CFLAGS} $^ -o $@ -lcrypt

%.o: %.cpp
	${CC} ${CFLAGS} -c $<

clean:
	rm -f ${PROGS} ${OBJS} *.bak *~
