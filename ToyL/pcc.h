#ifndef PCC_H
#include <stdio.h>
#define FatalError(Str) printf("%s\n", Str), exit(-1)
#define Error(Str) FatalError(Str)
#define TKSIZE 1024
#define STRSIZE 20
#define HASHSIZE 1000

/********************** pcc.c **********************/
extern FILE *FP;

/********************** lex.c **********************/
#define IsLetter(ch) ('a'<= ch && ch <='z' || 'A'<= ch && ch <= 'Z')
#define IsDigit(ch) ('0'<= ch && ch <='9')
#define Is_Layout(ch) (ch != EOF && ch <= ' ')
#define IsUnderline(ch) (ch == '_')
#define IsDot(ch) (ch == '.')
typedef struct TokenWord *PtrToToken;
typedef PtrToToken Position;
typedef PtrToToken Token;

/*���ʴ洢�ṹ*/
struct TokenWord {
	int TkCode;		//���ʱ���
	char *String;	//�����ַ���
	Token Next;		//��ϣ��ͻ����һ��Token
};

extern Token token;
void NextChar();
void NextAvailChar();
void RecognizeNote();
void NextAvailChar();
void InitLex();
void RecognizeIden();
void RecognizeNum();
void RecognizeConst();
void RecognizeNote();
void Next_token();


enum TkCode {
	/*�ؼ���*/
	BREAK,		//�ؼ��� break
	CHAR,		//�ؼ��� char
	CONTINUE,	//�ؼ��� continue
	DOUBLE,		//�ؼ��� double
	ELSE,		//�ؼ��� else
	FOR,		//�ؼ��� for
	IF,			//�ؼ��� if
	INT,		//�ؼ��� int
	RETURN,		//�ؼ��� return
	VOID,		//�ؼ��� void
	
	/*�����*/
	PLUS,		//�ӷ������ '+'
	MINUS,		//��������� '-'
	MULTI,		//�˷������ '*'
	DIVIDE,		//��������� '/'
	MOD,		//ȡģ����� '%'
	ASSIGN,		//��ֵ����� '='
	EQ,			//�������� '=='
	NEQ,		//���������� '!='
	LT,			//С������� '<'
	GT,			//��������� '>'
	LE,			//С�ڵ�������� '<='
	GE,			//���ڵ�������� '>='
	OR,			//������� '||'
	AND,		//������� '&&'

	/*�ָ���*/
	L_PAREN,	//С���� '('
	R_PAREN,	//С���� ')'
	L_BRACK,	//������ '['
	R_BRACK,	//������ ']'
	L_BRACE,		//������ '{'
	R_BRACE,		//������ '}'
	COMMA,		//���� ','
	SEMI,		//�ֺ� ';'
	_EOF,		//�ļ�������

	/*����*/
	C_INT,		//���ͳ���		
	C_DOUBLE,	//�����ͳ���
	C_CHAR,		//�ַ��ͳ���
	C_STR,		//�ַ�������
	
	/*��ʶ��*/
	IDENT		//��ʶ��
};


/******************** dynamic.c ********************/

/*��̬�ַ����ṹ����*/
#define MinStringSize 20
typedef struct DynamicString *String;
struct DynamicString {
	int Size;
	int Capacity;
	char *Data;
};
String InitString(int StringSize);
void StringAdd(String Str, char c);
void StringReset(String Str);

/*��ϣ��ṹ����*/
#define MinTableSize 100
typedef struct HashTbl *HashTable;
struct HashTbl {
	int TableSize;
	Token *TheTokens;
};
int Hash(char *Key, HashTable H);
HashTable InitHash(int TableSize);
Position Find(char *Str, HashTable H);

/*��̬����ṹ����*/
#define MinVectorSize 1000
typedef struct DynamicVector *Vector;
struct DynamicVector {
	int Size;
	int Capacity;
	Token *Data;
};
Vector InitVector(int VectorSize);
Token VectorAdd(Vector Vec, HashTable H, String Str);
void VectorFree(Vector Vec);


#endif //PCC_H