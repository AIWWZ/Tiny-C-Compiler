#include "pcc.h"
#include <stdio.h>
#include <stdlib.h>

/******** ����ȫ�ֱ��� ********/
Vector Token_Table;		//���ʱ�
HashTable Hash_Table;	//��ϣ��
String Str;				//�ַ���
Token token;			//����
char ch;

/* ���ַ� */
void NextChar() {
	ch = fgetc(FP);
}

/* �����˵Ķ��ַ� */
void NextAvailChar() {
	NextChar();
	while (1) {
		if (Is_Layout(ch)) {
			NextChar();
		}
		else if (ch == '/') {
			NextChar(); //���һ���ж��ǲ���ע��
			if (ch == '*') {
				RecognizeNote();
			}
			else { //��д
				if (ungetc(ch, FP) == EOF) {
					Error("Write back error!");
				}
				ch = '/';
				break;
			}
		}
		else
			break;
	}
}

/* lex��ʼ�� */
void InitLex() {
	Token_Table = InitVector(TKSIZE);
	Hash_Table = InitHash(HASHSIZE);
	Str = InitString(STRSIZE);
	/*�Ƚ����ֵ��ʲ��뵥�ʱ�*/
	struct TokenWord TkWords[] = {
		{ BREAK,	"break",	NULL },
		{ CHAR,		"char",		NULL },
		{ CONTINUE,	"continue", NULL },
		{ DOUBLE,	"double",	NULL },
		{ ELSE,		"else",		NULL },
		{ FOR,		"for",		NULL },
		{ IF,		"if",		NULL },
		{ INT,		"int",		NULL },
		{ RETURN,	"reutrn",	NULL },
		{ VOID,		"void",		NULL },
		{ PLUS,		"+",		NULL },
		{ MINUS,	"-",		NULL },
		{ MULTI,	"*",		NULL },
		{ DIVIDE,	"/",		NULL },
		{ MOD,		"%",		NULL },
		{ ASSIGN,	"=",		NULL },
		{ EQ,		"==",		NULL },
		{ NEQ,		"!=",		NULL },
		{ LT,		"<",		NULL },
		{ GT,		">",		NULL },
		{ LE,		"<=",		NULL },
		{ GE,		">=",		NULL },
		{ OR,		"||",		NULL },
		{ AND,		"&&",		NULL },
		{ L_PAREN,	"(",		NULL },
		{ R_PAREN,	")",		NULL },
		{ L_BRACK,	"[",		NULL },
		{ R_BRACK,	"]",		NULL },
		{ L_BRACE,	"{",		NULL },
		{ R_BRACE,	"}",		NULL },
		{ COMMA,	",",		NULL },
		{ SEMI,		";",		NULL },
		{ _EOF,		"EOF",		NULL },
		{ C_INT,	"int",		NULL },
		{ C_DOUBLE,	"double",	NULL },
		{ C_CHAR,	"char",		NULL },
		{ C_STR,	"string",	NULL },
		{ 0,		0,			0 }
	};
	int i;
	Token Tk;
	for (i = 0; TkWords[i].String != 0; i++) {
		Tk = malloc(sizeof(struct TokenWord));
		if (Tk == NULL) {
			FatalError("Out of space!");
		}
		Tk->String = malloc(sizeof(char) * strlen(TkWords[i].String) + 1);
		if (Tk->String == NULL) {
			FatalError("Out of space!");
		}
		strcpy(Tk->String, TkWords[i].String);
		Tk->TkCode = TkWords[i].TkCode;
		Tk->Next = Hash_Table->TheTokens[Hash(Tk->String, Hash_Table)];
		Hash_Table->TheTokens[Hash(Tk->String, Hash_Table)] = Tk;
		Token_Table->Data[Token_Table->Size++] = Tk;
	}
}


/* ������ʶ�� */
void RecognizeIden() {
	StringReset(Str);
	while (IsLetter(ch) || IsDigit(ch) || IsUnderline(ch)) {
		StringAdd(Str, ch);
		NextChar();
	}
	printf("%c\n", ch);
	if (ungetc(ch, FP) == EOF) { /*��д*/
		Error("Write back error!");
	}
	printf("%s\n", Str->Data);
	token = VectorAdd(Token_Table, Hash_Table, Str);
	token->TkCode = IDENT;
	printf("%s\n", token->String);
}

