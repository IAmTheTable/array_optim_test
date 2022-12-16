#File Directory things (might be overkill idk yet)
INCLUDE = -I$(SRC_DIR)/headers
SRC_DIR = src
OBJ_DIR = obj
LOG_DIR = logs

#Compiler and linker things
CC = g++
CCFLAGS = -g -Wall -Wextra -std=c++20 -fexceptions
#CCFLAGS += -Wno-write-strings # figure out how to make a make recipe for suppressed warnings rather than forcing it
LD = ld
LDFLAGS =

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# speed up the build process 


#Essential files and groups
OBJ = Neural-Network 
SRCS = $(call rwildcard, $(SRC_DIR), *.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(OBJ) 
	@mkdir -p $(@D)
	@mkdir -p $(LOG_DIR)
	@echo ---- Generating $^ ---

$(OBJ): $(OBJS)
	@echo ---- Linking $^ ----
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo ---- Compiling $^ ----
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/
	rm -rf $(LOG_DIR)/
