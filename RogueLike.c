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
		if(mesma_posicao(e.inimigo[i], x, y)) {
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
		if(mesma_posicao(e.obstaculo[i], x, y)) {
			return 1;
		}
	}
	return 0;
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
 * Função que verifica se a posição (x,y) está ocupada pelo jogador ou por um inimigo ou por um obstáculo ou pela saída do jogo.
 * @param e Estado atual do programa
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 -> Livre;\n
 *         1 -> Ocupado.
 */
int invariante(ESTADO e, int x, int y) {
	if(tem_jogador(e, x, y)) return 1;
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
	e.jogador.x = TAMANHO / 2;
	e.jogador.y = TAMANHO - 1;

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
		e.inimigo[e.num_inimigos++] = p;
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
		e.obstaculo[e.num_obstaculos++] = p;
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
 * Função que inicializa o estado do jogo.
 * @return Estado.
 */
ESTADO inicializar() {
	ESTADO e;

	e = inicializar_jogador(e);
	e = inicializar_saida(e);
	e = inicializar_obstaculos(e);
	e = inicializar_inimigos(e);

	e.nivel = 1;
	e.score = 0;
	e.vida_jogador = 10;

	return e;
}

/**
 * Função que atualiza o estado do jogo a partir da passagem do 1º para o 2º nivel.
 * @param e Estado atual
 * @return Estado novo.
 */
ESTADO novo_nivel(ESTADO e) {
	ESTADO novo;

	novo = inicializar_jogador(novo);
	novo = inicializar_saida(e);
	novo = inicializar_obstaculos(novo);
	novo = inicializar_inimigos(novo);

	novo.nivel = e.nivel + 1;
	novo.score = e.score + 20;
	novo.vida_jogador = 10;

	return novo;
}

/**
 * Função que desloca os inimigos de acordo com a posição do jogador.
 * @param e Estado atual
 * @return Estado novo.
 */ 
ESTADO movimento_inimigos(ESTADO e) {
	int x = e.jogador.x, y = e.jogador.y;

	for (int i = 0; i < ((int)e.num_inimigos); i++) {

		/* Coordenadas do inimigo */
		int x1 = e.inimigo[i].x, y1 = e.inimigo[i].y;

		int dx = (x == x1)? 0 : ((x > x1)? 1 : -1);
		int dy = (y == y1)? 0 : ((y > y1)? 1 : -1);

		int dist = (abs(x-x1) > abs(y-y1))? abs(x-x1): abs(y-y1);

		if (!invariante(e, x1 + dx, y1 + dy) && !tem_saida(e, x1 + dx, y1 + dy)) {
			x1 += dx;
			y1 += dy;
		}
		else if (!invariante(e, x1, y1+dy) && !tem_saida(e, x1+dx, y1+dy)) {
			y1 += dy;
		}
		else if (!invariante(e, x1+dx, y1) && !tem_saida(e, x1+dx, y1+dy)) {
			x1 += dx;
		}
		if (dist > 1) {
			e.inimigo[i].x = x1;
			e.inimigo[i].y = y1;
		}
		else {
			e.vida_jogador--;
		}
	}
	return e;
}

/**
 * Função que, dada uma posição possível do jogador, sinaliza-a com uma determinada cor e cria um link com a informação do futuro estado.
 * @param Estado atual
 * @param dx Deslocação na abcissa do ponto atual do jogador
 * @param dy Deslocação na ordenada do ponto atual do jogador
 */
void movimento(ESTADO e, int dx, int dy) {
    /* (x,y) da quadrícula que se encontra à volta da posição do jogador */
	int x = e.jogador.x + dx, y = e.jogador.y + dy;

    char link[MAX];

    if(!posicao_valida(x, y)) {
        return;
    }

    else if(tem_obstaculo(e, x, y)) {
        return;
    }
	
    else if(tem_inimigo(e, x, y)) {
        for (int i = 0; i < e.num_inimigos; i++) {
			/* Se na casa (x,y) estiver um inimigo, esta será sinalizada e clicável de forma a que o jogador saiba que o pode matar */ 
            if (x == e.inimigo[i].x && y == e.inimigo[i].y) {
                sprintf(link,"http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Mata", e.inimigo[i].x, e.inimigo[i].y);
                ABRIR_LINK(link);
                colour2(x,y);
                IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "dwarf_12.png");
                FECHAR_LINK;
            }
        }
    }

	/* Se o jogador estiver perto da saída, a casa desta será assinalada e clicável */
    else if (tem_saida(e, x, y)) {
        sprintf(link, "http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Movimento_saida", 0, 0);
        ABRIR_LINK(link);
        colour1(x,y);
        IMAGEM(e.saida.x, e.saida.y, ESCALA, "trapdoor1.png");
        FECHAR_LINK;
    }

	/* Se não se verificar nenhum dos casos anteriores, o jogador pode se movimentar para (x,y), sendo que esta casa será sinalizada e clicável */
    else {
        sprintf(link,"http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Movimento_jogador", x, y);
        ABRIR_LINK(link);
        colour1(x,y);
        FECHAR_LINK;
    }
}

