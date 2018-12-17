#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;
/* Corresponding rows values.
 * Used to validate tokens.
 * {LPAREN_T,EOF_T,IDENT_T,RPAREN_T,DEFINE_T,NUMLIT_T,STRLIT_T,SQUOTE_T,ELSE_T,IF_T,COND_T,LISTOP_T,CONS_T,AND_T,OR_T,NOT_T,NUMBERP_T,LISTP_T,ZEROP_T,NULLP_T,STRINGP_T,PLUS_T,MINUS_T,DIV_T,MULT_T,MODULO_T,ROUND_T,EQUALTO_T,GT_T,LT_T,GTE_T,LTE_T,DISPLAY_T,NEWLINE_T} */
static int syntacticalRuleNumbers [][34] = 
{
	{1,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83}, 	// <program> 			0
	{83,82,3,83,2,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83}, 		// <more_defines>		1
	{82,83,83,83,4,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83},		// <define>				2
	{5,83,5,6,83,5,5,5,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83}, 			// <stmt_list>			3
	{9,83,8,82,83,7,7,7,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83}, 		// <stmt>				4
	{82,83,82,82,83,10,11,12,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83}, 	// <literal>			5
	{13,83,13,82,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13}, 	// <quoted_lit>			6
	{14,83,14,15,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14}, 	// <more_tokens>		7
	{83,83,16,17,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83}, 	// <param_list>			8
	{18,83,18,19,83,18,18,18,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83}, 	// <else_part>			9
	{20,83,83,21,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83}, 	// <stmt_pair>			10
	{22,83,22,82,83,22,22,22,23,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83}, 	// <stmt_pair_body>		11
	{83,83,47,82,83,83,83,83,83,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,48,49}, 	// <action>				12
	{50,83,51,82,62,52,53,79,81,55,80,58,54,59,60,61,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,56,57} 	//<any_other_token>		13
};

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{	
	funk = false;
	/* Initializing mapping values
	 * for tokens.
	 * row[token_T] -> token_M */
	row[LPAREN_T] = LPAREN_M;
	row[EOF_T] = EOF_M;
	row[IDENT_T] = IDENT_M;
	row[RPAREN_T] = RPAREN_M;
	row[DEFINE_T] = DEFINE_M;
	row[NUMLIT_T] = NUMLIT_M;
	row[STRLIT_T] = STRLIT_M;
	row[SQUOTE_T] = SQUOTE_M;
	row[ELSE_T] = ELSE_M;
	row[IF_T] = IF_M; // 10
	row[COND_T] = COND_M;
	row[LISTOP_T] = LISTOP_M;
	row[CONS_T] = CONS_M;
	row[AND_T] = AND_M;
	row[OR_T] = OR_M;
	row[NOT_T] = NOT_M;
	row[NUMBERP_T] = NUMBERP_M;
	row[LISTP_T] = LISTP_M;
	row[ZEROP_T] = ZEROP_M;
	row[NULLP_T] = NULLP_M; // 20
	row[STRINGP_T] = STRINGP_M;
	row[PLUS_T] = PLUS_M;
	row[MINUS_T] = MINUS_M;
	row[DIV_T] = DIV_M;
	row[MULT_T] = MULT_M;
	row[MODULO_T] = MODULO_M;
	row[ROUND_T] = ROUND_M;
	row[EQUALTO_T] = EQUALTO_M;
	row[GT_T] = GT_M; 
	row[LT_T] = LT_M; // 30
	row[GTE_T] = GTE_M;
	row[LTE_T] = LTE_M;
	row[DISPLAY_T] = DISPLAY_M;
	row[NEWLINE_T] = NEWLINE_M; // 34


	lex = new LexicalAnalyzer (filename);
	gen = new CodeGen(filename, lex);

	string name = filename;
	name = name.substr(0, name.length()-3);

	string p2FileName = name + ".p2";
	p2file.open(p2FileName);

	token = lex->GetToken();
	previousToken = token;
	int ttlErrs = program ();
	cout << "Total syntax errors: " << ttlErrs << endl;

}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	p2file.close();
	delete lex;
	delete gen;
}

void SyntacticalAnalyzer::writeLstExpected(const token_type token) {
	/* 'TOKEN_T' expected */
	lex->ReportError("\'"
			+ lex->GetTokenName(token) 
			+ "\'"
			+ " expected ");
}

void SyntacticalAnalyzer::writeLstUnexpected(){
	/* 'lexeme' unexpected */
	lex->ReportError("\'" 
			+ lex->GetLexeme() 
			+ "\'"
			+ " unexpected ");
}

void SyntacticalAnalyzer::printP2File(const string &functionName, 
		const string &tokenName, 
		const string &lex) {
	/* Contains a list of rules applied and functions visited 
	 * Format:
	 * Entering Program function; current token is: LPAREN_T, lexeme: ( */
	p2file << "Entering " << functionName << " function; ";
	p2file << "current token is: " << tokenName << ", ";
	p2file << "lexeme: " << lex << endl;
}

