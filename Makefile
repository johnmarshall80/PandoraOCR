ifdef MONITORING
    DEFINES = -DMONITORING=1
endif

# C MNIST Library

C = gcc
CFLAGS = -c -g -fPIC -O2 -w
CFLAGS_LIB =-g -fPIC -O2 -w -shared
ifdef BUILD_32BIT_COMPATIBLE
    CFLAGS += -m32
	CFLAGS_LIB += -m32
endif

C_INCLUDE_DIR = $(PROJECT_DIR)/mnist/include/
C_INCLUDES = -I $(C_INCLUDE_DIR)

C_SOURCES +=  $(wildcard $(PROJECT_DIR)/mnist/src/ihead/*.c)
C_SOURCES +=  $(wildcard $(PROJECT_DIR)/mnist/src/image/*.c)
C_SOURCES +=  $(wildcard $(PROJECT_DIR)/mnist/src/mis/*.c)
C_SOURCES +=  $(wildcard $(PROJECT_DIR)/mnist/src/util/*.c)

C_OBJECTS = $(C_SOURCES:.c=.o)
C_DEPENDS = $(C_OBJECTS:.o=.d)

C_LIBRARY_DIR = $(PROJECT_DIR)/lib
C_LIBRARY = $(C_LIBRARY_DIR)/libMNIST.so

# C++ Application

CC = g++
CCFLAGS = -c -g -fPIC -O2 -Wall -Wextra -pedantic -Wno-long-long -Wshadow -Werror -std=c++11
ifdef BUILD_32BIT_COMPATIBLE
    CCFLAGS += -m32
endif

CCFLAGS_LIB += -L$(PANDORA_DIR)/lib -lPandoraSDK
ifdef MONITORING
    CCFLAGS_LIB += $(shell root-config --glibs --evelibs)
    CCFLAGS_LIB += -lPandoraMonitoring
endif
ifdef BUILD_32BIT_COMPATIBLE
    CCFLAGS_LIB += -m32
endif

CC_INCLUDES  = -I $(PROJECT_DIR)/include/
CC_INCLUDES += -I $(PROJECT_DIR)/mnist/include/
CC_INCLUDES += -I $(PANDORA_DIR)/PandoraSDK/include/
ifdef MONITORING
    CC_INCLUDES += -I $(shell root-config --incdir)
    CC_INCLUDES += -I $(PANDORA_DIR)/PandoraMonitoring/include/
endif

CC_SOURCES  =  $(wildcard $(PROJECT_DIR)/src/*.cc)
CC_SOURCES +=  $(wildcard $(PROJECT_DIR)/test/*.cc)

CC_OBJECTS = $(CC_SOURCES:.cc=.o)
CC_DEPENDS = $(CC_OBJECTS:.o=.d)

CC_BINARY = $(PROJECT_DIR)/bin/PandoraOCR

# Make commands

ifndef PROJECT_DIR
PROCEED = no
endif
ifndef PANDORA_DIR
PROCEED = no
endif

ifeq ($(PROCEED), no)
$(info Must specify PROJECT_DIR and PANDORA_DIR!)
all:
else
all: library binary
endif

library: $(C_SOURCES) $(C_OBJECTS)
	$(C) $(CFLAGS_LIB) $(C_SOURCES) $(C_INCLUDES) -g -fPIC -O2 -w -shared -o $(C_LIBRARY)

binary: $(CC_OBJECTS) 
	$(CC) $(CCFLAGS_LIB) $(CC_OBJECTS) -L$(C_LIBRARY_DIR) -lMNIST -o $(CC_BINARY)

-include $(C_DEPENDS) $(CC_DEPENDS)

%.o:%.c
	$(C) $(CFLAGS) $(C_INCLUDES) $(DEFINES) -MP -MMD -MT $*.o -MT $*.d -MF $*.d -o $*.o $*.c

%.o:%.cc
	$(CC) $(CCFLAGS) $(CC_INCLUDES) $(DEFINES) -MP -MMD -MT $*.o -MT $*.d -MF $*.d -o $*.o $*.cc

clean:
	rm -f $(C_OBJECTS) $(C_DEPENDS)
	rm -f $(C_LIBRARY)
	rm -f $(CC_OBJECTS) $(CC_DEPENDS)
	rm -f $(CC_BINARY)

install:
ifdef INCLUDE_TARGET
	rsync -r --exclude=.svn $(C_INCLUDE_DIR) ${INCLUDE_TARGET}
endif
ifdef LIB_TARGET
	cp $(C_LIBRARY) ${LIB_TARGET}
endif
