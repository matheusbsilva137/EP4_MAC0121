CC     = gcc
CFLAGS = -Wall -ansi -g -pedantic -Wno-unused-result
RM     = rm
OBJS   = util.o objetos.o lexer.o stack.o posfixa.o st.o eval.o main.o
#------------------------------------------------

# chama o linker
pitao:  $(OBJS)
	$(CC)  $(OBJS) -o pitao -lm

main.o: main.c categorias.h util.h objetos.h lexer.h   
	$(CC) $(CFLAGS) -c main.c 

eval.o: eval.c eval.h util.h objetos.h categorias.h stack.h
	$(CC) $(CFLAGS) -c eval.c 

st.o: st.c eval.h util.h objetos.h categorias.h stack.h
	$(CC) $(CFLAGS) -c st.c 

posfixa.o: posfixa.c categorias.h objetos.h stack.h posfixa.h
	$(CC) $(CFLAGS) -c posfixa.c

stack.o: stack.c util.h objetos.h stack.h 
	$(CC) $(CFLAGS) -c stack.c

lexer.o: lexer.c categorias.h util.h objetos.h lexer.h 
	$(CC) $(CFLAGS) -c lexer.c 

objetos.o: objetos.c categorias.h util.h objetos.h
	$(CC) $(CFLAGS) -c objetos.c 

util.o: util.c categorias.h util.h
	$(CC) $(CFLAGS) -c util.c 

clean:
	$(RM) *.o *~ pitao
