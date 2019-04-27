#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"

/**
@file Roguelike.c
Implementação de funções que tratam da impressão da interface do jogo das funcionalidades do mesmo.
*/

/**
\brief Função que determina o sinal de um valor.
\param x Valor numérico
\returns 1 --> Valor positivo\n
		-1 --> Valor negativo\n
		 0 --> Caso contrário
*/
#define sign(x) (x > 0) ? 1 : ((x < 0) ? -1 : 0)

/** \brief Número de linhas e colunas */
#define TAMANHO		15

/** \brief Número de píxeis por casa */
#define ESCALA		40

/**
\brief Função que verifica se uma posição está dentro do tabuleiro de jogo.
@param x Coluna
@param y Linha
@returns 1 --> Sim\n
         0 --> Não
*/
int posicao_valida(int x, int y) {
	return x >= 0 && y >= 0 && x < TAMANHO && y < TAMANHO;
}

/**
\brief Função que verifica se uma posição é igual a um par de coordenadas.
@param p Posição
@param x Coluna
@param y Linha
@returns 1 --> Sim\n
         0 --> Não
*/
int posicao_igual(POSICAO p, int x, int y) {
	return p.x == x && p.y == y;
}

/**
\brief Função que verifica se uma posição é adjacente a outra.
@param p1 Posição 1
@param p2 Posição 2
@returns 1 --> Sim\n
         0 --> Não
*/
int posicao_adjacente(POSICAO p1, POSICAO p2){
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if ((p1.x + dx == p2.x) && (p1.y + dy == p2.y)) {
				return 1;
			}
		}
	}
	return 0;
}

/**
\brief Função que verifica se uma casa tem um inimigo.
@param e Estado
@param x Coluna
@param y Linha
@returns 1 --> Sim\n
         0 --> Não
*/
int tem_inimigo(ESTADO e, int x, int y) {
	for (int i = 0; i < e.num_inimigos; i++) {
		if (posicao_igual(e.inimigo[i], x, y)) {
			return 1;
		}
	}
	return 0;
}

/**
\brief Função que verifica se uma casa tem um obstáculo.
@param e Estado
@param x Coluna
@param y Linha
@returns 1 --> Sim\n
         0 --> Não
*/
int tem_obstaculo(ESTADO e, int x, int y) {
	for (int i = 0; i < e.num_obstaculos; i++) {
		if (posicao_igual(e.obstaculo[i], x, y)) {
			return 1;
		}
	}
	return 0;
}

/**
\brief Função que verifica se uma casa tem a poção.
@param e Estado
@param x Coluna
@param y Linha
@returns 1 --> Sim\n
         0 --> Não
*/
int tem_pocao(ESTADO e, int x, int y) {
	return posicao_igual(e.pocao, x, y);
}

/**
\brief Função que verifica se uma casa tem o jogador.
@param e Estado
@param x Coluna
@param y Linha
@returns 1 --> Sim\n
         0 --> Não
*/
int tem_jogador(ESTADO e, int x, int y) {
	return posicao_igual(e.jogador, x, y);
}

/**
\brief Função que verifica se uma casa tem a saída.
@param e Estado
@param x Coluna 
@param y Linha
@returns 1 --> Sim\n
         0 --> Não
*/
int tem_saida(ESTADO e, int x, int y) {
	return posicao_igual(e.saida, x, y);
}

/**
\brief Função que verifica se uma casa tem a entrada.
@param e Estado
@param x Coluna 
@param y Linha
@returns 1 --> Sim\n
         0 --> Não
*/
int tem_entrada(ESTADO e, int x, int y) {
	if (e.nivel == 0) {
		return 0;
	}
	else {
		return posicao_igual(e.entrada, x, y);
	}
}

/**
\brief Função que verifica se uma posição está ocupada.
@param e Estado
@param x Coluna
@param y Linha
@returns 1 --> Sim\n
         0 --> Não
*/
int posicao_ocupada(ESTADO e, int x, int y) {
	if (tem_inimigo(e, x, y)) return 1;
	if (tem_obstaculo(e, x, y)) return 1;
	if (tem_jogador(e, x, y)) return 1;
	if (tem_saida(e, x, y)) return 1;
	if (tem_entrada(e, x, y)) return 1;
	if (tem_pocao(e, x, y)) return 1;
	return 0;
}

