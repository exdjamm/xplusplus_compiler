#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

void Parser::advance()
{
	lToken = scanner->nextToken();
}

void Parser::match(int t, string msg)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error(msg);
}

bool Parser::checklToken(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		return true;
	else
		return false;
}

void Parser::run()
{
	advance();

	program();

	cout << "Compilação encerrada com sucesso!\n";
}

/*
Program -> ClassList | E
*/
void Parser::program()
{
	if (checklToken(CLASS))
		classList();

	match(END_OF_FILE, "Erro: esperado encontrar fim de arquivo.");
}

/*
ClassList -> ClassDecl ClassList'
*/
void Parser::classList()
{
	classDecl();
	classList_();
}

/*
ClassList' -> ClassList | E
*/
void Parser::classList_()
{
	if (checklToken(CLASS))
		classList();
}

/*
ClassDecl -> class ID ClassDecl'
*/
void Parser::classDecl()
{
	match(CLASS, "Erro inesperado: Esperado encontar \"class\".");
	match(ID, "Erro inesperado: Esperado Identificador.");
	classDecl_();
}

/*
ClassDecl' -> ClassBody | extends ID ClassBody
*/
void Parser::classDecl_()
{
	if (checklToken(EXTENDS))
	{
		match(EXTENDS, "Erro inesperado: Esperado encontrar \"extends\".");
		match(ID, "Erro inesperado: Esperado inicio de classe.");
		classBody();
	}
	else
	{
		classBody();
	}
}

/*
ClassBody -> { VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt }
*/
void Parser::classBody()
{
	match(SEP_LBRACE, "Erro: Corpo de Classe, esperado encontrar abre chaves.");
	// Nao e possivel com LL(1) sem modificacao, First Follow Conflit
	if (checklToken(CONSTRUCTOR))
	{
		constructDeclListOpt();
		methodDeclListOpt();
	}
	else if (checklToken(STRING) || checklToken(ID) || checklToken(INT))
	{
		classBody_conflit();
	}
	match(SEP_RBRACE, "Erro: Corpo de Classe, esperado encontrar fecha chaves.");
}

void Parser::classBody_conflit()
{
	type();
	if (checklToken(SEP))
	{
		match(SEP_LBRACKET, "Erro: esperado '['.");
		match(SEP_RBRACKET, "Erro: esperado ']'.");
	}
	match(ID, "Erro: esperado um identificador.");

	classBody_conflit_();
}

void Parser::classBody_conflit_()
{
	if (checklToken(SEP_COMMA)) // varDecl
	{
		varDeclOpt();
		match(SEP_SEMICOLON, "Erro: esperado ';'");
		classBody_conflit_2();
	}
	else if (checklToken(SEP_SEMICOLON)) // varDecl
	{
		match(SEP_SEMICOLON, "Erro: esperado ';'");
		classBody_conflit_2();
	}
	else if (checklToken(SEP_LPARENTHESIS)) // MethodDecl
	{
		methodBody();
		methodDeclList_();
	}
	else
	{
		error("Erro: esperado encontrar declaracao de variaveis, construtor ou metodos.");
	}
}

void Parser::classBody_conflit_2()
{
	if (checklToken(CONSTRUCTOR))
	{
		constructDeclListOpt();
		methodDeclListOpt();
	}
	else if (checklToken(STRING) || checklToken(ID) || checklToken(INT))
	{
		classBody_conflit();
	}
}

/*
VarDeclListOpt -> VarDeclList | E
*/
void Parser::varDeclListOpt()
{

	if (lToken->name == ID || lToken->name == STRING || lToken->name == INT)
	{
		varDeclList();
	}
}

/*
VarDeclList -> VarDecl VarDeclList'
*/
void Parser::varDeclList()
{

	varDecl();
	varDeclList_();
}

/*
VarDeclList' -> VarDecl VarDeclList' | E
*/
void Parser::varDeclList_()
{
	if (lToken->name == ID || lToken->name == STRING || lToken->name == INT)
	{
		varDecl();
		varDeclList_();
	}
}

/*
VarDecl -> Type ID VarDeclOpt ; | Type [] ID VarDeclOpt ;
*/
void Parser::varDecl()
{

	type();
	if (lToken->name != ID)
	{
		match(SEP_LBRACKET, "Erro: esperado abre cholchetes.");
		match(SEP_RBRACKET, "Erro: esperado fecha cholchetes.");
	}
	match(ID, "Erro: esperado um identifiador.");
	varDeclOpt();
	match(SEP_SEMICOLON, "Erro: esperado ponto e virgula.");
}

