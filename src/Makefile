MAIN_PROJ_NAME=Calculator_v2
MODEL_TEST_NAMES=calculator
MODEL_PATH=$(MAIN_PROJ_NAME)$(SEP)Model_module
CONTROLLER_PATH=$(MAIN_PROJ_NAME)$(SEP)Controller_module
VIEV_PATH=$(MAIN_PROJ_NAME)$(SEP)View_module
TEST_PATH=$(MAIN_PROJ_NAME)$(SEP)Test_module
TESTBUILD_FILES=$(TEST_PATH)$(SEP)$(MODEL_TEST_NAMES)_test.cc \
				$(MODEL_PATH)$(SEP)*$(MODEL_TEST_NAMES)*.h \
				$(MODEL_PATH)$(SEP)*$(MODEL_TEST_NAMES)*.cc
EXECUTABLE=$(MAIN_PROJ_NAME)_test.out
COMPILER=g++
STD=-std=c++17
TEST_FLAGS=-lgtest -pthread
CLANG_FILE=.clang-format
CLANG_FILE_WAY=..$(SEP)materials$(SEP)linters$(SEP)$(CLANG_FILE)
CHECK_FILES=$(MODEL_PATH)$(SEP)*.cc $(MODEL_PATH)$(SEP)*.h \
			$(CONTROLLER_PATH)$(SEP)*.cc $(CONTROLLER_PATH)$(SEP)*.h \
			$(VIEV_PATH)$(SEP)*.cc $(VIEV_PATH)$(SEP)*.h $(TEST_PATH)$(SEP)*.cc \
			$(TEST_PATH)$(SEP)*.h
CPPCH_SETUP=--enable=warning,performance,portability  -v --language=c++ -$(STD)
VALGRIND_SETUP=--tool=memcheck --leak-check=full --show-leak-kinds=all
BUILD_DIR=..$(SEP)build
APP=$(MAIN_PROJ_NAME).app
APP_PACKAGE=$(APP)$(SEP)Contents$(SEP)MacOS$(SEP)$(MAIN_PROJ_NAME)
BUILD_FILES_TO_DELETE=`ls | grep -v $(APP)`
ICON_RESOURCES=$(MAIN_PROJ_NAME)$(SEP)Resources
PLIST=$(MAIN_PROJ_NAME)$(SEP)Info.plist
ICON_RESOURCES_TARGET=$(BUILD_DIR)$(SEP)$(APP)$(SEP)Contents$(SEP)
DOC_DIR=..$(SEP)documentation
TO_DELETE_FILES=*.o *.out *.gch .DS_Store $(EXECUTABLE) $(CLANG_FILE)
TO_DELETE_FOLDERS=$(DOC_DIR) $(TEST_BUILD_DIR) *.dSYM

#Crossplatform specs
APP_NAME=$(MAIN_PROJ_NAME).exe
SEP='\'
MAKEDIR=md
DELETE_FOLDER=RD /S/Q
DELETE_FILE=DEL /F
COPY=copy
ARCHIVE=tar.exe -a -c -f
OPEN=start
OS=$(shell uname)

ifeq ($(OS), Linux)
	QSPEC=-spec linux-$(COMPILER)
	OPEN=xdg-open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp
	ARCHIVE=tar -cvf
	APP_NAME=$(MAIN_PROJ_NAME)
endif

ifeq ($(OS), Darwin)
	QSPEC=-spec macx-$(COMPILER)
	OPEN=open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp
	ARCHIVE=tar -cvf
	APP_NAME=$(MAIN_PROJ_NAME).app
endif

all: clean uninstall install tests

install:
	# $(MAKEDIR) $(BUILD_DIR)
	# qmake6 -o $(BUILD_DIR)$(SEP)Makefile $(MAIN_PROJ_NAME)$(SEP)$(MAIN_PROJ_NAME).pro && \
	# cd $(BUILD_DIR) && \
	# make && \
	$(OPEN) $(APP_NAME)

uninstall:
	$(DELETE_FOLDER) $(BUILD_DIR)

dvi: get_html
	$(OPEN) $(DOC_DIR)$(SEP)index.html

get_html:
	makeinfo --html -o $(DOC_DIR) dvi$(SEP)info.texi

dist:	
	$(ARCHIVE) $(MAIN_PROJ_NAME).tar $(MAIN_PROJ_NAME)

check:
	cppcheck $(CPPCH_SETUP) $(CHECK_FILES)
	$(COPY) $(CLANG_FILE_WAY) $(CLANG_FILE)
	clang-format -i --style=Google $(CHECK_FILES)
	clang-format -n --style=Google $(CHECK_FILES)

tests: clean
	$(COMPILER) -g $(STD) $(TESTBUILD_FILES) -o $(EXECUTABLE) $(TEST_FLAGS)
	./$(EXECUTABLE)

clean:
	$(DELETE_FOLDER) $(TO_DELETE_FOLDERS)
	$(DELETE_FILE) $(TO_DELETE_FILES)
