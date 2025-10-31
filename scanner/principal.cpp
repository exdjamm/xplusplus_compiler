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
    vet = new string[29];

    vet[0] = "UNDEF";
    vet[1] = "ID";
    vet[2] = "INTEGER_LITERAL";
    vet[3] = "OP";
    vet[4] = "OP_LESS";
    vet[5] = "OP_GREAT";
    vet[6] = "OP_LEQUAL";
    vet[7] = "OP_GEQUAL";
    vet[8] = "OP_PLUS";
    vet[9] = "OP_MINUS";
    vet[10] = "OP_PRODUCT";
    vet[11] = "OP_DIVISION";
    vet[12] = "OP_MOD";
    vet[13] = "OP_ATTR";
    vet[14] = "OP_EQ";
    vet[15] = "OP_NEQ";
    vet[16] = "SEP";
    vet[17] = "SEP_LPARENTHESIS";
    vet[18] = "SEP_RPARENTHESIS";
    vet[19] = "SEP_LBRACKET";
    vet[20] = "SEP_RBRACKET";
    vet[21] = "SEP_LBRACE";
    vet[22] = "SEP_RBRACE";
    vet[23] = "SEP_SEMICOLON";
    vet[24] = "SEP_COMMA";
    vet[25] = "STRING_LITERAL";
    vet[26] = "COMMENT_LINE";
    vet[27] = "COMMENT_BLOCK";
    vet[28] = "END_OF_FILE";
}

void freeVetor()
{
    delete[] vet;
}

void print(Token *t)
{
    cout << vet[t->name] << " " << vet[t->attribute] << " " << t->lexeme << endl;
}
