/*
 * privilegio.c — uma instrucao privilegiada dentro de um programa comum.
 *
 * Pratica da Aula 03 — Parte A3
 * Sistemas Operacionais · TADS/IFRN · 2026.1
 *
 * Compilar:  gcc -Wall -Wextra -o privilegio privilegio.c
 * Executar:  ./privilegio
 *            echo "codigo de saida: $?"
 *
 * O fflush() da linha "antes" nao e decoracao. Num terminal a libc usa buffer
 * de linha e o \n ja descarrega a mensagem; com a saida redirecionada para
 * arquivo ela usa buffer cheio de 4 KiB, e sem o fflush os 58 bytes morrem
 * dentro do processo junto com ele. Compare:
 *
 *     ./privilegio                 -> a linha aparece
 *     ./privilegio > saida.txt     -> 58 bytes no arquivo
 *     (a mesma coisa sem o fflush) -> 0 bytes no arquivo
 */
#include <stdio.h>

#if defined(__aarch64__)
   /* SCTLR_EL1 e o registrador de controle do sistema: liga e desliga a MMU,
    * os caches e a checagem de alinhamento. O sufixo _EL1 diz o nivel de
    * excecao minimo para toca-lo; um processo de usuario roda em EL0.       */
#  define ALVO "ler SCTLR_EL1"
#elif defined(__x86_64__) || defined(__i386__)
   /* cli desliga as interrupcoes mascaraveis. Instrucao de nivel 0.         */
#  define ALVO "executar cli"
#else
#  error "arquitetura nao prevista neste exercicio"
#endif

int main(void)
{
    unsigned long valor = 0;

    printf("antes:  processo em modo usuario, prestes a %s\n", ALVO);
    fflush(stdout);

#if defined(__aarch64__)
    __asm__ volatile("mrs %0, sctlr_el1" : "=r"(valor));
#else
    __asm__ volatile("cli");
#endif

    /* Esta linha nao e alcancada em nenhuma das duas arquiteturas. */
    printf("depois: a instrucao privilegiada passou, valor=0x%lx\n", valor);
    return 0;
}
