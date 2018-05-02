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

OBJ_FILES  = a.o
OBJ_FILES += b.o
OBJ_FILES += c.o
OBJ_FILES += d.o
OBJ_FILES += e.o
OBJ_FILES += f.o
OBJ_FILES += g.o
OBJ_FILES += h.o
OBJ_FILES += i.o
OBJ_FILES += j.o
OBJ_FILES += k.o
OBJ_FILES += l.o
OBJ_FILES += m.o
OBJ_FILES += n.o
OBJ_FILES += o.o
OBJ_FILES += p.o
OBJ_FILES += q.o
OBJ_FILES += r.o

all:
	c++ -c WMDestroyHandler.cpp -o r.o ${CXXFLAGS}
	c++ -c WMCommandHandler.cpp -o q.o ${CXXFLAGS}
	c++ -c MMWimCloseHandler.cpp -o p.o ${CXXFLAGS}
	c++ -c MMWimDataHandler.cpp -o o.o ${CXXFLAGS}
	c++ -c MMWimOpenHandler.cpp -o n.o ${CXXFLAGS}
	c++ -c WMUserStopHandler.cpp -o m.o ${CXXFLAGS}
	c++ -c WMUserStartHandler.cpp -o l.o ${CXXFLAGS}
	c++ -c WMSizeHandler.cpp -o k.o ${CXXFLAGS}
	c++ -c WMSetFocusHandler.cpp -o j.o ${CXXFLAGS}
	c++ -c WMCreateHandler.cpp -o i.o ${CXXFLAGS}
	c++ -c Console.cpp -o h.o ${CXXFLAGS}
	c++ -c SocketScope.cpp -o g.o ${CXXFLAGS}
	c++ -c SocketListener.cpp -o f.o ${CXXFLAGS}
	c++ -c AddrInfo.cpp -o e.o ${CXXFLAGS}
	c++ -c WSAStarter.cpp -o d.o ${CXXFLAGS}
	c++ -c SocketThread.cpp -o c.o ${CXXFLAGS}
	c++ -c SocketThread_priv.cpp -o b.o ${CXXFLAGS}
	c++ -c main.cpp -o a.o ${CXXFLAGS}
	c++ ${OBJ_FILES} -o remote-audiorec.exe ${CXXFLAGS} ${LIBS}

main:
	c++ -c main.cpp -o a.o ${CXXFLAGS}
