#include "scanner.h"

// Construtor que recebe uma string com o nome do arquivo
// de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input, SymbolTable *table)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: "
         << input.length() << endl;*/
    pos = 0;
    line = 1;

    st = table;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile, line))
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else
        cout << "Unable to open file\n";

    // A próxima linha deve ser comentada posteriormente.
    // Ela é utilizada apenas para verificar se o
    // preenchimento de input foi feito corretamente.
    // cout << this->input;
}

int Scanner::getLine()
{
    return line;
}

// Método que retorna o próximo token da entrada
Token *
Scanner::nextToken()
{
    Token *tok;
    string lexeme;

    int state = 0;

    while (true)
    {
        switch (state)
        {
        case 0:
            if (input[pos] == '\0')
            {
                tok = new Token(END_OF_FILE);
                return tok;
            }
            else if (isspace(input[pos]) || input[pos] == '\n')
            {
                if (input[pos] == '\n')
                    line++;
                pos++;
            }
            else if (input[pos] == '"')
            {
                state = 1;
                lexeme.push_back(input[pos]);
                pos++;
            }
            else if (isalpha(input[pos]) || input[pos] == '_')
            {
                state = 2;
                lexeme.push_back(input[pos]);
                pos++;
            }
            else if (isdigit(input[pos]))
            {
                state = 3;
                lexeme.push_back(input[pos]);
                pos++;
            }
            else if (input[pos] == '/')
            {
                state = 4;
                pos++;
            }
            else if (input[pos] == '<')
            {
                state = 8;
                pos++;
            }
            else if (input[pos] == '>')
            {
                state = 9;
                pos++;
            }
            else if (input[pos] == '=')
            {
                state = 10;
                pos++;
            }
            else if (input[pos] == '!')
            {
                state = 11;
                pos++;
            }
            else if (input[pos] == '+')
            {
                pos++;
                tok = new Token(OP, OP_PLUS);
                return tok;
            }
            else if (input[pos] == '-')
            {
                pos++;
                tok = new Token(OP, OP_MINUS);
                return tok;
            }
            else if (input[pos] == '*')
            {
                pos++;
                tok = new Token(OP, OP_PRODUCT);
                return tok;
            }
            else if (input[pos] == '%')
            {
                pos++;
                tok = new Token(OP, OP_MOD);
                return tok;
            }
            else if (input[pos] == '(')
            {
                pos++;
                tok = new Token(SEP, SEP_LPARENTHESIS);
                return tok;
            }
            else if (input[pos] == ')')
            {
                pos++;
                tok = new Token(SEP, SEP_RPARENTHESIS);
                return tok;
            }
            else if (input[pos] == '[')
            {
                pos++;
                tok = new Token(SEP, SEP_LBRACKET);
                return tok;
            }
            else if (input[pos] == ']')
            {
                pos++;
                tok = new Token(SEP, SEP_RBRACKET);
                return tok;
            }
            else if (input[pos] == '{')
            {
                pos++;
                tok = new Token(SEP, SEP_LBRACE);
                return tok;
            }
            else if (input[pos] == '}')
            {
                pos++;
                tok = new Token(SEP, SEP_RBRACE);
                return tok;
            }
            else if (input[pos] == ';')
            {
                pos++;
                tok = new Token(SEP, SEP_SEMICOLON);
                return tok;
            }
            else if (input[pos] == ',')
            {
                pos++;
                tok = new Token(SEP, SEP_COMMA);
                return tok;
            }
            else if (input[pos] == '.')
            {
                pos++;
                tok = new Token(SEP, SEP_PERIOD);
                return tok;
            }
            else
            {
                lexicalError("Token invalido.");
            }
            break;
        case 1:
            if (input[pos] == '"')
            {
                lexeme.push_back(input[pos]);
                pos++;
                tok = new Token(STRING_LITERAL, lexeme);
                return tok;
            }
            else if (input[pos] == '\0')
            {
                lexicalError("String Aberta: Encontrado fim do arquivo onde espera-se fim de STRING.");
            }
            else if (input[pos] == '\n')
            {
                line++;
            }
            lexeme.push_back(input[pos]);
            pos++;
            break;
        case 2:
            if (isalnum(input[pos]) || input[pos] == '_')
            {
                lexeme.push_back(input[pos]);
                pos++;
            }
            else
            {
                state = 12;
            }
            break;
        case 3:
            if (isdigit(input[pos]))
            {
                lexeme.push_back(input[pos]);
                pos++;
            }
            else
            {
                tok = new Token(INTEGER_LITERAL, lexeme);
                return tok;
            }
            break;
        case 4:
            if (input[pos] == '/')
            {
                state = 5;
                pos++;
            }
            else if (input[pos] == '*')
            {
                state = 6;
                pos++;
            }
            else
            {
                tok = new Token(OP, OP_DIVISION);
                return tok;
            }

            break;
        case 5:
            if (input[pos] == '\n')
            {
                line++;
                pos++;
                // tok = new Token(COMMENT_LINE);
                // return tok;
                state = 0;
            }
            else
            {
                pos++;
            }

            break;
        case 6:
            if (input[pos] == '*')
            {
                state = 7;
                pos++;
            }
            else if (input[pos] == '\0')
            {
                lexicalError("Encontrado fim de arquivo onde espera-se fim de comentario de bloco.");
            }
            else
            {
                if (input[pos] == '\n')
                    line++;
                pos++;
            }
            break;
        case 7:
            if (input[pos] == '/')
            {
                pos++;
                // tok = new Token(COMMENT_BLOCK);
                // return tok;
                state = 0;
            }
            else
            {
                state = 6;
            }
            break;
        case 8:
            if (input[pos] == '=')
            {
                pos++;
                tok = new Token(REL_OP, OP_LEQUAL);
                return tok;
            }
            else
            {
                tok = new Token(REL_OP, OP_LESS);
                return tok;
            }
            break;
        case 9:
            if (input[pos] == '=')
            {
                pos++;
                tok = new Token(REL_OP, OP_GEQUAL);
                return tok;
            }
            else
            {
                tok = new Token(REL_OP, OP_GREAT);
                return tok;
            }
            break;
        case 10:
            if (input[pos] == '=')
            {
                pos++;
                tok = new Token(REL_OP, OP_EQ);
                return tok;
            }
            else
            {
                tok = new Token(OP, OP_ATTR);
                return tok;
            }
            break;
        case 11:
            if (input[pos] == '=')
            {
                pos++;
                tok = new Token(REL_OP, OP_NEQ);
                return tok;
            }
            else
            {
                lexicalError("Esperado desigualdade encontrado outro.");
            }
            break;
        case 12:
            // como as palavras reservadas ja estao em ST, nao podem ser regravadas
            STEntry *obj = st->get(lexeme);
            if (!obj)
                tok = new Token(ID, lexeme);
            else
                tok = new Token(obj->token->name);

            return tok;
            break;
        }
    }

    return tok;
}

void Scanner::lexicalError(string msg)
{
    cout << "Linha " << line << ": " << msg << endl;

    exit(EXIT_FAILURE);
}
