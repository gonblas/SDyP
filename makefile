# Directories
DIR = Practicas/Practica4
UTILS_DIR = utils

# Source files
SOURCES = $(UTILS_DIR)/utils.c $(DIR)/ej4.c

# Default arguments for the 'run' target (single execution)
ARGS = 512

# List of arguments for the 'loop' target (ensure proper quoting for spaces)
ARGS_LIST = "512" "1024" "2048" "4096"

# Executable file
EXE = results

# --- Compiler Configuration ---
# Default compiler is gcc
# Set CC = mpicc here OR override on command line: make CC=mpicc
CC = mpicc

# --- MPI Configuration ---
# Default number of processes for the 'run' target (single execution)
# Override with: make run CC=mpicc NP=<number>
NP = 4
# List of process counts for the 'loop' target when using MPI
# Override with: make loop CC=mpicc NP_LIST="2 8 16"
NP_LIST = 2 4 8

# --- Compilation and Linking Flags ---
CFLAGS = -Wall -Werror -std=c11
LDFLAGS =
LIBS = -lm

# --- Object Files ---
OBJECTS = $(SOURCES:%.c=%.o)

# --- Main Rules ---
all: $(EXE) run clean

# Rule to generate the executable
$(EXE): $(OBJECTS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(EXE) $(LIBS)

# Rule to compile .c files to .o files
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# --- Execution Rules ---

# Rule to run the program ONCE with default ARGS
# Uses NP for MPI process count if CC=mpicc
run: $(EXE)
ifeq ($(CC), mpicc)
	@mpirun --oversubscribe -np $(NP) ./$(EXE) $(ARGS)
else
	@./$(EXE) $(ARGS)
endif

# Rule to loop through NP_LIST (if MPI) and ARGS_LIST
# Executes all combinations.
loop: $(EXE)
ifeq ($(CC), mpicc)
	@for num_procs in $(NP_LIST); do \
	  for arg_set in $(ARGS_LIST); do \
			echo -e "\n################## Running with arguments $$arg_set and $$num_procs processes ##################\n"; \
	    mpirun --oversubscribe -np $$num_procs ./$(EXE) $$arg_set; \
	  done; \
	done
else # CC is not mpicc (standard execution)
	# Single loop: Iterate only through ARGS_LIST
	@for arg_set in $(ARGS_LIST); do \
		echo -e "\n################## Running with arguments $$args ##################\n"; \
	  ./$(EXE) $$arg_set; \
	done
endif
	@$(MAKE) -s clean # Call clean silently after loop finishes


# --- Cleanup Rule ---
clean:
	@-rm -f $(EXE) $(OBJECTS) core.* *.o

# Phony targets prevent conflicts with files of the same name
.PHONY: all run clean loop