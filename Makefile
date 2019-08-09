
CC = gcc
CPP = g++ -std=c++17
F77 = gfortran
FC = gfortran

CFLAGS =
CXXFLAGS = -I/usr/include/eigen3/unsupported -I/usr/include/eigen3
F77FLAGS =
FCFLAGS =

CLIBS =
CPPLIBS =  -ljsoncpp
FLIBS =

OPTIONS =


PROGRAM = main
ARGS =
INSTALLDIR= ./

SOURCES.c = 

SOURCES.cpp = TG.cpp

SOURCES.f = 

SOURCES.F = 

SOURCES = ${SOURCES.c} ${SOURCES.cpp} ${SOURCES.f} ${SOURCES.F}
OBJECTS = ${SOURCES.c:%.c=%.o} ${SOURCES.cpp:%.cpp=%.o} ${SOURCES.f:%.f=%.o} ${SOURCES.F:%.F=%.o}

$(PROGRAM): ${OBJECTS}
	$(CPP) -o $(PROGRAM) ${OBJECTS} $(FLIBS) $(FCFLAGS)  $(CPPLIBS)

.c.o:
	$(CC) -c $(OPTIONS) $(CFLAGS) $< 

.f.o:
	$(FC) -c $(OPTIONS) $(FCFLAGS) $< 

.cpp.o:
	$(CPP) -c $(OPTIONS) $(CXXFLAGS) $< 

test:$(PROGRAM)
	./$(PROGRAM) ${ARGS}

clean:
	${RM} ${OBJECTS} ${PROGRAM}

install:$(PROGRAM)
	install -c ${PROGRAM} ${INSTALLDIR}

.PHONY:test,install