void SyntacticalAnalyzer::printP2FileUsing(const string &ruleNumber)
{
	/* Used to print a list of terminal and non-terminal symbols 
	 * Format: 
	 * Using Rule 1 */
	p2file << "Using Rule " << ruleNumber << endl;
}

void SyntacticalAnalyzer::printP2Exiting(const string &funkyName, const string &tokenName)
{
	/* Format:
	 * Exiting Program function; current token is: EOF_T */
	p2file << "Exiting " << funkyName << " function; current token is: " << tokenName << endl;
}

/* All transition programs follow the same pattern.
 * They report the total number of errors found, 
 * the most critical part of the function is the beginning
 * because they write to the p2 file there function name,
 * the current token, and the lexeme. Then they call
 * validateToken(FUNCTIONNAME_F) which validates the
 * token by advancing on a non EOF_T on an invalid token
 * for that given function or if the token is already valid
 * it doesn't do any advancing of the token. If there is 
 * a single token expected that is not the token received 
 * an error is reported by calling writeLstExpected(TOKEN_T).
 * If there are more than one possible tokens that would 
 * have made a valid token then an error is reported by
 * calling writeLstUnexpected(). Upon exiting a given transition
 * function the functions final write to the p2 file is done 
 * signaling the function exit */
int SyntacticalAnalyzer::program(){
	int errors = 0;
	printP2File("Program", lex->GetTokenName(token), lex->GetLexeme());
	validateToken(PROGRAM_F);

	if(token==LPAREN_T)
	{
		printP2FileUsing("1");
		token = lex->GetToken();  
	}
	else {
		errors++; 
		writeLstExpected(LPAREN_T);
	}
	if (token == DEFINE_T)
	{
		errors += define();
	}
	else
	{
		errors++;
		writeLstExpected(DEFINE_T);
	}
	if (token == LPAREN_T)
	{
		token = lex->GetToken();
	}
	else
	{
		errors++;
		writeLstExpected(LPAREN_T);
	}

	errors += more_defines();

	if (token != EOF_T)
	{
		errors++;
		writeLstExpected(EOF_T);
	}

	printP2Exiting("Program", lex->GetTokenName(token));
	return errors;
}

int SyntacticalAnalyzer::stmt(){
	int errors = 0;
	printP2File("Stmt", lex->GetTokenName(token), lex->GetLexeme());
	validateToken(STMT_F);

	if(token==IDENT_T){
		printP2FileUsing("8");
		gen->WriteCode(0,lex->GetLexeme());
		token = lex->GetToken();
	}
	else if (token == LPAREN_T){
		//in a nest
		//need to push the lparen in
		builder.push(lex->GetLexeme());
		printP2FileUsing("9");
		
		token = lex->GetToken();
		if(previousToken == DISPLAY_T && builder.size()>1){//to clear the leading LPAREN and
		       						   //starting parent in stack
			builder.pop();//pop (
			builder.pop();//pop (
			previousToken = EOF_T;//using EOF just to change previousToken value
		}
		if(previousToken == DISPLAY_T && builder.size() > 0){//incase there is no paren following display
			builder.pop();//pop (
			cout << "builder size should = 0: " << builder.size() << endl;
			previousToken = EOF_T;//using EOF just to change previousToken value
		}	
		errors+= action();
		if(token == RPAREN_T){
			token = lex->GetToken();
		}
		else{
			writeLstExpected(RPAREN_T);
			errors++;
		}
	}
	else if (token == NUMLIT_T || token ==  STRLIT_T || token ==  SQUOTE_T) {
		printP2FileUsing("7");
		builder.push("Object(" + lex->GetLexeme() + ")");
		if(previousToken == DISPLAY_T && builder.size() > 1){//incase there is no paren following display
			builder.pop();//pop (
			builder.pop();
			cout << "builder size should = 0: " << builder.size() << endl;
			previousToken = EOF_T;//using EOF just to change previousToken value
		}	
		if(previousToken == DISPLAY_T && builder.size() > 0){//incase there is no paren following display
			builder.pop();//pop (
			cout << "builder size should = 0: " << builder.size() << endl;
			previousToken = EOF_T;//using EOF just to change previousToken value
		}	
		//gen->WriteCode(0, "Object(" + lex->GetLexeme() + ");\n");
		errors+=literal();
	}
	else {
		errors++;
		writeLstUnexpected();
	}

	printP2Exiting("Stmt", lex->GetTokenName(token));
	//gen->WriteCode(0,";");  
	//if I exit with LPAREN_T then print out final expression??
	if(token == LPAREN_T && expression.size() > 0){
		gen->WriteCode(0, expression.top() + ";\n");
		expression.pop();
	}
	else if(token == LPAREN_T){//only one statement

	}

	/*
	//TODO: not sure
	if(token == LPAREN_T){
		//make sure builder is clear. 
		while(!builder.empty()){
			builder.pop();	
		}
	}
	*/
	
	return errors;
}

