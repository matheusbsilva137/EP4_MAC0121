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

  Nome:

  main.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include <stdio.h>   /* fprintf(), fgets(), printf() */
#include <stdlib.h>  /* exit(), EXIT_FAILURE */
#include <string.h>  /* strncmp */

#include "util.h"    /* ITENS, VALORES, Bool, String, ERRO(), AVISO(),
                        getLine() */
#include "lexer.h"   /* lexer(), freeFilaItens */
#include "eval.h"    /* itensParaValores(), eval() */
#include "objetos.h" /* CelObjeto, mostreValor(), mostreObjeto(), 
                        mostreListaObjetos(), freObjeto(), freeListaObjetos() */
#include "posfixa.h" /* infixaParaPosfixa() */
#include "st.h"      /* initST(), showST() e freeST() */

#ifdef __linux__
#    define  SYSTEM "linux"
#elif __FreeBSD__
#    define  SYSTEM "FreeBSD"
#elif __MACH__
#    define  SYSTEM "Mac OS X"
#elif __MSDOS__
#    define  SYSTEM "Windows"
#else
#    define  SYSTEM "SO deconhecido..."
#endif

/*---------------------------------------------------------------*/
/*
 *  P R O T Ó T I P O S
 */
static void 
mostreUso (char *nomePrograma);

/*---------------------------------------------------------------*/
/* 
 *  M A I N 
 */
