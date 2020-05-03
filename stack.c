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

  stack.c
  Pitao I

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerado plágio.

  Exemplo:
  - função mallocc retirada de: 
 
    http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/* interface para o uso de uma pilha */
#include "stack.h"
#include <stdlib.h>

/* 
 * 
 * STACK.c: IMPLEMENTACAO DE UMA PILHA 
 *
 * TAREFA
 *
 * Faca aqui a implementacao de uma pilha atraves de uma 
 * __lista encadeada com cabeca__. 
 * 
 * A implementacao deve respeitar a interface que voce declarar em 
 * stack.h. 
 *
 */

Stack stackInit(){
    Stack s = mallocSafe( sizeof(stackNode) );

    s->prox = NULL; /*Cabeça da Lista*/

    return s;
}

int stackEmpty(Stack s){
    return s->prox == NULL;
}

void stackPush(Stack s, CelObjeto* celu){
    celu->prox = s->prox;
    s->prox = celu;
}

stackNode* stackPop(Stack s){
    stackNode* topo = NULL;
    if( s != NULL ) topo = s->prox;

    if( topo == NULL || stackEmpty(s) ){
        AVISO(stackPop em stack.c: pilha vazia...);
    }else{
        s->prox = topo->prox;
        topo->prox = NULL;
    }
    
    return topo;
}

stackNode* stackTop(Stack s){
    /*O elemento do topo é aquele que está imediatamente acima da cabeça da lista*/
    return s->prox;
}

void stackFree(Stack s){
    freeListaObjetos(s);
}

void stackDump(Stack s){
    stackNode* node = s->prox;

    printf("\n==========================\n");
    printf("  Pilha de execucao\n");
    printf("  valor\n");
    printf(". . . . . . . . . . . . . . .\n");

    while(node != NULL){
        if(node->categoria == FLOAT)
            printf("  %f\n", node->valor.vFloat);
        else if(node->categoria == BOOL)
            if((int)node->valor.vFloat == 1) printf("  True\n");
            else printf("  False\n");
        else if(node->categoria == ID)
            printf("  %s\n", node->valor.pStr);
        else
            printf("  %d\n", node->valor.vInt);
        
        node = node->prox;
    }

    printf("\n");
}