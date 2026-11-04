/*
 * ola.c — um módulo de kernel Linux, o menor possível que ainda prova algo.
 *
 * Prática da Aula 03 — Estrutura de um Sistema Operacional
 * Sistemas Operacionais · TADS/IFRN · 2026.1
 *
 * Este arquivo NÃO é um programa: não tem main(), não vira executável e não
 * roda em modo usuário. É código que o kernel liga a si mesmo em tempo de
 * execução — ou seja, código seu executando com privilégio de núcleo.
 * (Esse nível se chama "ring 0" no x86 e "EL1" no ARM; o roteiro usa o nome
 * neutro, modo núcleo.)
 *
 * Compilação: veja o Makefile ao lado e o LEIAME.md.
 */

#include <linux/module.h>   /* MODULE_*, module_init/module_exit */
#include <linux/kernel.h>   /* printk, KERN_INFO                 */
#include <linux/init.h>     /* __init, __exit                    */
#include <linux/sched.h>    /* current                           */

/* ❶ O kernel VERIFICA esta linha na carga. Sem uma licença compatível com a
 *    GPL o módulo até carrega, mas o kernel é marcado como "tainted" e
 *    centenas de símbolos EXPORT_SYMBOL_GPL ficam invisíveis para ele. */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Turma de SO - TADS/IFRN");
MODULE_DESCRIPTION("Prova de que estamos dentro do kernel");

/* ❷ Não existe main(). Um módulo é uma biblioteca que o kernel liga a si
 *    mesmo: module_init() aponta a função de entrada, module_exit() a de
 *    saída. A marca __init avisa que a função só serve uma vez — depois da
 *    carga o kernel devolve a memória do código dela. */
static int __init ola_init(void)
{
	/* ❸ printk, não printf. A libc NÃO existe aqui: nada de printf, malloc
	 *    ou fopen. Do lado de dentro são printk, kmalloc e kfree. */
	printk(KERN_INFO "ola: carregado - estou em modo nucleo!\n");

	/* ❹ current aponta para o processo cujo contexto está em uso AGORA.
	 *    Vai imprimir "insmod": prova de que o seu código executou
	 *    "emprestando" o contexto de quem pediu a carga. Guarde o nome —
	 *    current reaparece na Unidade 2, quando estudarmos o PCB. */
	printk(KERN_INFO "ola: quem me carregou? comm=%s pid=%d\n",
	       current->comm, current->pid);

	/* ❺ O retorno importa. Devolver != 0 faz o kernel ABORTAR a carga e
	 *    desfazer tudo. É a única chance de o módulo dizer "não consegui
	 *    inicializar". */
	return 0;
}

static void __exit ola_exit(void)
{
	printk(KERN_INFO "ola: removido. Ate mais.\n");
}

module_init(ola_init);
module_exit(ola_exit);