/**
 * Função que averigua o ponto de situação de todas as quadrículas para onde o jogador se pode movimentar.
 * Nota: O jogador pode-se deslocar para os lados ou para cima/baixo ou nas diagonais.
 * @param Estado atual
 */
void imprime_movimentos_jogador(ESTADO e) {
	movimento(e,  0, -1);
	movimento(e,  0, +1);
	movimento(e, -1,  0);
	movimento(e, +1,  0);
	movimento(e, -1, -1);
	movimento(e, -1, +1);
	movimento(e, +1, -1);
	movimento(e, +1, +1);
}

/**
 * Função que imprime a imagem do jogador no tabuleiro e as possivéis jogadas do mesmo.
 * @param e Estado atual
 */ 
void imprime_jogador(ESTADO e) {
	IMAGEM(e.jogador.x, e.jogador.y, ESCALA, "character_21.png");
    imprime_movimentos_jogador(e);
}

/**
 * Função que imprime as imagens das vidas do jogador na interface do jogo.
 * @param e Estado atual
 */ 
void imprime_vida_jogador(ESTADO e) {
	for (int i = 0; i < e.vida_jogador; i++) {
		IMAGEM(i, 26, 20, "heart.png");
	}
}

/**
 * Função que imprime as imagens dos inimigos no tabuleiro.
 * @param e Estado atual
 */ 
void imprime_inimigos(ESTADO e) {
	for(int i = 0; i < e.num_inimigos; i++) {
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "dwarf_12.png");
	}
}

/**
 * Função que imprime as imagens dos obstáculos no tabuleiro.
 * @param e Estado atual
 */ 
void imprime_obstaculo(ESTADO e) {
	for (int i = 0; i < e.num_obstaculos; i++) {
        IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "lava_pool1.png");
	}
}

/**
 * Função que imprime a imagem da saída no tabuleiro.
 * @param e Estado atual
 */ 
void imprime_saida(ESTADO e) {
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
 * Função que extrai o menor valor do top 5 das pontuações.
 * @return Menor valor do top 5 das pontuações.
 */
int menor_valor_ranking(ESTADO e) {
	return e.scores[NUM_SCORES - 1];
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
 * Função que lê o estado do programa.
 * @param args String do estado
 * @return Estado.
 */
ESTADO ler_estado(char *args)
{
    ESTADO e;
    int r;

	/* Se a QUERY_STRING for nula, dá-se a inicialização do jogo */
    if(strlen(args) == 0) {
        e = inicializar();
    }
    else { 
    	FILE *fp;
    	ACTION action;

		/* Leitura da ação do jogador e os respetivos valores x, y. */
    	sscanf(args, "%[^,],%d,%d", action.nome, &action.x, &action.y);
		/* fp é um apontador que fica encarregado de ler o ficheiro "score" */
    	fp = fopen("/var/www/html/estado","r");
    	
		/* Caso em que o ficheiro "estado" não existe. fp aponta para NULL */
    	if(fp == NULL) {
    		perror("Não consegui abrir o ficherio do estado!");
    		e = inicializar();
    	}

    	else {
    		r = fread(&e, sizeof(ESTADO), 1, fp);
      		if(r < 1) {
      			e = inicializar();
      		}
      		else {
      			e.action = action;
      		}
    		fclose(fp);
    	}
    }

    return e;
}

/**
 * Função que trata de deslocar o jogador para as cordenadas indicadas.
 * @param e Estado atual
 * @return Estado novo.
 */ 
ESTADO move_jogador(ESTADO e) {
	e.jogador.x = e.action.x;
	e.jogador.y = e.action.y;
	return e;
}

/**
 * Função que trata de matar os inimigos, removendo-os do tabuleiro, incrementando o score.
 * @param e Estado atual
 * @return Estado novo.
 */
ESTADO mata_inimigos(ESTADO e){
	for(int i = 0; i < e.num_inimigos; i++) {
		if (e.action.x == e.inimigo[i].x && e.action.y == e.inimigo[i].y) {
			e.inimigo[i] = e.inimigo[e.num_inimigos--];
			e.score += 10;
			break;
		}	
	}
	return e;
}

/**
 * Função que consoante a ação do jogador, realiza uma determinada tarefa.
 * @param e Estado atual
 * @return Estado novo.
 */
ESTADO ler_acao(ESTADO e) {
	if(strcmp(e.action.nome, "Inicio")) {
        return inicializar();
    }
    if(strcmp(e.action.nome, "Movimento_jogador")) {
        e = move_jogador(e);
    }
    if(strcmp(e.action.nome, "Movimento_saida")) {
        e = novo_nivel(e);
    }
    if(strcmp(e.action.nome, "Mata")) {
        e = mata_inimigos(e);
    }

    e = movimento_inimigos(e);
    
    return e;
}

/**
 * Função main do programa RogueLike.
 * @return 0
 */
int main()
{
	int valor = 0;
	char NIVEL[1024] = {0};
	char SCORE[1024] = {0};
	char VALOR[1024] = {0};

	srandom(time(NULL));

	ESTADO e = ler_estado(getenv("QUERY_STRING"));

	COMECAR_HTML;
	ABRIR_SVG(1300,650);

	e = ler_acao(e);

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
	}

	FECHAR_SVG;

	estado2ficheiro(e);
	
	return 0;
}