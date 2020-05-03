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

  Nome: Matheus Barbosa Siva - NUSP: 11221741

  eval.c
  Pitao I

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerado plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/*------------------------------------------------------------*/
/* interface para as funcoes deste modulo */
#include "eval.h" 

/*------------------------------------------------------------*/
#include <stdlib.h>  /* atoi(), atof() */
#include <string.h>  /* strncmp(), strlen(), strncpy(), strcpy(), strcmp() */
#include <math.h>    /* pow() */

/*------------------------------------------------------------*/
#include "categorias.h" /* Categoria, MAX_OPERADORES */
#include "util.h"       /* ERRO(), AVISO(), mallocSafe() */
#include "objetos.h"    /* CelObjeto, mostreObjeto(), freeObjeto()
                           mostreListaObjetos(), 
                           freeListaObjetos() */

#include "stack.h"      /* funcoes que manipulam uma pilha */ 
                        /* essa pilha sera usada para implementar 
                           a pilha de execucao */
#include "st.h"


/*------------------------------------------------------------*/
/* Protipos de funcoes auxiliares */
double getVal(stackNode* node);

/*------------------------------------------------------------*/
/* Tabela com uma representacao da precedencia dos operadores
 * atraves de valores inteiros. 
 * Quanto maior o valor, maior a precedencia.
 *
 *  http://www.ime.usp.br/~pf/algoritmos/apend/precedence.html
 */
static const int precedencia[MAX_OPERADORES] =
{
    /* 4 operadores relacionais com 2 simbolos  */
     4   /* "==" */ /* da esquerda para a direita */
    ,4   /* "!=" */ /* da esquerda para a direita */
    ,5   /* ">=" */ /* da esquerda para a direita */
    ,5   /* "<=" */ /* da esquerda para a direita */
    
    /* 2 operadores aritmeticos com 2 simbolos */
    ,8 /* ,"**" */ /* da direita para a esquerda */
    ,7 /* ,"//" */ /* da esquerda para a direita */
 
    /* 2 operadores relacionais com 1 simbolo */
    ,5  /* ">"  */ /* da esquerda para a direita */
    ,5  /* "<"  */ /* da esquerda para a direita */ 
    
    /* 6 operadores aritmeticos */
    ,7  /* "%" */ /* da esquerda para a direita */
    ,7  /* "*" */ /* da esquerda para a direita */
    ,7  /* "/" */ /* da esquerda para a direita */
    ,6  /* "+" */ /* da esquerda para a direita */
    ,6  /* "-" */ /* da esquerda para a direita */
    ,8  /* "_" */ /* da direita para a esquerda */
    
    /* 3 operadores logicos  */
    ,3  /* "and" */ /* da esquerda para a direita */ 
    ,2  /* "or" */ /* da esquerda para a direita */
    ,8  /* "not"  */ /* da direita para a esquerda */
    
    /* operador de indexacao */
    ,9  /* "$"  */ /* da esquerda para a direita (IGNORAR)*/

    /* atribuicao */ 
    ,1  /* "=" */ /* da direita para a esquerda EP4 */ 
}; 

/*-------------------------------------------------------------
 *  itensParaValores
 *  
 *  RECEBE uma lista ligada com cabeca INIFILAITENS representando uma
 *  fila de itens lexicos. Inicialmente, o campo VALOR de cada celula
 *  contem um string com o item lexico.
 *
 *  A funcao CONVERTE o campo VALOR de cada celula para um inteiro 
 *  ou double, como descrito a seguir. 
 *
 *  IMPLEMENTACAO
 *
 *  O campo VALOR de uma CelObjeto tem 3 subcampos:
 *
 *      - valor.vInt   (int)
 *      - valor.vFloat (float)
 *      - valor.pStr   (string)
 *
 *  Nessa conversao, o campo VALOR de cada celula recebe um valor 
 *  da seguinte maneira.
 *
 *     - Se o campo CATEGORIA da celula indica que o item e' um
 *       string representando um int (INT_STR) entao o campo 
 *       valor.vFloat deve receber esse numero inteiro. 
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e' um
 *       string representando um float (FLOAT_STR) entao o campo 
 *       valor.vFloat deve receber esse float;
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e' um
 *       string representando um Bool (BOOL_STR) entao o campo 
 *       valor.vFloat deve receber o inteiro correspondente 
 *       (False = 0, True = 1);
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e' um
 *       string representando um operador (OPER_*) entao o campo 
 *       valor.vInt deve receber o inteiro correspondente 
 *       a precedencia desse operador. 
 *       
 *       Para isto utilize o vetor PRECEDENCIA declarado antes 
 *       desta funcao. 
 *
 * Nesta funcao (e nas outras) voce pode utilizar qualquer funcao da
 * biblioteca string do C, como, por exemplo, atoi(), atof().
 *
 * Esta funcao (e todas as outras) devem 'dar' free nas estruturas que
 * deixarem de ser necessarias.
 *
 * Esse e o caso dos strings dos itens das categorias FLOAT_STR e INT_STR.  
 *
 * Os campos strings de objetos OPER_* e BOOL_STR apontam para
 * strings em tabelas definidas no modulo lexer.h.  Nesse
 * caso, tentar liberar essa memoria e' um erro.
 *
 */