int 
main(int argc, char *argv[])
{
    String nomePrograma = argv[0];
    String linha; /* recebe linha do arquivo de entrada */
    
    /* opcoes de execucao e seus valores default */
    Bool modoInterativo      = TRUE;
    Bool mostreItensLexicos  = FALSE;
    Bool mostreValores       = FALSE;
    Bool mostrePilhaExecucao = FALSE;
    Bool mostreExpressaoPosfixa = FALSE;
    Bool mostreTabelaSimbolos = FALSE;

    FILE *fEntrada   = stdin; 
    String nomeScript = NULL; /* nome do modo com instrucoes a serem executadas
				 (default = ler do shell) *
                              */
    int nLinha   = -1;  

    /*-------------------------------------------------------------*/
    /* 1 pegue as opcoes da linha de comando */

    /*-------------------------------------------------------------*/
    /* 1 pegue as opcoes da linha de comando */
    /* TAREFA EP4: incluir as opcoes "-e" e "-t" */
    while (--argc) 
    {
	if      (!strncmp(argv[argc], "-h", 2)) mostreUso(argv[0]);
	else if (!strncmp(argv[argc], "-i", 2)) modoInterativo      = TRUE;
	else if (!strncmp(argv[argc], "-l", 2)) mostreItensLexicos  = TRUE;
	else if (!strncmp(argv[argc], "-v", 2)) mostreValores       = TRUE;
	else if (!strncmp(argv[argc], "-p", 2)) mostrePilhaExecucao = TRUE;
    else if (!strncmp(argv[argc], "-e", 2)) mostreExpressaoPosfixa = TRUE;
    else if (!strncmp(argv[argc], "-t", 2)) mostreTabelaSimbolos = TRUE;
	else if (!strncmp(argv[argc], "-s", 2))
	{  /* teremos um arquivo com as instrucoes */
	    modoInterativo = FALSE;
	    nomeScript     = argv[argc]+2; 
	}
	else 
	{ /* opcao invalida */
	    fprintf(stderr, "%s: opcao invalida '%s'\n", nomePrograma, argv[argc]); 
	    mostreUso(nomePrograma); 
	}	
    }

    /*-------------------------------------------------------------*/
    /* 2 verifique se ha um scritp a ser interpretado */
    if (!modoInterativo)
    {  
	fEntrada = fopen(nomeScript, "r");
	if (fEntrada == NULL) 
	{
	    printf("Nao consegui abrir o arquivo '%s'\n", nomeScript);
	    exit(EXIT_FAILURE);
	}
    }

    /*------------------------------------------------------------*/
    /* 3 imprima o cabecalho */
    printf("MAC0121 2019 - EP4\n");
    printf("Pitao (%s, %s)\n", __DATE__, __TIME__);
    printf("[GCC %s] on %s\n", __VERSION__, SYSTEM);

    /* mostre o prompt se o modo e iterativo */
    if (modoInterativo)
    {
	printf(">>> "); /* prompt */
    }	   

    /*------------------------------------------------------------*/
    /* 4 crie a tabela de simbolos                                */
    /* TAREFA EP4 */
    initST();

    /*------------------------------------------------------------*/
    /* 5 interprete cada uma das linhas dadas */
    while ((linha = getLine(fEntrada)) != NULL) 
    {
	/* 5.1 fila com a representacao de uma expressao posfixa */
	CelObjeto *iniFila   = NULL; 
	CelObjeto *resultado = NULL; 

	/* 5.2 imprime a linha, utilizado no modo script */
	if (!modoInterativo)
	{
	    printf("Linha %d: %s\n", ++nLinha, linha);
	}
	/* 5.3 construa fila de itens lexicos e mostre os itens se
           o programa tiver sido chamado com a opcao "-l" */
	iniFila = crieFilaItens(linha);
	if (iniFila && mostreItensLexicos)
	{
	    /* TAREFA EP3: a funcao a seguir ja deve ter sido escrita
               para o EP3. Veja o modulo objetos.c. */
	    mostreListaObjetos(iniFila, ITENS);
	}

        /* 5.4 linha nao e mais necessaria */
        free(linha); /* alocada por getLine() */
	linha = NULL; 

	/* 5.5 substitua os itens por valores */
	/* TAREFA EP3: a funcao a seguir ja deve ter sido escrita
           para o EP3. Veja o modulo eval.c */
	itensParaValores(iniFila); 
	if (iniFila && mostreValores)
	{
            /* TAREFA EP3: a funcao a seguir ja deve ter sido escrita
               para o EP3. Veja o modulo objetos.c */ 
 	    mostreListaObjetos(iniFila, VALORES);
	}

        /* 5.6 transforme a expressao para a notacao posfixa */
    iniFila = infixaParaPosfixa(iniFila);
    if(mostreExpressaoPosfixa){
        mostreListaObjetos(iniFila, POSFIXA);
    }

        /* TAREFA EP4: aqui voce deve usar a funcao infixaParaPosfixa
           que deve ser escrita em posfixa.c.  Alem disso, se o
           programa tiver sido chamado com a opcao '-e' a expressao
           posfixa obtida deve ser exibida. A funcao
           mostreListaObjetos deve ser adaptada para isso. Veja o
           arquivo objetos.c  */
        
	/* 5.6 calcule o valor da expressao posfixa */
	/* TAREFA EP3: a funcao a seguir deve ter sido escrita para 
           o EP3. Veja o modulo eval.c */
        /* TAREFA EP4: a funcao a seguir deve ser adaptada para 
           tratar o operador atribuicao e variaveis */
	resultado = eval(iniFila, mostrePilhaExecucao); 
        
	/* 5.7 mostre o resultado/valor da expressao */
	if (resultado)
	{ 
            /* TAREFA EP3: a funcao a seguir deve ter sido escrita
               para o EP3. Veja o modulo objetos.c */
	    mostreValor(resultado);
	    freeObjeto(resultado);
            resultado = NULL; 
	}
        
	/* acho que essa pulada de linha e para algo ficar bonito */ 
	if (!modoInterativo)
	{
	    printf("\n");
	} 
	
	/* 5.8 mostre a tabela de simbolos, opcao "-t" */
        /* TAREFA EP4 */
    if (mostreTabelaSimbolos){
        showST();
    }

	/* 5.9 libere os itens da Fila */
        /*     idealmente, nesse ponto, so deveria ser liberada a
               celula cabeca */
	/* TAREFA EP3: a funcao a seguir deve ter sido escrita
           para o EP3. Veja o modulo objetos.c. */
	/* TAREFA EP4: a funcao devera ser adaptada para o EP4 devido
               aos nomes de variaves (categoria ID). 
               Veja o modulo objetos.c. */
	freeListaObjetos(iniFila);
        iniFila = NULL; 
	if (modoInterativo)
	{
	    printf(">>> "); /* prompt */
	}	  
    }
    
    /*------------------------------------------------------------*/
    /* 6 libere a tabela de simbolos                              */
    /* TAREFA EP4 */
    freeST();

    if (modoInterativo)
    {
	printf("\n"); 
    }

    fclose(fEntrada);
    
    return EXIT_SUCCESS;
}

/*---------------------------------------------------------------*/
/* 
 *  I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S   DA  
 *                     A U X I L I A R E S 
 */
static void 
mostreUso (char *nomePrograma)
{
    /* TAREFA EP4: incluir as opcoes "-e" e "-t" na mensagem */
    fprintf(stderr,"%s: Uso \n"
	    "prompt> %s [-h] [-i | -s<nome-script> ] [-l] [-v] [-p] [-e] [-t]\n"
	    "    -h = mostra como usar o programa e abandona o programa\n"
	    "    -i = executa em modo (i)nterativo\n"
	    
	    "    -s<nome-script> = calcula as expressões no arquivo <nome-script>\n"
	    "    -l = mostra fila de itens (l)exicos\n"
	    "    -v = mostra fila de (v)alores\n"
	    "    -p = mostra a (p)ilha de execucao apos cada operacao\n"
        "    -e = mostra a (e)xpressao na notacao posfixa (EP4)\n"
        "    -t = mostra a (t)abela de simbolos (EP4)\n"
	    ,nomePrograma, nomePrograma);
    exit(EXIT_FAILURE);
}
