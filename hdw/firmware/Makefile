CC = sdcc
PARAMETERS = -V --use-non-free -mpic14 -p16f628a

firmware:
	${CC} ${PARAMETERS} code.c

clean:
	rm code.asm code.cod code.o code.lst code.hex