/*
VarDeclOpt -> , ID VarDeclOpt | E
*/
void Parser::varDeclOpt()
{
	if (lToken->attribute == SEP_COMMA)
	{
		match(SEP_COMMA, "Erro: esperado ,.");
		match(ID, "Erro: esperado um identifiador.");
		varDeclOpt();
	}
}

/*
Type -> int | string | ID
*/
void Parser::type()
{
	if (lToken->name == ID)
	{
		match(ID, "Erro: esperado um identifiador.");
	}
	else if (lToken->name == STRING)
	{
		match(STRING, "Erro: esperado 'string'.");
	}
	else if (lToken->name == INT)
	{
		match(INT, "Erro: esperado \'int\'.");
	}
	else
	{
		error("Erro: Esperado ID, string ou int.");
	}
}

/*
ConstructDeclListOpt -> ConstructDeclList | E
*/
void Parser::constructDeclListOpt()
{

	if (lToken->name == CONSTRUCTOR)
	{
		constructDeclList();
	}
}

/*
ConstructDeclList -> ConstructDecl ConstructDeclList'
*/
void Parser::constructDeclList()
{
	constructDecl();
	constructDeclList_();
}

/*
ConstructDeclList' -> ConstructDecl ConstructDeclList' | E
*/
void Parser::constructDeclList_()
{
	if (lToken->name == CONSTRUCTOR)
	{
		constructDecl();
		constructDeclList_();
	}
}

/*
ConstructDecl -> constructor MethodBody
*/
void Parser::constructDecl()
{
	match(CONSTRUCTOR, "Erro: Esperado 'constructor'");
	methodBody();
}

/*
MethodBody -> ( ParamListOpt ) { StatementsOpt }
*/
void Parser::methodBody()
{
	match(SEP_LPARENTHESIS, "Erro: esperado '('");
	paramListOpt();
	match(SEP_RPARENTHESIS, "Erro: esperado ')'");

	match(SEP_LBRACE, "Erro: esperado '{'");
	statementsOpt();
	match(SEP_RBRACE, "Erro: esperado '}'");
}

/*
ParamListOpt -> ParamList | E
*/
void Parser::paramListOpt()
{

	if (lToken->name == ID || lToken->name == STRING || lToken->name == INT)
	{
		paramList();
	}
}

/*
ParamList -> Param ParamList'
*/
void Parser::paramList()
{
	param();
	paramList_();
}

/*
ParamList' -> , Param ParamList' | E
*/
void Parser::paramList_()
{
	if (lToken->attribute == SEP_COMMA)
	{
		match(SEP_COMMA, "Erro: esperado ','");
		param();
		paramList_();
	}
}

/*
Param -> Type ID | Type [] ID
*/
void Parser::param()
{
	type();
	if (lToken->name != ID)
	{
		match(SEP_LBRACKET, "Erro: esperado abre cholchetes.");
		match(SEP_RBRACKET, "Erro: esperado fecha cholchetes.");
	}
	match(ID, "Erro: esperado identificador");
}

/*
StatementsOpt -> Statements | E
*/
void Parser::statementsOpt()
{
	switch (lToken->name)
	{
	case PRINT:
	case READ:
	case RETURN:
	case SUPER:
	case IF:
	case FOR:
	case BREAK:
	case STRING:
	case INT:
	case ID:
		statements();
		break;

	default:
		if (lToken->attribute == SEP_SEMICOLON)
		{
			statements();
		}
		break;
	}
}

/*
Statements -> Statement Statements'
*/
void Parser::statements()
{
	statement();
	statements_();
}

/*
Statements' -> Statement Statements' | E
*/
void Parser::statements_()
{
	switch (lToken->name)
	{
	case PRINT:
	case READ:
	case RETURN:
	case SUPER:
	case IF:
	case FOR:
	case BREAK:
	case STRING:
	case INT:
	case ID:
		statement();
		statements_();
		break;

	default:
		if (lToken->attribute == SEP_SEMICOLON)
		{
			statement();
			statements_();
		}
		break;
	}
}

