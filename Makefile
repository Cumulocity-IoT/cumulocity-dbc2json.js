CXX=/Users/anilanar/Downloads/emsdk_portable/emscripten/1.35.0/em++
DBC=/Users/anilanar/Desktop/vector_dbc
CXXFLAGS=-std=c++11 -O3
LFLAGS=-O3 -s ALLOW_MEMORY_GROWTH=1 --memory-init-file 0
EXPORT=-s EXPORTED_FUNCTIONS="['_run']" -s ASSERTIONS=1

DBC_INC=$(DBC)/include/Vector
DBC_SRC=$(DBC)/src/Vector/DBC
CUSTOM_HEADERS=$(DBC_INC)/DBC/config.h $(DBC_INC)/DBC/vector_dbc_export.h
BC=build/main.bc lib/Vector_DBC/lib/libVector_DBC.bc build/jsonxx.bc

DBC_CPP := $(wildcard $(DBC_SRC)/*.cpp)
DBC_H   := $(wildcard $(DBC_INC)/DBC/*.h)

.PHONY: clean distclean


all: $(BC)
				@mkdir -p dist
			  $(CXX) $(LFLAGS) $(EXPORT) -o dist/main.js $^

#build/dbc: $(DBC_CPP) $(DBC_INC)/DBC.h $(CUSTOM_HEADERS) $(DBC_H)
#				@mkdir -p $@/Vector/DBC
#				cp $(DBC_CPP) $@/Vector/DBC
#				cp $(DBC_INC)/DBC.h $@/Vector/DBC.h
#				cp $(DBC_H) $@/Vector/DBC
#				cp $(CUSTOM_HEADERS) $@/Vector/DBC

lib/Vector_DBC/lib/libVector_DBC.bc:
				@mkdir -p lib/Vector_DBC/build
				cmake -E chdir lib/Vector_DBC/build \
								cmake .. -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_CXX_FLAGS=-std=c++11 -DCMAKE_CC_COMPILER=$(CC)
				make -C lib/Vector_DBC/build
				make -C lib/Vector_DBC/build install DESTDIR=..
				mv lib/Vector_DBC/lib/libVector_DBC.dylib lib/Vector_DBC/lib/libVector_DBC.bc

lib/Vector_DBC/include/DBC.h: lib/Vector_DBC/lib/libVector_DBC.bc

build/jsonxx.bc: lib/jsonxx/jsonxx.cc
				@mkdir -p $(@D)
				$(CXX) $(CXXFLAGS) -o $@ $^

SOURCE=$(wildcard src/*.cpp)
build/main.bc: $(wildcard src/*.cpp) lib/Vector_DBC/include/DBC.h
				@mkdir -p build
				$(CXX) -I./lib/Vector_DBC/include -I./lib/jsonxx $(CXXFLAGS) $(EXPORT) -o $@ $(SOURCE)

clean:
				rm -rf build

distclean:
				rm -rf dist

