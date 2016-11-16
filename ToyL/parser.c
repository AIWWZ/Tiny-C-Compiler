#include "pcc.h";

/*������ǰ����*/
void Skip(int TkCode) {
	if (token->TkCode == TkCode) {
		NextToken();
		return;
	}
	else {
		printf("Absence %s\n", Token_Table->Data[TkCode]);
	}
}

/*�ж��Ƿ�����˵˵����*/
int IsType(int TkCode) {
	switch (TkCode) {
	case INT:
	case DOUBLE:
	case CHAR:
	case VOID:
		return TRUE;
	default:
		return FALSE;
	}
}

/*
<�﷨����> -> {<ȫ������>}<�ļ�������>
*/
void Parser() {
	NextToken();
	while (token->TkCode != EOF) {
		Declaration();
	} 
}

/*
<����> -> <��������><������>( <������>
							 | ["="<��ֵ���ʽ>]{","<������>["="<��ֵ���ʽ>]}";" )

<��������> -> "void" | "char" | "int" | "double"
<������> -> "��ʶ��"<��������׺>
<��������׺> -> { "(" ")"
				| "(" <�β��б�> ")"
				| "[" "]" 
				| "[" "���ͳ���" "]" }
<������> -> <�������>
*/
void Declaration() {
	TypeState();
	Declarator();
	while (TRUE) {
		if (token->TkCode == L_BRACE) {
			FuncBody();
			break;
		}
		else {
			if (token->TkCode == ASSIGN) {
				NextToken();
				AssignExpr();
			}
			else if (token->TkCode == COMMA) {
				NextToken();
				Declarator();
			}
			else {
				Skip(SEMI);
				break;
			}
		}

	}
}

/*
<��������> -> "void" | "char" | "int" | "double"
*/
void TypeState(){
	switch (token->TkCode) {
	case CHAR:
	case VOID:
	case INT:
	case DOUBLE:
		NextToken();
		break;
	default:
		Error("C_error: Absence type statement!");
		break;
	}
}

/*
<������> -> "��ʶ��"<��������׺>
*/
void Declarator() {
	if (token->TkCode < IDENT) {
		Error("C_error: Absence identifier!");
	}
	else {
		NextToken();
	}
	DeclarationSuffix();
}

/*
<��������׺> -> { "(" ")"
				| "(" <�β��б�> ")"
				| "[" "]"
				| "[" "���ͳ���" "]" }
*/
void DeclarationSuffix() {
	if (token->TkCode == L_PAREN) {
		FormalParaList();
		Skip(R_PAREN);
	}
	else if (token->TkCode == L_BRACK) {
		NextToken();
		if (token->TkCode == C_INT) {
			NextToken();
		}
		Skip(R_BRACK);
		DeclarationSuffix();
	}
}

/*
<�β��б�>-><����˵����>[<������>]{","<����˵����><������>}
*/
void FormalParaList() {
	NextToken();
	while (token->TkCode != R_PAREN) {
		TypeState();
		Declarator();
		NextToken();
		if (token->TkCode == R_PAREN) {
			break;
		}
		else {
			Skip(COMMA);
		}
	}
}

/*
<������> -> <�������>
*/
void FuncBody() {
	ComplexStatement();
	Skip(R_BRACE);
}

/*
<���> -> <�������>
		 |<�������>
		 |<ѭ�����>
		 |<return���>
*/
void Statement() {
	switch (token->TkCode) {
	case L_BRACE:
		ComplexStatement();
		break;
	case IF:
		ConditionStatement();
		break;
	case FOR:
		LoopStatement();
		break;
	case  RETURN:
		ReturnStatement();
		break;
	default:
		break;
	}
}

/*
<�������> -> {{<����>}{<���>}}
*/
void ComplexStatement(){
	NextToken();
	while (IsType(token->TkCode)) {
		Declaration();
	}
	while (token->TkCode != R_BRACE) {
		Statement();
	}
}

/*
<�������> -> "if""("<���ʽ>")"<���>["else"<���>]
*/
void ConditionStatement() {
	NextToken();
	Skip(L_PAREN);
	Expression();
	Skip(R_PAREN);
	Statement();
	if (token->TkCode == ELSE) {
		NextToken();
		Statement();
	}
}

