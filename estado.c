#include "estado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Header functions */
ESTADO inicializar(int mostrar_ecra, int nivel, int vida_jogador, int score_atual, int *scores);
ESTADO movimentar_inimigos(ESTADO e);
ESTADO matar_inimigos(ESTADO e);
ESTADO atualizar_ranking_scores(ESTADO e);


ESTADO ficheiro2estado() {
	FILE *f;
	f = fopen(FICHEIRO_ESTADO, "r");
	if (f == NULL) {
		perror("Erro a ler o ficheiro de estado!");
		exit(1);
	}

	ESTADO e;
	int *p = (int *) &e;
	unsigned int i;
	int d;

	for(i = 0; i < (sizeof(ESTADO) / sizeof(int)); i++) {
		if (fscanf(f, "%d", &d) != 1) {
			e = inicializar(1, 1, VIDAS, 0, NULL);
			estado2ficheiro(e);
			break;
		}
		p[i] = d;
	}

	fclose(f);

	return e;
}

void estado2ficheiro(ESTADO e) {
	FILE *f;
	f = fopen(FICHEIRO_ESTADO, "w");
	if (f == NULL) {
		perror("Erro a escrever no ficheiro de estado!");
		exit(1);
	}

	int *p = (int *) &e;
	unsigned int i;

	for(i = 0; i < (sizeof(ESTADO) / sizeof(int)); i++)
		fprintf(f, "%d\n", p[i]);

	fclose(f);
}

ESTADO ler_estado(char *args) {
	char acao[32];
	int x = 0, y = 0;
	int lidos = sscanf(args, "%[^,],%d,%d", acao, &x, &y);

	if (lidos >= 1) {
		aplicar_acao(acao, x, y);
	}
	else {
		aplicar_acao("Menu", x, y);
	}

	ESTADO e = ficheiro2estado();
    return e;
}

void aplicar_acao(char *acao, int x, int y) {
	ESTADO e = ficheiro2estado();

	if (strcmp(acao, "Movimento_Jogador") == 0) {
		e = movimentar_inimigos(e);
		e.jogador.x = x;
		e.jogador.y = y;
	}

	else if (strcmp(acao, "Movimento_Saída") == 0) {
		e.nivel += 1;
		e.vida_jogador += 1;
		e.score_atual += 10;
		e = inicializar(0, e.nivel, e.vida_jogador, e.score_atual, e.scores);
	}

	else if (strcmp(acao, "Matar_Inimigo") == 0) {
		e.score_atual += 5;
		e = matar_inimigos(e);
		e = movimentar_inimigos(e);
		e.jogador.x = x;
		e.jogador.y = y;
	}

	else if (strcmp(acao, "Início_Jogo") == 0) {
		e = inicializar(0, 1, VIDAS, 0, e.scores);
	}	

	else if (strcmp(acao, "Menu") == 0) {
		e.mostrar_ecra = 1;
	}

	else if (strcmp(acao, "Ranking") == 0) {
		e.mostrar_ecra = 2;
	}

	else if (strcmp(acao, "Ajuda") == 0) {
		e.mostrar_ecra = 3;
	}

	else if (strcmp(acao, "Reset") == 0) {
		e = inicializar(1, 1, VIDAS, 0, NULL);
	}

	else {
		e.mostrar_ecra = 1;
	}

	if (e.vida_jogador <= 0){
		e = atualizar_ranking_scores(e);
		e = inicializar(2, 1, VIDAS, e.score_atual, e.scores);
	}

	estado2ficheiro(e);
}