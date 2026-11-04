/*
 * excecao.c — a travessia involuntária: uma exceção de hardware.
 *
 * Prática da Aula 03 — Parte A2
 * Sistemas Operacionais · TADS/IFRN · 2026.1
 *
 * Compilar:  gcc -Wall -Wextra -O0 -o excecao excecao.c
 * Executar:  ./excecao ; echo "codigo de saida: $?"
 *            strace ./excecao
 *
 * Este programa morre de propósito. Não é erro do roteiro nem da sua máquina.
 *
 * Diferença para o chamada.c: lá o processo PEDE um serviço ao núcleo. Aqui
 * ele não pede nada — escrever num ponteiro é uma instrução comum de modo
 * usuário. O que acontece é que o endereço não é válido para este processo, o
 * processador detecta a violação enquanto executa a instrução e transfere o
 * controle para o tratamento de exceções do sistema operacional.
 *
 * O -O0 importa, mas não pelo motivo que se costuma dizer. Medido nesta imagem
 * (GCC 13.3.0, aarch64): com -O2 o programa continua morrendo com SIGSEGV e
 * saída 139 — o que muda é que o gcc passa a EMITIR O AVISO
 * "null pointer dereference [-Wnull-dereference]", e o silêncio do compilador
 * é justamente a descoberta do Passo A2. Compile com -O0.
 */
#include <stdio.h>

int main(void)
{
	int *p = NULL;

	/* O fflush é necessário: sem ele, com a saída redirecionada para arquivo,
	 * esta linha fica parada no buffer da biblioteca C e morre com o processo. */
	printf("Antes do acesso\n");
	fflush(stdout);

	*p = 42;    /* acesso inválido — a partir daqui quem decide é a CPU */

	printf("Depois do acesso\n");
	fflush(stdout);

	return 0;
}
