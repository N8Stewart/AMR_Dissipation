# Common arguments called with all compilation statements
COMMON_ARGUMENTS          := -Wall -Werror #-g #-pg
OPTIMIZATION_LEVEL        := -O3
THREAD_ARGUMENTS          := -lpthread
OPENMP_ARGUMENTS          := -fopenmp
COMMON_FILES              := stewart_nate_dissipation.h stewart_nate_structures.h
SERIAL_FILE               := stewart_nate_serial.c
SERIAL_OUTPUT             := serial
PTHREAD_DISPOSABLE_FILE   := stewart_nate_disposable.c
PTHREAD_DISPOSABLE_OUTPUT := disposable
PTHREAD_PERSISTENT_FILE   := stewart_nate_persistent.c
PTHREAD_PERSISTENT_OUTPUT := persistent
OPENMP_DISPOSABLE_FILE    := stewart_nate_omp_disposable.c
OPENMP_DISPOSABLE_OUTPUT  := ompDisposable
BINARIES := $(PTHREAD_DISPOSABLE_OUTPUT) $(PTHREAD_PERSISTENT_OUTPUT) $(SERIAL_OUTPUT) $(OPENMP_DISPOSABLE_OUTPUT)

all: serial pd pp op
# Serial execution. Lab1
serial: $(COMMON_FILES) $(SERIAL_FILE) 
	gcc $(COMMON_ARGUMENTS) $(OPTIMIZATION_LEVEL) $(SERIAL_FILE) -o $(SERIAL_OUTPUT)
# Pthread disposable threads. Lab2a
pd: $(COMMON_FILES) $(PTHREAD_DISPOSABLE_FILE)
	gcc $(COMMON_ARGUMENTS) $(OPTIMIZATION_LEVEL) $(THREAD_ARGUMENTS) $(PTHREAD_DISPOSABLE_FILE) -o $(PTHREAD_DISPOSABLE_OUTPUT)
# Pthread persistent threads. Lab2b
pp: $(COMMON_FILES) $(PTHREAD_PERSISTENT_FILE)
	gcc $(COMMON_ARGUMENTS) $(OPTIMIZATION_LEVEL) $(THREAD_ARGUMENTS) $(PTHREAD_PERSISTENT_FILE) -o $(PTHREAD_PERSISTENT_OUTPUT)
# OpenMP disposable threads. lab3
op: $(COMMON_FILES) $(OPENMP_DISPOSABLE_FILE)
	gcc $(COMMON_ARGUMENTS) $(OPTIMIZATION_LEVEL) $(OPENMP_ARGUMENTS) $(OPENMP_DISPOSABLE_FILE) -o $(OPENMP_DISPOSABLE_OUTPUT)
# OpenMP persistent threads. Lab3

clean:
	rm -f $(BINARIES) *.out *.o  
