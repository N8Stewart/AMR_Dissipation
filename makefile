# Common arguments called with all compilation statements
COMMON_ARGUMENTS := -Wall -Werror -g -O3
COMMON_FILES := stewart_nate_dissipation.c stewart_nate_dissipation.h stewart_nate_structures.h
SERIAL_FILE := stewart_nate_dissipation.c
all: serial 

serial: $(COMMON_FILES) 
	gcc $(COMMON_ARGUMENTS) $(SERIAL_FILE) 
clean:
	rm *.out 
