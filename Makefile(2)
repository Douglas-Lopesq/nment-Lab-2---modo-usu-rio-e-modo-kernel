# Makefile do módulo ola — Prática da Aula 03 (TADS/IFRN 2026.1)
#
# ATENÇÃO: a indentação das receitas TEM que ser TAB, nunca espaços.
#
# Você não compila um módulo com "gcc ola.c". Ele precisa dos cabeçalhos e das
# flags exatas de UM kernel específico, então quem manda é o Makefile do
# próprio kernel. Este arquivo só diz: "o alvo é ola.o, e o código está aqui".

obj-m += ola.o

# Qual árvore de cabeçalhos usar.
#
#   1. O ideal é a do kernel em execução: /lib/modules/$(uname -r)/build
#   2. No container da disciplina isso NÃO existe: o kernel em execução é o da
#      VM do Docker (…-linuxkit) e ninguém publica cabeçalhos para ele. Então
#      caímos na primeira árvore instalada pelo linux-headers-generic.
#
# É por isso que o módulo compilado no container não carrega nele: o vermagic
# vai apontar para outro kernel. Isso é o experimento 5 do LEIAME.md, não um bug.
KVER  ?= $(shell uname -r)
KDIR  ?= $(firstword $(wildcard /lib/modules/$(KVER)/build) \
                     $(wildcard /lib/modules/*/build))

all:
	@test -n "$(KDIR)" || { \
	  echo "ERRO: nenhuma árvore de cabeçalhos encontrada em /lib/modules/*/build."; \
	  echo "      Rode:  apt-get update && apt-get install -y linux-headers-generic"; \
	  exit 1; }
	@echo "==> kernel em execução : $(KVER)"
	@echo "==> compilando contra  : $(KDIR)"
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	@test -n "$(KDIR)" && $(MAKE) -C $(KDIR) M=$(PWD) clean || rm -f *.o *.ko *.mod* Module.symvers modules.order

# Mostra o descasamento entre o kernel em execução e o do módulo.
info: all
	@echo
	@echo "kernel em execução : $(KVER)"
	@echo -n "vermagic do módulo : "; modinfo -F vermagic ola.ko

.PHONY: all clean info
