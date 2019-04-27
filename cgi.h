#ifndef ___CGI_H___
#define ___CGI_H___

#include <stdio.h>

/**
@file cgi.h
Macros úteis para gerar CGIs
*/

/**
\brief Caminho para as imagens
*/
#define IMAGE_PATH								"http://localhost/Imagens/"

/**
\brief Macro para começar o html
*/
#define COMECAR_HTML							printf("Content-Type: text/html\n\n")

/**
\brief Macro para abrir um svg
@param tamx O comprimento do svg
@param tamy A altura do svg
*/
#define ABRIR_SVG(tamx, tamy)					printf("<svg width=%f height=%f>\n", tamx, tamy)

/**
\brief Macro para fechar um svg
*/
#define FECHAR_SVG								printf("</svg>\n")

/**
\brief Macro para criar uma imagem
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala da imagem
@param FICHEIRO O caminho para o link do ficheiro
*/
#define IMAGEM(X, Y, ESCALA, FICHEIRO)			printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
														ESCALA * X, ESCALA * Y, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)

/**
\brief Macro para criar um quadrado vermelho
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala do quadrado
*/
#define QUADRADO(X, Y, ESCALA, COLOR)			printf("<rect x=%d y=%d width=%d height=%d opacity=0.25 style=fill:%s />\n", \
														ESCALA * X, ESCALA* Y, ESCALA, ESCALA, COLOR)

/**
\brief Macro para criar um quadrado transparente
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala do quadrado
*/
#define QUADRADO_TRANSPARENTE(X, Y, ESCALA)		printf("<rect x=%d y=%d width=%d height=%d opacity=0 />\n", \
														ESCALA * X, ESCALA* Y, ESCALA, ESCALA)

/**
\brief Macro para criar texto
@param X A coordenada X do canto inferior esquerdo
@param Y A coordenada Y do canto inferior esquerdo
@param FILL A cor do texto
@param TEXTO O texto para escrever
*/
#define TEXTO(X, Y, FILL, TIPO, TEXTO)			printf("<text x=%f y=%f fill=%s font-weight=%s font-size=\"18\" font-family=\"Arial\" >%s</text>\n", \
														X, Y, FILL, TIPO, TEXTO)

/**
\brief Macro para abrir um link
@param link O caminho para o link
*/
#define ABRIR_LINK(link)						printf("<a xlink:href=%s>\n", link)

/**
\brief Macro para fechar um link
*/
#define FECHAR_LINK								printf("</a>\n")

#endif