/**
\brief Função que inicializa a posição da entrada de um determinado nível do jogo.
@param e Estado
@returns Estado modificado
*/
ESTADO inicializar_entrada(ESTADO e) {
	e.entrada.x = 0;
	e.entrada.y = TAMANHO-1;
	return e;
}

/**
\brief Função que inicializa a posição da saída de um determinado nível do jogo.
@param e Estado
@returns Estado modificado
*/
ESTADO inicializar_saida(ESTADO e) {
	e.saida.x = TAMANHO-1;
	e.saida.y = 0;
	return e;
}

/**
\brief Função que define a posição inicial do jogador.
@param e Estado
@returns Estado modificado
*/
ESTADO inicializar_jogador(ESTADO e) {
	if (e.nivel == 1) {
		e.jogador = (POSICAO){0, TAMANHO-1};
	}
	else {
		e.jogador = (POSICAO){1, TAMANHO-1};
	}
	return e;
}

/**
\brief Função que define a posição de um inimigo.
@param e Estado
@returns Estado modificado
*/
ESTADO inicializar_inimigo(ESTADO e) {
	int x, y;
	do {
		x = random() % TAMANHO;
		y = random() % TAMANHO;
	} while (posicao_ocupada(e, x, y) || (x <= 3 && y >= TAMANHO - 3));

	POSICAO p = {x, y};
	e.inimigo[e.num_inimigos++] = p;
	return e;
}

/**
\brief Função que define a posição de todos os inimigos.
@param e Estado
@param num Número de inimigos
@returns Estado modificado
*/
ESTADO inicializar_inimigos(ESTADO e, int num) {
	e.num_inimigos = 0;
	for (int i = 0; i < num; i++) {
		e = inicializar_inimigo(e);
	}
	return e;
}

/**
\brief Função que define a posição de um obstáculo.
@param e Estado
@returns Estado modificado
*/
ESTADO inicializar_obstaculo(ESTADO e) {
	int x, y;
	do {
		x = random() % TAMANHO;
		y = random() % TAMANHO;
	} while (posicao_ocupada(e, x, y) || (x <= 3 && y >= TAMANHO - 3));

	POSICAO p = {x, y};
	e.obstaculo[e.num_obstaculos++] = p;
	return e;
}

/**
\brief Função que define a posição de todos os obstáculos.
@param e Estado
@param num Número de inimigos
@returns Estado modificado
*/
ESTADO inicializar_obstaculos(ESTADO e, int num) {
	e.num_obstaculos = 0;
	for (int i = 0; i < num; i++) {
		e = inicializar_obstaculo(e);
	}

	return e;
}

/**
\brief Função que define a posição de uma poção.
@param e Estado
@returns Estado modificado
*/
ESTADO inicializar_pocao(ESTADO e) {
	int x, y;
	do {
		x = random() % TAMANHO;
		y = random() % TAMANHO;
	} while (posicao_ocupada(e, x, y) || (x <= 3 && y >= TAMANHO - 3));

	POSICAO p = {x, y};
	e.pocao = p;
	return e;
}

/**
\brief Função que inicializa o array de pontuações.
@param e Estado
@param *scores Array de pontuações.
@returns Estado modificado
*/
ESTADO inicializar_scores(ESTADO e, int *scores) {
	if (scores == NULL) return e;

	for (int i = 0; i < NUM_SCORES; i++) {
		e.scores[i] = scores[i];
	}
	return e;
}

/**
\brief Função que cria um nível, atualizando o estado do jogo.
@param nivel Nível
@param score_atual Pontuação atual
@param scores Array de pontuações
@param vidas_jogador Vidas do jogador
@param inimigos_mortos Inimigos mortos
@param mostrar_ecra 0 --> Tabuleiro\n
                    1 --> Menu\n
					2 --> Ranking\n
					3 --> Ajuda
@param mostrar_possiveis_casas_inimigos 1 --> Sim\n
                                        0 --> Não
@param mostrar_possiveis_casas_jogador 1 --> Sim\n
                                       0 --> Não
@param idx_ultimo_score Índice do array correspondente à última pontuação
@returns Estado modificado
*/
ESTADO inicializar_estado(int nivel, int score_atual, int *scores, int vidas_jogador, int inimigos_mortos, int mostrar_ecra, int mostrar_possiveis_casas_inimigos, int mostrar_possiveis_casas_jogador, int idx_ultimo_score) {
	ESTADO e;
	memset(&e, 0, sizeof(ESTADO));

	e.nivel = nivel;
	e.score_atual = score_atual;
	e.vidas_jogador = vidas_jogador;
	e.inimigos_mortos = inimigos_mortos;
	e.mostrar_ecra = mostrar_ecra;
	e.mostrar_possiveis_casas_inimigos = mostrar_possiveis_casas_inimigos;
	e.mostrar_possiveis_casas_jogador = mostrar_possiveis_casas_jogador;
	e.idx_ultimo_score = idx_ultimo_score;

	e = inicializar_entrada(e);
	e = inicializar_saida(e);
	e = inicializar_jogador(e);
	e = inicializar_inimigos(e, 10 + e.nivel*2);
	e = inicializar_obstaculos(e, 20);
	e = inicializar_pocao(e);
	e = inicializar_scores(e, scores);

	return e;
}

