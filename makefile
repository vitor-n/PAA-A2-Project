.DEFAULT_GOAL := help

SRC_DIR = src
STRUCT_DIR = src/structures
IDIR = include

CXX = g++
CXXFLAGS = -Wall -I $(IDIR)

SRC = $(wildcard $(SRC_DIR)/*.cpp $(STRUCT_DIR)/*.cpp $(SRC_DIR)/trafficAPI/*.cpp)

ARGS = gen data/city-1 -p
MODE = -p

.PHONY: build help run clean

all: build run

build: $(SRC) ## Build the project
	$(CXX) $(CXXFLAGS) -o out $(SRC)

help:
	@./scripts/help.sh $(MAKEFILE_LIST)

graphgen: ## Generates the city graph WIP
	python extras/graphgen/graphgen.py $(ARGS)

graphread: ## Reads the city graph WIP
	python extras/graphgen/graphgen.py read data/city-1 -p

run: ## Run the project
	./out

test:
	$(CXX) $(CXXFLAGS) -o test $(wildcard ./*.cpp)
	./test

clean: ## Clean created files
	rm -f $(wildcard ./*.exe) $(wildcard ./*out*)
