CXXFLAGS  = -mwindows
CXXFLAGS += -mthreads
CXXFLAGS += -D_MT
CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -pedantic
CXXFLAGS += -Weffc++
CXXFLAGS += -std=c++17
CXXFLAGS += -O2
CXXFLAGS += -m64

LIBS  = -lWs2_32
LIBS += -lwinmm

OBJ_FILES  = main.o
OBJ_FILES += SocketThread.o
OBJ_FILES += SocketThread_priv.o
OBJ_FILES += WSAStarter.o
OBJ_FILES += AddrInfo.o
OBJ_FILES += SocketListener.o
OBJ_FILES += SocketScope.o

all:
	c++ -c SocketScope.cpp -o SocketScope.o ${CXXFLAGS}
	c++ -c SocketListener.cpp -o SocketListener.o ${CXXFLAGS}
	c++ -c AddrInfo.cpp -o AddrInfo.o ${CXXFLAGS}
	c++ -c WSAStarter.cpp -o WSAStarter.o ${CXXFLAGS}
	c++ -c SocketThread.cpp -o SocketThread.o ${CXXFLAGS}
	c++ -c SocketThread_priv.cpp -o SocketThread_priv.o ${CXXFLAGS}
	c++ -c main.cpp -o main.o ${CXXFLAGS}
	c++ ${OBJ_FILES} -o remote-audiorec.exe ${CXXFLAGS} ${LIBS}