void
itensParaValores(CelObjeto *iniFilaItens)
{
    CelObjeto* p;
    char *c;
    for(p = iniFilaItens->prox; p!= NULL; p = p->prox){
        c = p->valor.pStr;
        if(p->categoria >= 0 && p->categoria <= 18){
            /*Operadores correspondem aos itens 0 a 18 da lista de categorias*/
            p->valor.vInt = precedencia[p->categoria];
        }else if(p->categoria == 23){
            /*O 23º item da lista de categorias é BOOL_STR*/
             if ( strncmp(p->valor.pStr, "True", 4) == 0 )
                p->valor.vFloat = 1;
            else
                p->valor.vFloat = 0;
            p->categoria = BOOL;
        }else if(p->categoria == 24 || p->categoria == 25){
            /*O 24º item da lista de categorias é FLOAT_STR*/
            /*O 25º item da lista de categorias é INT_STR*/
            if(p->categoria == 25)
                p->valor.vFloat = atoi(p->valor.pStr);  
            else
                p->valor.vFloat = atof(p->valor.pStr);
            p->categoria = FLOAT;
            free(c);
        }else if(p->categoria == 19 || p->categoria == 20){
            /*ABRE E FECHA PARENTESES*/
            p->valor.vInt = 0;
        }
    }
}

/*-------------------------------------------------------------
 * eval
 * 
 * RECEBE uma lista ligada como cabeca INIPOSFIXA que representa
 * uma expressao em notacao posfixa. 
 *
 * RETORNA o endereco de uma celula com o valor da expressao.
 *
 * PRE-CONDICAO: a funcao supoe que a expressao esta' sintaticamente
 *               correta.
 *
 * IMPLEMENTACAO
 *
 * Para o calculo do valor da expressao, deve ser usada uma pilha. 
 * O endereco retornado sera' o da celula no topo no topo dessa  
 * "pilha de execucao".
 * 
 * A funcao percorre a expressao calculando os valores resultantes.
 * Para isto e' utilizada uma pilha de execucao. 
 * 
 * A implementacao das funcoes que manipulam a pilha devem ser
 * escritas no modulo stack.c.
 * 
 * O arquivo stack.h contem a interface para essas funcoes. 
 * A pilha de execucao so deve ser acessada atraves dessa interface
 * (caso contrario nao chamariamos stack.h de interface).
 * 
 * O parametro mostrePilhaExecucao indica se os valores
 * na pilha de execucao devem ser exibidos depois de qualquer 
 * alteracao.
 * 
 * Esta funcao deve "dar free" nas estruturas que deixarem de ser 
 * necessarias.
 *
 */