/*
Statement -> VarDeclList
| AtribStat ;
| PrintStat ;
| ReadStat ;
| ReturnStat ;
| SuperStat ;
| IfStat
| ForStat
| break ;
| ;
*/
void Parser::statement()
{
	switch (lToken->name)
	{
	case SEP:
		match(SEP_SEMICOLON, "Erro: esperado um ';'");
		break;
	case PRINT:
		printStat();
		match(SEP_SEMICOLON, "Erro: esperado um ';'");
		break;
	case READ:
		readStat();
		match(SEP_SEMICOLON, "Erro: esperado um ';'");
		break;
	case RETURN:
		returnStat();
		match(SEP_SEMICOLON, "Erro: esperado um ';'");
		break;
	case SUPER:
		superStat();
		match(SEP_SEMICOLON, "Erro: esperado um ';'");
		break;
	case BREAK:
		match(BREAK, "Erro: Esperado um 'break'");
		match(SEP_SEMICOLON, "Erro: esperado um ';'");
		break;
	case IF:
		ifStat();
		break;
	case FOR:
		forStat();
		break;
	case STRING:
	case INT:
		varDeclList();
		break;
	case ID:
		match(ID, "Erro: esperado um ID.");
		statement_VarDelcListAtribStatConflit();
		break;

	default:
		error("Erro: Esperado um statement.");
		break;
	}
}

/*
Conflit -> ID VarDeclOpt ; VarDeclList'
| [] ID VarDeclOpt ; VarDeclList'
| LValueComp = AtribStat' ;
*/
void Parser::statement_VarDelcListAtribStatConflit()
{

	if (checklToken(SEP_PERIOD) || checklToken(OP_EQ))
	{
		lValueComp();
		match(OP_EQ, "Erro: esperaod '='.");
		atribStat_();
		match(SEP_SEMICOLON, "Erro: esperaod ';'.");
	}
	else if (checklToken(ID))
	{
		match(ID, "Erro: esperado ID");
		varDeclOpt();
		match(SEP_SEMICOLON, "Erro: esperado ';'");
		varDeclList_();
	}
	else if (checklToken(SEP_LBRACKET))
	{
		match(SEP_LBRACKET, "Erro: esperado encontrar [");
		statement_VarDelcListAtribStatConflit_();
	}
	else
	{
		error("Erro: esperado ., =, ID ou [");
	}
}

/*
Conflit' -> ] ID VarDeclOpt ; VarDeclList'
| Expression ] LValueComp = AtribStat' ;
*/
void Parser::statement_VarDelcListAtribStatConflit_()
{
	if (checklToken(SEP_RBRACKET))
	{
		match(SEP_LBRACKET, "Erro: esperado ']'");
		match(ID, "Erro: esperado ID");
		varDeclOpt();
		match(SEP_SEMICOLON, "Erro: esperado ';'");
		varDeclList_();
	}
	else
	{
		expression();
		match(SEP_LBRACKET, "Erro: esperado ']'");
		lValueComp();
		match(OP_EQ, "Erro: esperado '='");
		atribStat_();
		match(SEP_SEMICOLON, "Erro: esperado ';'");
	}
}

/*
PrintStat -> print Expression
*/
void Parser::printStat()
{
	match(PRINT, "Erro: Esperado 'print'");
	expression();
}

/*
ReadStat -> read LValue
*/
void Parser::readStat()
{
	match(READ, "Erro: Esperado encontrar 'read'");
	lValue();
}

/*
ReturnStat -> return Expression
*/
void Parser::returnStat()
{
	match(RETURN, "Erro: Esperado encontrar 'return'");
	expression();
}

/*
SuperStat -> super ( ArgListOpt )
*/
void Parser::superStat()
{
	match(SUPER, "Erro: Esperado encontrar 'super'");
	match(SEP_LPARENTHESIS, "Erro: Esperado encontrar '('");
	argListOpt();
	match(SEP_RPARENTHESIS, "Erro: Esperado encontrar ')'");
}

/*
IfStat -> if ( Expression ) { Statements } IfStat'
*/
void Parser::ifStat()
{
	match(IF, "Erro: Esperado encontrar 'if'");

	match(SEP_LPARENTHESIS, "Erro: Esperado encontrar '('");
	expression();
	match(SEP_RPARENTHESIS, "Erro: Esperado encontrar ')'");

	match(SEP_LBRACE, "Erro: Esperado encontrar '{'");
	statements();
	match(SEP_RBRACE, "Erro: Esperado encontrar '}'");
	ifStat_();
}

/*
IfStat' -> else { Statements } | E
*/
void Parser::ifStat_()
{
	if (lToken->name == ELSE)
	{
		match(ELSE, "Erro: esperado encontrar 'else'");
		match(SEP_LBRACE, "Erro: Esperado encontrar '{'");
		statements();
		match(SEP_RBRACE, "Erro: Esperado encontrar '}'");
	}
}

