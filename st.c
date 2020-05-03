/*
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Matheus Barbosa Silva - NUSP: 11221741

  st.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/* interface para o uso da funcao deste módulo */
#include "st.h"      /* CelST */

#include <stdlib.h>  /* free(), NULL */
#include <string.h>  /* strcmp(), strlen(), strncpy() */
#include "util.h"    /* String, mallocSafe() */
#include "objetos.h" /* Valor, CelObjeto */

/*-----------------------------------------------------------
 * A tabela de simbolos dever ser implementada atraves de uma 
 * __lista ligada com cabeca__. 
 *
 * Todas as listas, filas e pilhas neste EP sao implementadas
 * atraves de listas ligadas com cabeca.
 */

/* ponteiro para a celula cabeca da lista que representa a tabela
   de simbolos */
static CelST *ini; 

/* funcao auxiliar */
static CelST *
endVarST(char *nomeVar);

/*-------------------------------------------------------------
 * initST()
 *
 * CRIA uma tabela de simbolos vazia.
 *  
 * Em outras palavras, cria uma lista ligada vazia.
 * A lista ligada e' com cabeca
 * Cada celula da lista e' do tipo CelST.
 */
void 
initST()
{
    /*ALOCA A CABEÇA DA LISTA LIGADA*/
    ini = mallocSafe(sizeof(CelST));
}

/*-------------------------------------------------------------
 *  getValorST
 *
 *  RECEBE um string NOMEVAR que e' o nome de uma variavel.
 *  Procura essa variavel na tabela de simbolos.
 *  Se NOMEVAR esta na tabela de simbolos, entao a funcao CRIA uma
 *  celula do tipo CelObjeto que no campo:
 *
 *      - categoria contem  FLOAT;
 *      - valor.vFloat contem o valor atual da variavel; e
 *      - prox contem NULL.
 *   
 *  A funcao RETORNA o endereco dessa celula.  Se NOMEVAR nao
 *  estiver na tabela de simbolos a funcao RETORNA NULL.
 *
 *  Esta funcao deve utilizar a funcao endVarST().
 */
CelObjeto *
getValorST(char *nomeVar)
{
    CelST *p = endVarST(nomeVar);
    CelObjeto* cel;

    if (p == NULL)
        return NULL;
    cel = mallocSafe(sizeof(*cel));
    cel->categoria = FLOAT;
    cel->valor.vFloat = p->valorVar.vFloat;
    cel->prox = NULL;
    return cel;
}

/*-------------------------------------------------------------
 *  setValorST
 *
 * RECEBE um string NOMEVAR que e' o nome de uma variavel, e
 * um ponteiro PVALOR para uma CelObjeto que representa  um 
 * float, ou seja, na celula o campo 
 * 
 *     - categoria    contem FLOAT;  
 *     - valor.Vfloat contem um double, e
 *     - prox         contem um endereco irrelevante. 
 *
 * Se NOMEVAR nao faz parte da tabela de simbolos a funcao cria
 * uma nova celula do tipo CelST que deve ser inserida na tabela de 
 * simbolos. O campo 
 *
 *     - nomeVar dessa nova celula deve conter uma copia do 
 *       string NOMEVAR.
 * 
 * A funcao ATRIBUI os campos 
 * 
 *    categoria e vFloat 
 * 
 * de pVALOR aos campos 
 * 
 *    tipoVar e valorVar 
 *
 * da celula na tabela de simbolo responsavel por conter o valor
 * da variavel NOMEVAR.
 * 
 * Esta funcao deve utilizar a funcao endVarST().
 *
 * Esta funcao utiliza a funcao mallocSafe().
 *
 * Para copia de strings podem ser utilizadas quaisquer funcoes da 
 * biblioteca string.h: strcpy(), strncpy()...
 * 
 * A funcao _nao_ deve alterar a celula PVALOR.
 */
void
setValorST(char *nomeVar, CelObjeto *pValor)
{
    CelST* variavel = endVarST(nomeVar);
    if( variavel == NULL ){
        variavel = mallocSafe(sizeof(*variavel));
        variavel->nomeVar = mallocSafe(sizeof(String));
        strcpy(variavel->nomeVar, nomeVar);
        variavel->tipoVar = pValor->categoria;
        variavel->proxVar = ini->proxVar;
        ini->proxVar = variavel;
    }
    if (pValor->categoria == FLOAT || pValor->categoria == BOOL)
        variavel->valorVar.vFloat = pValor->valor.vFloat;
    else
        variavel->valorVar.vFloat = getValorST(pValor->nomeID)->valor.vFloat;
    
}

/*-------------------------------------------------------------
 * freeST()
 *
 * Funcao que libera toda a memoria alocada para a tabela
 * de simbolos.
 *
 * Funcao utiliza a funcao free().
 */
void 
freeST()
{
    CelST *p = ini;
    while( p != NULL ){
        ini = ini->proxVar;
        free(p->nomeVar);
        p->nomeVar = NULL;
        free(p);
        p = ini;
    }
}

/*-------------------------------------------------------------
 * showST()
 *
 * Funcao que para cada variavel na tabela de simbolos exibe:
 *
 *      - seu nome; e
 *     - seu valor atual.
 *
 * Exemplo de saida que pode ser produzida por esta funcao.
 * 
 *     ==========================
 *     Tabela de simbolos
 *     'nome': valor
 *     . . . . . . . . . . . . . .
 *     'prova3': 4.5
 *     'prova2': 6.5
 *     'prova1': 7
 *     'soma': 18
 */
void
showST()
{
    CelST *p;

    printf("\n==========================\n");
    printf("  Tabela de simbolos\n");
    printf("  'nome': valor\n");
    printf(". . . . . . . . . . . . . . .\n");
    if(ini->proxVar == NULL){
        printf("  tabela vazia.\n\n");
    }else{
        for(p = ini->proxVar; p!= NULL; p = p->proxVar){
            printf("  '%s': %f\n", p->nomeVar, p->valorVar.vFloat);
        }
        printf("\n");
    }
}

/*-------------------------------------------------------------
 * endVarST
 *
 * RECEBE um string NOMEVAR com o nome de uma variavel e RETORNA
 * o endereco da celula na tabela de simbolos que representa a 
 * variavel. Se NOMEVAR nao esta na tabela de simbolos a funcao
 * RETORNA NULL.
 *
 * Considere a utilizacao de funcoes da biblioteca string.h, tais
 * como strcpy, strncpy, strcmp,....
 */
static CelST *
endVarST(char *nomeVar)
{
    CelST *p;

    for( p = ini->proxVar; p != NULL && strcmp(p->nomeVar, nomeVar) != 0;  p = p->proxVar);
    return p; 
}
