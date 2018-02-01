#-----------------------
# IMPORTANTE: Los valores de los parametros de los ejercicio?_test deben ajustarse.
# Se asume que los ficheros fuente se llaman ejercicio1.c, ejercicio2.c,...,ordenar.h
#-----------------------

CC = gcc -std=gnu99 -pedantic -ggdb -D_XOPEN_SOURCE=700 -D _BSD_SOURCE -pthread
CFLAGS = -Wall
EXE = m_background theindiependencegame

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o background.o object.o places.o player.o rectangles.o stats.o interact.o interface.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o background.o object.o places.o player.o rectangles.o stats.o interact.o interface.o

background.o : background.c background.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

player.o : player.c player.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

object.o : object.c object.h object.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

places.o : places.c places.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

rectangles.o : rectangles.c rectangles.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

stats.o : stats.c stats.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

interact.o : interact.c interact.h player.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

interface.o : interface.c interface.h interact.h player.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

theindiependencegame.o : theindiependencegame.c interact.h interface.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

m_background.o : m_background.c background.h
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<
