#
# Makefile de EXEMPLO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# É NECESSARIO ADAPTAR ESSE ARQUIVO de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "support.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
# 

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TST_DIR=./testes

all: clean octhread alib geraexec exec

#gera objeto da cthread.c e move para a pasta bin
octhread: $(SRC_DIR)/cthread.c
	$(CC) -c $^ -Wall
	mv cthread.o $(BIN_DIR)

#gera a libcthread.a e move para a pasta lib
alib: $(BIN_DIR)/cthread.o $(BIN_DIR)/support.o
	ar crs libcthread.a $^
	mv libcthread.a $(LIB_DIR)
	
#gera o executavel e move para pasta testes
geraexec: $(SRC_DIR)/main.c $(LIB_DIR)/libcthread.a
	$(CC) -o main $(SRC_DIR)/main.c -L$(LIB_DIR) -lcthread -Wall
	mv main $(TST_DIR)

#executa a main
exec:
	./$(TST_DIR)/main

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/cthread.o


