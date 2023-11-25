#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

char look; /* O caracter lido "antecipadamente" (lookahead) */
#define MAXVAR 26
int var[MAXVAR];

/* protótipos */
void init();
void initVar();
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
void assignment();
void printVar(char name);
void newLine();
void input();
void output();

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
    do {
        switch (look) {
            case '?':
                input();
                break;
            case '!':
                output();
                break;
            default:
                assignment();
        }

        assignment();
        newLine();
    } while (look != ';');  /* no slide da aula tava pra usar '.', mas eu quero ';' - ezboy =) */
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
    } else {
        if (isalpha(look))             
            val = var[getName() - 'A'];
        else             
            val = getNum();
    }
    return val;
}

/* avalia um comando de atribuição */
void assignment() {
    char name;
    name = getName();
    match('=');
    var[name - 'A'] = expression();
}

/* captura um caracter de nova linha */
void newLine() {
    if (look == '\n')
        nextChar();
}

/* inicialização do compilador */
void init()
{
    initVar();
	nextChar();
}

void initVar() {
    for (int i = 0; i < sizeof(var) / sizeof(var[0]); i++) {
        var[i] = 0;
    }
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

/* comando pra input, uso: ?<var> */
void input() {
    char name;
    char buffer[20];
    match('?');
    name = getName();
    printf("%c? ", name);   /* isso é opcional mas é interessante pro usuário */
    fgets(buffer, 20, stdin);
    var[name - 'A'] = atoi(buffer); /*atoi == argument to integer */
}

void output() {
    char name;
    match('!');
    name = getName();
    printf("%c -> %d\n", name, var[name - 'A']);
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

void printVar(char name) {
    if (!isalpha(name))
		expected("Name");
    name = toupper(name);
    printf("%d", var[name - 'A']);
}