/* �������ֳ���--������ʵ�� */
void RecognizeNum() {
	StringReset(Str);
	int cnt = 0;
	while (IsDigit(ch) || IsDot(ch)) {
		if (IsDot(ch)) {
			cnt++;
		}
		StringAdd(Str, ch);
		NextChar();
	}
	if (ungetc(ch, FP) == EOF) { /*��д*/
		Error("Write back error!");
	}
	if (cnt == 0) { //���ͳ���
		token = VectorAdd(Token_Table, Hash_Table, Str);
		token->TkCode = C_INT;
	}
	else if (cnt == 1) { //�����ͳ���
		token = VectorAdd(Token_Table, Hash_Table, Str);
		token->TkCode = C_DOUBLE;
	}
	else {
		Error("�ʷ�����");
	}
}

/* ��������--�ַ����ַ��� */
void RecognizeConst(){
	StringReset(Str);
	char c = ch;
	while (1) {
		NextChar();
		if (ch == c) {
			break;
		}
		else {
			StringAdd(Str, ch);
			NextChar();
		}
	}
	if (ungetc(ch, FP) == EOF) { /*��д*/
		Error("Write back error!");
	}
	if (c == '\'') {
		token = VectorAdd(Token_Table, Hash_Table, Str);
		token->TkCode = C_CHAR;
	}
	else {
		token = VectorAdd(Token_Table, Hash_Table, Str);
		token->TkCode = C_STR;
	}
}


/* ����ע�� */
void RecognizeNote() {
	while (1) {
		NextChar();
		if (ch == '*') {
			NextChar();
			if (ch == '/') {
				break;
			}
		}
	}
}

/* ��ȡtoken */
void Next_token(){
	NextAvailChar();
	switch (ch)
	{
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
	case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case '_':
	{
		RecognizeIden(); /*��ʶ��*/
		break;
	}
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7':
	case '8': case '9':
		RecognizeNum(); /*���ֳ���*/
		break;
	case '\'':
	case '\"':
		RecognizeConst(); /*�ַ����ַ�������*/
		break;
	case '+':
		token = Find("+", Hash_Table);
		break;
	case '-':
		token = Find("-", Hash_Table);
		break;
	case '*':
		token = Find("*", Hash_Table);
		break;
	case '/':
		token = Find("/", Hash_Table);
		break;
	case '%':
		token = Find("%", Hash_Table);
		break;
	case '=':
		NextChar();
		if (ch == '='){
			token = Find("==", Hash_Table);
		}
		else {
			if (ungetc(ch, FP) == EOF) { /*��д*/
				Error("Write back error!");
			}
			token = Find("=", Hash_Table);
		}
		break;
	case '!':
		NextChar();
		if (ch == '='){
			token = Find("!=", Hash_Table);
		}
		else {
			if (ungetc(ch, FP) == EOF) { /*��д*/
				Error("Write back error!");
			}
		}
		break;
	case '<':
		NextChar();
		if (ch == '='){
			token = Find("<=", Hash_Table);
		}
		else {
			if (ungetc(ch, FP) == EOF) { /*��д*/
				Error("Write back error!");
			}
			token = Find("<", Hash_Table);
		}
		break;
	case '>':
		if (ch == '='){
			token = Find(">=", Hash_Table);
		}
		else {
			if (ungetc(ch, FP) == EOF) { /*��д*/
				Error("Write back error!");
			}
			token = Find(">", Hash_Table);
		}
		break;
	case '&':
		NextChar();
		if (ch == '&') {
			token = Find("&&", Hash_Table);
		}
		else {
			Error("�ʷ�����");
		}
		break;
	case '(':
		token = Find("(", Hash_Table);
		break;
	case '[':
		token = Find("[", Hash_Table);
		break;
	case '{':
		token = Find("{", Hash_Table);
		break;
	case ')':
		token = Find(")", Hash_Table);
		break;
	case ']':
		token = Find("]", Hash_Table);
		break;
	case '}':
		token = Find("}", Hash_Table);
		break;
	case ',':
		token = Find(",", Hash_Table);
		break;
	case ';':
		token = Find(";", Hash_Table);
		break;
	case EOF:
		token = Find("EOF", Hash_Table);
		break;
	default:
		Error("�޷�ʶ����ַ���");
		break;
	}
	printf("%s\n", token->String);
}