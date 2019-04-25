/**
 * @file RogueLike.c
 * Ficheiro com as funções relativas às funcionalidades do programa RogueLike.
 */

#include "cgi.h"
#include "estado.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

/**
 * Função que devolve o sinal de um valor.
 * @param x Valor
 * @return Sinal do valor.
 */ 
#define sign(x) (x > 0) ? 1 : ((x < 0) ? -1 : 0)

/**
 * Escala da respetiva imagem.
 */
#define ESCALA 50

/**
 * Valor que indica o tamanho máximo do array associado a um link.
 */
#define MAX 10240

/**
 * Dimensão do tabuleiro.
 */
#define TAMANHO 10

/**
 * Nº de obstáculos.
 */
#define NUM_OBSTACULOS 20

/**
 * Nº de inimigos.
 */
#define NUM_INIMIGOS 10

/**
 * Função que vai colorir a/as quadrícula/as para onde é possivel o jogador movimentar-se.
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 */
void colour1(int x, int y) {
	QUADRADO2(x, y, ESCALA, "grey", 1.0, "yellow");
}

/**
 * Função que vai colorir a/as quadricula/as onde estão os inimgos possiveis de matar.
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 */
void colour2(int x, int y) {
	QUADRADO2(x, y, ESCALA, "red", 2.0, "black");
}

/**
 * Função que verifica se o ponto (x,y) representa uma coordenada válida no tabuleiro.
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 --> False;\n
 *         1 --> True.
 */
int posicao_valida(int x, int y) {
	return x >= 0 && y >= 0 && x < TAMANHO && y < TAMANHO;
}

/**
 * Função que verifica se dois pontos têm as mesmas coordenadas.
 * @param p Ponto 1
 * @param x Abcissa do ponto 2
 * @param y Ordenada do ponto 2
 * @return 0 --> False;\n
 *         1 --> True.
 */
int mesma_posicao(POSICAO p, int x, int y) {
	return p.x == x && p.y == y;
}

/**
 * Função que verifica se a posição (x,y) está ocupada pelo jogador.
 * @param e Estado atual do programa
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 --> False;\n
 *         1 --> True.
 */
int tem_jogador(ESTADO e, int x, int y) {
	return mesma_posicao(e.jogador, x, y);
}

/**
 * Função que verifica se a posição (x,y) está ocupada por um inimigo.
 * @param e Estado atual do programa
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 --> False;\n
 *         1 --> True.
 */
int tem_inimigo(ESTADO e, int x, int y) {
	for(int i = 0; i < e.num_inimigos; i++) {
		if(mesma_posicao(e.inimigos[i], x, y)) {
			return 1;
		}
	}
	return 0;
}

/**
 * Função que verifica se a posição (x,y) está ocupada por um obstáculo.
 * @param e Estado atual do programa
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 --> False;\n
 *         1 --> True.
 */
int tem_obstaculo(ESTADO e, int x, int y) {
	for(int i = 0; i < e.num_obstaculos; i++) {
		if(mesma_posicao(e.obstaculos[i], x, y)) {
			return 1;
		}
	}
	return 0;
}

/**
 * Função que verifica se a posição (x,y) coincide com a entrada de um determinado nível do jogo.
 * @param e Estado atual do programa
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 --> False;\n
 *         1 --> True.
*/
int tem_entrada(ESTADO e, int x, int y) {
	return posicao_igual(e.entrada, x, y);
}

/**
 * Função que verifica se a posição (x,y) coincide com a saída de um determinado nível do jogo.
 * @param e Estado atual do programa
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 --> False;\n
 *         1 --> True.
 */
int tem_saida(ESTADO e, int x, int y) {
	return mesma_posicao(e.saida, x, y);
}

/**
 * Função que verifica se a posição (x,y) está ocupada pelo jogador ou por um inimigo ou por um obstáculo ou pela entrada ou saída do jogo.
 * @param e Estado atual do programa
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 -> Livre;\n
 *         1 -> Ocupado.
 */
int invariante(ESTADO e, int x, int y) {
	if(tem_jogador(e, x, y)) return 1;
	if(tem_entrada(e, x, y)) return 1;
	if(tem_saida(e, x, y)) return 1;
	if(tem_inimigo(e, x, y)) return 1;
	if(tem_obstaculo(e, x, y)) return 1;
	return 0;
}