/**
\brief Função que move todos os inimigos.
@param e estado
@param novojogx Nova abcissa da posição do jogador
@param novojogy Nova ordenada da posição do jogador
@returns Estado modificado
*/
ESTADO movimentar_inimigos(ESTADO e, int novojogx, int novojogy) {
	POSICAO novojog = {novojogx, novojogy};

	for (int i = 0; i < e.num_inimigos; i++) {
		if (posicao_adjacente(novojog, e.inimigo[i])) {
			e.vidas_jogador--;
		} 
		else {
			int dx = e.jogador.x - e.inimigo[i].x;
			int dy = e.jogador.y - e.inimigo[i].y;
			dx = sign(dx);
			dy = sign(dy);
			int x = e.inimigo[i].x + dx;
			int y = e.inimigo[i].y + dy;
			if (!posicao_ocupada(e, x, y)) {
				e.inimigo[i].x = x;
				e.inimigo[i].y = y;
			}
		}
	}
	return e;
}

/**
\brief Função que imprime o tabuleiro de jogo.
*/
void imprimir_tabuleiro() {
	for(int y = 0; y < TAMANHO; y++) {
		for(int x = 0; x < TAMANHO; x++) {
			IMAGEM(x, y, ESCALA, "grid.png");
		}
	}
}

/**
\brief Função que imprime a entrada.
@param e Estado
*/
void imprimir_entrada(ESTADO e) {
	if (e.nivel >= 2) {
		IMAGEM(e.entrada.x, e.entrada.y, ESCALA, "stone_stairs_up.png");
	}
}

/**
\brief Função que imprime a saída.
@param e Estado
*/
void imprimir_saida(ESTADO e) {
	IMAGEM(e.saida.x, e.saida.y, ESCALA, "stone_stairs_down.png");
}

/**
\brief Função que imprime uma casa transparente no tabuleiro de jogo.
@param x Coluna
@param y Linha
*/
void imprimir_casa_transparente(int x, int y) {
	QUADRADO_TRANSPARENTE(x, y, ESCALA);
}

/**
\brief Função que imprime uma ação do jogador.
@param e Estado
@param dx Coluna da ação em relação ao jogador
@param dy Linha da ação em relação ao jogador
*/
void imprimir_acao(ESTADO e, int dx, int dy) {
	int x = e.jogador.x + dx;
	int y = e.jogador.y + dy;
	char acao[32];

	if (!posicao_valida(x, y) || tem_obstaculo(e, x, y) || tem_jogador(e, x, y) || tem_entrada(e, x, y))
		return;

	if (tem_pocao(e, x, y)) {
		strcpy(acao, "Apanhar_Pocao");
	}

	else if (tem_inimigo(e, x, y)) {
		strcpy(acao, "Atacar_Inimigo");
	}

	else if (tem_saida(e, x, y)) {
		strcpy(acao, "Movimentar_Saida");
	}

	else {
		strcpy(acao, "Movimentar_Jogador");
	}

	char link[2048];
	sprintf(link, "http://localhost/cgi-bin/Roguelike?%s,%d,%d", acao, x, y);
	ABRIR_LINK(link);
	imprimir_casa_transparente(x, y);
	FECHAR_LINK;
}

/**
\brief Função que imprime as ações do jogador.
@param e Estado
*/
void imprimir_acoes(ESTADO e) {
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			imprimir_acao(e, dx, dy);
		}
	}
}

/**
\brief Função que imprime o jogador.
@param e Estado
*/
void imprimir_jogador(ESTADO e) {
	IMAGEM(e.jogador.x, e.jogador.y, ESCALA, "player1.png");
	IMAGEM(e.jogador.x, e.jogador.y, ESCALA, "player2.png");
	IMAGEM(e.jogador.x, e.jogador.y, ESCALA, "player3.png");
	IMAGEM(e.jogador.x, e.jogador.y, ESCALA, "player4.png");
	imprimir_acoes(e);
}

