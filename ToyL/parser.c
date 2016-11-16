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

/*
<������Ԫ> -> {<ȫ������>}<�ļ�������>
*/
void Parser_Unit() {
	while (token->TkCode != EOF) {
		Extern_Declaration();
	}
}

/*
<ȫ������> -> <��������>(  ";"
						  | <������><������>
						  | <������>["="<��ֵ���ʽ>]{","<������>["="<��ֵ���ʽ>]}";" )

<��������> -> "void" | "char" | "int" | "double"
<������> -> <��ʶ��><��������׺>
<��������׺> -> { "(" ")"
				| "[" "]"
				| "(" <�β��б�> ")"
				| "[" "���ͳ���" "]" }
<������> -> <�������>
*/
void Extern_Decalration() {
	if (!TypeState()) {
		Error("Absence type statement!");
	}
	if (token->TkCode == SEMI) {
		NextToken();
		return;
	}
	while (TRUE) {
		Declaration();
		if (token->TkCode == L_BRACE) {
			FunBody();
		}
		else {
			if (token->TkCode == ASSIGN) {
				AssignExpr();
			}
			else if (token->TkCode == COMMA) {
				NextToken();
			}
			else {
				Skip(SEMI);
				return;
			}
		}
	}
}

/*
<��������> -> "void" | "char" | "int" | "double"
*/
int TypeState(){
	switch (token->TkCode) {
	case CHAR:
	case VOID:
	case INT:
	case DOUBLE:
		NextToken();
		return TRUE;
	default:
		return FALSE;
	}
}