#include "pcc.h"
#include <stdio.h>
#include <stdlib.h>

/******** ����ȫ�ֱ��� ********/
Vector Token_Table;		//���ʱ�
HashTable Hash_Table;	//��ϣ��
String Str;				//�ַ���
Token token;			//����
FILE *FP;
char ch;

/* ���ַ� */ 
void NextChar() {
	ch = fgetc(FP);
}

/* �����˵Ķ��ַ� */
void NextAvailChar() {
	NextChar();
	while (1){
		if (IsLayout(ch)) {
			NextChar();
		}
		else if (ch == '\/') {
			NextChar(); //���һ���ж��ǲ���ע��
			if (ch == '*') {
				RecognizeNote();
			}
			else { //�������д��stream
				if (ungetc(ch, FP) == EOF) {
					Error("Write back error!");
				}
				ch = '\/';
				break;
			}
		}
		else
			break;
	}
}

/* ������ʶ�� */
void RecognizeIden() {
	StringReset(Str);
	while (IsLetter(ch) || IsDigit(ch) || IsUnderline(ch)) {
		StringAdd(Str, ch);
		NextChar();
	}
	token = VectorAdd(Token_Table, Hash_Table, Str);
	token->TkCode = IDENT;
}

/* �������ֳ��� */
void RecognizeNum() {
	StringReset(Str);
	int M, cnt = 0;
	double N;
	while (IsDigit(ch) || IsDot(ch)) {
		if (IsDot(ch)) {
			cnt++;
		}
		StringAdd(Str, ch);
		NextChar();
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
			if (ch == '\/') {
				break;
			}
		}
	}
}

/* lex��ʼ�� */
void InitLex() {
	Token_Table = InitVector(TKSIZE);
	Hash_Table = InitHash(HASHSIZE);
	Str = InitString(STRSIZE);
	Token_Table->Data[]
}