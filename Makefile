#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -Wall `pkg-config gtk+-3.0 --cflags` -rdynamic  -g -std=c99 -Wall -Wextra  -Wstrict-prototypes -Wold-style-definition -Wno-unused-parameter -Wno-unused-result
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = `pkg-config gtk+-3.0 --libs` -export-dynamic

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/Oeil_De_Surimi

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/Oeil_De_Surimi

INC_TEST_NN_RELEASE = $(INC)
CFLAGS_TEST_NN_RELEASE = $(CFLAGS) -O2
RESINC_TEST_NN_RELEASE = $(RESINC)
RCFLAGS_TEST_NN_RELEASE = $(RCFLAGS)
LIBDIR_TEST_NN_RELEASE = $(LIBDIR)
LIB_TEST_NN_RELEASE = $(LIB)
LDFLAGS_TEST_NN_RELEASE = $(LDFLAGS) -s
OBJDIR_TEST_NN_RELEASE = obj/Test_NN_Release
DEP_TEST_NN_RELEASE = 
OUT_TEST_NN_RELEASE = bin/Test_NN_Release/Oeil_De_Surimi

INC_TEST_NN_DEBUG = $(INC)
CFLAGS_TEST_NN_DEBUG = $(CFLAGS) -g
RESINC_TEST_NN_DEBUG = $(RESINC)
RCFLAGS_TEST_NN_DEBUG = $(RCFLAGS)
LIBDIR_TEST_NN_DEBUG = $(LIBDIR)
LIB_TEST_NN_DEBUG = $(LIB)
LDFLAGS_TEST_NN_DEBUG = $(LDFLAGS)
OBJDIR_TEST_NN_DEBUG = obj/Test_NN_Debug
DEP_TEST_NN_DEBUG = 
OUT_TEST_NN_DEBUG = bin/Test_NN_Debug/Oeil_De_Surimi

INC_TEST_XOR_CONSOLE = $(INC)
CFLAGS_TEST_XOR_CONSOLE = $(CFLAGS) -g
RESINC_TEST_XOR_CONSOLE = $(RESINC)
RCFLAGS_TEST_XOR_CONSOLE = $(RCFLAGS)
LIBDIR_TEST_XOR_CONSOLE = $(LIBDIR)
LIB_TEST_XOR_CONSOLE = $(LIB)
LDFLAGS_TEST_XOR_CONSOLE = $(LDFLAGS)
OBJDIR_TEST_XOR_CONSOLE = obj/Test_XOR_Console
DEP_TEST_XOR_CONSOLE = 
OUT_TEST_XOR_CONSOLE = bin/Test_XOR_Console/Oeil_De_Surimi

OBJ_DEBUG = $(OBJDIR_DEBUG)/gui/image_operation.o $(OBJDIR_DEBUG)/gui/main.o $(OBJDIR_DEBUG)/image_treatment/img_treatment.o $(OBJDIR_DEBUG)/neural_network/neural_network.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/gui/image_operation.o $(OBJDIR_RELEASE)/gui/main.o $(OBJDIR_RELEASE)/image_treatment/img_treatment.o $(OBJDIR_RELEASE)/neural_network/neural_network.o

OBJ_TEST_NN_RELEASE = $(OBJDIR_TEST_NN_RELEASE)/neural_network/main_test.o $(OBJDIR_TEST_NN_RELEASE)/neural_network/neural_network.o

OBJ_TEST_NN_DEBUG = $(OBJDIR_TEST_NN_DEBUG)/neural_network/main_test.o $(OBJDIR_TEST_NN_DEBUG)/neural_network/neural_network.o

OBJ_TEST_XOR_CONSOLE = $(OBJDIR_TEST_XOR_CONSOLE)/neural_network/main_xor.o $(OBJDIR_TEST_XOR_CONSOLE)/neural_network/neural_network.o

all: debug release test_nn_release test_nn_debug test_xor_console

