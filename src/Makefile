CXXFLAGS  = -mwindows
CXXFLAGS += -mthreads
CXXFLAGS += -D_MT
CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -pedantic
CXXFLAGS += -Weffc++
CXXFLAGS += -std=c++17
CXXFLAGS += -O2

LIBS  = -lWs2_32
LIBS += -lwinmm

all:
	c++ -c main.cpp -o main.o ${CXXFLAGS}
	c++ main.o -o remote-audiorec.exe ${CXXFLAGS} ${LIBS}
