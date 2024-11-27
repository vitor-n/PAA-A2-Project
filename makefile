.DEFAULT_GOAL := help

SRC_DIR = src
IDIR = include

CXX = g++
CXXFLAGS = -Wall -I $(IDIR)

SRC = $(wildcard $(SRC_DIR)/*.cpp)

ARGS = gen data/city-1 -p

.PHONY: build help run clean

all: build run

build: $(SRC) ## Build the project
	$(CXX) $(CXXFLAGS) -o out $(SRC)

help:
	@./scripts/help.sh $(MAKEFILE_LIST)

graphgen: ## Generates the city graph WIP
	python extras/graphgen/graphgen.py $(ARGS)

run: ## Run the project
	./out

test:
	$(CXX) $(CXXFLAGS) -o test $(wildcard ./*.cpp)
	./test

clean: ## Clean created files
	rm -f out $(wildcard ./*.exe) 