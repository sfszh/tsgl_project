AR=ar
CC=g++
RM=rm -f
INSTALL=/usr/bin/install
PREFIX=/usr/local

SRC_PATH=src/TSGL/
TESTS_PATH=src/tests/
OBJ_PATH=build/
VPATH=SRC_PATH:TESTS_PATH:OBJ_PATH

HEADERS  := $(wildcard src/TSGL/*.h)
SOURCES  := $(wildcard src/TSGL/*.cpp)
TESTS    := $(wildcard src/tests/*.cpp)
OBJS     := $(patsubst src/TSGL/%.cpp,build/TSGL/%.o,${SOURCES})
TESTOBJS := $(patsubst src/tests/%.cpp,build/tests/%.o,${TESTS})
NOWARN   := -Wno-unused-parameter -Wno-unused-function -Wno-narrowing
UNAME    := $(shell uname)

ifeq ($(UNAME), Linux)
	OS_LFLAGS :=
	OS_LDIRS := -L/opt/AMDAPP/lib/x86_64/
	OS_GLFW := glfw
	OS_GL := -lGL
endif

ifeq ($(UNAME), Darwin)
	OS_LFLAGS := -framework Cocoa -framework OpenGl -framework IOKit -framework Corevideo
	OS_LDIRS :=
	OS_GLFW := glfw3
	OS_GL :=
endif

CXXFLAGS=-O3 -g3 \
	-Wall -Wextra \
	-D__GXX_EXPERIMENTAL_CXX0X__ \
	-I/usr/local/include/Cellar/glfw3/3.1.1/include/ \
	-I${SRC_PATH}/ \
	-I/usr/include/ \
	-I/usr/local/include/ \
	-I/opt/AMDAPP/include/ \
	-I/opt/local/include/freetype2 \
	-I/usr/include/c++/4.6/ \
	-I/usr/include/c++/4.6/x86_64-linux-gnu/ \
	-I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ \
	-I/usr/include/freetype2  \
	-I/usr/include/freetype2/freetype  \
	-I/usr/local/include/freetype2  \
	-I./ \
  -std=c++0x -fopenmp \
  ${NOWARN} -fpermissive
  # -pedantic-errors

LFLAGS=-Llib/ \
	-L/opt/local/lib/ \
	-L/usr/lib/ \
	-L/usr/local/lib/ \
	-L/usr/X11/lib/ \
	${OS_LDIRS} \
	-ltsgl -lfreetype -lGLEW -l${OS_GLFW} \
	-lX11 ${OS_GL} -lXrandr -fopenmp \
	${OS_LFLAGS} 

DEPFLAGS=-MMD -MP

BINARIES= \
  bin/test_specs \
	bin/testAlphaRectangle \
	bin/testAura \
	bin/testBallroom \
	bin/testBlurImage \
	bin/testCalcPi \
	bin/testColorPoints \
	bin/testColorWheel \
	bin/testConcavePolygon \
	bin/testConway \
	bin/testCosineIntegral \
	bin/testDumbSort \
	bin/testFireworks \
	bin/testForestFire \
	bin/testFunction \
	bin/testGetPixels \
	bin/testGradientWheel \
	bin/testGraydient \
	bin/testGreyscale \
	bin/testHighData \
	bin/testImage \
	bin/testImageCart \
	bin/testInverter \
	bin/testLangton \
	bin/testLineChain \
	bin/testLineFan \
	bin/testMandelbrot \
	bin/testMouse \
	bin/testNewtonPendulum \
	bin/testPhilosophers \
	bin/testPong \
	bin/testProgressBar \
	bin/testProjectiles \
	bin/testScreenshot \
	bin/testSeaUrchin \
	bin/testSmartSort \
	bin/testSpectrogram \
	bin/testSpectrum \
	bin/testText \
	bin/testTextCart \
	bin/testTextTwo \
	bin/testUnits \
	bin/testVoronoi \

all: dif tsgl tests docs tutorial

debug: dif tsgl tests

dif: build/build

ifeq ($(UNAME), Linux)  
tsgl: lib/libtsgl.a lib/libtsgl.so
endif

ifeq ($(UNAME), Darwin)
tsgl: lib/libtsgl.a
endif

tests: ${BINARIES}

docs: docs/html/index.html

tutorial: tutorial/docs/html/index.html

cleanall: clean cleandocs

clean:
	$(RM) -r bin/* build/* lib/* tutorial/docs/html/* *~ *# *.tmp

cleandocs:
	$(RM) -r docs/html/*

# -include build/*.d

install:
	test -d $(PREFIX) || mkdir $(PREFIX)
	test -d $(PREFIX)/lib || mkdir $(PREFIX)
	test -d $(PREFIX)/include || mkdir $(PREFIX)
	install -m 0644 lib/libtsgl.a $(PREFIX)/lib
	install -m 0755 lib/libtsgl.so $(PREFIX)/lib
	cp -r src/TSGL $(PREFIX)/include


build/build: ${HEADERS} ${SOURCES} ${TESTS}
	@echo 'Files that changed:'
	@echo $(patsubst src/%,%,$?)
	
ifeq ($(UNAME), Linux)
lib/libtsgl.so: ${OBJS}
	@echo 'Building $(patsubst lib/%,%,$@)'
	$(CC) -shared -o $@ $?
	@touch build/build
endif

lib/libtsgl.a: ${OBJS}
	@echo 'Building $(patsubst lib/%,%,$@)'
	mkdir -p lib
	$(AR) -r $@ $?
	@touch build/build

#List additional dependencies for test binaries
bin/testLangton: build/tests/Langton/AntFarm.o build/tests/Langton/LangtonAnt.o
bin/testVoronoi: build/tests/Voronoi/Voronoi.o build/tests/Voronoi/ShadedVoronoi.o
bin/testConway: build/tests/Conway/LifeFarm.o
bin/testInverter: build/tests/ImageInverter/ImageInverter.o
bin/testPong: build/tests/Pong/Pong.o build/tests/Pong/Paddle.o build/tests/Pong/Ball.o
bin/testSeaUrchin: build/tests/SeaUrchin/SeaUrchin.o
bin/testMandelbrot: build/tests/Mandelbrot/Mandelbrot.o \
	build/tests/Mandelbrot/GradientMandelbrot.o \
	build/tests/Mandelbrot/Buddhabrot.o \
	build/tests/Mandelbrot/Julia.o \
	build/tests/Mandelbrot/Nova.o

#General compilation recipes for test binaries (appended to earlier dependencies)
bin/test%: build/tests/test%.o lib/libtsgl.a
	mkdir -p bin
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

build/%.o: src/%.cpp
	mkdir -p ${@D}
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c -fpic $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

#Doxygen stuff
docs/html/index.html: ${HEADERS} doxyfile
	mkdir -p docs
	@echo 'Generating Doxygen'
	@doxygen doxyfile

tutorial/docs/html/index.html: ${HEADERS} tutDoxyFile
	@echo 'Generating Doxygen'
	mkdir -p tutorial/docs
	doxygen tutDoxyFile

.PHONY: all debug clean tsgl tests docs tutorial dif
.SECONDARY: ${OBJS} ${TESTOBJS} $(OBJS:%.o=%.d)
