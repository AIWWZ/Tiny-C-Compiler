#include "pcc.h"
#include <stdio.h>
#include <ctype.h>

/******** ����ȫ�ֱ��� ********/
Vector TokenTable;		//���ʱ�
HashTable TokenHash;	//��ϣ��
String Str;				//�ַ���
Token token;			//����
FILE *FP;
char ch;

/* lex��ʼ�� */
void InitLex() {
	TokenTable = InitVector(TKSIZE);
	TokenHash = InitHash(HASHSIZE);
	Str = InitString(STRSIZE);
}

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
				RecognizeAnnot();
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
	token = VectorAdd(TokenTable, TokenHash, Str);
	token->TkCode = IDENT;
}

/* ��������--�ַ����ַ��� */
void RecognizeConst() {

}