/*
ForStat -> for ( AtribStatOpt ; ExpressionOpt ; AtribStatOpt ) { Statements }
*/
void Parser::forStat()
{
	match(FOR, "Erro: esperado encontrar 'for'");

	match(SEP_LPARENTHESIS, "Erro: Esperado encontrar '('");

	atribStatOpt();
	match(SEP_SEMICOLON, "Erro: esperado um ';'");
	expressionOpt();
	match(SEP_SEMICOLON, "Erro: esperado um ';'");
	atribStatOpt();
	match(SEP_SEMICOLON, "Erro: esperado um ';'");

	match(SEP_RPARENTHESIS, "Erro: Esperado encontrar ')'");

	match(SEP_LBRACE, "Erro: Esperado encontrar '{'");
	statements();
	match(SEP_RBRACE, "Erro: Esperado encontrar '}'");
}

/*
ExpressionOpt -> Expression | E
*/
void Parser::expressionOpt()
{
	if (checklToken(OP_PLUS) || checklToken(OP_MINUS) || checklToken(INTEGER_LITERAL) || checklToken(STRING_LITERAL) || checklToken(SEP_LPARENTHESIS) || checklToken(ID))
	{
		expression();
	}
}

/*
AtribStatOpt -> AtribStat | E
*/
void Parser::atribStatOpt()
{
	if (lToken->name == ID)
	{
		atribStat();
	}
}

/*
AtribStat -> LValue = AtribStat'
*/
void Parser::atribStat()
{
	lValue();
	match(OP_EQ, "Erro: esperado encontrar '='.");
	atribStat_();
}

/*
AtribStat' -> Expression | AllocExpression
*/
void Parser::atribStat_()
{
	if (lToken->name == NEW || lToken->name == STRING || lToken->name == INT || lToken->name == ID)
	{
		allocExpression();
	}
	else if (lToken->attribute == OP_PLUS || lToken->attribute == OP_MINUS)
	{
		expression();
	}
	else
	{
		error("Erro: esperado encontrar expressao ou alloc expressao.");
	}
}

void Parser::allocExpression()
{
	if (checklToken(NEW))
	{
		match(NEW, "Erro: esperado 'new'.");
		match(ID, "Erro: esperado ID.");

		match(SEP_LPARENTHESIS, "Erro: esperado '('.");
		argListOpt();
		match(SEP_RPARENTHESIS, "Erro: esperado ')'.");
	}
	else
	{
		type();
		match(SEP_LBRACKET, "Erro: esperado '['");
		expression();
		match(SEP_RBRACKET, "Erro: esperado '['");
	}
}

/*
Expression -> NumExpression Expression'
*/
void Parser::expression()
{
	numExpression();
	expression_();
}

/*
Expression' -> RelOp NumExpression | E
*/
void Parser::expression_()
{
	if (lToken->name == REL_OP)
	{
		match(REL_OP, "Erro: esperado um operador de relacao.");
		numExpression();
	}
}

/*
NumExpression -> Term NumExpression'
*/
void Parser::numExpression()
{
	term();
	numExpression_();
}

/*
NumExpression' -> + Term| - Term | E
*/
void Parser::numExpression_()
{
	switch (lToken->attribute)
	{
	case OP_PLUS:
		match(OP_PLUS, "Erro: esperado um operador +.");
		term();
		break;
	case OP_MINUS:
		match(OP_MINUS, "Erro: esperado um operador -.");
		term();
		break;
	default:
		break;
	}
}

/*
Term -> UnaryExpression Term'
*/
void Parser::term()
{
	unaryExpression();
	term_();
}

/*
Term' -> * UnaryExpression
| / UnaryExpression
| % UnaryExpression
| E
*/
void Parser::term_()
{
	switch (lToken->attribute)
	{
	case OP_PRODUCT:
		match(OP_PRODUCT, "Erro: esperado um operador *.");
		unaryExpression();
		break;
	case OP_DIVISION:
		match(OP_DIVISION, "Erro: esperado um operador /.");
		unaryExpression();
		break;
	case OP_MOD:
		match(OP_MOD, "Erro: esperado um operador %.");
		unaryExpression();
		break;
	default:
		break;
	}
}

/*
UnaryExpression -> + Factor
| - Factor
| Factor
*/
void Parser::unaryExpression()
{
	switch (lToken->attribute)
	{
	case OP_PLUS:
		match(OP_PLUS, "Erro: esperado um operador +.");
		factor();
		break;
	case OP_MINUS:
		match(OP_MINUS, "Erro: esperado um operador -.");
		factor();
		break;
	case SEP_LPARENTHESIS:
		factor();
		break;
	default:
		if (lToken->name == ID || lToken->name == STRING_LITERAL || lToken->name == INTEGER_LITERAL)
		{
			factor();
		}
		else
		{
			error("Erro: esperado expressao unaria.");
		}
		break;
	}
}

