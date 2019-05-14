all: servidor_ncurses principal cliente

servidor_ncurses: servidor_ncurses.c comun.c definiciones.h comun.h
	cc servidor_ncurses.c comun.c -o servidor_ncurses -lncurses
principal: principal.c comun.c comun.h
	cc principal.c comun.c -o principal
cliente: comun.c comun.h
	cc cliente.c comun.c -o cliente
