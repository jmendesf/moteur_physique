CC = gcc

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

modules = g2x_window.o g2x_tools.o g2x_capture.o g2x_draw.o g2x_geoalgo.o g2x_transfo.o g2x_polygon.o g2x_pnm.o

%.o : $(src)%.c
	@echo "module $@"
	@$(CC) -fpic $(CFLAGS) $(inc) -c $< -o $@
	@echo "------------------------"

all : g2x clean

g2x : $(modules)
	@echo "assemblage bibliotheque dynamique $(bin)/lib$@$(EXT2).so"
	@$(CC) -fpic -shared $^ $(lib) -o $(bin)/lib$@$(EXT2).so
	$(VERS)
#	@ln -fs lib$@$(EXT2).so$(VERS) $(bin)/lib$@$(EXT2).so
#	@echo "assemblage bibliotheque d'archive $(bin)/lib$@$(EXT2).a"
#	@ar r $(bin)/lib$@$(EXT2).a $^
	@echo "------------------------                                "

% : %.o
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(lib) -o $@
	@echo "------------------------"

.PHONY : clean
clean : 
	rm -f *.o */*~