clean: clean_debug clean_release clean_test_nn_release clean_test_nn_debug clean_test_xor_console

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/gui || mkdir -p $(OBJDIR_DEBUG)/gui
	test -d $(OBJDIR_DEBUG)/image_treatment || mkdir -p $(OBJDIR_DEBUG)/image_treatment
	test -d $(OBJDIR_DEBUG)/neural_network || mkdir -p $(OBJDIR_DEBUG)/neural_network

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/gui/image_operation.o: gui/image_operation.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c gui/image_operation.c -o $(OBJDIR_DEBUG)/gui/image_operation.o

$(OBJDIR_DEBUG)/gui/main.o: gui/main.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c gui/main.c -o $(OBJDIR_DEBUG)/gui/main.o

$(OBJDIR_DEBUG)/image_treatment/img_treatment.o: image_treatment/img_treatment.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_treatment/img_treatment.c -o $(OBJDIR_DEBUG)/image_treatment/img_treatment.o

$(OBJDIR_DEBUG)/neural_network/neural_network.o: neural_network/neural_network.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c neural_network/neural_network.c -o $(OBJDIR_DEBUG)/neural_network/neural_network.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/gui
	rm -rf $(OBJDIR_DEBUG)/image_treatment
	rm -rf $(OBJDIR_DEBUG)/neural_network

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE)/gui || mkdir -p $(OBJDIR_RELEASE)/gui
	test -d $(OBJDIR_RELEASE)/image_treatment || mkdir -p $(OBJDIR_RELEASE)/image_treatment
	test -d $(OBJDIR_RELEASE)/neural_network || mkdir -p $(OBJDIR_RELEASE)/neural_network

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/gui/image_operation.o: gui/image_operation.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c gui/image_operation.c -o $(OBJDIR_RELEASE)/gui/image_operation.o

$(OBJDIR_RELEASE)/gui/main.o: gui/main.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c gui/main.c -o $(OBJDIR_RELEASE)/gui/main.o

$(OBJDIR_RELEASE)/image_treatment/img_treatment.o: image_treatment/img_treatment.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_treatment/img_treatment.c -o $(OBJDIR_RELEASE)/image_treatment/img_treatment.o

$(OBJDIR_RELEASE)/neural_network/neural_network.o: neural_network/neural_network.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c neural_network/neural_network.c -o $(OBJDIR_RELEASE)/neural_network/neural_network.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)/gui
	rm -rf $(OBJDIR_RELEASE)/image_treatment
	rm -rf $(OBJDIR_RELEASE)/neural_network

before_test_nn_release: 
	test -d bin/Test_NN_Release || mkdir -p bin/Test_NN_Release
	test -d $(OBJDIR_TEST_NN_RELEASE)/neural_network || mkdir -p $(OBJDIR_TEST_NN_RELEASE)/neural_network

after_test_nn_release: 

test_nn_release: before_test_nn_release out_test_nn_release after_test_nn_release

out_test_nn_release: before_test_nn_release $(OBJ_TEST_NN_RELEASE) $(DEP_TEST_NN_RELEASE)
	$(LD) $(LIBDIR_TEST_NN_RELEASE) -o $(OUT_TEST_NN_RELEASE) $(OBJ_TEST_NN_RELEASE)  $(LDFLAGS_TEST_NN_RELEASE) $(LIB_TEST_NN_RELEASE)

$(OBJDIR_TEST_NN_RELEASE)/neural_network/main_test.o: neural_network/main_test.c
	$(CC) $(CFLAGS_TEST_NN_RELEASE) $(INC_TEST_NN_RELEASE) -c neural_network/main_test.c -o $(OBJDIR_TEST_NN_RELEASE)/neural_network/main_test.o

$(OBJDIR_TEST_NN_RELEASE)/neural_network/neural_network.o: neural_network/neural_network.c
	$(CC) $(CFLAGS_TEST_NN_RELEASE) $(INC_TEST_NN_RELEASE) -c neural_network/neural_network.c -o $(OBJDIR_TEST_NN_RELEASE)/neural_network/neural_network.o

clean_test_nn_release: 
	rm -f $(OBJ_TEST_NN_RELEASE) $(OUT_TEST_NN_RELEASE)
	rm -rf bin/Test_NN_Release
	rm -rf $(OBJDIR_TEST_NN_RELEASE)/neural_network