/**
\brief Função que imprime os inimigos.
@param e Estado
*/
void imprimir_inimigos(ESTADO e) {
	for(int i = 0; i < e.num_inimigos; i++) {
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "enemy.png");
	}
}

/**
\brief Função que sinaliza a vermelho as casas para onde os inimigos se podem deslocar e, se for caso disso, atacar.
@param e Estado
*/
void imprimir_casas_atacadas(ESTADO e) {
	if (e.mostrar_possiveis_casas_inimigos == 0) {
		ABRIR_LINK("http://localhost/cgi-bin/Roguelike?Casas_Possiveis_Inimigo_Ativado");
		TEXTO((TAMANHO + 1.0) * ESCALA, (TAMANHO - 1.0) * ESCALA, "#000000", "bold", "Mostrar casas onde os inimigos podem atacar");
		FECHAR_LINK;
	} 
	else {
		ABRIR_LINK("http://localhost/cgi-bin/Roguelike?Casas_Possiveis_Inimigo_Desativado");
		TEXTO((TAMANHO + 1.0) * ESCALA, (TAMANHO - 1.0) * ESCALA, "#ff0000", "bold", "Ocultar casas onde os inimigos podem atacar");
		FECHAR_LINK;

		for (int k = 0; k < e.num_inimigos; k++) {
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					int x = e.inimigo[k].x + dx;
					int y = e.inimigo[k].y + dy;
					if (posicao_valida(x, y) && !posicao_igual(e.inimigo[k], x, y) && !tem_pocao(e, x, y) && !tem_obstaculo(e, x, y) && !tem_saida(e, x, y) && !tem_entrada(e, x, y)) {
						QUADRADO(x, y, ESCALA, "red");
					}
				}
			}
		}
	}
}

/**
\brief Função que sinaliza a amarelo as casas para onde o jogador se pode movimentar no tabuleiro.
@param e Estado
*/
void imprimir_casas_possiveis_jogador(ESTADO e) {
	if (e.mostrar_possiveis_casas_jogador == 0) {
		ABRIR_LINK("http://localhost/cgi-bin/Roguelike?Casas_Possiveis_Jogador_Ativado");
		TEXTO((TAMANHO + 1.0) * ESCALA, (TAMANHO - 0.1) * ESCALA, "#000000", "bold", "Mostrar casas para onde o jogador se pode deslocar");
		FECHAR_LINK;
	} 
	else {
		ABRIR_LINK("http://localhost/cgi-bin/Roguelike?Casas_Possiveis_Jogador_Desativado");
		TEXTO((TAMANHO + 1.0) * ESCALA, (TAMANHO - 0.1) * ESCALA, "#ffef00", "bold", "Ocultar casas para onde o jogador se pode deslocar");
		FECHAR_LINK;

		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				int x = e.jogador.x + dx;
				int y = e.jogador.y + dy;
				if (posicao_valida(x, y) && !tem_obstaculo(e, x, y) && !tem_jogador(e, x, y) && !tem_entrada(e, x, y)) {
					QUADRADO(x, y, ESCALA, "yellow");
				}
			}
		}
	}
}

/**
\brief Função que imprime os obstáculos.
@param e Estado
*/
void imprimir_obstaculos(ESTADO e) {
	for(int i = 0; i < e.num_obstaculos; i++) {
		IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "obstacle.png");
	}
}

/** 
\brief Função que imprime a poção.
@param e Estado
*/
void imprimir_pocao(ESTADO e) {
	if (e.pocao.x != -1 && e.pocao.y != -1) {
		IMAGEM(e.pocao.x, e.pocao.y, ESCALA, "potion.png");
	}
}

/**
\brief Função que imprime o score atual.
@param e Estado
*/
void imprimir_score(ESTADO e) {
	char s1[1000];
	sprintf(s1, "Score: %d", e.score_atual);
	TEXTO((TAMANHO + 1.0) * ESCALA, 20.0, "#FF8C00", "bold", s1);
}

/**
\brief Função que imprime o nível atual.
@param e Estado
*/
void imprimir_nivel(ESTADO e) {
	char s1[1000];
	sprintf(s1, "Nivel: %d", e.nivel);
	TEXTO((TAMANHO + 1.0) * ESCALA, 60.0, "#4169E1", "bold", s1);	
}

