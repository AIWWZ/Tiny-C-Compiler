#include "toyc.h";

/*������ǰ����*/
void Skip(int TkCode) {
	if (token->TkCode == TkCode) {
		NextToken();
		return;
	}
	else {
		printf("C_error:ȱ��%s\n", Token_Table->Data[TkCode]->String);
		exit(-1);
	}
}

/*�ж��Ƿ�����˵˵����*/
int IsType(int TkCode) {
	switch (TkCode) {
	case CHAR:
	case STRING:
	case VOID:
	case INT:
	case DOUBLE:
		return TRUE;
	default:
		return FALSE;
	}
}

/*
<�﷨����> -> {<����>}<�ļ�������>
*/
void Parser() {
	NextToken();
	while (token->TkCode != _EOF) {
		Declaration();
	} 
}

/*
<����> -> <ͷ�ļ�����>
		 | <��������><������>( <������>
							 | ["="<��ֵ���ʽ>]{","<������>["="<��ֵ���ʽ>]}";" )
*/
void Declaration() {
	if (token->TkCode == INCLUDE) {
		IncludeHeader();
	}
	else if (IsType(token->TkCode)) {
		TypeState();
		Declarator(); 
		if (token->TkCode == L_BRACE) {
			FuncBody();
		}
		else {
				if (token->TkCode == ASSIGN) {
					NextToken();
					AssignExpr();
				}
				while (token->TkCode == COMMA) {
					NextToken();
					Declarator();
					if (token->TkCode == ASSIGN) {
						NextToken();
						AssignExpr();
					}
				}
				Skip(SEMI);
		}
	}
}

/*
<ͷ�ļ�����> -> "#include" "ͷ�ļ�����"
*/
void IncludeHeader() {
	NextToken();
	Skip(C_HEADER);
}

/*
<��������> -> "void" | "char" | "int" | "double"
*/
void TypeState(){
	switch (token->TkCode) {
	case CHAR:
	case STRING:
	case VOID:
	case INT:
	case DOUBLE:
		NextToken();
		break;
	default:
		Error("C_error:ȱ������˵����!");
		break;
	}
}

/*
<������> -> "��ʶ��"<��������׺>
*/
void Declarator() {
	Token tokenTmp = token;
	if (token->TkCode < IDENT) {
		Error("C_error:ȱ�ٱ�ʶ��!");
	}
	NextToken();
	if (token->TkCode == L_PAREN) {
		tokenTmp->TkCode = FUNC;
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
}

/*
<���> -> <�������>
		 |<�����������>
		 |<if�������>
		 |<switch�������>
		 |<forѭ�����>
		 |<whileѭ�����>
		 |<case���>
		 |<default���>
		 |<break���>
		 |<return���>
		 |<���ʽ���>
*/
void Statement() {
	switch (token->TkCode) {
	case L_BRACE:
		ComplexStatement();
		break;
	case FUNC:
		FuncCall();
		break;
	case IF:
		IfStatement();
		break;
	case SWITCH:
		SwitchStatement();
		break;
	case FOR:
		ForStatement();
		break;
	case WHILE:
		WhileStatement();
		break;
	case CASE:
		CaseStatement();
		break;
	case DEFAULT:
		DefaultStatement();
		break;
	case BREAK:
		BreakStatement();
		break;
	case  RETURN:
		ReturnStatement();
		break;
	default:
		ExprStatement();
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
	Skip(R_BRACE);
}

/*
<�����������> -> "������""("[<���ʽ>]")"
*/
void FuncCall() {
	NextToken();
	Skip(L_PAREN);
	if (token->TkCode != R_PAREN) {
		Expression();
	}
	Skip(R_PAREN);
	Skip(SEMI);
}

/*
<if�������> -> "if""("<���ʽ>")"<���>["else"<���>]
*/
void IfStatement() {
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
<switch�������> -> "switch""("<���ʽ>")"<���>
*/
void SwitchStatement() {
	NextToken();
	Skip(L_PAREN);
	Expression();
	Skip(R_PAREN);
	Statement();
}

/*
<forѭ�����> -> "for""("([<����>]|[<���ʽ>";"])[<���ʽ>]";"[<���ʽ>]")"<���>
*/
void ForStatement() {
	NextToken();
	Skip(L_PAREN);
	if (IsType(token->TkCode)) {
		Declaration();
	}
	else {
		Expression();
		Skip(SEMI);
	}
	if (token->TkCode != SEMI) {
		Expression();
	}
	Skip(SEMI);
	if (token->TkCode != R_PAREN) {
		Expression();
	}
	Skip(R_PAREN);
	Statement();
}

/*
<whileѭ�����> -> "while""("<���ʽ>")"<���> 
*/
void WhileStatement() {
	NextToken();
	Skip(L_PAREN);
	Expression();
	Skip(R_PAREN);
	Statement();
}

/*
<case���> -> "case"("���ͳ���"|"�ַ��ͳ���")":"
*/
void CaseStatement() {
	NextToken();
	if (token->TkCode == C_INT || token->TkCode == C_CHAR) {
		NextToken();
	}
	else {
		Error("C_error:ȱ�����ͳ������ʽ!");
	}
	Skip(COLON);
}

/*
<default���> -> "default"":"
*/
void DefaultStatement() {
	NextToken();
	Skip(COLON);
}

/*
<break���> -> "break"";"
*/
void BreakStatement() {
	NextToken();
	Skip(SEMI);
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
<���ʽ���> -> <���ʽ>";"
*/
void ExprStatement() {
	Expression();
	Skip(SEMI);
}

/*
<���ʽ> -> <��ֵ���ʽ>{","<��ֵ���ʽ>}
*/
void Expression() {
		AssignExpr();
		while (token->TkCode == COMMA) {
			NextToken();
			AssignExpr();
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
	while (token->TkCode == OR) {
		NextToken();
		LogicAndExpr();
	}
}

/*
<�߼�����ʽ> -> <��ȱ��ʽ>{"&&"<��ȱ��ʽ>}
*/
void LogicAndExpr() {
	EqualExpr();
	while (token->TkCode == AND) {
		NextToken();
		EqualExpr();
	}
}

/*
<��ȱ��ʽ> -> <��ϵ���ʽ>{("=="|"!=")<��ϵ���ʽ>}
*/
void EqualExpr() {
	RelationExpr();
	while (token->TkCode == EQ || token->TkCode == NEQ) {
		NextToken();
		RelationExpr();
	}
}

/*
<��ϵ���ʽ> -> <�Ӽ����ʽ>{("<"|">"|"<="|">=")<�Ӽ����ʽ>}
*/
void RelationExpr() {
	AddMinusExpr();
	while (token->TkCode == LT || token->TkCode == GT || token->TkCode == LE || token->TkCode == GE) {
		NextToken();
		AddMinusExpr();
	}
}

/*
<�Ӽ����ʽ> -> <�˳����ʽ>{("+"|"-")<�˳����ʽ>}
*/
void AddMinusExpr() {
	MultiDivideExpr();
	while (token->TkCode == PLUS || token->TkCode == MINUS) {
		NextToken();
		MultiDivideExpr();
	}
}

/*
<�˳����ʽ> -> <�������ʽ>{("*"|"/"|"%")<�������ʽ>}
*/
void MultiDivideExpr() {
	BaseExpr();
	while (token->TkCode == MULTI || token->TkCode == DIVIDE || token->TkCode == MOD) {
		NextToken();
		BaseExpr();
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
			NextToken();
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
		if (token->TkCode >= IDENT) {
			NextToken();
			break;
		}
		else {
			Error("C_error:ȱ�ٱ�ʶ������!");
		}
	}
}