/**
 * Função que imprime uma quadrícula com cor alternada.
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 */
void imprimir_quadricula(int x, int y) {
	char *cor[] = { "#000000" , "#FFFFFF" };
	int idx = (x + y) % 2;
	QUADRADO(x, y, ESCALA, cor[idx]);
}

/**
 * Função que imprime o tabuleiro de jogo.
 */
void imprimir_tabuleiro() {
	for(int x = 0; x < TAMANHO; x++) {
		for(int y = 0; y < TAMANHO; y++) {
			imprimir_quadricula(x, y);
		}
	}
}

/**
 * Função que define a posição inicial do jogador.
 * @param e Estado
 * @return Estado novo.
*/
ESTADO inicializar_jogador(ESTADO e) {
	e.entrada.x = e.jogador.x = TAMANHO / 2;
	e.entrada.y = e.jogador.y = TAMANHO - 1;

	return e;
}

/**
 * Função que define as posições de todos os inimigos.
 * @param e Estado
 * @return Estado novo.
*/
ESTADO inicializar_inimigos(ESTADO e) {
	e.num_inimigos = 0;
	int x, y;
	for(int i = 0; i < NUM_INIMIGOS; i++) {
		do {
			x = random() % TAMANHO;
			y = random() % TAMANHO;
		} while(invariante(e, x, y));

		POSICAO p = {x, y};
		e.inimigos[e.num_inimigos++] = p;
	}

	return e;
}

/**
 * Função que define as posições de todos os obstáculos.
 * @param e Estado
 * @return Estado novo.
*/
ESTADO inicializar_obstaculos(ESTADO e) {
	e.num_obstaculos = 0;
	int x, y;
	for(int i = 0; i < NUM_OBSTACULOS; i++) {
		do {
			x = random() % TAMANHO;
			y = random() % TAMANHO;
		} while(invariante(e, x, y));

		POSICAO p = {x, y};
		e.obstaculos[e.num_obstaculos++] = p;
	}

	return e;
}

/**
 * Função que define a posição da saída.
 * @param e Estado
 * @return Estado novo.
*/
ESTADO inicializar_saida(ESTADO e) {
	int x, y = 0;
	do {
		x = random() % TAMANHO;
	} while(invariante(e, x, y));

	e.saida.x = x;
	e.saida.y = y;

	return e;
}

/**
 * Função que inicializa o TOP 10 de pontuações do jogo.
 * @param e Estado
 * @param *scores Array de scores
 * @return Estado novo.
*/
ESTADO inicializar_scores(ESTADO e, int *scores) {
	if (scores == NULL) return e;

	for (int i = 0; i < NUM_SCORES; i++)
		e.scores[i] = scores[i];

	return e;
}

/**
 * Função que inicializa o estado do jogo.
 * @return Estado.
 */
ESTADO inicializar(int mostrar_ecra, int nivel, int vida_jogador, int score_atual, int *scores) {
	ESTADO e;
	memset(&e, 0, sizeof(ESTADO));

	e.mostrar_ecra = mostrar_ecra;
	e.nivel = nivel;
	e.score_atual = score_atual;
	e.vida_jogador = vida_jogador;
	e = inicializar_jogador(e);
	e = inicializar_saida(e);
	e = inicializar_obstaculos(e);
	e = inicializar_inimigos(e);
	e = inicializar_scores(e, scores);

	return e;
}

/**
 * Função que trata de deslocar o jogador para as cordenadas indicadas.
 * @param e Estado atual
 * @return Estado novo.
 */ 
ESTADO movimentar_jogador(ESTADO e) {
	e.jogador.x = e.acao.x;
	e.jogador.y = e.acao.y;
	return e;
}