/*
<ѭ�����> -> "for""("<���ʽ>";"<���ʽ>";"<���ʽ>")"<���>
*/
void LoopStatement() {
	NextToken();
	Skip(L_PAREN);
	Expression();
	Skip(SEMI);
	Expression();
	Skip(SEMI);
	Expression();
	Skip(R_PAREN);
	Statement();
}

/*
<return���> -> "return" [<���ʽ>] ";"
*/
void ReturnStatement() {
	NextToken();
	if (token->TkCode != SEMI) {
		Expression();
		Skip(SEMI);
	}
	else {
		Skip(SEMI);
	}
}

/*
<���ʽ> -> <��ֵ���ʽ>{","<��ֵ���ʽ>}
*/
void Expression() {
	while (TRUE) {
		AssignExpr();
		if (token->TkCode == COMMA) {
			AssignExpr();
		}
		else {
			break;
		}
	}
}

/*
<��ֵ���ʽ> -> <�߼�����ʽ>["="<��ֵ���ʽ>]
*/
void AssignExpr() {
	LogicOrExpr();
	if (token->TkCode == ASSIGN) {
		NextToken();
		AssignExpr();		
	}
}

/*
<�߼�����ʽ> -> <�߼�����ʽ>{"||"<�߼�����ʽ>}
*/
void LogicOrExpr() {
	LogicAndExpr();
	while (TRUE) {
		if (token->TkCode == OR) {
			NextToken();
			LogicAndExpr();
		}
		else {
			break;
		}
	}
}

/*
<�߼�����ʽ> -> <��ȱ��ʽ>{"&&"<��ȱ��ʽ>}
*/
void LogicAndExpr() {
	EqualExpr();
	while (TRUE) {
		if (token->TkCode == AND) {
			NextToken();
			EqualExpr();
		}
		else {
			break;
		}
	}
}

/*
<��ȱ��ʽ> -> <��ϵ���ʽ>{("=="|"!=")<��ϵ���ʽ>}
*/
void EqualExpr() {
	RelationExpr();
	while (TRUE) {
		if (token->TkCode == EQ || token->TkCode == NEQ) {
			NextToken();
			RelationExpr();
		}
		else {
			break;
		}
	}
}

/*
<��ϵ���ʽ> -> <�Ӽ����ʽ>{("<"|">"|"<="|">=")<�Ӽ����ʽ>}
*/
void RelationExpr() {
	AddMinusExpr();
	while (TRUE) {
		if (token->TkCode == LT || token->TkCode == GT
			|| token->TkCode == LE || token->TkCode == GE) {
			NextToken();
			AddMinusExpr();
		}
		else {
			break;
		}
	}
}

/*
<�Ӽ����ʽ> -> <�˳����ʽ>{("+"|"-")<�˳����ʽ>}
*/
void AddMinusExpr() {
	MultiDivideExpr();
	while (TRUE) {
		if (token->TkCode == PLUS || token->TkCode == MINUS) {
			NextToken();
			MultiDivideExpr();
		}
		else {
			break;
		}
	}
}

/*
<�˳����ʽ> -> <�������ʽ>{("*"|"/")<�������ʽ>}
*/
void MultiDivideExpr() {
	BaseExpr();
	while (TRUE) {
		if (token->TkCode == MULTI || token->TkCode == DIVIDE) {
			NextToken();
			BaseExpr();
		}
		else {
			break;
		}
	}
}

/*
<�������ʽ> -> <Ԫ���ʽ>{"["<���ʽ>"]"
						  |"("[<���ʽ>]")"}
*/
void BaseExpr() {
	ElementExpr();
	while (TRUE) {
		if (token->TkCode == L_BRACK) {
			Expression();
			Skip(R_BRACK);
		}
		else if (token->TkCode == L_PAREN) {
			NextToken();
			if (token->TkCode != R_PAREN) {
				Expression();
				Skip(R_PAREN);
			}
			else{
				Skip(R_PAREN);
			}
		}
		else {
			break; 
		}
	}
}

/*
<Ԫ���ʽ> -> "��ʶ��" | "���ͳ���" | "�����ͳ���" | "�ַ�����" | "�ַ�������"
			| "("<���ʽ> ")"
*/
void ElementExpr() {
	switch (token->TkCode) {
	case IDENT:
	case C_INT:
	case C_DOUBLE:
	case C_CHAR:
	case C_STR:
		NextToken();
		break;
	case L_PAREN:
		NextToken();
		Expression();
		Skip(R_PAREN);
		break;
	default:
		Error("ȱ�ٱ�ʶ������!");
	}
}