int SyntacticalAnalyzer::stmt_pair_body(){
	int errors = 0;
	printP2File("Stmt_Pair_Body", lex->GetTokenName(token), lex->GetLexeme());
	validateToken(STMT_PAIR_BODY_F);

	if(token == ELSE_T)
	{
		printP2FileUsing("23");
		token = lex->GetToken();  
		gen->WriteCode(1, "else{\n ");
		errors+=stmt();
		gen->WriteCode(0, ";\n");
		if(token==RPAREN_T){
			token = lex->GetToken();
			gen->WriteCode(1, "}\n");
		}
		else 
		{
			writeLstExpected(RPAREN_T);
			errors++;
		}
	}

	else 
	{
		printP2FileUsing("22");
		if(previousToken == COND_T){
			gen->WriteCode(1, "if(");
			//setting previousToken to EOF_T so we hit the else 
			//in the next pass through
			previousToken = EOF_T;
		}
		else{
			gen->WriteCode(1, "else if(");
		}
		errors+=stmt();
		gen->WriteCode(0, "){ \n");
		errors+=stmt();
		gen->WriteCode(0, ";\n");
		if(token==RPAREN_T){
			gen->WriteCode(1, "}\n");
			token = lex->GetToken();
		}
		else 
		{
			errors++;
			writeLstExpected(RPAREN_T);
		}
		errors+=stmt_pair();
	}

	printP2Exiting("Stmt_Pair_Body", lex->GetTokenName(token));
	return errors;
}

int SyntacticalAnalyzer::stmt_list()
{
	string tempbuilder = "";
	//string building = "";
	vector<string> temp;
	string saveOp;
	int errors = 0;
	bool expressionFirst;
	printP2File("Stmt_List", lex->GetTokenName(token), lex->GetLexeme());
	validateToken(STMT_LIST_F);
	//bool funk = (token == IDENT_T) ? true:false;
	if (token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		cout << "lexeme: " << lex->GetLexeme() << endl;
		//in a nest of statements, so push the lexeme into stack
		/*do in stmt
		if(token == NUMLIT_T){//convert to object
			cout << "pushing into builder: " << lex->GetLexeme() << endl;
			builder.push("Object(" + lex->GetLexeme() + ")");
			cout << "builder size: " << builder.size() << endl;
		}
		else{
			cout << "pushing into builder: " << lex->GetLexeme() << endl;
			builder.push(lex->GetLexeme());
			cout << "builder size: " << builder.size() << endl;
		}
		*/
		printP2FileUsing("5");
		errors += stmt();
		if(funk && (lex->pos != lex->line.length() -1 ) )
			gen->WriteCode(0,", ");

		//gen->WriteCode(0, " \n ");  
		errors+= stmt_list();
	}

	else if (token == RPAREN_T){
		//here we end a parens in a nested statement
		//build expression 
		previousToken = token;
		if(builder.size() > 1){//we have work to do!
			cout << "builder size: " << builder.size() << endl;
			
			while(builder.size() > 0 && builder.top() != "("){
				temp.push_back(builder.top());
				builder.pop();
			}
				cout << "printing out temp:\n";
			for(auto i = temp.begin(); i < temp.end(); i++){
				cout << *i << " ";
			}
			cout << endl;
			if(builder.size() >0){
				builder.pop();//pop off "("
				if(isOperator(builder.top())){
					expressionFirst = true;
				}
				else{
					expressionFirst = false;
				}
			}
			if(temp.size() == 3){//means we have a full expression, so we can format
				//and push onto stack. 	
				//because temp is in the format operator, operator, operand
				if(temp[2] == "modulo"){
					temp[2] = "%";
				}
				tempbuilder = "(" + temp[1] + temp[2] + temp[0] + ")";  	    
				
				expression.push(tempbuilder);
				tempbuilder = "";
			}
			else if(temp.size() == 2){//the second operand is the expression in expression stack
				//could be expression operator operand or other way around. 
				if(expressionFirst){
					if(temp[1] == "round"){
						tempbuilder = "(round(" + temp[0] + "))";
					}
					else if(temp[1] == "modulo"){
						tempbuilder = "(" + expression.top() + " % " + temp[0] + ")";
						expression.pop();
					}
					else{
						tempbuilder = "(" + expression.top() + temp[1] + temp[0] + ")";
						expression.pop();
					}
				}
				else{
					if(temp[1] == "round"){
						tempbuilder = "(round(" + temp[0] + ")" + expression.top() + ")";
					}
					else if(temp[1] == "modulo"){
						tempbuilder = "(" + temp[0] + " % " + expression.top() + ")";
						expression.pop();
					}
					else{
						tempbuilder = "(" + temp[0] + temp[1] + expression.top() + ")";
						expression.pop();
					}
				}
				//cout << "temp0: " << temp[0] << endl;
				cout << "expression size: " << expression.size() << endl;
				expression.push(tempbuilder);
				tempbuilder = "";
			}
			else{//there is multiple operands with one operator
				for(auto i = temp.begin(); i < temp.end(); i++){
					//find what operator the expression has
					if(isOperator(*i)){
						saveOp = *i;
					}
				}
				//now generate the c++ expression
				tempbuilder = "(";
				for(auto i = temp.begin(); i < temp.end(); i++){
					if(!isOperator(*i)){
						if(i == temp.begin()){
							tempbuilder += (*i);
						}
						else{
							tempbuilder += (saveOp + " " + *i);
						}
					}
				}
				tempbuilder += ")";
				expression.push(tempbuilder);
				tempbuilder = "";
			}
			cout << "TEMP SIZE = " << temp.size() << endl;

		}

		printP2FileUsing("6");
	}
	else
	{
		errors++;
		writeLstUnexpected();
	}

	printP2Exiting("Stmt_List", lex->GetTokenName(token));

/*	
	if(token == RPAREN_T){
		//make sure builder is clear. 
		while(!builder.empty()){
			builder.pop();	
		}
	}
*/
	return errors;
}

