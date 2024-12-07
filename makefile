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
TAREFA3 = $(wildcard $(SRC_DIR)/*.cpp $(STRUCT_DIR)/*.cpp $(TRAFFIC_DIR)/*.cpp $(DRIVER_DIR)/tarefa3.cpp)

ARGS = gen data/city-1 -p
MODE = -p

.PHONY: build help run clean

all: tarefa1 tarefa2 tarefa3
tarefa1: buildtarefa1 runtarefa1 ## Build and run tarefa1
tarefa2: buildtarefa2 runtarefa2 ## Build and run tarefa2
tarefa3: buildtarefa3 runtarefa3 ## Build and run tarefa3

build: $(SRC)
	$(CXX) $(CXXFLAGS) -o out $(SRC)

buildtarefa1: $(TAREFA1)
	  $(CXX) $(CXXFLAGS) -o tarefa1 $(TAREFA1)
runtarefa1:	  
	./tarefa1

buildtarefa2: $(TAREFA2)
	  $(CXX) $(CXXFLAGS) -o tarefa2 $(TAREFA2)
runtarefa2:
	./tarefa2

buildtarefa3: $(TAREFA3)
	  $(CXX) $(CXXFLAGS) -o tarefa3 $(TAREFA3)
runtarefa3:
	./tarefa3

help: ## Show this help
	@./scripts/help.sh $(MAKEFILE_LIST)

graphgen: ## Generates the city graph
	python extras/graphgen/graphgen.py $(ARGS)

graphread: ## Reads the city graph
	python extras/graphgen/graphgen.py read data/city-1 -p

test:
	$(CXX) $(CXXFLAGS) -o test $(wildcard ./*.cpp)
	./test

clean: ## Clean created files
	rm -f $(wildcard ./*.exe) $(wildcard ./*out*) $(wildcard ./tarefa*)