/**
 * Função que, dada uma ação possível do jogador, sinaliza a posição associada à mesma com uma determinada cor e cria um link com a informação do futuro estado.
 * @param e Estado
 * @param dx deslocação ao longo do eixo das abcissas em relação à posição atual do jogador
 * @param dy deslocação ao longo do eixo das ordenadas em relação à posição atual do jogador
*/
void imprimir_acao_jogador(ESTADO e, int dx, int dy) {
	int x = e.jogador.x + dx;
	int y = e.jogador.y + dy;

	if (!posicao_valida(x, y) || tem_obstaculo(e, x, y))
		return;
	}

	else if (tem_inimigo(e, x, y)) {
		for (int i = 0; i < e.num_inimigos; i++) {
            if (x == e.inimigos[i].x && y == e.inimigos[i].y) {
                sprintf(link,"http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Matar_Inimigo", x, y);
                ABRIR_LINK(link);
                colour2(x,y);
                IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "dwarf_12.png");
                FECHAR_LINK;
            }
        }
	}

	else if (tem_saida(e, x, y)) {
		sprintf(link, "http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Movimento_Saída", x, y);
        ABRIR_LINK(link);
        colour1(x,y);
        IMAGEM(e.saida.x, e.saida.y, ESCALA, "trapdoor1.png");
        FECHAR_LINK;
	}

	else {
		sprintf(link,"http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Movimento_Jogador", x, y);
        ABRIR_LINK(link);
        colour1(x,y);
        FECHAR_LINK;
	}
}

/**
 * Função que imprime as ações do jogador.
 * @param e estado
*/
void acao_jogador(ESTADO e) {
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++)
			imprime_acao_jogador(e, dx, dy);
}

/**
 * Função que desloca os inimigos de acordo com a posição do jogador.
 * @param e Estado atual
 * @return Estado novo.
 */ 
ESTADO movimentar_inimigos(ESTADO e) {
	int dx = e.jogador.x - e.inimigo[i].x;
	int dy = e.jogador.y - e.inimigo[i].y;

	dx = sign(dx);
	dy = sign(dy);

	int x = e.inimigos[i].x + dx;
	int y = e.inimigos[i].y + dy;

	if (!invariante(e, x, y)) {
		e.inimigos[i].x = x;
		e.inimigos[i].y = y;
	}

	return e;
}

/**
 * Função que trata de matar os inimigos, removendo-os do tabuleiro, incrementando o score.
 * @param e Estado atual
 * @return Estado novo.
 */
ESTADO matar_inimigos(ESTADO e) {
	for(int i = 0; i < e.num_inimigos; i++) {
		if (e.acao.x == e.inimigos[i].x && e.acao.y == e.inimigos[i].y) {
			e.inimigos[i] = e.inimigos[e.num_inimigos--];
			break;
		}	
	}
	return e;
}

/**
 * Função que imprime a imagem do jogador no tabuleiro e as possivéis jogadas do mesmo.
 * @param e Estado atual
 */ 
void imprimir_jogador(ESTADO e) {
	IMAGEM(e.jogador.x, e.jogador.y, ESCALA, "character_21.png");
	acao_jogador(e);
}

/**
 * Função que imprime as imagens das vidas do jogador na interface do jogo.
 * @param e Estado atual
 */ 
void imprimir_vida_jogador(ESTADO e) {
	for (int i = 0; i < e.vida_jogador; i++) {
		IMAGEM(i, 26, 20, "heart.png");
	}
}

/**
 * Função que imprime as imagens dos inimigos no tabuleiro.
 * @param e Estado atual
 */ 
void imprimir_inimigos(ESTADO e) {
	for(int i = 0; i < e.num_inimigos; i++) {
		IMAGEM(e.inimigos[i].x, e.inimigos[i].y, ESCALA, "dwarf_12.png");
	}
}

/**
 * Função que imprime as imagens dos obstáculos no tabuleiro.
 * @param e Estado atual
 */ 
void imprimir_obstaculos(ESTADO e) {
	for (int i = 0; i < e.num_obstaculos; i++) {
        IMAGEM(e.obstaculos[i].x, e.obstaculos[i].y, ESCALA, "lava_pool1.png");
	}
}

/**
 * Função que imprime a imagem da saída no tabuleiro.
 * @param e Estado atual
 */ 
void imprimir_saida(ESTADO e) {
	IMAGEM(e.saida.x, e.saida.y, ESCALA, "trapdoor1.png");
}

/**
 * Função que imprime o top 10 das pontuações na interface do jogo.
 * @param e Estado atual
 */
void imprimir_ranking_scores(ESTADO e) {
	for(int i = 0; i < NUM_SCORES; i++) {
		char buf[NUM_SCORES];
		sprintf(buf, "%d", e.scores[i]);
		TEXTO(15.6, 4.0 + (i*42), ESCALA, buf);
	}
}

/** 
 * Função que, dada uma pontuação, atualiza o top 10 das pontuações obtidas até ao momento.
 * @param e Estado atual
 */
