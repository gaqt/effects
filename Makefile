
CFLAGS := --std=c++20 -Wall -Wextra -Wpedantic -rdynamic
DEBUGFLAGS := -DDEBUG -ftrapv -fstrict-overflow -Og -ggdb
RELEASEFLAGS := -DNDEBUG -O3 -mtune=native
SHAREDLIBS := -lm -lX11 -lpthread -lrt
STATICLIBS := dist/lib/libraylib.a
INCLUDE := -I./lib/raylib/src -I./lib/raygui

LIBS := $(STATICLIBS) $(SHAREDLIBS)

.PHONY: build buildDebug run runDebug buildRun buildRunDebug setup clean

build:
	@echo	"Make: Building project"
	@mkdir -p dist/release
	@g++ $(CFLAGS) $(RELEASEFLAGS) $(INCLUDE) src/main.cpp $(LIBS) -o dist/release/out

buildDebug:
	@echo	"Make: Building project in debug mode"
	@mkdir -p dist/debug
	@g++ $(CFLAGS) $(DEBUGFLAGS) $(INCLUDE) src/main.cpp $(LIBS) -o dist/debug/out

run:
	@echo "Make: Running project"
	@./dist/release/out

runDebug:
	@echo "Make: Running project in debug mode"
	@./dist/debug/out

buildRun: build run

buildRunDebug: buildDebug runDebug

setup:
	@echo "Make: Setting up project"
	@./setup.sh

clean:
	@echo "Make: Cleaning project"
	@rm -r dist
