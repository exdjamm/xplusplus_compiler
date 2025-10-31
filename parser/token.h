#include <iostream>
#include <ctype.h> //Funções de caracteres
#include <string>

using namespace std;

enum Names
{
    UNDEF,
    // Continuar
    // Nomes e atributos dos tokens da linguagem

    ID,
    INTEGER_LITERAL,
    OP,
    OP_PLUS,
    OP_MINUS,
    OP_PRODUCT,
    OP_DIVISION,
    OP_MOD,
    OP_ATTR,
    REL_OP,
    OP_EQ,
    OP_NEQ,
    OP_LESS,
    OP_GREAT,
    OP_LEQUAL,
    OP_GEQUAL,
    SEP,
    SEP_LPARENTHESIS,
    SEP_RPARENTHESIS,
    SEP_LBRACKET,
    SEP_RBRACKET,
    SEP_LBRACE,
    SEP_RBRACE,
    SEP_SEMICOLON,
    SEP_COMMA,
    SEP_PERIOD,
    STRING_LITERAL,
    COMMENT_LINE,
    COMMENT_BLOCK,
    // Palavras Reservadas
    // RESERVED_WORD,
    CLASS,
    EXTENDS,
    INT,
    STRING,
    BREAK,
    PRINT,
    READ,
    RETURN,
    SUPER,
    IF,
    ELSE,
    FOR,
    NEW,
    CONSTRUCTOR,
    // fim Palavras Reservadas
    END_OF_FILE
};

class Token
{
public:
    int name;
    int attribute;
    string lexeme;

    Token(int name)
    {
        this->name = name;
        attribute = UNDEF;
    }

    Token(int name, string l)
    {
        this->name = name;
        attribute = UNDEF;
        lexeme = l;
    }

    Token(int name, int attr)
    {
        this->name = name;
        attribute = attr;
    }
};
