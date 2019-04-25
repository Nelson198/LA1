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

/** Número inicial de vidas do jogador */
#define VIDAS				10

/** Caminho do ficheiro de estado */
#define FICHEIRO_ESTADO		"/var/www/html/estado"

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
typedef struct acao {
    char nome[100];
    int x;
    int y;
} ACAO;

/**
 * Estrutura que armazena o estado do jogo.
*/
typedef struct estado {
	/** Puntuação atual */
	int score_atual;
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
	/** Ecrã a ser exibido (0 tabuleiro, 1 menu, 2 melhores scores, 3 ajuda) */
	int mostrar_ecra;
	/** A posição do jogador */
	POSICAO jogador;
	/** A posição da entrada */
	POSICAO entrada;
	/** A posição da saida */
	POSICAO saida;
	/** Array com a posição dos inimigos */
	POSICAO inimigos[MAX_INIMIGOS];
	/** Array com a posição dos obstáculos */
	POSICAO obstaculos[MAX_OBSTACULOS];
	/** Ação */
	ACAO acao;

} ESTADO;

/**
 * Função que converte o conteúdo do ficheiro de estado num estado.
 * @return Estado correspondente ao conteúdo do ficheiro de estado.
*/
ESTADO ficheiro2estado();

/**
 * Função que converte um estado no ficheiro de estado.
 * @param e Estado
*/
void estado2ficheiro(ESTADO e);

/**
 * Função que lê o estado do programa, isto é, o URL.
 * @param args String do estado
 * @return Estado.
 */
ESTADO ler_estado(char *args);

/**
 * Função que aplica uma ação ao ficheiro de estado
 * @param acao a ação a aplicar
 * @param x coordenada x
 * @param y coordenada y
*/
void aplicar_acao(char *acao, int x, int y);

#endif