CelObjeto *
eval (CelObjeto *iniPosfixa, Bool mostrePilhaExecucao)
{
    Stack s = stackInit();
    CelObjeto *p;
    CelObjeto* res, *aux, *valRet;
    double num, num2;

    if(iniPosfixa != NULL && iniPosfixa->prox != NULL){
        p = iniPosfixa->prox;
        while ( p!= NULL ){
            switch (p->categoria){
            case FLOAT:
            case BOOL:
            case ID:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = p->categoria;
                if (p->categoria == ID){
                    res->nomeID = mallocSafe(sizeof(String));
                    strcpy(res->nomeID, p->nomeID);
                }else{
                    res->valor.vFloat = p->valor.vFloat;
                }
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_ADICAO:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = FLOAT;
                res->valor.vFloat = getVal(stackPop(s)) + getVal(stackPop(s));
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_SUBTRACAO:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = FLOAT;
                num = getVal(stackPop(s));
                res->valor.vFloat = getVal(stackPop(s)) - num;
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_MULTIPLICACAO:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = FLOAT;
                res->valor.vFloat = getVal(stackPop(s)) * getVal(stackPop(s));
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_DIVISAO:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = FLOAT;
                num = getVal(stackPop(s));
                res->valor.vFloat = getVal(stackPop(s)) / num;
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_RESTO_DIVISAO:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = FLOAT;
                num = getVal(stackPop(s));
                res->valor.vFloat = (float) fmod(getVal(stackPop(s)), num );
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_EXPONENCIACAO:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = FLOAT;
                num = getVal(stackPop(s));
                res->valor.vFloat = pow( getVal(stackPop(s)), num );
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_MENOS_UNARIO:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = FLOAT;
                res->valor.vFloat = getVal(stackPop(s)) * -1;
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_DIFERENTE:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = BOOL;
                if(getVal(stackPop(s)) != getVal(stackPop(s)))
                    res->valor.vFloat = 1;
                else
                    res->valor.vFloat = 0;
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_IGUAL:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = BOOL;
                if(getVal(stackPop(s)) == getVal(stackPop(s)))
                    res->valor.vFloat = 1;
                else
                    res->valor.vFloat = 0;
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_MAIOR:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = BOOL;
                num = getVal(stackPop(s));
                if( getVal(stackPop(s)) > num )
                    res->valor.vFloat = 1;
                else
                    res->valor.vFloat = 0;
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_MAIOR_IGUAL:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = BOOL;
                num = getVal(stackPop(s));
                if( getVal(stackPop(s)) >= num )
                    res->valor.vFloat = 1;
                else
                    res->valor.vFloat = 0;
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_MENOR:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = BOOL;
                num = getVal(stackPop(s));
                if( getVal(stackPop(s)) < num )
                    res->valor.vFloat = 1;
                else
                    res->valor.vFloat = 0;
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_MENOR_IGUAL:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = BOOL;
                num = getVal(stackPop(s));
                if( getVal(stackPop(s)) <= num )
                    res->valor.vFloat = 1;
                else
                    res->valor.vFloat = 0;
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_DIVISAO_INT:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = FLOAT;
                num = getVal(stackPop(s));
                res->valor.vFloat = (int) ( getVal(stackPop(s)) / num );
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_LOGICO_AND:
                res = mallocSafe(sizeof(CelObjeto));
                num = getVal(stackPop(s));
                num2 = getVal(stackPop(s));

                if(num >= 0 && num <= 1 && num2 >= 0 && num2 <= 1){
                    res->categoria = BOOL;
                    if( (int)num * (int)num2 == 1)
                        res->valor.vFloat = 1;
                    else
                        res->valor.vFloat = 0;
                }else{
                    if((int)num == 0 || (int)num2 == 0) res->valor.vFloat = 0;
                    else{
                        /*Se os valores não são booleanos, retorna o valor
                        float do elemento no topo da pilha*/
                        res->valor.vFloat = num;
                    }
                    res->categoria = FLOAT;
                }

                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_LOGICO_OR:
                res = mallocSafe(sizeof(CelObjeto));
                num = getVal(stackPop(s));
                num2 = getVal(stackPop(s));

                if(num >= 0 && num <= 1 && num2 >= 0 && num2 <= 1){
                    res->categoria = BOOL;
                    if( (int)num + (int)num2 >= 1)
                        res->valor.vFloat = 1;
                    else
                        res->valor.vFloat = 0;
                }else{
                    res->categoria = FLOAT;
                    if((int)num == 0 || (int)num2 == 0) res->valor.vFloat = num+num2;
                    else{
                        res->valor.vFloat = num2;
                        /*Se os valores não são booleanos, retorna o valor
                        float do segundo elemento retirado da pilha*/
                    }
                }
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_LOGICO_NOT:
                res = mallocSafe(sizeof(CelObjeto));
                res->categoria = BOOL;
                num = getVal(stackPop(s));

                if(num >= 0 && num <= 1){
                    if( (int)num == 0)
                        res->valor.vFloat = 1;
                    else
                        res->valor.vFloat = 0;
                }else{
                    res->valor.vFloat = 0;
                    /*Se os valores não são booleanos, retorna sempre falso*/
                }
                stackPush(s, res);
                p = p->prox;
                break;
            case OPER_ATRIBUICAO:
                res = stackPop(s);
                aux = stackPop(s);
                setValorST(aux->nomeID, res);
                stackPush(s, res);

                freeObjeto(aux);
                p = p->prox;
                break;
            default:
                break;
            }
            if(mostrePilhaExecucao) stackDump(s);
        }
    }else{
        stackFree(s);
        return NULL;
    }

    if(stackTop(s)->prox != NULL){
        AVISO(eval em eval.c: retornando valor no topo da pilha de execucao);
        AVISO(eval em eval.c: pilha de execucao ainda nao esta vazia);
    }

    p = stackTop(s);
    if(p->categoria == ID){
        valRet = getValorST(p->nomeID);
        valRet->prox = NULL;
    }
    else{
        valRet = mallocSafe(sizeof(CelObjeto));
        valRet->valor = p->valor;
        valRet->categoria = p->categoria;
        valRet->prox = NULL;
    }
    stackFree(s);
    
    return valRet;
}

/*-------------------------------------------------------------
 * getVal
 * 
 * RECEBE um ponteiro para celObjeto e RETORNA o seu valor associado
 * Caso o celObjeto represente uma variável, o valor retornado é o valor
 * presente na tabela de símbolos para a variável com o nome do campo 
 * valor.pStr da celObjeto.
 * Caso a celObjeto represente um float, o valor retornado é o float no
 * campo valor da celObjeto.
 * 
 */
double getVal(stackNode* node){
    double valObj;
    CelObjeto *p;
    if(node->categoria == ID){
        p = getValorST(node->valor.pStr);
        valObj = p->valor.vFloat;
        freeObjeto(p);
        freeObjeto(node);
        return valObj;
    }else{
        valObj = node->valor.vFloat;
        freeObjeto(node);
        return valObj;
    }
}