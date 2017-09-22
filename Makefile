.PHONY = help clean run all
.DEFAULT_GOAL := all

LIBS_DIR = ./libs

BASE_NAME = flexgl
FLEXGL_SRC = ./src/ui.c ./src/main.c
FLEXGL_FILES = ./src/ui.h ./src/ui.c ./src/main.c

BUILD_DIR = ./build

CFLAGS = -Wall -Werror -O3 -arch x86_64 -std=c11 -L$(LIBS_DIR) -lflex
LDFLAGS =

MACOS_MIN_TARGET = '10.10'

XCODE_DIR = /Applications/Xcode.app
XCODE_BIN_DIR = $(XCODE_DIR)/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
XCODE_CC = $(XCODE_BIN_DIR)/clang

MACOS_BUILD_DIR = $(BUILD_DIR)/macos
MACOS_FLEXGL_OUT = $(MACOS_BUILD_DIR)/flexgl.out
MACOS_CFLAGS = -mmacosx-version-min=$(MACOS_MIN_TARGET)

help:
	@echo ""
	@echo "> make help"
	@echo "  Display this help"
	@echo ""
	@echo "> make run"
	@echo "  Run the project"
	@echo ""
	@echo "> make clean"
	@echo "  Remove the $(BUILD_DIR) directory"
	@echo ""
	@echo "> make all"
	@echo "  Build the project to $(BUILD_DIR)"
	@echo ""

$(MACOS_FLEXGL_OUT): $(FLEX_FILES) $(FLEXGL_FILES)
	/bin/mkdir -p $(MACOS_BUILD_DIR)
	$(XCODE_CC) $(CFLAGS) $(MACOS_CFLAGS) \
		$(FLEXGL_SRC) \
		-lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework Carbon -lGLEW \
		-o $(MACOS_FLEXGL_OUT)

run: $(MACOS_FLEXGL_OUT)
	$(MACOS_FLEXGL_OUT)

all: $(MACOS_FLEXGL_OUT)

clean:
	/bin/rm -rf $(BUILD_DIR)
