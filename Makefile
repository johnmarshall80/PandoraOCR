PROJECT_LIBRARY_DIR = $(PROJECT_DIR)/lib

CC = g++
CFLAGS = -c -g -fPIC -O2 -Wall -Wextra -pedantic -Wno-long-long -Wshadow -Werror -std=c++11
ifdef BUILD_32BIT_COMPATIBLE
    CFLAGS += -m32
endif

LIBS += -L$(PANDORA_DIR)/lib -lPandoraSDK
ifdef MONITORING
    LIBS += $(shell root-config --glibs --evelibs)
    LIBS += -lPandoraMonitoring
endif
ifdef BUILD_32BIT_COMPATIBLE
    LIBS += -m32
endif

PROJECT_BINARY = $(PROJECT_DIR)/bin/PandoraOCR

INCLUDES  = -I $(PROJECT_DIR)/include/
INCLUDES += -I $(PROJECT_DIR)/mnist/include/
INCLUDES += -I $(PANDORA_DIR)/PandoraSDK/include/
ifdef MONITORING
    INCLUDES += -I $(shell root-config --incdir)
    INCLUDES += -I $(PANDORA_DIR)/PandoraMonitoring/include/
endif

ifdef MONITORING
    DEFINES = -DMONITORING=1
endif

SOURCES  =  $(wildcard $(PROJECT_DIR)/src/*.cc)
SOURCES +=  $(wildcard $(PROJECT_DIR)/test/*.cc)
OBJECTS = $(SOURCES:.cc=.o)
DEPENDS = $(OBJECTS:.o=.d)

CCompiler = gcc
CCompilerFLAGS = -c -g -fPIC -O2 -Wall -Wextra -pedantic -Wno-long-long -Wshadow

MNIST_INCLUDES = -I $(PROJECT_DIR)/mnist/include/

MNIST_SOURCES +=  $(wildcard $(PROJECT_DIR)/mnist/src/ihead/*.c)
MNIST_SOURCES +=  $(wildcard $(PROJECT_DIR)/mnist/src/image/*.c)
MNIST_SOURCES +=  $(wildcard $(PROJECT_DIR)/mnist/src/mis/*.c)
MNIST_SOURCES +=  $(wildcard $(PROJECT_DIR)/mnist/src/util/*.c)

MNIST_OBJECTS = $(MNIST_SOURCES:.c=.o)
MNIST_DEPENDS = $(MNIST_OBJECTS:.o=.d)

MNIST_LIBRARY = $(PROJECT_LIBRARY_DIR)/libMNIST.so

all: library binary

library: $(MNIST_SOURCES) $(MNIST_OBJECTS)
	$(CCompiler) $(MNIST_SOURCES) $(MNIST_INCLUDES) -fPIC -shared -o $(MNIST_LIBRARY)

binary: $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBS) -L$(PROJECT_LIBRARY_DIR) -lMNIST -o $(PROJECT_BINARY)

-include $(MNIST_DEPENDS) $(DEPENDS)

%.o:%.c
	$(CCompiler) $(CCompilerFLAGS) $(MNIST_INCLUDES) $(DEFINES) -MP -MMD -MT $*.o -MT $*.d -MF $*.d -o $*.o $*.c

%.o:%.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -MP -MMD -MT $*.o -MT $*.d -MF $*.d -o $*.o $*.cc

clean:
	rm -f $(MNIST_OBJECTS) $(MNIST_DEPENDS)
	rm -f $(MNIST_LIBRARY)
	rm -f $(OBJECTS) $(DEPENDS)
	rm -f $(PROJECT_BINARY)
