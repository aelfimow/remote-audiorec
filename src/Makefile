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

all:
	c++ -c SocketListener.cpp -o SocketListener.o ${CXXFLAGS}
	c++ -c AddrInfo.cpp -o AddrInfo.o ${CXXFLAGS}
	c++ -c WSAStarter.cpp -o WSAStarter.o ${CXXFLAGS}
	c++ -c SocketThread.cpp -o SocketThread.o ${CXXFLAGS}
	c++ -c SocketThread_priv.cpp -o SocketThread_priv.o ${CXXFLAGS}
	c++ -c main.cpp -o main.o ${CXXFLAGS}
	c++ main.o SocketThread.o SocketThread_priv.o WSAStarter.o AddrInfo.o SocketListener.o -o remote-audiorec.exe ${CXXFLAGS} ${LIBS}
