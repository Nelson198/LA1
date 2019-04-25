#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/**
 * @file estado.h
 * Definição do estado e das funções que convertem estados em strings e vice-versa.
*/

/** O nº máximo de inimigos. */
#define MAX_INIMIGOS		100

/** O nº máximo de obstáculos. */
#define MAX_OBSTACULOS		100

/** Número máximo de pontuações */
#define NUM_SCORES			10

/**
 * Estrutura que armazena uma posição.
*/
typedef struct posicao {
	int x;
	int y;
} POSICAO;

/**
 * Estrutura de dados que armazena informação relatica a uma ação.
 */
typedef struct action {
    char nome[100];
    int x;
    int y;
} ACTION;

/**
 * Estrutura que armazena o estado do jogo.
*/
typedef struct estado {
	/** Puntuação atual */
	int score;
	/** Array com as melhores 10 pontuações */
	int scores[NUM_SCORES];
	/** Nivel atual do jogo */
	int nivel;
	/** Vida atual do jogador */
	int vida_jogador;
	/** O nº de inimigos */
	int num_inimigos;
	/** O nº de obstáculos */
	int num_obstaculos;
	/** A posição do jogador */
	POSICAO jogador;
	/** A posição da saida */
	POSICAO saida;
	/** Array com a posição dos inimigos */
	POSICAO inimigo[MAX_INIMIGOS];
	/** Array com a posição dos obstáculos */
	POSICAO obstaculo[MAX_OBSTACULOS];
	/** Ação */
	ACTION action;

} ESTADO;

/**
 * Função que converte um estado numa string.
 * @param e O estado
 * @return A string correspondente ao estado e.
*/
char *estado2str(ESTADO e);

/**
 * Função que converte uma string num estado.
 * @param argumentos Uma string contendo os argumentos passados à CGI
 * @return O estado correspondente à string dos argumentos.
*/
ESTADO str2estado(char *argumentos);

/**
 * Função que grava o estado no ficheiro "estado".
 * @param e Estado
 */
void estado2ficheiro(ESTADO e);

#endif