SRC_DIRS = BrownianSDL
VPATH = ${SRC_DIRS}
OBJS =	GameEngine.o \
	BrownianEngine.o \
	Particle.o \
	Utilities.o \
	Averager.o \
	main.o
OUTPUT = brownian

CXX = g++
CXXFLAGS = -std=c++11 -ggdb -D_GNU_SOURCE=1 -D_REENTRANT -IBrownian -I/usr/include -I/usr/include/eigen2
LDFLAGS = -L/usr/lib -lSDL -lSDL_gfx -lSDL_ttf -lpthread -lGL -lGLU -lglut -L/usr/lib64 -lbsd

${OUTPUT}: ${OBJS}
	${CXX} ${LDFLAGS} ${OBJS} -o ${OUTPUT}

clean:
	rm -f ${OUTPUT} ${OBJS}
