CXX=g++
CXXFLAGS=-std=c++20 -Wall -g -MMD -I .
EXEC=main
CCFILES=$(shell find -name "*.cc" -type f)
OBJECTS=${CCFILES:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}
${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}
-include ${DEPENDS}

PHONY: clean run
clean:
	rm ${EXEC} ${OBJECTS} ${DEPENDS}

run: ${EXEC}
	$(MAKE) runprint --no-print-directory

runprint: ${EXEC}
	./${EXEC}
