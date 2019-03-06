#
# Les variables d'environnement libG3X, incG3X
# sont definies dans le fichier ~/.bashrc par le script ../install.sh
#
#compilateur
CC = gcc

#compil en mode 'debug' ou optmisée (-O2)
DBG = yes

ifeq ($(DBG),yes) #en mode debug
  CFLAGS = -g -Wpointer-arith -Wall -ansi -std=c11
else              #en mode normal
  CFLAGS = -O2 -ansi -std=c11 -g
endif

# assemblage des infos de lib. et inc.
lib =   $(libG3X)
# fichiers *.c locaux
src = src/
# fichiers *.h locaux et lib.
inc = -I./include $(incG3X) 

exec = g3x_example g3x_squelette massressort_grav

all : $(exec) clean

# règle générique de création de xxx.o à partir de src/xxx.c
%.o : $(src)%.c
	@echo "module $@"
	@$(CC) $(CFLAGS) $(inc) -c $< -o $@
	@echo "------------------------"

# règle générique de création de l'executable xxx à partir de src/xxx.c (1 seul module)
% : %.o
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(lib) -o $@
	@echo "------------------------"
	
.PHONY : clean cleanall ?

# informations sur les paramètres de compilation       
? :
	@echo "---------compilation informations----------"
	@echo "  processor      : $(PROCBIT)"
	@echo "  compiler       : $(CC)"
	@echo "  options        : $(CFLAGS)"
	@echo "  lib g2x/OpenGl : $(libG2X)$(COMP)"
	@echo "  headers        : $(incG2X)"
clean : 
	@rm -f *.o
cleanall :
	@rm -f *.o $(exec)
	
