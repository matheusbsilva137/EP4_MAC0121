/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO
 *
 * As funcoes neste arquivo foram copias, com pequenas adaptacoes,
 * da biblioteca de Eric Roberts.
 * 
 * https://github.com/mblair/roberts_abstractions/tree/master/book_code
 */

#include "util.h"   /* interface para este modulo */

#include <stdlib.h> /* exit(), malloc(), EXIT_FAILURE */ 
#include <stdio.h>  /* fprinf(), stderr */
#include <string.h> /* strcpy(), strncpy() */

/*
 *
 * File: simpio.c
 * Version: 1.0
 * Last modified on Fri Jul 15 14:10:41 1994 by eroberts
 * -----------------------------------------------------
 * This file implements the simpio.h interface.
 */
static String 
readLine (FILE *infile);
 
/*
 * Function: GetLine
 * -----------------
 * This function is a simple wrapper; all the work is done by
 * ReadLine.
 */

/* string GetLine(void) */
String 
getLine (FILE *infile) 
{
    return (readLine(infile)); /* ReadLine(stdin); */
}

/*
 * Function: ReadLine
 * ------------------
 * This function operates by reading characters from the file
 * into a dynamically allocated buffer.  If the buffer becomes
 * full before the end of the line is reached, a new buffer
 * twice the size of the previous one is allocated.
 */

/* string  ReadLine (FILE *infile) */
static String 
readLine (FILE *infile) 
{
    String line  = NULL; /* string */ 
    String nline = NULL;
    int n, ch, size;

    n = 0;
    size = INITIALBUFFERSIZE; /* Initialbuffersize; */
    line = mallocSafe((size+1)*sizeof(char)); /* GetBlock(size + 1);*/
    while ((ch = getc(infile)) != '\n' && ch != EOF) {
        if (n == size) {
            size *= 2;
            nline = mallocSafe((size+1)*sizeof(char)); /* GetBlock(size + 1);*/
            strncpy(nline, line, n);
            free(line); /* FreeBlock(line);*/
            line = nline;
        }
        line[n++] = ch;
    }
    if (n == 0 && ch == EOF) {
        free(line); /* FreeBlock(line); */
        return NULL;
    }
    line[n] = '\0';
    nline = mallocSafe((n+1)*sizeof(char)); /* (string) GetBlock(n + 1); */
    strcpy(nline, line);
    free(line); /* FreeBlock(line); */
    return nline;
}


/*-------------------------------------------------------------*/ 
/* mallocSafe 
 *
 * O parâmetro de mallocSafe é do tipo size_t.  
 * Em muitos computadores, size_t é equivalente a unsigned int.  
 * A função mallocSafe não está em nenhuma biblioteca e é desconhecida 
 * fora destas notas de aula. 
 * Ela é apenas uma abreviatura conveniente.
 * Fonte:  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
 *
 */
void *
mallocSafe (size_t nbytes)
{
    void *ptr;

    ptr = malloc(nbytes);
    if (ptr == NULL) 
    {
        ERRO(Socorro! malloc devolveu NULL!);
        exit(EXIT_FAILURE);
    }

    return ptr;
}
