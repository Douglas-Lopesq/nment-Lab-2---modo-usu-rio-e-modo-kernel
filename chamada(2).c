/*
 * chamada.c — a travessia pedida: uma chamada de sistema, vista de fora.
 *
 * Prática da Aula 03 — Parte A1
 * Sistemas Operacionais · TADS/IFRN · 2026.1
 *
 * Compilar:  gcc -Wall -Wextra -O0 -o chamada chamada.c
 * Executar:  ./chamada
 *            strace -e trace=openat,read,lseek,close ./chamada
 *
 * O mesmo serviço do núcleo — ler bytes de um arquivo — é pedido duas vezes,
 * por dois caminhos diferentes:
 *
 *   ❶ read(fd, buffer, 4096)        pela função da biblioteca C
 *   ❷ syscall(SYS_read, fd, ...)    sem a função da biblioteca C
 *
 * O strace mostra a MESMA linha nas duas vezes. É a evidência de que read()
 * não é "a função que roda no núcleo": read() é código do seu processo,
 * executando em modo usuário, que prepara os argumentos e emite a instrução
 * de chamada de sistema. Quem executa o serviço do outro lado é o núcleo.
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>

#define TAM 4096

int main(int argc, char *argv[])
{
	const char *caminho = (argc > 1) ? argv[1] : "/etc/hostname";
	char    buffer[TAM + 1];
	int     fd;
	ssize_t n;

	/* ❶ Modo usuário, e só. Daqui até o open() nada sai do processo:
	 *    atribuição, comparação, chamada de função — tudo isso é código seu,
	 *    executado em modo usuário, e o núcleo nem fica sabendo. */
	printf("[usuario] 1. codigo do processo; nada pedido ao nucleo ainda\n");
	fflush(stdout);

	fd = open(caminho, O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	/* ❷ O pedido. read() é uma função da biblioteca C: ela coloca os
	 *    argumentos nos registradores combinados e emite a instrução que faz
	 *    o processador trocar de modo e desviar para o ponto de entrada do
	 *    núcleo. A função é sua; o serviço é dele. */
	printf("[usuario] 2. pedindo %d bytes ao nucleo com read()\n", TAM);
	fflush(stdout);

	n = read(fd, buffer, TAM);

	/* ❸ De volta. O núcleo terminou o serviço, devolveu o processador ao modo
	 *    usuário e a execução continuou na instrução seguinte, como se read()
	 *    tivesse sido uma chamada de função comum. */
	printf("[usuario] 3. de volta em modo usuario; read() devolveu %zd bytes\n", n);

	/* ❹ O mesmo serviço, sem o invólucro. syscall() emite a instrução
	 *    diretamente, sem passar pela função read() da biblioteca. Compare as
	 *    duas linhas no strace: são iguais. */
	lseek(fd, 0, SEEK_SET);
	n = syscall(SYS_read, fd, buffer, (size_t)TAM);
	printf("[usuario] 4. mesmo servico sem a funcao read(): %zd bytes\n", n);

	close(fd);

	if (n > 0) {
		if (buffer[n - 1] == '\n')
			n--;
		buffer[n] = '\0';
		printf("[usuario] 5. conteudo lido de %s: \"%s\"\n", caminho, buffer);
	}
	return 0;
}
