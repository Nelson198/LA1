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
#define SIZE 10

/**
 * Nº de obstáculos.
 */
#define NUM_OBS 20

/**
 * Nº de inimigos.
 */
#define NUM_INI 10

/**
 * Função que insere um elemento x num array de inteiros previamente ordenado.
 * @param x Elemento a ser inserido
 * @param v Vetor relativo ao top 5 de pontuações
 */
void insere(int x, int v[5]) {
	int i = 0, N = 5, j;
	while(v[i] > x && i < N) {
		i++;
	}
	for (j = N-1; j > i; j--) {
		v[j] = v[j-1];
	}
	v[i] = x;
}

/**
 * Função que verifica se o ponto (x,y) representa uma coordenada válida no tabuleiro.
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 --> False;\n
 *         1 --> True.
 */
int posicao_valida(int x, int y) {
	return x >= 0 && y >= 0 && x < SIZE && y < SIZE;
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
 * Função que verifica se a posição (x,y) está ocupada por um inimigo.
 * @param e Estado atual do programa
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 --> False;\n
 *         1 --> True.
 */
int posicao_inimigo(ESTADO e, int x, int y) {
	int i;
	for(i = 0; i < e.num_inimigos; i++) {
		if(mesma_posicao(e.inimigo[i], x, y)) {
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
int posicao_saida(ESTADO e, int x, int y) {
	return mesma_posicao(e.saida, x, y);
}
/**
 * Função que vai colorir a/as quadrícula/as para onde é possivel o jogador movimentar-se.
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 */
void colour(int x, int y) {
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
 * Função que gera quadrículas com cores alternadas.
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 */
void quadrado(int x, int y) {
	char *cor[] = { "#000000" , "#FFFFFF" };
	int idx = (x + y) % 2;
	QUADRADO(x, y, ESCALA, cor[idx]);
}

/**
 * Função que gera o tabuleiro de jogo.
 */
void tabuleiro() {
	for(int x = 0; x < SIZE; x++) {
		for(int y = 0; y < SIZE; y++) {
			quadrado(x, y);
		}
	}
}

/**
 * Função que verifica se a posição (x,y) está ocupada pelo jogador ou por um inimigo ou por um obstáculo.
 * @param e Estado atual do programa
 * @param x Abcissa do ponto
 * @param y Ordenada do ponto
 * @return 0 -> Livre;\n
 *         1 -> Ocupado.
 */
int invariante(ESTADO e, int x, int y) {
	int i;
	/* Verifica jogador */
	if(e.jog.x == x && e.jog.y == y) {
		return 1;
	}

	/* Verifica obstáculos */
	i = 0;
	while((e.obstaculo[i].x != x || e.obstaculo[i].y != y) && i < e.num_obstaculos) {
		i++;
	}
	if(i < e.num_obstaculos) {
		return 1;
	}
	else {
		i = 0;
		/* Verifica os inimigos */
		while((e.inimigo[i].x != x || e.inimigo[i].y != y) && i < e.num_inimigos) {
			i++;
		}

		if(i < e.num_inimigos) {
			return 1;
		}
	}
	return 0;
}

/**
 * Função que ...\n
 * --> gera aleatoriamente as posições dos inimigos e dos obstáculos;\n
 * --> gera aleatoriamente a posição da saída de um determinado nível ao longo da 1ª linha do tabuleiro;\n
 * --> fixa na posição (5,9) o ponto de partida do jogador;\n
 * --> define a vida inicial do jogador;\n
 * --> define o nível inicial do jogo;\n
 * --> define a pontuação inicial do jogador.
 * @return Estado.
 */
ESTADO inicializar() {
	int x, y; 
	ESTADO e = estadoAzero;
	srandom(getpid() + time(NULL));

	/*Jogador-------------------------------*/
	e.jog.x = 5;
	e.jog.y = 9;

	/*Inimigos------------------------------*/
	e.num_inimigos = NUM_INI;
	x = 0, y = 0;
	for(int i = 0; i < e.num_inimigos; i++)
	{
		while(invariante(e,x,y) == 1)
		{
			x = random() % SIZE;
			y = random() % 7;
		}
		e.inimigo[i].x = x;
		e.inimigo[i].y = y;
	}

	/*Obstáculos----------------------------*/
	e.num_obstaculos = NUM_OBS;
	x = 0; y = 0;
	for(int i = 0; i < e.num_obstaculos; i++)
	{
		while(invariante(e,x,y) == 1)
		{
			x = random() % SIZE;
			y = random() % SIZE;
		}
		e.obstaculo[i].x = x;
		e.obstaculo[i].y = y;
	}

	/*Saida---------------------------------*/
	x = random() % SIZE; y = 0;
	while(invariante(e, x, y) == 1)
	{
		x = random() % SIZE;
	}
	e.saida.x = x;
	e.saida.y = y;

	/*Nível---------------------------------*/
	e.nivel = 1;

	/*Score---------------------------------*/
	e.score = 0;

	/*Vida do Jogador-----------------------*/
	e.vida_jog = 10;

	return e;
}

/**
 * Função que ...\n
 * --> gera aleatoriamente as posições dos inimigos e dos obstáculos;\n
 * --> gera aleatoriamente a posição da saída de um determinado nível ao longo da 1ª linha do tabuleiro;\n
 * --> fixa na posição (5,9) o ponto de partida do jogador;\n
 * --> atualiza a vida inicial do jogador;\n
 * --> atualiza o nível inicial do jogo;\n
 * --> atualiza a pontuação inicial do jogador.\n\n
 * Nota: Esta função começa a ser utilizada a partir da passagem do 1º para o 2º nivel.
 * @param e Estado atual
 * @return Estado novo.
 */
ESTADO novo_nivel(ESTADO e) {
	int x, y;
	ESTADO novo = estadoAzero;
	srandom(getpid() + time(NULL));

	/*Jogador-------------------------------*/
	novo.jog.x = 5;
	novo.jog.y = 9;

	/*Inimigos------------------------------*/
	novo.num_inimigos = NUM_INI;
	x = 0, y = 0;
	for(int i = 0; i < novo.num_inimigos; i++)
	{
		while(invariante(novo,x,y) == 1)
		{
			x = random() % SIZE;
			y = random() % 7;
		}
		novo.inimigo[i].x = x;
		novo.inimigo[i].y = y;
	}

	/*Obstáculos----------------------------*/
	novo.num_obstaculos = NUM_OBS;
	x = 0; y = 0;
	for(int i = 0; i < novo.num_obstaculos; i++)
	{
		while(invariante(novo,x,y) == 1)
		{
			x = random() % SIZE;
			y = random() % SIZE;
		}
		novo.obstaculo[i].x = x;
		novo.obstaculo[i].y = y;
	}

	/*Saida---------------------------------*/
	x = random() % SIZE; y = 0;
	while(invariante(novo,x,y) == 1)
	{
		x = random() % SIZE;
	}
	novo.saida.x = x;
	novo.saida.y = y;

	/*Nível---------------------------------*/
	novo.nivel = e.nivel + 1;

	/*Score---------------------------------*/
	novo.score = e.score + 20;

	/*Vida do Jogador-----------------------*/
	novo.vida_jog = 10;

	return novo;
}

/**
 * Função que desloca os inimigos de acordo com a posição do jogador.
 * @param e Estado atual
 * @return Estado novo.
 */ 
ESTADO movimento_inimigos(ESTADO e) {
	int x1, y1, dx, dy, dist;
	int x = e.jog.x, y = e.jog.y;

	for (int i = 0; i < ((int)e.num_inimigos); i++) {

		/* Coordenadas do inimigo */
		x1 = e.inimigo[i].x, y1 = e.inimigo[i].y;

		dx = (x == x1)? 0 : ((x > x1)? 1 : -1);
		dy = (y == y1)? 0 : ((y > y1)? 1 : -1);

		dist = (abs(x-x1) > abs(y-y1))? abs(x-x1): abs(y-y1);

		if (!invariante(e, x1 + dx, y1 + dy) && !posicao_saida(e, x1 + dx, y1 + dy)) {
			x1 += dx;
			y1 += dy;
		}
		else if (!invariante(e, x1, y1+dy) && !posicao_saida(e, x1+dx, y1+dy)) {
			y1 += dy;
		}
		else if (!invariante(e, x1+dx, y1) && !posicao_saida(e, x1+dx, y1+dy)) {
			x1 += dx;
		}
		if (dist > 1) {
			e.inimigo[i].x = x1;
			e.inimigo[i].y = y1;
		}
		else {
			e.vida_jog--;
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
	int x = e.jog.x + dx, y = e.jog.y + dy;

    char link[MAX];

    if(!posicao_valida(x, y)) {
        return;
    }
	
    if(posicao_inimigo(e, x, y)) {
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

    if(invariante(e, x, y) == 1) {
        return;
    }
	/* Se o jogador estiver perto da saída, a casa desta será assinalada e clicável */
    if (x == e.saida.x && y == e.saida.y) {
        sprintf(link, "http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Movimento_saida", 0, 0);
        ABRIR_LINK(link);
        colour(x,y);
        IMAGEM(e.saida.x, e.saida.y, ESCALA, "trapdoor1.png");
        FECHAR_LINK;
    }
	/* Se não se verificar nenhum dos casos anteriores, o jogador pode se movimentar para (x,y), sendo que esta casa será sinalizada e clicável */
    else {
        sprintf(link,"http://localhost/cgi-bin/RogueLike?%s,%d,%d", "Movimento_jogador", x, y);
        ABRIR_LINK(link);
        quadrado(x,y);
        colour(x,y);
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
 * @param Estado atual
 */ 
void imprime_jogador(ESTADO e) {
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "character_21.png");
    imprime_movimentos_jogador(e);
}

/**
 * Função que imprime as imagens das vidas do jogador na interface do jogo.
 * @param Estado atual
 */ 
void imprime_vida_jogador(ESTADO e) {
	int num_vidas = (int) e.vida_jog;
	for (int i = 0; i < num_vidas; i++) {
		IMAGEM(i, 26, 20, "heart.png");
	}
}

/**
 * Função que imprime as imagens dos inimigos no tabuleiro.
 * @param Estado atual
 */ 
void imprime_inimigos(ESTADO e) {
	for(int i = 0; i < e.num_inimigos; i++) {
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "dwarf_12.png");
	}
}

/**
 * Função que imprime as imagens dos obstáculos no tabuleiro.
 * @param Estado atual
 */ 
void imprime_obstaculo(ESTADO e) {
	for (int i = 0; i < e.num_obstaculos; i++) {
        IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "lava_pool1.png");
	}
}

/**
 * Função que imprime a imagem da saída no tabuleiro.
 * @param Estado atual
 */ 
void imprime_saida(ESTADO e) {
	IMAGEM(e.saida.x, e.saida.y, ESCALA, "trapdoor1.png");
}

/**
 * Função que imprime o top 5 das pontuações na interface do jogo.
 */
void imprime_score_web() {
	int i, flag, vetor[5] = {0};
	FILE *pt;
	/* pt é um apontador que fica encarregado de ler o ficheiro "score" */
	pt = fopen("/var/www/html/pontuações","r");

	/* Caso em que o ficheiro "score" não existe. pt aponta para NULL */
	if(pt == NULL) {
		perror("Não consegui ler o ficheiro com o Score");
	}
	else {
		flag = 1;
		/* Leitura dos valores de scores. */
		for (i = 0; i < 5 && flag != -1; i++) {
			flag = fscanf(pt, "%d\n", &vetor[i]);
		}
		/* Escrita dos scores na interface do jogo. */
		for(i = 0; i < 5; i++) {
			char buf[7];
			sprintf(buf, "%d", vetor[i]);
			TEXTO(15.6, 4.0 + (i*42), ESCALA, buf);
		}
	}
	fclose(pt);
}

/** 
 * Função que, dada uma pontuação, atualiza o top 5 das pontuações imprimindo o mesmo no ficheiro "score".
 * @param score Pontuação
 */
void imprime_score_ficheiro(int score)
{
	FILE *pt;
	int i, flag;
	int vetor[5] = {0};

	/* pt é um apontador que fica encarregado de ler o ficheiro "score" */
	pt = fopen("/var/www/html/pontuações","r");

	/* Caso em que o ficheiro "score" não existe. pt aponta para NULL */
	if(pt == NULL) {
		perror("Não consegui ler o ficheiro com o Score");
		/* Dado que não existe, cria-se o ficheiro, imprimindo o score nele */
		pt = fopen("/var/www/html/pontuações", "w");
		fprintf(pt, "%d\n", score);
	}
	else {
		flag = 1;
		/* Leitura dos valores dos scores */
		for (i = 0; i < 5 && flag != -1; i++) {
			flag = fscanf(pt, "%d\n", &vetor[i]);
		}

		insere(score, vetor);

		/* Output dos scores no ficheiro segundo a ordem do vetor (decrescente) */
		pt = freopen("/var/www/html/pontuações","w", pt);
		for(i = 0; i < 5; i++) {
			fprintf(pt, "%d\n", vetor[i]);
		}
	}
	fclose(pt);
}

/**
 * Função que extrai o menor valor do top 5 das pontuações.
 * @return Menor valor do top 5 das pontuações.
 */
int menor_valor_ranking()
{
	FILE *pt;
	int i, flag, n = 0;
	int vetor[5] = {0};

	pt = fopen("/var/www/html/pontuações","r"); /* pt é um apontador que fica encarregado de ler o ficheiro "score" */

	if(pt == NULL) /* Caso em que o ficheiro "score" não existe. pt aponta para NULL */
	{
		perror("Não consegui ler o ficheiro com o Score"); 
	}
	else
	{
		flag = 1;
		for (i = 0; i < 5 && flag != -1; i++)
		{
			flag = fscanf(pt, "%d\n", &vetor[i]); /* Leitura dos valores do top 5 do ranking de scores */
		}
		n = vetor[4]; /* Como está ordenado de forma decrescente, n ficará com o menor valor deste ranking */
	}
	fclose(pt);
	return n;
}

/**
 * Função que mostra ao jogador que perdeu num determinado nível do jogo.\n
 * Apresentação do top 5 das pontuações dado que a sua pontuação foi superior ou igual à mais baixa.\n
 * Esta informação é exibida na interface do jogo.
 */
void perdeste1()
{
	char link[MAX];

	printf("<rect x=%d y=%d width=%f height=%f fill=%s stroke=%s />\n", ESCALA * 11 , 0, ESCALA * 10.0, ESCALA * 10.0, "grey", "black");
	printf("<text x=%f y=%f>\n%s\n</text>\n", ESCALA * 14.6, ESCALA * 1.5, "Nivel Falhado");
	printf("<text x=%f y=%f>\n%s\n</text>\n", ESCALA * 14.0, ESCALA * 2.9, "Ranking: Top 5 Scores:");

	imprime_score_web();

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
void perdeste2()
{
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
    		perror("Não consegui abrir o ficherio do estado");
    		e = inicializar();
    	}

    	else {
    		r = fread(&e, sizeof(ESTADO), 1, fp);
      		if(r < 1) {
      			e = inicializar();
      		}
      		else{
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
ESTADO move_jog(ESTADO e) {
	e.jog.x = e.action.x;
	e.jog.y = e.action.y;
	return e;
}

/**
 * Função que trata de matar os inimigos, removendo-os do tabuleiro, incrementando o score.
 * @param e Estado atual
 * @return Estado novo.
 */
ESTADO mata_inimigos(ESTADO e){
	int flag = 1;
	for(int i = 0; i < e.num_inimigos && flag; i++) {
		if (e.action.x == e.inimigo[i].x && e.action.y == e.inimigo[i].y) {
			e.inimigo[i] = e.inimigo[e.num_inimigos];
			e.num_inimigos--;
			e.score += 10;
			flag = 0;
		}	
	}
	return e;
}

/**
 * Função que consoante a ação do jogador, realiza uma determinada tarefa.
 * @param e Estado atual
 * @return Estado novo.
 */
ESTADO nova_acao(ESTADO e) {
    if(strcmp(e.action.nome, "Movimento_jogador") == 0) {
        e = move_jog(e);
    }

    if(strcmp(e.action.nome, "Movimento_saida") == 0) {
        e = novo_nivel(e);
    }

    if(strcmp(e.action.nome, "Mata") == 0) {
        e = mata_inimigos(e);
    }

	if(strcmp(e.action.nome,"Inicio") == 0) {
        return inicializar();
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

	ESTADO e = ler_estado(getenv("QUERY_STRING"));

	COMECAR_HTML;
	ABRIR_SVG(1300,650);

	e = nova_acao(e);

	tabuleiro();
	imprime_inimigos(e);
	imprime_obstaculo(e);
	imprime_saida(e);

	valor = menor_valor_ranking();

	sprintf(NIVEL, "Nivel: %d", e.nivel);
	TEXTO(0.0, 11.20, ESCALA, NIVEL);

	sprintf(SCORE, "Score: %d", e.score);
	TEXTO(0.0, 11.65, ESCALA, SCORE);

	sprintf(VALOR, "Menor score ranking: %d", valor);
	TEXTO(5.7, 11.20, ESCALA, VALOR);

	if (e.vida_jog > 0) {
		imprime_jogador(e);
		imprime_vida_jogador(e);
	}
	else if(e.score < valor) {
		perdeste2();
	}
	else {
		imprime_score_ficheiro(e.score);
		perdeste1();
	}

	FECHAR_SVG;

	estado2ficheiro(e);

	return 0;
}