void atualizar_ranking_scores(ESTADO e) {
	int i;
	for(i = 0; e.score < e.scores[i] && i < NUM_SCORES; i++);
	for (int j = NUM_SCORES - 1; j > i; j--) {
		e.scores[j] = e.scores[j-1];
	}
	e.scores[i] = e.score;
}

/**
 * Função que mostra ao jogador que perdeu num determinado nível do jogo.\n
 * Apresentação do top 5 das pontuações dado que a sua pontuação foi superior ou igual à mais baixa.\n
 * Esta informação é exibida na interface do jogo.
 */
void perdeste1(ESTADO e) {
	char link[MAX];

	printf("<rect x=%d y=%d width=%f height=%f fill=%s stroke=%s />\n", ESCALA * 11 , 0, ESCALA * 10.0, ESCALA * 10.0, "grey", "black");
	printf("<text x=%f y=%f>\n%s\n</text>\n", ESCALA * 14.6, ESCALA * 1.5, "Nivel Falhado");
	printf("<text x=%f y=%f>\n%s\n</text>\n", ESCALA * 14.0, ESCALA * 2.9, "Ranking: Top 5 Scores:");

	imprimir_ranking_scores(e);

	sprintf(link,"http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Inicio", 0, 0);
	ABRIR_LINK(link);
    printf("<text x=%f y=%f>\n%s\n</text>\n", ESCALA * 14.3, ESCALA * 8.7, "Tentar novamente ...");
    FECHAR_LINK;
}

/**
 * Função que mostra ao jogador que perdeu num determinado nível do jogo.\n
 * Não é apresentadoo top 5 das pontuações dado que a sua pontuação foi inferior à mais baixa.\n
 * Esta informação é exibida na interface do jogo. 
 */
void perdeste2() {
	char link[MAX];

	printf("<rect x=%d y=%d width=%f height=%f fill=%s stroke=%s />\n", ESCALA * 11 , 0, ESCALA * 10.0, ESCALA * 10.0, "grey", "black");
	sprintf(link,"http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Inicio", 0, 0);
	ABRIR_LINK(link);
    printf("<text x=%f y=%f>\n%s\n</text>\n", ESCALA * 14.3, ESCALA * 8.7, "Tentar novamente ...");
    FECHAR_LINK;
}

/**
\brief Função que imprime um estado
@param e estado
*/
void imprime_estado(ESTADO e) {
	if (e.mostrar_ecra == 0) {
		imprimir_tabuleiro();
		imprimir_entrada(e);
		imprimir_jogador(e);
		imprimir_saida(e);
		imprimir_inimigos(e);
		imprimir_obstaculos(e);
		imprimir_score(e);
		imprimir_vida_jogador(e);
		imprimir_nivel(e);
		imprime_regressar_menu_jogo();
	}

	else if (e.mostrar_ecra == 1) {
		imprime_menu();
	}

	else if (e.mostrar_ecra == 2) {
		imprime_melhores_scores(e);
	}

	else if (e.mostrar_ecra == 3) {
		imprime_ajuda();
	}
}

/**
 * Função main do programa RogueLike.
 * @return 0
 */
int main()
{
	/*int valor = 0;
	char NIVEL[1024] = {0};
	char SCORE[1024] = {0};
	char VALOR[1024] = {0};*/

	srandom(time(NULL));

	ESTADO e = ler_estado(getenv("QUERY_STRING"));

	COMECAR_HTML;
	ABRIR_SVG(1300,650);

	/*e = ler_acao(e);

	imprimir_tabuleiro();
	imprime_jogador(e);
	imprime_vida_jogador(e);
	imprime_inimigos(e);
	imprime_obstaculo(e);
	imprime_saida(e);

	valor = menor_valor_ranking(e);

	sprintf(NIVEL, "Nivel: %d", e.nivel);
	TEXTO(0.0, 11.20, ESCALA, NIVEL);

	sprintf(SCORE, "Score: %d", e.score);
	TEXTO(0.0, 11.65, ESCALA, SCORE);

	sprintf(VALOR, "Menor score ranking: %d", valor);
	TEXTO(5.7, 11.20, ESCALA, VALOR);

	if(e.vida_jogador == 0 && e.score < valor) {
		perdeste2();
	}
	else if (e.vida_jogador == 0 && e.score >= valor) {
		perdeste1(e);
		atualizar_ranking_scores(e);
	}*/

	imprime_estado(e);

	FECHAR_SVG;
	
	return 0;
}