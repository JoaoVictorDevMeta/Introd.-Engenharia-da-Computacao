# Nome do executável
TARGET = simulador

# Compilador e flags de compilação
CC = gcc
CFLAGS = -Wall -I../include

# Bibliotecas necessárias para a Raylib no Linux
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Busca automaticamente todos os arquivos .c na pasta src
SRCS = $(wildcard *.c)

# Regra padrão executada ao digitar apenas 'make'
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Regra para rodar o simulador
run: $(TARGET)
	./$(TARGET)

# Regra para limpar o executável (comando 'make clean')
clean:
	rm -f $(TARGET)
