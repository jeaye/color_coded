TARGET = ${OUT_DIR}libcolor_coded_boost_system.a

OBJS := $(patsubst %.cpp,%.cpp.o,$(wildcard src/*.cpp))
OBJS := $(filter-out ${EXCLUDE},${OBJS})
SRCS := ${OBJS:.cpp.o=.cpp}
OUT_OBJS := $(foreach obj,${OBJS}, ${OBJ_DIR}${obj})

log = echo "$(1)" 1>&2

all: ${TARGET}

${TARGET}_setup: setup
	$(call log,"Building ${TARGET}")

${TARGET}: ${TARGET}_setup ${OBJS}
	$(call log,"Linking $@")
	ar crs $@ ${OUT_OBJS} ${LD_LIBS}

setup:
	mkdir -p ${OBJ_DIR} ${OUT_DIR}

clean:
	rm -f ${TARGET} ${OUT_OBJS}
	$(call log,"Cleaned ${TARGET}")

%.cpp.o: %.cpp
	$(call log,"  Compiling $<")
	mkdir -p ${OBJ_DIR}$(dir $<)
	${CXX} -c ${CXX_FLAGS} -w $< -o ${OBJ_DIR}$@

.SILENT:

.PHONY: all setup ${TARGET}_setup clean