int SyntacticalAnalyzer::stmt_list(string s)
{
	int errors = 0;
	printP2File("Stmt_List", lex->GetTokenName(token), lex->GetLexeme());
	validateToken(STMT_LIST_F);

	bool funk = 0;
	if (token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		if (token == IDENT_T)
			funk = 1;
		printP2FileUsing("5");
		errors += stmt();
		if (s == ">" || s == "<" || s == "<=" || s == ">=" || !funk || s == "and" || s=="or") {
			if(s=="and") gen->WriteCode(0," && ");
			else if (s == "or") gen->WriteCode(0, " || ");
			else gen->WriteCode(0, " " + s + " ");
		}
		//if(!funk)
		//    gen->WriteCode(0, s + " ");  
		//gen->WriteCode(0, " \n ");  
		errors+= stmt_list();
		if(s != ">" && s != "<" && s != ">=" && s != "<="){//there might be some other conditionals in actions to check. 
			gen->WriteCode(0, ";\n"); 
		}
	}

	else if (token == RPAREN_T){
		printP2FileUsing("6");
	}

	else
	{
		errors++;
		writeLstUnexpected();
	}

	printP2Exiting("Stmt_List", lex->GetTokenName(token));
	//gen->WriteCode(0,";\n");  
	return errors;
}

int SyntacticalAnalyzer::more_defines(){
	int errors = 0;
	printP2File("More_Defines", lex->GetTokenName(token), lex->GetLexeme());
	validateToken(MORE_DEFINES_F);

	if (token == IDENT_T)
	{
		printP2FileUsing("3");
		token = lex->GetToken();
		errors += stmt_list();
		if (token == RPAREN_T)
		{
			token = lex->GetToken();
		}
		else 
		{
			errors++;
			writeLstExpected(RPAREN_T);
		}
	}

	else if (token == DEFINE_T)
	{
		printP2FileUsing("2");
		errors += define();

		if (token == LPAREN_T)
			token = lex->GetToken();

		else
		{
			errors++;
			writeLstExpected(LPAREN_T);   
		}

		errors += more_defines();
	}

	else
	{
		errors++;
		writeLstUnexpected();
	}

	printP2Exiting("More_Defines", lex->GetTokenName(token));
	return errors;

}

