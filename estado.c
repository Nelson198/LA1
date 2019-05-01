#include "estado.h"

/**
@file estado.c
Código do estado e das funções que convertem estados em ficheiros e vice-versa
*/

/* <----------------------------------------- Headers de Funções de Roguelike.c ----------------------------------------------> */
ESTADO inicializar_estado(float x, int dif, int nivel, int score_atual, int *scores, int vidas_jogador, int inimigos_mortos, int mostrar_ecra, \
                          int mostrar_possiveis_casas_inimigos, int mostrar_possiveis_casas_jogador, int idx_ultimo_score);
ESTADO atualizar_scores(ESTADO e);
ESTADO matar_inimigo(ESTADO e, int x, int y);
ESTADO movimentar_inimigos(ESTADO e, int a, int b);
/* <--------------------------------------------------------------------------------------------------------------------------> */

ESTADO ficheiro2estado() {
	FILE *f;
	f = fopen(FICHEIRO_ESTADO, "r");
	if (f == NULL) {
		perror("Erro a ler o ficheiro de estado");
		exit(1);
	}

	ESTADO e;
	int *p = (int *) &e;
	unsigned int i;
	int d;

	for(i = 0; i < (sizeof(ESTADO) / sizeof(int)); i++) {
		if (fscanf(f, "%d", &d) != 1) {
			e = inicializar_estado(0.5, 1, 1, 0, NULL, VIDAS, 0, 1, 0, 0, -1);
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
		perror("Erro a escrever o ficheiro de estado");
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

	if (strcmp(acao, "Movimentar_Jogador") == 0) {
		e = movimentar_inimigos(e, x, y);
		e.jogador.x = x;
		e.jogador.y = y;
		e.jogadas++;

		if(e.jogadas - e.x == 3) {
			e.dif = 1;
			e.x = 0.5;
		}
	}

	else if(strcmp(acao, "Apanhar_Pocao1") == 0) {
		e = movimentar_inimigos(e, x, y);
		e.jogador.x = x;
		e.jogador.y = y;
		e.vidas_jogador++;
		e.score_atual += 2;
		e.pocao1.x = -1;
		e.pocao1.y = -1;
		e.jogadas++;

		if(e.jogadas - e.x == 3) {
			e.dif = 1;
			e.x = 0.5;
		}
	}

	else if(strcmp(acao, "Apanhar_Pocao2") == 0) {
		e = movimentar_inimigos(e, x, y);
		e.jogador.x = x;
		e.jogador.y = y;
		e.pocao2.x = -1;
		e.pocao2.y = -1;
		e.dif = 2;
		e.score_atual += 3;
		e.jogadas++;
		e.x = e.jogadas;
	}

	else if (strcmp(acao, "Movimentar_Saida") == 0) {
		if (e.nivel <= 10) {
			e.nivel++;
			e.score_atual += 10;
			e.vidas_jogador += 3;
			e = inicializar_estado(0.5, 1, e.nivel, e.score_atual, e.scores, e.vidas_jogador, e.inimigos_mortos, 0, e.mostrar_possiveis_casas_inimigos, e.mostrar_possiveis_casas_jogador, -1);
		} else {
			e.score_atual += 10;
			e.score_atual += e.vidas_jogador * 2;
			e = atualizar_scores(e);
			e = inicializar_estado(0.5, 1, 1, e.score_atual, e.scores, VIDAS, 0, 2, 0, 0, e.idx_ultimo_score);
		}
		e.jogadas++;
	}

	else if (strcmp(acao, "Matar_Inimigo") == 0) {
		e.score_atual += 5;
		e = matar_inimigo(e, x, y);
		e = movimentar_inimigos(e, x, y);
		e.jogador.x = x;
		e.jogador.y = y;
		e.jogadas++;

		if(e.jogadas - e.x == 3) {
			e.dif = 1;
			e.x = 0.5;
		}
	}

	else if (strcmp(acao, "Inicio") == 0) {
		e = inicializar_estado(0.5, 1, 1, 0, e.scores, VIDAS, 0, 0, 0, 0, -1);
	}

	else if (strcmp(acao, "Menu") == 0) {
		e.idx_ultimo_score = -1;
		e.mostrar_ecra = 1;
	}

	else if (strcmp(acao, "Ranking") == 0) {
		e.mostrar_ecra = 2;
	}

	else if (strcmp(acao, "Ajuda") == 0) {
		e.mostrar_ecra = 3;
	}

	else if (strcmp(acao, "Reset") == 0) {
		e = inicializar_estado(0.5, 1, 1, 0, NULL, VIDAS, 0, 1, 0, 0, -1);
	}

	else if (strcmp(acao, "Casas_Possiveis_Inimigo_Ativado") == 0) {
		e.mostrar_possiveis_casas_inimigos = 1;
	}

	else if (strcmp(acao, "Casas_Possiveis_Inimigo_Desativado") == 0) {
		e.mostrar_possiveis_casas_inimigos = 0;
	}

	else if (strcmp(acao, "Casas_Possiveis_Jogador_Ativado") == 0) {
		e.mostrar_possiveis_casas_jogador = 1;	
	}

	else if (strcmp(acao, "Casas_Possiveis_Jogador_Desativado") == 0) {
		e.mostrar_possiveis_casas_jogador = 0;
	}

	else {
		e.mostrar_ecra = 1;
	}

	if (e.vidas_jogador <= 0){
		e = atualizar_scores(e);
		e = inicializar_estado(0.5, 1, 1, e.score_atual, e.scores, VIDAS, 0, 2, 0, 0, e.idx_ultimo_score);
	}

	estado2ficheiro(e);
}