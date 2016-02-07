all: amr_dissipation

amr_dissipation: a.out

a.out: dissipation.c dissipation.h
	gcc -g dissipation.c -o amr

clean:
	rm amr 