before_test_nn_debug: 
	test -d bin/Test_NN_Debug || mkdir -p bin/Test_NN_Debug
	test -d $(OBJDIR_TEST_NN_DEBUG)/neural_network || mkdir -p $(OBJDIR_TEST_NN_DEBUG)/neural_network

after_test_nn_debug: 

test_nn_debug: before_test_nn_debug out_test_nn_debug after_test_nn_debug

out_test_nn_debug: before_test_nn_debug $(OBJ_TEST_NN_DEBUG) $(DEP_TEST_NN_DEBUG)
	$(LD) $(LIBDIR_TEST_NN_DEBUG) -o $(OUT_TEST_NN_DEBUG) $(OBJ_TEST_NN_DEBUG)  $(LDFLAGS_TEST_NN_DEBUG) $(LIB_TEST_NN_DEBUG)

$(OBJDIR_TEST_NN_DEBUG)/neural_network/main_test.o: neural_network/main_test.c
	$(CC) $(CFLAGS_TEST_NN_DEBUG) $(INC_TEST_NN_DEBUG) -c neural_network/main_test.c -o $(OBJDIR_TEST_NN_DEBUG)/neural_network/main_test.o

$(OBJDIR_TEST_NN_DEBUG)/neural_network/neural_network.o: neural_network/neural_network.c
	$(CC) $(CFLAGS_TEST_NN_DEBUG) $(INC_TEST_NN_DEBUG) -c neural_network/neural_network.c -o $(OBJDIR_TEST_NN_DEBUG)/neural_network/neural_network.o

clean_test_nn_debug: 
	rm -f $(OBJ_TEST_NN_DEBUG) $(OUT_TEST_NN_DEBUG)
	rm -rf bin/Test_NN_Debug
	rm -rf $(OBJDIR_TEST_NN_DEBUG)/neural_network

before_test_xor_console: 
	test -d bin/Test_XOR_Console || mkdir -p bin/Test_XOR_Console
	test -d $(OBJDIR_TEST_XOR_CONSOLE)/neural_network || mkdir -p $(OBJDIR_TEST_XOR_CONSOLE)/neural_network

after_test_xor_console: 

test_xor_console: before_test_xor_console out_test_xor_console after_test_xor_console

out_test_xor_console: before_test_xor_console $(OBJ_TEST_XOR_CONSOLE) $(DEP_TEST_XOR_CONSOLE)
	$(LD) $(LIBDIR_TEST_XOR_CONSOLE) -o $(OUT_TEST_XOR_CONSOLE) $(OBJ_TEST_XOR_CONSOLE)  $(LDFLAGS_TEST_XOR_CONSOLE) $(LIB_TEST_XOR_CONSOLE)

$(OBJDIR_TEST_XOR_CONSOLE)/neural_network/main_xor.o: neural_network/main_xor.c
	$(CC) $(CFLAGS_TEST_XOR_CONSOLE) $(INC_TEST_XOR_CONSOLE) -c neural_network/main_xor.c -o $(OBJDIR_TEST_XOR_CONSOLE)/neural_network/main_xor.o

$(OBJDIR_TEST_XOR_CONSOLE)/neural_network/neural_network.o: neural_network/neural_network.c
	$(CC) $(CFLAGS_TEST_XOR_CONSOLE) $(INC_TEST_XOR_CONSOLE) -c neural_network/neural_network.c -o $(OBJDIR_TEST_XOR_CONSOLE)/neural_network/neural_network.o

clean_test_xor_console: 
	rm -f $(OBJ_TEST_XOR_CONSOLE) $(OUT_TEST_XOR_CONSOLE)
	rm -rf bin/Test_XOR_Console
	rm -rf $(OBJDIR_TEST_XOR_CONSOLE)/neural_network

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release before_test_nn_release after_test_nn_release clean_test_nn_release before_test_nn_debug after_test_nn_debug clean_test_nn_debug before_test_xor_console after_test_xor_console clean_test_xor_console

