all: servidor_ncurses principal

servidor_ncurses: servidor_ncurses.c comun.c definiciones.h comun.h
	cc servidor_ncurses.c comun.c -o servidor_ncurses -lncurses
principal: principal.c comun.c comun.h
	cc principal.c comun.c -o principal
