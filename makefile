# Define o compilador C
CC := gcc

# Nome do arquivo final
BINARY := musical

# Estrutura de diretórios do código
BINDIR := .
BUILDDIR := build
INCDIR := include
SRCDIR := src

# Flags do compilador
CFLAGS := -Wall -I $(INCDIR)

# Flags do linker
LDFLAGS := 

# Nomes dos arquivos .c e .o
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

# Regra para compilar e gerar o arquivo binário final
all: $(BINDIR)/$(BINARY)

$(BINDIR)/$(BINARY): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

# Regra para compilar os arquivos de objeto
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa os diretórios BIN e BUILD
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR)/$(BINARY)
