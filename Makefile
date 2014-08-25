CXX = c++
CXX_WARN = -Wall -Wextra -pedantic
CXX_NOWARN =
CXX_OPT = -O3
CXX_MISC = -fpic
CXX_INCLUDE = -Iinclude -Ilib/juble/include -Ilib/juble/lib/ruby/include -Ilib/juble/lib/ruby/.ext/include/x86_64-linux
CXX_PLATFORM_FLAGS =
CXX_FLAGS=-std=c++1y ${CXX_WARN} ${CXX_NOWARN} ${CXX_OPT} ${CXX_MISC} ${CXX_INCLUDE} ${CXX_PLATFORM_FLAGS}

LD_PLATFORM_LIBS =
LD_LIBS = -lclang -Llib/juble/lib/ruby -lruby-static ${LD_PLATFORM_LIBS}

OUT_DIR = bin/
OBJ_DIR = ${OUT_DIR}obj/
TARGET = ${OUT_DIR}color_coded.so

OBJS := $(patsubst %.cpp,%.cpp.o,$(wildcard src/*.cpp))
OBJS := $(filter-out ${EXCLUDE},${OBJS})
SRCS := ${OBJS:.cpp.o=.cpp}
OUT_OBJS := $(foreach obj,${OBJS}, ${OBJ_DIR}${obj})

.PHONY: all setup run ${TARGET}_setup

.SILENT:

log = echo "$(1)" 1>&2

all: ${TARGET}

${TARGET}_setup: setup
	$(call log,"Building ${TARGET}")

${TARGET}: ${TARGET}_setup ${OBJS}
	$(call log,"Linking $@")
	${CXX} -shared ${CXX_FLAGS} ${OUT_OBJS} ${LD_LIBS} -o $@

setup:
	mkdir -p ${OBJ_DIR} ${OUT_DIR}

run:
	gvim -c ":source ${PWD}/color_coded.vim" test.cpp

%.cpp.o: %.cpp
	$(call log,"  Compiling $<")
	mkdir -p ${OBJ_DIR}$(dir $<)
	${CXX} -c ${CXX_FLAGS} $< -o ${OBJ_DIR}$@
