.DEFAULT_GOAL := help

SRC_DIR = src
STRUCT_DIR = src/structures
TRAFFIC_DIR = src/trafficAPI
DRIVER_DIR = src/drivers
IDIR = include

CXX = g++
CXXFLAGS = -Wall -I $(IDIR)

SRC = $(wildcard $(SRC_DIR)/*.cpp $(STRUCT_DIR)/*.cpp $(TRAFFIC_DIR)/*.cpp)
TAREFA1 = $(wildcard $(SRC_DIR)/*.cpp $(STRUCT_DIR)/*.cpp $(TRAFFIC_DIR)/*.cpp $(DRIVER_DIR)/tarefa1.cpp)
TAREFA2 = $(wildcard $(SRC_DIR)/*.cpp $(STRUCT_DIR)/*.cpp $(TRAFFIC_DIR)/*.cpp $(DRIVER_DIR)/tarefa2.cpp)

ARGS = gen data/city-1 -p
MODE = -p

.PHONY: build help run clean

all: build run

build: $(SRC) ## Build the project
	$(CXX) $(CXXFLAGS) -o out $(SRC)

tarefa1: $(TAREFA1)
	  $(CXX) $(CXXFLAGS) -o tarefa1 $(TAREFA1) && ./tarefa1

tarefa2: $(TAREFA2)
	  $(CXX) $(CXXFLAGS) -o tarefa2 $(TAREFA2) && ./tarefa2

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
