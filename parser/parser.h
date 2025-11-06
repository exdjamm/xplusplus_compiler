#include "scanner.h"

class Parser
{
private:
	Scanner *scanner;
	Token *lToken;

	void advance();
	void match(int, string);
	bool checklToken(int);

public:
	Parser(string);
	void run();
	void program();

	void classBody();
	void varDecl();
	void type();
	void constructDecl();
	void methodDecl();
	void param();
	void statement();
	void printStat();
	void readStat();
	void returnStat();
	void forStat();
	void lValue();
	void allocExpression();
	void unaryExpression();
	void factor();
	void methodBody();
	void superStat();

	void classList();
	void classList_();
	void classDecl();
	void classDecl_();
	void varDeclList();
	void varDeclList_();
	void constructDeclList();
	void constructDeclList_();
	void methodDeclList();
	void methodDeclList_();
	void paramList();
	void paramList_();
	void statements();
	void statements_();
	void ifStat();
	void ifStat_();
	void argList();
	void argList_();
	void expression();
	void expression_();
	void numExpression();
	void numExpression_();
	void term();
	void term_();
	void lValueComp();
	void lValueComp_();
	void atribStat();
	void atribStat_();

	void varDeclListOpt();
	void constructDeclListOpt();
	void methodDeclListOpt();
	void varDeclOpt();
	void paramListOpt();
	void statementsOpt();
	void atribStatOpt();
	void expressionOpt();
	void argListOpt();

	// void atribStat_lValueAllocExpressionConflict();
	// void atribStat_lValueAllocExpressionConflict_();
	void statement_VarDelcListAtribStatConflit();
	void statement_VarDelcListAtribStatConflit_();
	void classBody_conflit();
	void classBody_conflit_();
	void classBody_conflit_2();

	void error(string);
};