/*
Factor -> INTEGER_LITERAL
| STRING_LITERAL
| LValue
| ( Expression )
*/
void Parser::factor()
{
	switch (lToken->name)
	{
	case STRING_LITERAL:
		match(STRING_LITERAL, "Erro: esperado uma string");
		break;
	case INTEGER_LITERAL:
		match(INTEGER_LITERAL, "Erro: esperado um inteiro");
		break;
	case ID:
		lValue();
	default:
		if (lToken->attribute == SEP_LPARENTHESIS)
		{
			match(SEP_LPARENTHESIS, "Erro: esperado '('");
			expression();
			match(SEP_RPARENTHESIS, "Erro: esperado ')'");
		}
		else
		{
			error("Erro: esperado string, literal_value, expressao ou inteiro");
		}

		break;
	}
}

/*
LValue -> ID LValueComp
*/
void Parser::lValue()
{
	match(ID, "Erro: esperado um identificador.");
	lValueComp();
}

/*
LValueComp -> . ID LValueComp_
| [ Expression ] LValueComp
| E
*/
void Parser::lValueComp()
{
	if (lToken->attribute == SEP_PERIOD)
	{
		match(SEP_PERIOD, "Erro: esperado '.'");
		match(ID, "Erro: esperado identificador.");
		lValueComp_();
	}
	else if (lToken->attribute == SEP_LBRACKET)
	{
		match(SEP_LBRACKET, "Erro: esperado '['");
		expression();
		match(SEP_RBRACKET, "Erro: esperado ']'");
		lValueComp();
	}
}

/*
LValueComp_ -> . ID LValueComp_
| [ Expression ] LValueComp
| ( ArgListOpt ) LValueComp
| E
*/
void Parser::lValueComp_()
{
	if (lToken->attribute == SEP_PERIOD)
	{
		match(SEP_PERIOD, "Erro: esperado '.'");
		match(ID, "Erro: esperado ID");
		lValueComp_();
	}
	else if (lToken->attribute == SEP_LBRACKET)
	{
		match(SEP_LBRACKET, "Erro: esperado '['");
		expression();
		match(SEP_RBRACKET, "Erro: esperado ']'");
		lValueComp();
	}
	else if (lToken->attribute == SEP_LPARENTHESIS)
	{
		match(SEP_LPARENTHESIS, "Erro: esperado '('");
		argListOpt();
		match(SEP_RPARENTHESIS, "Erro: esperado ')'");
		lValueComp();
	}
}

/*
ArgListOpt -> ArgList
| E
*/
void Parser::argListOpt()
{
	if (checklToken(OP_PLUS) || checklToken(OP_MINUS) || checklToken(INTEGER_LITERAL) || checklToken(STRING_LITERAL) || checklToken(SEP_LPARENTHESIS) || checklToken(ID))
	{
		argList();
	}
}

/*
ArgList -> Expression  ArgList'
*/
void Parser::argList()
{
	expression();
	argList_();
}

/*
ArgList' -> , Expression ArgList' | E
*/
void Parser::argList_()
{
	if (lToken->attribute == SEP_COMMA)
	{
		match(SEP_COMMA, "Erro: esperado ','.");
		expression();
		argList_();
	}
}

/*
MethodDeclListOpt -> MethodDeclList | E
*/
void Parser::methodDeclListOpt()
{
	if (lToken->name == ID || lToken->name == STRING_LITERAL || lToken->name == INTEGER_LITERAL)
	{
		methodDeclList();
	}
}

/*
MethodDeclList -> MethodDecl MethodDeclList'
*/
void Parser::methodDeclList()
{
	methodDecl();
	methodDeclList_();
}

/*
MethodDeclList' -> MethodDecl MethodDeclList' | E
*/
void Parser::methodDeclList_()
{
	if (lToken->name == ID || lToken->name == STRING_LITERAL || lToken->name == INTEGER_LITERAL)
	{
		methodDecl();
		methodDeclList_();
	}
}

/*
MethodDecl -> Type ID MethodBody | Type [] ID MethodBody
*/
void Parser::methodDecl()
{
	type();
	if (lToken->name == SEP)
	{
		match(SEP_LBRACKET, "Erro: Esperado encontrar um '['.");
		match(SEP_RBRACKET, "Erro: Esperado encontrar um ']'.");
	}
	match(ID, "Erro: Esperado encontrar um ID.");
	methodBody();
}

void Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
