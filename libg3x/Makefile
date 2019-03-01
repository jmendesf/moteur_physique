CC = g++

ifeq ($(CC),g++)
  EXT = .$(PROCBIT)++
else
  EXT = .$(PROCBIT)
endif

ifeq ($(DEBUG),yes)
  CFLAGS  = -O2 -g -Wpointer-arith -Wall -ansi
  DBG_LIB = 
  EXT2 = $(EXT).dbg
else
  CFLAGS = -O2 -ansi
  DBG_LIB = 
  EXT2 = $(EXT)
endif

lib = -L/usr/lib -lm $(DBG_LIB) -lGL -lGLU -lglut
inc = -I/usr/include -I./include
src =  src/

# cible pour les binaires (.a,.so) 
bin = .

modules = g3x_window.o g3x_tools.o g3x_capture.o g3x_transfo.o g3x_GLtransfo.o g3x_quaternions.o g3x_pnm.o

%.o : $(src)%.c
	@echo "module $@"
	@$(CC) -fpic $(CFLAGS) $(inc) -c $< -o $@
	@echo "------------------------"

all : g3x clean

g3x :    $(modules)
	@echo "massemblage bibliotheque dynamique $(bin)/lib$@$(EXT2).so"
	@$(CC) -fpic -shared $^ $(lib) -o $(bin)/lib$@$(EXT2).so
#	@echo "massemblage bibliotheque d'archive $(bin)/lib$@$(EXT2).a"
#	@ar r $(bin)/lib$@$(EXT2).a $^
	@echo "------------------------                                "

% : %.o
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(lib) -o $@
	@echo "------------------------"

.PHONY : clean
clean : 
	rm -f *.o 
