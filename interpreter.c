#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

char look; /* O caracter lido "antecipadamente" (lookahead) */

/* protótipos */
void init();
void nextChar();
void error(const char* fmt, ...);
void fatal(const char* fmt, ...);
void expected(const char* fmt, ...);
void match(char c);
char getName();
int getNum();
void emit(const char* fmt, ...);
char isAddOp(char);
char isMulOp(char);
int expression();
int term();
int factor();

/* PROGRAMA PRINCIPAL */
/*
    -> Tabela de definições (mais atualizada)
    
    # <expression> ::= <term> [<addop> <term>]*
    # <term> ::= <factor> [<mulop> <factor>]*
    # <factor> ::= <NUM> | (<expression) | <variable>
*/  
int main()
{
	init();
    printf("%d",expression());
	return 0;
}

/*
    Avalia o resultado de uma expressão
    1. <expression> ::= NUM [<addop> NUM]*
    2. <expression> ::= <term> [<addop> <term>]*
*/
int expression() {
    int val;

    if (isAddOp(look))
        val = 0;
    else 
        val = term();

    while(isAddOp(look)) {
        switch(look) {
            case '+':
                match('+');
                val += term();
                break;
            case '-':
                match('-');
                val -= term();
                break;
        }
    }
    return val;
}

/*
    Avalia um termo
    <term ::= <NUM> [<mulop> <NUM>]*
*/
int term() {
    int val;
    val = getNum();
    while(isMulOp(look)) {
        switch (look) {
        case '*':
            match('*');
            val *= getNum();
            break;
        case '/':
            match('/');
            val /= getNum();
            break;
        }
    }
    return val;
}

/*
    1. <factor> ::= <NUM> | (<expression>) // avalia um fator
    2. <factor> ::= <NUM> | (<expression>) | <variable> // avalia e traduz um fator
*/
int factor() {
    int val;
    if (look = '(') {
        match('(');
        val = expression();
        match(')');
    } else if (isalpha(look)) {
        emit("MOV AX, [%c]", getName());
    }  else {
        emit("MOV AX, %c", getNum());
    }
    return val;
}

/* inicialização do compilador */
void init()
{
	nextChar();
}

/* lê próximo caracter da entrada */
void nextChar()
{
	look = getchar();
}

/* exibe uma mensagem de erro formatada */
void error(const char* fmt, ...)
{
	va_list args;

	fputs("Error: ", stderr);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fputc('\n', stderr);
}

/* exibe uma mensagem de erro formatada e sai */
void fatal(const char* fmt, ...)
{
	va_list args;

	fputs("Error: ", stderr);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fputc('\n', stderr);

	exit(1);
}

/* alerta sobre alguma entrada esperada */
void expected(const char* fmt, ...)
{
	va_list args;

	fputs("Error: ", stderr);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fputs(" expected!\n", stderr);

	exit(1);
}

/* verifica se entrada combina com o esperado */
void match(char c)
{
	if (look != c)
		expected("'%c'", c);
	nextChar();
}

/* recebe o nome de um identificador */
char getName()
{
	char name;

	if (!isalpha(look))
		expected("Name");
	name = toupper(look);
	nextChar();

	return name;
}

/* recebe um número inteiro */
int getNum()
{
    int i;
    i = 0;
    if (!isdigit(look))
        expected("Inteiro");
    
    while(isdigit(look)) {
        i *= 10;
        i += look - '0';
        nextChar();
    }
    return i;
}

/* emite uma instrução seguida por uma nova linha */
void emit(const char* fmt, ...)
{
	va_list args;

	putchar('\t');

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	putchar('\n');
}
char isAddOp(char x) {
	return (x == '+' || x == '-');
}
char isMulOp(char x) {
	return (x == '*' || x == '/');
}