/**
\brief Função que imprime o número de inimigos mortos.
@param e Estado
*/
void imprimir_inimigos_mortos(ESTADO e) {
	char s1[1000];
	sprintf(s1, "Inimigos mortos: %d", e.inimigos_mortos);
	TEXTO((TAMANHO + 1.0) * ESCALA, 100.0, "#808080", "bold", s1);
}

/**
\brief Função que imprime o número de vidas do jogador.
@param e Estado
*/
void imprimir_vidas(ESTADO e) {
	if (e.vidas_jogador > 50){
		char s1[1000];
		sprintf(s1, "Vidas: %d", e.vidas_jogador);
		TEXTO((TAMANHO + 1.0) * ESCALA, 140.0, "#FF0000", "bold", s1);
	} 
	else {
		TEXTO((TAMANHO + 1.0) * ESCALA, 140.0, "#FF0000", "bold", "Vidas:");

		int v1 = e.vidas_jogador % 10;
		int v2 = (e.vidas_jogador - v1) / 10;
		int l, c;

		if (v1 == 0) {
			for(l = 0; l < v2; l++) {
				for(c = 0; c < 10; c++) {
					printf("<image x=%f y=%d width=%d height=%d xlink:href=%s />\n", (TAMANHO+2.5)*ESCALA + c*44, 115 + 44*l, 40, 40, "http://localhost/Imagens/heart.png");
				}
			}
		}

		else if (v2 == 0) {
			for(c = 0; c < v1; c++) {
				printf("<image x=%f y=%d width=%d height=%d xlink:href=%s />\n", (TAMANHO+2.5)*ESCALA + c*44, 115, 40, 40, "http://localhost/Imagens/heart.png");
			}
		}

		else {
			for(l = 0; l < v2; l++) {
				for(c = 0; c < 10; c++) {
					printf("<image x=%f y=%d width=%d height=%d xlink:href=%s />\n", (TAMANHO+2.5)*ESCALA + c*44, 115 + 44*l, 40, 40, "http://localhost/Imagens/heart.png");
				}
			}
			l++;
			for(c = 0; c < v1; c++) {
				printf("<image x=%f y=%d width=%d height=%d xlink:href=%s />\n", (TAMANHO+2.5)*ESCALA + c*44, 115 + 44*l, 40, 40, "http://localhost/Imagens/heart.png");
			}
		}
	}	
}

/**
\brief Função que imprime o menu.
*/
void imprimir_menu() {
	printf("<image x=%d y=%d width=%d height=%f xlink:href=%s />\n", \
			0, 0, (TAMANHO+10)*ESCALA, (TAMANHO - 0.5)*ESCALA, "http://localhost/Imagens/MenuBackground.jpg");

	IMAGEM(4, 3, ESCALA, "play.svg");
	ABRIR_LINK("http://localhost/cgi-bin/Roguelike?Inicio");
	TEXTO((TAMANHO - 9.5) * ESCALA, (TAMANHO/2 - 3.3) * ESCALA, "#ffff00", "bold", "Jogar");
	FECHAR_LINK;

	IMAGEM(4, 5, ESCALA, "help.svg");
	ABRIR_LINK("http://localhost/cgi-bin/Roguelike?Ajuda");
	TEXTO((TAMANHO - 9.5) * ESCALA, (TAMANHO/2 - 1.3) * ESCALA, "#ffffff", "bold", "Ajuda");
	FECHAR_LINK;

	IMAGEM(4, 7, ESCALA, "ranking.svg");
	ABRIR_LINK("http://localhost/cgi-bin/Roguelike?Ranking");
	TEXTO((TAMANHO - 9.5) * ESCALA, (TAMANHO/2 + 0.7) * ESCALA, "#ffffff", "bold", "Ranking");
	FECHAR_LINK;
}

/**
\brief Função que imprime o botão de regresso ao menu.
*/
void imprimir_regressar_menu() {
	ABRIR_LINK("http://localhost/cgi-bin/Roguelike?Menu");
	IMAGEM(1, 1, ESCALA, "cross.svg");
	FECHAR_LINK;
}

