#include "scanner.h"

string *vet;

void print(Token *);
void allocVetor();
void freeVetor();

/*
Nao esta bem definido na descricao do trabalho os comportamentos
de erro ou indesejaveis para Analise Lexica.
Nao existe erros para caracteres invalidos ou necessidade de separacoes
para estar correto, lexemas como "\n" ou 777aa sao aceitos e lidos
como uma string com quebra de linha e, um numero e um identificador, respectivamente.

Tokens com caracteres de abertura (string e comentario de bloco) tem o problema de
rodar para sempre pois nao e especificado a parada ao encontrar o fim do arquivo,
espera-se consumir o fechamento.

O que espera-se de 4654""?
String pode comportar-se como bloco?
"


" e valido?

Tem variavel line, nao e dito o que fazer com ela.

Outro problema e como trabalhar palavras reservadas, aqui super e super4, um gera
SUPER e outro ID.
*/

int main(int argc, char *argv[])
{
    // Verifica se foi executado corretamente
    // Essa main espera receber o nome do arquivo a ser
    // executado na linha de comando.
    if (argc != 2)
    {
        cout << "Uso: ./compiler nome_arquivo.mj\n";
        return 1;
    }

    string input;

    // getline(cin, input);

    Scanner *scanner = new Scanner(argv[1]);

    Token *t;

    allocVetor();

    do
    {
        t = scanner->nextToken();
        cout << t->name << " ";
        print(t);

    } while (t->name != END_OF_FILE);

    delete scanner;

    freeVetor();

    return 0;
}

void allocVetor()
{
    vet = new string[45];

    vet[0] = "UNDEF";
    vet[1] = "ID";
    vet[2] = "INTEGER_LITERAL";
    vet[3] = "OP";
    vet[4] = "OP_PLUS";
    vet[5] = "OP_MINUS";
    vet[6] = "OP_PRODUCT";
    vet[7] = "OP_DIVISION";
    vet[8] = "OP_MOD";
    vet[9] = "OP_ATTR";
    vet[10] = "REL_OP";
    vet[11] = "OP_EQ";
    vet[12] = "OP_NEQ";
    vet[13] = "OP_LESS";
    vet[14] = "OP_GREAT";
    vet[15] = "OP_LEQUAL";
    vet[16] = "OP_GEQUAL";
    vet[17] = "SEP";
    vet[18] = "SEP_LPARENTHESIS";
    vet[19] = "SEP_RPARENTHESIS";
    vet[20] = "SEP_LBRACKET";
    vet[21] = "SEP_RBRACKET";
    vet[22] = "SEP_LBRACE";
    vet[23] = "SEP_RBRACE";
    vet[24] = "SEP_SEMICOLON";
    vet[25] = "SEP_COMMA";
    vet[26] = "SEP_PERIOD";
    vet[27] = "STRING_LITERAL";
    vet[28] = "COMMENT_LINE";
    vet[29] = "COMMENT_BLOCK";
    vet[30] = "CLASS";
    vet[31] = "EXTENDS";
    vet[32] = "INT";
    vet[33] = "STRING";
    vet[34] = "BREAK";
    vet[35] = "PRINT";
    vet[36] = "READ";
    vet[37] = "RETURN";
    vet[38] = "SUPER";
    vet[39] = "IF";
    vet[40] = "ELSE";
    vet[41] = "FOR";
    vet[42] = "NEW";
    vet[43] = "CONSTRUCTOR";
    vet[44] = "END_OF_FILE";
}

void freeVetor()
{
    delete[] vet;
}

void print(Token *t)
{
    cout << vet[t->name] << " " << vet[t->attribute] << " " << t->lexeme << endl;
}
