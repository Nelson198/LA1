#ifndef ___CGI_H___
#define ___CGI_H___

/**
 * @file cgi.h
 * Macros úteis para gerar CGIs.
*/

#include <stdio.h>

/**
* Caminho para as imagens.
*/
#define IMAGE_PATH												"http://localhost/Imagens/"

/**
 * Macro para começar o html.
*/
#define COMECAR_HTML											printf("Content-Type: text/html\n\n")

/**
 * Macro para abrir um svg.
 * @param tamx O comprimento do svg
 * @param tamy A altura do svg
*/
#define ABRIR_SVG(tamx, tamy)									printf("<svg width=%d height=%d>\n", tamx, tamy)

/**
 * Macro para fechar um svg.
*/
#define FECHAR_SVG												printf("</svg>\n")

/**
 * Macro para criar uma imagem.
 * @param X A coordenada X do canto superior esquerdo
 * @param Y A coordenada Y do canto superior esquerdo
 * @param ESCALA A escala da imagem
 * @param FICHEIRO O caminho para o link do ficheiro
*/
#define IMAGEM(X, Y, ESCALA, FICHEIRO)							printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", ESCALA * X, ESCALA * Y, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)

/**
 * Macro para criar um quadrado.
 * @param X A coordenada X do canto superior esquerdo
 * @param Y A coordenada Y do canto superior esquerdo
 * @param ESCALA A escala da imagem
 * @param COR A cor de preenchimento do quadrado
*/
#define QUADRADO(X, Y, ESCALA, COR)								printf("<rect x=%d y=%d width=%d height=%d fill=%s />\n", ESCALA * X, ESCALA * Y, ESCALA, ESCALA, COR)

/**
 * Macro para criar um quadrado que vai assinalar as casas para as quais o jogador pode se mover.
 * @param X A coordenada x
 * @param Y A coordenada y
 * @param ESCALA Tamanho do quadrado
 * @param COR A cor do quadrado
 * @param OPACIDADE O quão opacado queremos que o quadrado seja
 * @param ARESTA Cor da aresta do quadrado
*/
#define QUADRADO2(X, Y, ESCALA, COR, OPACIDADE, ARESTA)			printf("<rect x=%d y=%d width=%d height=%d fill=%s opacity=%f stroke=%s />\n", ESCALA * X, ESCALA * Y, ESCALA, ESCALA, COR, OPACIDADE, ARESTA)

/**
 * Macro para criar um quadrado.
 * @param X A coordenada X do canto superior esquerdo
 * @param Y A coordenada Y do canto superior esquerdo
 * @param ESCALA A escala da imagem
*/
#define QUADRADO_TRANSPARENTE(X, Y, ESCALA)						printf("<rect x=%d y=%d width=%d height=%d opacity=0 />\n", ESCALA * X, ESCALA * Y, ESCALA, ESCALA)

/**
 * Macro para criar texto
 * @param X A coordenada X do canto inferior esquerdo
 * @param Y A coordenada Y do canto inferior esquerdo
 * @param FILL A cor do texto
 * @param TEXTO O texto para escrever
*/
#define TEXTO(X, Y, FILL, TEXTO)								printf("<text x=%d y=%d fill=%s>%s</text>\n", X, Y, FILL, TEXTO)

/**
 * Macro para abrir um link.
 * @param link O caminho para o link
*/
#define ABRIR_LINK(link)										printf("<a xlink:href=%s>\n", link)

/**
 * Macro para fechar o link.
*/
#define FECHAR_LINK												printf("</a>\n")

#endif