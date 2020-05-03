/*
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
 *
 * Aqui estao declarados o tipo CelST e o prototipo das funcoes 
 * que manipulam a tabela de simbolos.
 *  
 */

#ifndef _ST_H
#define _ST_H

#include "categorias.h" /* Categoria, FLOAT, INT, STR, BOOL */
#include "util.h"       /* String,  */
#include "objetos.h"    /* Valor, CelObjeto */
 
/*----------------------------------------------------------*/
/* estrutura basica da tabela de simbolos */
typedef struct celST CelST;
struct celST 
{
    /* ponteiro para o nome da variavel */
    String nomeVar; 

    /* tipo da variavel (neste EP e' sempre FLOAT) */
    Categoria tipoVar;

    /* valor da variavel (neste EP e' sempre um double) */
    Valor valorVar;

    /* proxima celula da tabela de simbolos */
    CelST *proxVar; 
};

/*-----------------------------------------------------------*/
/* prototipos das funcoes que manipulam a tabela de simbolos */
void  
initST();

CelObjeto *
getValorST(char *nomeVar);

void  
setValorST(char *nomeVar, CelObjeto *pValor);

void  
showST(); 

void  
freeST();

#endif /* _ST_H */
