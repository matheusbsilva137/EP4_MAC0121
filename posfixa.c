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

  posfixa.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:

  - função mallocc retirada de: 

  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/*------------------------------------------------------------*/
/* iterface para o uso da funcao deste módulo */
#include "posfixa.h"

/*------------------------------------------------------------*/
#include "categorias.h" /* Categoria, MAX_OPERADORES, INDEFINIDA, 
                           ABRE_PARENTESES, ... */
#include "objetos.h" /* tipo CelObjeto, freeObjeto(), freeListaObjetos() */
#include "stack.h"   /* stackInit(), stackFree(), stackPop() 
                        stackPush(), stackTop() */

/*-------------------------------------------------------------
 *  infixaParaPosfixa
 * 
 *  Recebe uma lista ligada com cabeca INIINFIXA representando uma
 *  fila de itens de uma expressao infixa e RETORNA uma lista ligada
 *  com cabeca contendo a fila com a representacao da correspondente
 *  expressao em notacao posfixa.
 */
 /*  As celulas da notacao infixa que nao forem utilizadas na notacao 
  *  posfixa (abre e fecha parenteses) devem ser liberadas 
  *  (freeObjeto()).
  */
CelObjeto *
infixaParaPosfixa(CelObjeto *iniInfixa)
{
    Stack sOper = stackInit();
    CelObjeto *iniPosfixa = iniInfixa;
    CelObjeto *p, *q;

    iniInfixa = iniInfixa->prox;
    p = iniPosfixa;
    while( iniInfixa != NULL){
      if ( iniInfixa->categoria >= 27 && iniInfixa->categoria <= 30 ){
          p->prox = iniInfixa;
          iniInfixa = iniInfixa->prox;
          p->prox->prox = NULL;
          p = p->prox;
      }else if( iniInfixa->categoria == FECHA_PARENTESES ){
          while( !stackEmpty(sOper) && stackTop(sOper)->categoria != ABRE_PARENTESES ){
              p->prox = stackPop(sOper);
              p->prox->prox = NULL;
              p = p->prox;
          }
          if( !stackEmpty(sOper) ) freeObjeto(stackPop(sOper));

          q = iniInfixa->prox;
          freeObjeto(iniInfixa);
          iniInfixa = q;
      }else{
          if ( iniInfixa->categoria != ABRE_PARENTESES &&
               iniInfixa->categoria != OPER_EXPONENCIACAO &&
               iniInfixa->categoria != OPER_ATRIBUICAO &&
               iniInfixa->categoria != OPER_MENOS_UNARIO &&
               iniInfixa->categoria != OPER_LOGICO_NOT){
            while( !stackEmpty(sOper) && stackTop(sOper)->valor.vInt >= iniInfixa->valor.vInt ){
              p->prox = stackPop(sOper);
              p->prox->prox = NULL;
              p = p->prox;
            }
          }
          q = iniInfixa->prox;
          stackPush(sOper, iniInfixa);
          iniInfixa = q;
      }
    }

    while( !stackEmpty(sOper) && p != NULL){
        p->prox = stackPop(sOper);
        p->prox->prox = NULL;
        p = p->prox;
    }
    
    stackFree(sOper);

    return iniPosfixa; 
}
