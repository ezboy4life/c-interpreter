#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

char look; /* O caracter lido "antecipadamente" (lookahead) */
#define MAXVAR 26
int var[MAXVAR];

#define MAXFILENAME 256
char readingFromFile = 0;
FILE *fileIn;

/* protótipos */
void init();
void initVar();
void nextChar();
void error(const char *fmt, ...);
void fatal(const char *fmt, ...);
void expected(const char *fmt, ...);
void match(char c);
char getName();
int getNum();
void emit(const char *fmt, ...);
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
void skipWhite();     // Versão Tiny
void readFile();      // Versão Large
void parseFileName(); // Versão Large

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
    do
    {
        switch (look)
        {
        case '?':
            input();
            break;
        case '!':
            output();
            break;
        case '\n':
            newLine(); /* hmm yes new line tasty - WMT 🍵*/
            break;
        case ' ': /* em tese isso permite ele sumir com os espaços no início de linhas tbm - WMT 🍵*/
        case '\t':
            skipWhite();
            break;
        case '@':
            printf("### DEBUG - captured @\n");
            skipWhite();
            printf("### DEBUG - skipped whites\n");
            readFile();
            break;
        default:
            assignment();
        }
        newLine();
    } while (look != ';'); /* no slide da aula tava pra usar '.', mas eu quero ';' - ezboy =) */
    return 0;
}

/*
    Avalia o resultado de uma expressão
    1. <expression> ::= NUM [<addop> NUM]*
    2. <expression> ::= <term> [<addop> <term>]*
*/
int expression()
{
    int val;

    if (isAddOp(look))
        val = 0;
    else
        val = term();

    while (isAddOp(look))
    {
        switch (look)
        {
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
int term()
{
    int val;
    val = factor();
    while (isMulOp(look))
    {
        switch (look)
        {
        case '*':
            match('*');
            val *= factor();
            break;
        case '/':
            match('/');
            val /= factor();
            break;
        }
    }
    return val;
}

/*
    1. <factor> ::= <NUM> | (<expression>) // avalia um fator
    2. <factor> ::= <NUM> | (<expression>) | <variable> // avalia e traduz um fator
*/
int factor()
{
    int val;
    if (look == '(')
    {
        match('(');
        val = expression();
        match(')');
    }
    else
    {
        if (isalpha(look))
            val = var[getName() - 'A'];
        else
            val = getNum();
    }
    return val;
}

/* avalia um comando de atribuição */
void assignment()
{
    char name;
    name = getName();
    match('=');
    var[name - 'A'] = expression();
}

/* captura um caracter de nova linha */
void newLine()
{
    if (look == '\n')
        nextChar();
}

/* inicialização do compilador */
void init()
{
    initVar();
    nextChar();
    skipWhite();
}

void initVar()
{
    for (int i = 0; i < sizeof(var) / sizeof(var[0]); i++)
    {
        var[i] = 0;
    }
}

/* lê próximo caracter da entrada */
/* caso esteja lendo do arquivo,
 *	1. verifica se o ch é EOF
 * 	2. se não for, atualiza look;
 * 	3. se for, muda look pra um endline, fecha o arquivo e troca a flag
 * - WMT 🍵*/
void nextChar()
{
    if (readingFromFile)
    {
        char ch = fgetc(fileIn);
        if (ch != EOF)
            look = ch;
        else
        {
            look = '\n';
            fclose(fileIn);
            readingFromFile = 0;
        }
    }
    else
        look = getchar();
}

/* exibe uma mensagem de erro formatada */
void error(const char *fmt, ...)
{
    va_list args;

    fputs("Error: ", stderr);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputc('\n', stderr);
}

/* exibe uma mensagem de erro formatada e sai */
void fatal(const char *fmt, ...)
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
void expected(const char *fmt, ...)
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
    skipWhite();
}

/* recebe o nome de um identificador */
char getName()
{
    char name;

    if (!isalpha(look))
        expected("Name");
    name = toupper(look);
    nextChar();
    skipWhite();

    return name;
}

/* recebe um número inteiro */
int getNum()
{
    int i;
    i = 0;
    if (!isdigit(look))
        expected("Inteiro");

    while (isdigit(look))
    {
        i *= 10;
        i += look - '0';
        nextChar();
    }
    skipWhite();
    return i;
}

/* comando pra input, uso: ?<var> */
void input()
{
    char name;
    char buffer[20];
    match('?');
    name = getName();
    printf("%c? ", name); /* isso é opcional mas é interessante pro usuário */
    fgets(buffer, 20, stdin);
    var[name - 'A'] = atoi(buffer); /*atoi == argument to integer */
}

void output()
{
    char name;
    match('!');
    name = getName();
    printf("%c -> %d\n", name, var[name - 'A']);
}

/* emite uma instrução seguida por uma nova linha */
void emit(const char *fmt, ...)
{
    va_list args;

    putchar('\t');

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    putchar('\n');
}
char isAddOp(char x)
{
    return (x == '+' || x == '-');
}
char isMulOp(char x)
{
    return (x == '*' || x == '/');
}

void printVar(char name)
{
    if (!isalpha(name))
        expected("Name");
    name = toupper(name);
    printf("%d", var[name - 'A']);
}

/* ingere caraceres de espaço e tabulações - WMT 🍵*/
void skipWhite()
{
    while (look == ' ' || look == '\t')
        nextChar();
}

/* pensando aqui como eu faria pra abrir um arquivo. - WMT 🍵 */
/* vou dar o comando @ seguido do endereço do arquivo entre aspas como fopen */
/* tipo chegou o arroba, ele come os espaços e cai aqui */
void readFile()
{
    char fileName[MAXFILENAME + 1];

    printf("### DEBUG - ENTERED readFile\n");

    match('@');
    if (look == '\'')
    {
        match('\'');
        printf("###DEBUG - in readFile after match 1\n");
        parseFileName(fileName);
        match('\'');
        fileIn = fopen(fileName, "r");
    }
    else if (look == '"')
    {
        match('"');
        parseFileName(fileName);
        match('"');
        fileIn = fopen(fileName, "r");
    }
    else
    {
        skipWhite();
    }

    readingFromFile = (fileIn != NULL);
}

/* função dedicada pra consumir o nome do arquivo de entrada */
void parseFileName(char *fileName)
{
    int i = 0;
    for (i = 0; i < MAXFILENAME; i++)
    {
        if (look == '\'' || look == '\"')
        {
            fileName[i] = '\0';
            return;
        }
        fileName[i] = look; // ainda é seguro, pq com i = 256 essa seria a posição MAXFILENAME + 1
        nextChar();
    }
    fatal("Nome de arquivo muito longo! :c");
    // se vc saiu do for, o nome do arquivo é muito longo :D
}