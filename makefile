all: opt0

opt0: dissipation.c dissipation.h structures.h
	gcc dissipation.c
opt1: dissipation.c dissipation.h structures.h
	gcc -O1 dissipation.c
opt2: dissipation.c dissipation.h structures.h
	gcc -O2 dissipation.c
opt3: dissipation.c dissipation.h structures.h
	gcc -O3 dissipation.c
debug: dissipation.c dissipation.h structures.h
	gcc -g dissipation.c

clean:
	rm *.out 