/**
\brief Função que imprime o botão de regresso ao menu durante o jogo.
*/
void imprimir_regressar_menu_jogo() {
	ABRIR_LINK("http://localhost/cgi-bin/Roguelike?Menu");
	printf("<image x=%f y=%d width=%d height=%d xlink:href=%s />\n", (TAMANHO+2.5)*ESCALA + 396, 0, 40, 40, "http://localhost/Imagens/cross.svg");
	FECHAR_LINK;
}

/**
\brief Função que imprime os melhores scores.
@param e Estado
*/
void imprimir_melhores_scores(ESTADO e) {
	char s1[1000];

	printf("<image x=%d y=%d width=%d height=%f xlink:href=%s />\n", \
			0, 0, (TAMANHO+10)*ESCALA, (TAMANHO - 0.5)*ESCALA, "http://localhost/Imagens/MenuBackground.jpg");

	for(int i = 0; i < NUM_SCORES; i++) {
		if (i == e.idx_ultimo_score) {
			sprintf(s1, "%d. %d", i + 1, e.scores[i]);
			TEXTO(6.0 * ESCALA, (4.0 + i) * ESCALA, "#00ff00", "bold", s1);
		} 
		else {
			sprintf(s1, "%d. %d", i + 1, e.scores[i]);
			TEXTO(6.0 * ESCALA, (4.0 + i) * ESCALA, "#ffffff", "bold", s1);
		}
	}

	if (e.idx_ultimo_score == -2) {
		sprintf(s1, "--. %d", e.score_atual);
		TEXTO(6.0 * ESCALA, 10.0*ESCALA, "#00ff00", "bold", s1);
	}

	imprimir_regressar_menu();
}

/**
\brief Função que imprime a página de ajuda.
*/
void imprimir_ajuda() {
	printf("<image x=%d y=%d width=%d height=%f xlink:href=%s />\n", \
			0, 0, (TAMANHO+10)*ESCALA, (TAMANHO - 0.5)*ESCALA, "http://localhost/Imagens/MenuBackground.jpg");

	TEXTO((float) ESCALA, 3.0 * ESCALA, "#ffffff", "normal", "Bem-vindo ao Roguelike, o melhor jogo de sempre!");
	TEXTO((float) ESCALA, 4.0 * ESCALA, "#ffffff", "normal", "Comecas com 5 vidas e ganhas 3 vidas por nivel.");
	TEXTO((float) ESCALA, 5.0 * ESCALA, "#ffffff", "normal", "O objetivo e ultrapassares os 11 niveis.");
	TEXTO((float) ESCALA, 6.0 * ESCALA, "#ffffff", "normal", "Pontuacao:");
	TEXTO(2.0 *ESCALA, 7.0 * ESCALA, "#ffffff", "normal", "5 pontos por inimigo morto;");
	TEXTO(2.0 *ESCALA, 8.0 * ESCALA, "#ffffff", "normal", "10 pontos por nivel concluido.");
	TEXTO((float) ESCALA, 9.0 * ESCALA, "#ffffff", "normal", "A partir do nivel 5 esperam-te surpresas.");
	TEXTO((float) ESCALA, 10.0 * ESCALA, "#ffffff", "normal", "Boa sorte!");

	imprimir_regressar_menu();
}

/**
\brief Função que imprime um estado.
@param e Estado
*/
void imprimir_estado(ESTADO e) {
	if (e.mostrar_ecra == 0) {
		imprimir_tabuleiro();
		imprimir_casas_atacadas(e);
		imprimir_casas_possiveis_jogador(e);
		imprimir_pocao(e);
		imprimir_inimigos(e);
		imprimir_obstaculos(e);
		imprimir_entrada(e);
		imprimir_saida(e);
		imprimir_jogador(e);
		imprimir_score(e);
		imprimir_vidas(e);
		imprimir_nivel(e);
		imprimir_inimigos_mortos(e);
		imprimir_regressar_menu_jogo();
	}

	else if (e.mostrar_ecra == 1) {
		imprimir_menu();
	}

	else if (e.mostrar_ecra == 2) {
		imprimir_melhores_scores(e);
	}

	else if (e.mostrar_ecra == 3) {
		imprimir_ajuda();
	}
}

/**
\brief Função que dá início ao programa.
@returns 0 Por convenção
*/
int main() {
	srandom(time(NULL));

	ESTADO e = ler_estado(getenv("QUERY_STRING"));

	COMECAR_HTML;
	ABRIR_SVG((TAMANHO+13.5)*ESCALA, (TAMANHO+0.5)*ESCALA);

	imprimir_estado(e);

	FECHAR_SVG;

	return 0;
}