int SyntacticalAnalyzer::define(){
	int errors = 0;
	printP2File("Define", lex->GetTokenName(token), lex->GetLexeme());
	validateToken(DEFINE_F);

	if(token == DEFINE_T){
		printP2FileUsing("4");
		token = lex->GetToken();

		if(token==LPAREN_T)
			token = lex->GetToken();
		else 
		{
			errors++;
			writeLstExpected(LPAREN_T);
		}

		if(token==IDENT_T){
			if(lex->GetLexeme() == "main"){
				gen->WriteCode(0, "int " + lex->GetLexeme() + "(");//start of a function  

			}
			else{								
				gen->WriteCode(0, "Object " + lex->GetLexeme() + "(");//start of a function  
			}

			token = lex->GetToken();
		}
		else 
		{
			errors++;
			writeLstExpected(IDENT_T);
		}

		errors += param_list();

		//at this point parameters are generated in c++ and the closing ) and opening { generated 
		//in param_list

		if(token==RPAREN_T)
			token = lex->GetToken();
		else
		{
			errors++;
			writeLstExpected(RPAREN_T);
		}

		errors += stmt();
		//gen->WriteCode(0,";\n");  
		errors += stmt_list();

		if (token == RPAREN_T)
			token = lex->GetToken();
		else
		{
			errors++;
			writeLstExpected(RPAREN_T);
		}
	}

	else
	{
		errors++;
		writeLstExpected(DEFINE_T);
	}

	printP2Exiting("Define", lex->GetTokenName(token));
	gen->WriteCode(1, "return 0;\n}\n");//function code generation completed. 
	return errors;
	}

	// Function "action" attempts to apply rules 24-49.
	int SyntacticalAnalyzer::action() {
		int errors = 0;
		string oldTok = "";
		string num1, num2;
		printP2File("Action", lex->GetTokenName(token), lex->GetLexeme());
		validateToken(ACTION_F);

		switch (token) {

			case IF_T:
				printP2FileUsing("24");
				gen->WriteCode(1, "if(");  
				token = lex->GetToken();
				errors += stmt();
				gen->WriteCode(0, "){\n");  
				gen->WriteCode(1, "");
				errors += stmt();
				gen->WriteCode(0, ";\n");
				gen->WriteCode(1,"}\n");  
				errors += else_part();
				gen->WriteCode(0, "\n");
				break;

			case COND_T:
				printP2FileUsing("25");
				previousToken = token;
				token = lex->GetToken();

				if (token == LPAREN_T) 
					token = lex->GetToken();

				else 
				{
					errors++;
					writeLstExpected(LPAREN_T);
				}

				errors += stmt_pair_body();
				break;

			case LISTOP_T:
				printP2FileUsing("26");
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				gen->WriteCode(1, "listop(\"" + oldTok + "\",");
				errors += stmt();
				break;

			case CONS_T:
				printP2FileUsing("27");
				token = lex->GetToken();
				//gen->WriteCode(1, "");  
				gen->WriteCode(1, "cons(");
				errors += stmt();
				gen->WriteCode(0, ",");
				//gen->WriteCode(0, " % ");  
				errors += stmt();
				gen->WriteCode(0, ");\n");
				//gen->WriteCode(0, ";\n");  
				break;

			case AND_T:
				printP2FileUsing("28");
				gen->WriteCode(1, "");  
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt_list(oldTok);
				break;

			case OR_T:
				printP2FileUsing("29");
				gen->WriteCode(1, "");  
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt_list(oldTok);
				break;

			case NOT_T:
				printP2FileUsing("30");
				token = lex->GetToken();
				errors += stmt();
				break;

			case NUMBERP_T:
				printP2FileUsing("31");
				token = lex->GetToken();
				gen->WriteCode(1, "numberp(");
				errors += stmt();
				gen->WriteCode(0, ")");
				break;

			case LISTP_T:
				printP2FileUsing("32");
				token = lex->GetToken();
				gen->WriteCode(1, "listp(");
				errors += stmt();
				gen->WriteCode(0, ")");
				break;

			case ZEROP_T:
				printP2FileUsing("33");
				token = lex->GetToken();
				gen->WriteCode(1, "zerop(");
				errors += stmt();
				gen->WriteCode(0, ")");
				break;

			case NULLP_T:
				printP2FileUsing("34");
				token = lex->GetToken();
				gen->WriteCode(1, "nullp(");
				errors += stmt();
				gen->WriteCode(0, ")");
				break;

			case STRINGP_T:
				printP2FileUsing("35");
				token = lex->GetToken();
				gen->WriteCode(1, "stringp(");
				errors += stmt();
				gen->WriteCode(0, ")");
				break;

			case PLUS_T:
				printP2FileUsing("36");
				//gen->WriteCode(1, "");  
				//this line is used to push operands and operators onto
				//a stack called builder, once we reach a RPAREN_T, we will build a 
				//properly formatted c++ expression and push it onto the 
				//expression stack. once builder is empty, we know the entire nested
				//scheme statement is complete so we can then write the final c++
				//formatted expression that was stored in the expression stack. 
				builder.push(lex->GetLexeme());
				//oldTok = lex->GetLexeme();
				token = lex->GetToken();
				//errors += stmt_list(oldTok);
				errors += stmt_list();
				break;

			case MINUS_T:
				printP2FileUsing("37");
				builder.push(lex->GetLexeme());
				//gen->WriteCode(1, "");  
				//oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt();
				//gen->WriteCode(0, " - ");  
				errors += stmt_list();
				//gen->WriteCode(0, ";\n");  
				break;

			case DIV_T:
				printP2FileUsing("38");
				builder.push(lex->GetLexeme());
				//gen->WriteCode(1, "");  
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt();
				//gen->WriteCode(0, " / ");  
				errors += stmt_list();
				//gen->WriteCode(0, ";\n");  
				break;

			case MULT_T:
				printP2FileUsing("39");
				builder.push(lex->GetLexeme());
				//gen->WriteCode(1, "");  
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				//errors += stmt_list(oldTok);
				errors += stmt_list();
				//gen->WriteCode(0, ";\n");
				break;

			case MODULO_T:
				printP2FileUsing("40");
				builder.push(lex->GetLexeme());
				token = lex->GetToken();
				//gen->WriteCode(1, "");  
				num1 = lex->GetLexeme();
				errors += stmt();
				num2 = lex->GetLexeme();
				//gen->WriteCode(0, " % ");  
				errors += stmt();
				if(token == RPAREN_T){//I think its a single statement? TODO NOPE
				//	gen->WriteCode(0, num1 + " % " + num2 + ";\n");	
				}
				//gen->WriteCode(0, ";\n");  
				break;

			case ROUND_T:
				//TODO: take care of individual round
				//had to put it into the stack for nesting!
				printP2FileUsing("41");
				//gen->WriteCode(1, "round(");  
				builder.push(lex->GetLexeme());
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt();
				if(token == RPAREN_T){//we only had a single statement so our nesting
						      //logic will not print. so we do here. TODO nope
				//gen->WriteCode(0, "round(Object(" + oldTok + "));\n");  

				}
				//gen->WriteCode(0, ");\n");
				break;

			case EQUALTO_T:
				printP2FileUsing("42");
				gen->WriteCode(1, "");  
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt_list(oldTok);
				break;

			case GT_T:
				printP2FileUsing("43");
				gen->WriteCode(0, "");  
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt_list(oldTok);
				//errors += stmt_list();
				break;

			case LT_T:
				printP2FileUsing("44");
				gen->WriteCode(1, "");  
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt_list(oldTok);
				break;

			case GTE_T:
				printP2FileUsing("45");
				gen->WriteCode(1, "");  
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt_list(oldTok);
				break;

			case LTE_T:
				printP2FileUsing("46");
				gen->WriteCode(1, "");  
				oldTok = lex->GetLexeme();
				token = lex->GetToken();
				errors += stmt_list(oldTok);
				break;

			case IDENT_T:
				printP2FileUsing("47");
				gen->WriteCode(1, "");  
				oldTok = lex->GetLexeme();
				gen->WriteCode(0,lex->GetLexeme() + "(");
				token = lex->GetToken();
				//errors += stmt_list(oldTok);
				funk = 1;
				errors += stmt_list();
				funk = 0;
				gen->WriteCode(0,");\n");
				break;

			case DISPLAY_T:
				printP2FileUsing("48");
				previousToken = token;
				token = lex->GetToken();
				gen->WriteCode(1, "cout << ");//start of a function  
				errors += stmt();
				break;

			case NEWLINE_T:
				printP2FileUsing("49");
				gen->WriteCode(1, "cout << endl;\n");  
				previousToken = DISPLAY_T;
				token = lex->GetToken();
				break;

			default:
				errors++;
				writeLstUnexpected();
				break;
		}

		printP2Exiting("Action", lex->GetTokenName(token));
		//gen->WriteCode(0, "; \n");//start of a function  
		return errors;
	}

	// Function "any_other_token" attempts to apply rules 50-81.
	int SyntacticalAnalyzer::any_other_token() {
		int errors = 0;
		string oldLexeme;
		printP2File("Any_Other_Token", lex->GetTokenName(token), lex->GetLexeme());
		validateToken(ANY_OTHER_TOKEN_F);
		cout << "in anyother, token is " + lex->GetTokenName(token) << endl;
		switch (token) {

			case LPAREN_T:
				previousToken = LPAREN_T;
				printP2FileUsing("50");
				//printing the lparen, because its in a quote
				gen->WriteCode(0, lex->GetLexeme());
				//gen->WriteCode(0,"Object(\"(");  
				token = lex->GetToken();
				errors += more_tokens();
				//gen->WriteCode(0,")\");\n");
				if (token == RPAREN_T) {
					//also writing rparen here because of a quote. 
					gen->WriteCode(0, lex->GetLexeme());
					token = lex->GetToken();
				}
				else {
					errors++;
					writeLstExpected(RPAREN_T);
				}
				break;

			case IDENT_T:
				printP2FileUsing("51");
				/*
				   if(previousToken == SQUOTE_T){
				   gen->WriteCode(1,"Object(" + lex->GetLexeme() + ")");//start of a function  
				   }
				   else{
				   gen->WriteCode(2,lex->GetLexeme());  
				   }
				   */
				oldLexeme = lex->GetLexeme();
				token = lex->GetToken();

				if(token == RPAREN_T){
					//here following a squote so dont need any Object?
					gen->WriteCode(0, oldLexeme);

					/*
					   if(previousToken == LPAREN_T){
					   gen->WriteCode(0, oldLexeme);

					//gen->WriteCode(1,"Object(" + oldLexeme + ")");//start of a function  
					}
					else{
					gen->WriteCode(0, oldLexeme + ";\n");
					//gen->WriteCode(0,"Object(" + oldLexeme + ");\n");//start of a function  
					}
					*/
				}
				else{
					//gen->WriteCode(0,lex->GetLexeme() + " ");  
					gen->WriteCode(0, oldLexeme + " ");
				}
				break;

			case NUMLIT_T:
				printP2FileUsing("52");
				oldLexeme = lex->GetLexeme();
				cout << "this should be 1: " + oldLexeme << endl;
				token = lex->GetToken();
				if(token == RPAREN_T){
					gen->WriteCode(0, oldLexeme);
				}
				else{
					gen->WriteCode(0, oldLexeme + " ");
					//gen->WriteCode(0,lex->GetLexeme() + " ");  
				}
				break;

			case STRLIT_T:
				printP2FileUsing("53");
				oldLexeme = lex->GetLexeme();
				token = lex->GetToken();
				if(token == RPAREN_T){
					gen->WriteCode(0, oldLexeme);
				}
				else{
					//gen->WriteCode(0,lex->GetLexeme() + " ");  
					gen->WriteCode(0, oldLexeme + " ");
				}
				break;

			case CONS_T:
				printP2FileUsing("54");
				token = lex->GetToken();
				break;

			case IF_T:
				printP2FileUsing("55");
				token = lex->GetToken();
				break;

			case DISPLAY_T:
				printP2FileUsing("56");
				token = lex->GetToken();
				break;

			case NEWLINE_T:
				printP2FileUsing("57");
				gen->WriteCode(1, "cout << endl;\n");  
				token = lex->GetToken();
				break;

			case LISTOP_T:
				printP2FileUsing("58");
				token = lex->GetToken();
				break;

			case AND_T:
				printP2FileUsing("59");
				token = lex->GetToken();
				break;

			case OR_T:
				printP2FileUsing("60");
				token = lex->GetToken();
				break;

			case NOT_T:
				printP2FileUsing("61");
				token = lex->GetToken();
				break;

			case DEFINE_T:
				printP2FileUsing("62");
				token = lex->GetToken();
				break;

			case NUMBERP_T:
				printP2FileUsing("63");
				token = lex->GetToken();
				break;

			case LISTP_T:
				printP2FileUsing("64");
				token = lex->GetToken();
				break;

			case ZEROP_T:
				printP2FileUsing("65");
				token = lex->GetToken();
				break;

			case NULLP_T:
				printP2FileUsing("66");
				token = lex->GetToken();
				break;

			case STRINGP_T:
				printP2FileUsing("67");
				token = lex->GetToken();
				break;

			case PLUS_T:
				printP2FileUsing("68");
				token = lex->GetToken();
				break;

			case MINUS_T:
				printP2FileUsing("69");
				token = lex->GetToken();
				break;

			case DIV_T:
				printP2FileUsing("70");
				token = lex->GetToken();
				break;

			case MULT_T:
				printP2FileUsing("71");
				token = lex->GetToken();
				break;

			case MODULO_T:
				printP2FileUsing("72");
				token = lex->GetToken();
				break;

			case ROUND_T:
				printP2FileUsing("73");
				token = lex->GetToken();
				break;

			case EQUALTO_T:
				printP2FileUsing("74");
				token = lex->GetToken();
				break;

			case GT_T:
				printP2FileUsing("75");
				token = lex->GetToken();
				break;

			case LT_T:
				printP2FileUsing("76");
				token = lex->GetToken();
				break;

			case GTE_T:
				printP2FileUsing("77");
				token = lex->GetToken();
				break;

			case LTE_T:
				printP2FileUsing("78");
				token = lex->GetToken();
				break;

			case SQUOTE_T:
				printP2FileUsing("79");
				cout << "into anyother squote\n";
				gen->WriteCode(0,lex->GetLexeme());  
				token = lex->GetToken();
				errors += any_other_token();
				break;

			case COND_T:
				printP2FileUsing("80");
				token = lex->GetToken();
				break;

			case ELSE_T:
				printP2FileUsing("81");
				token = lex->GetToken();
				break;

			default:
				errors++;
				writeLstUnexpected();
				break;
		}

		printP2Exiting("Any_Other_Token", lex->GetTokenName(token));
		//gen->WriteCode(0, ";\n");
		return errors;
	}

	// Function "stmt_pair" attempts to apply rules 20-21.
	int SyntacticalAnalyzer::stmt_pair() {
		int errors = 0;
		printP2File("Stmt_Pair", lex->GetTokenName(token), lex->GetLexeme());
		validateToken(STMT_PAIR_F);

		if (token == LPAREN_T) {
			printP2FileUsing("20");
			token = lex->GetToken();
			errors += stmt_pair_body();
		}

		else if (token == RPAREN_T)
			printP2FileUsing("21");

		else
		{
			errors++;
			writeLstUnexpected();
		}


		printP2Exiting("Stmt_Pair", lex->GetTokenName(token));
		return errors;
	}

	// Function "param_list" attempts to apply rules 16-17.
	int SyntacticalAnalyzer::param_list() {
		int errors = 0;
		printP2File("Param_List", lex->GetTokenName(token), lex->GetLexeme());
		validateToken(PARAM_LIST_F);

		if (token == IDENT_T) {
			string save_ident = lex->GetLexeme();
			printP2FileUsing("16");
			token = lex->GetToken(); 
			if(token == RPAREN_T){//paramlist is done. 
				//gen -> WriteCode(0, "Object " + save_ident + "){\n"); 
				gen -> WriteCode(0, "Object " + save_ident); 

			}
			else{
				gen -> WriteCode(0, "Object " + save_ident + ", "); 
			}
			errors += param_list();
		}

		else if (token == RPAREN_T){
			gen -> WriteCode(0, "){\n"); 
			printP2FileUsing("17");	

		}
		else 
		{
			errors++;
			writeLstUnexpected();
		}

		printP2Exiting("Param_List", lex->GetTokenName(token));
		return errors;
	}

	int SyntacticalAnalyzer::else_part()
	{
		int errors = 0;
		printP2File("Else_Part", lex->GetTokenName(token), lex->GetLexeme());
		validateToken(ELSE_PART_F);
		gen->WriteCode(1, "else{\n");  

		if (token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
		{
			printP2FileUsing("18");
			gen->WriteCode(2, "");
			errors += stmt();
			gen->WriteCode(0, ";\n");
		}

		else if (token == RPAREN_T)
		{
			printP2FileUsing("19");
		}

		else 
		{
			errors++;
			writeLstUnexpected();
		}

		printP2Exiting("Else_Part", lex->GetTokenName(token));
		gen->WriteCode(1, "}");  
		return errors;
	}

	int SyntacticalAnalyzer::quoted_lit()
	{
		int errors = 0;
		printP2File("Quoted_Lit", lex->GetTokenName(token), lex->GetLexeme());
		validateToken(QUOTED_LIT_F);

		// These two tokens are the only possible errors
		if (token == EOF_T || token == RPAREN_T) {
			errors++;
			writeLstUnexpected();
		}

		else
		{
			printP2FileUsing("13");
			cout << "going into anyother, lexeme = " +lex->GetLexeme() << endl;
			gen->WriteCode(0, "\"");
			errors += any_other_token();
		}
		gen->WriteCode(0, "\";\n");
		printP2Exiting("Quoted_Lit", lex->GetTokenName(token));
		return errors;
	}

	int SyntacticalAnalyzer::literal()
	{
		int errors = 0;
		printP2File("Literal", lex->GetTokenName(token), lex->GetLexeme());
		validateToken(LITERAL_F);

		if (token == NUMLIT_T)
		{
			printP2FileUsing("10");
			if(builder.empty()){//not a nest of statements so just generate
				gen->WriteCode(0,"Object(" + lex->GetLexeme() + ");\n");  

			}
			token = lex->GetToken();
			if(lex->GetLexeme() == ")"){
				//gen->WriteCode(0, ";\n");
			}
		}

		else if (token == STRLIT_T)
		{
			gen->WriteCode(0,lex->GetLexeme());//start of a function  
			printP2FileUsing("11");
			token = lex->GetToken();
			if(lex->GetLexeme() == ")"){
				gen->WriteCode(0, ";\n");
			}
		}

		else if (token == SQUOTE_T)
		{
			printP2FileUsing("12");
			previousToken = token;
			token = lex->GetToken();
			//cout << "lexeme in literal = " + lex->GetLexeme() << endl;
			//gen->WriteCode(0,lex->GetLexeme());  
			errors += quoted_lit();
		}

		else 
		{
			errors++;
			writeLstUnexpected();
		}

		printP2Exiting("Literal", lex->GetTokenName(token));
		return errors;
	}


	int SyntacticalAnalyzer::more_tokens()
	{
		int errors = 0;
		printP2File("More_Tokens", lex->GetTokenName(token), lex->GetLexeme());
		validateToken(MORE_TOKENS_F);

		// This is the only rule that can throw an err
		if (token == EOF_T) 
		{
			errors++;
			writeLstUnexpected();
		}

		else if (token == RPAREN_T)
			printP2FileUsing("15");

		/* If the token is not RPARENT_T or EOF_T 
		 * apply rule 14 */
		else 
		{
			printP2FileUsing("14");
			errors += any_other_token();
			errors += more_tokens();
		}

		printP2Exiting("More_Tokens", lex->GetTokenName(token));
		return errors;
	}

	bool SyntacticalAnalyzer::isValidToken(functionRuleNumberMapping fMap)
	{
		/* This function takes the calling
		 * functions enum and the current
		 * token. Then the function retrieves
		 * its value from the syntactical
		 * analyzer table. If the retrieved
		 * value is 82 or 83, the token is 
		 * not valid and the function returns
		 * false. For any other values the 
		 * function returns true. */
		tokenMapper token_M = row[token];

		if (syntacticalRuleNumbers[fMap][token_M] != 82 && syntacticalRuleNumbers[fMap][token_M] != 83)
			return true;

		return false;
	}

	void SyntacticalAnalyzer::validateToken(functionRuleNumberMapping fMap)
	{   
		/* This function is to be placed at the
		 * beginning of each transition function. 
		 * Its job is to if the first token is invalid then
		 * report it and advance the token unless 
		 * that token is an EOF_T and advance all tokens that are invalid
		 * until either a valid token is found or 
		 * if an EOF_T is found token advancing 
		 * will halt. */

		/* If a token != EOF_T only then should
		 * the token be advanced. 
		 * 
		 * Everytime this function is called this 
		 * expression is only evaluated once and 
		 * if an err was to occur it's to only be 
		 * reported once. */
		if (!isValidToken(fMap))
		{
			writeLstUnexpected();

			if (token != EOF_T)
				token = lex->GetToken();
		}

		/* If we are entering a function and we have an EOF_T 
		 * there is nothing to advance */
		while (!isValidToken(fMap) && token != EOF_T)
			token = lex->GetToken();
	}
	bool SyntacticalAnalyzer::isOperator(string x){
		if(x == "+" || x == "-" || x == "/" || x == "round" ||x == "modulo" || x == "*"){
			return true;
		}	
		else{
			return false;
		}
  	} 
