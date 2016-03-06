# Common arguments called with all compilation statements
COMMON_ARGUMENTS          := -Wall -Werror -g -O3
COMMON_FILES              := stewart_nate_dissipation.h stewart_nate_structures.h
SERIAL_FILE               := stewart_nate_dissipation.c
PTHREAD_DISPOSABLE_FILE   := stewart_nate_disposable.c
PTHREAD_DISPOSABLE_OUTPUT := disposable
PTHREAD_PERSISTENT_FILE   := stewart_nate_persistent.c
PTHREAD_PERSISTENT_OUTPUT := persistent
BINARIES := $(PTHREAD_DISPOSABLE_OUTPUT) $(PTHREAD_PERSISTENT_OUTPUT)
all: serial 

# Serial execution. Lab1
serial: $(COMMON_FILES) $(SERIAL_FILE) 
	gcc $(COMMON_ARGUMENTS) $(SERIAL_FILE) 
# Pthread disposable threads. Lab2a
pd: $(COMMON_FILES) $(PTHREAD_DISPOSABLE_FILE)
	gcc $(COMMON_ARGUMENTS) $(PTHREAD_DISPOSABLE_FILE) -o $(PTHREAD_DISPOSABLE_OUTPUT)
# Pthread persistent threads. Lab2b
pp: $(COMMON_FILES) $(PTHREAD_PERSISTENT_FILE)
	gcc $(COMMON_ARGUMENTS) $(PTHREAD_PERSISTENT_FILE) -o $(PTHREAD_PERSISTENT_OUTPUT)
clean:
	rm -f $(BINARIES) *.out *.o  
