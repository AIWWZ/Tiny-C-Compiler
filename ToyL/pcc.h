#ifndef PCC_H
#define PCC_H
#include <stdio.h>
#define FatalError(Str) printf("%s\n", Str), exit(-1)
#define Error(Str) FatalError(Str)
#define TRUE 1
#define FALSE 0
#define TKSIZE 1024
#define STRSIZE 20
#define HASHSIZE 1000

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
void NextChar();
void NextAvailChar();
void RecognizeNote();
void NextAvailChar();
void InitLex();
void RecognizeInclude();
void RecognizeIden();
void RecognizeNum();
void RecognizeConst();
void RecognizeNote();
void NextToken();


enum TkCode {
	/*�ؼ���*/
	BREAK,		//�ؼ��� break
	CHAR,		//�ؼ��� char
	CASE,		//�ؼ��� case
	CONTINUE,	//�ؼ��� continue
	DEFAULT,	//�ؼ��� default
	DOUBLE,		//�ؼ��� double
	ELSE,		//�ؼ��� else
	FOR,		//�ؼ��� for
	IF,			//�ؼ��� if
	INCLUDE,	//�ؼ��� #include
	INT,		//�ؼ��� int
	RETURN,		//�ؼ��� return
	STRING,		//�ؼ��� string
	SWITCH,		//�ؼ��� switch
	VOID,		//�ؼ��� void
	WHILE,		//�ؼ��� while

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
	COLON,		//ð�� ':'
	SEMI,		//�ֺ� ';'

	/*����*/
	C_INT,		//���ͳ���		
	C_DOUBLE,	//�����ͳ���
	C_CHAR,		//�ַ��ͳ���
	C_STR,		//�ַ�������
	C_HEADER,	//ͷ�ļ�����
	
	/*�ļ�������*/
	_EOF,

	/*��ʶ��*/
	IDENT,		//��ʶ��
	FUNC		//������
	
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
void StringAdd(char c);
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
Position Find(char *Str);

/*��̬����ṹ����*/
#define MinVectorSize 1000
typedef struct DynamicVector *Vector;
struct DynamicVector {
	int Size;
	int Capacity;
	Token *Data;
};
Vector InitVector(int VectorSize);
Token VectorAdd(String Str);
void VectorFree(Vector Vec);

/******************ȫ�ֱ���******************/
extern FILE *FP;
extern Token token;
extern Vector Token_Table;
extern HashTable Hash_Table;
extern String Str;

/******************** parser.c ********************/

void Skip(int TkCode);
int IsType(int TkCode);
void Parser();
void Declaration();
void IncludeHeader();
void TypeState();
void Declarator();
void DeclarationSuffix();
void FormalParaList();
void FuncBody();
void Statement();
void ComplexStatement();
void FuncCall();
void IfStatement();
void SwitchStatement();
void ForStatement();
void WhileStatement();
void CaseStatement();
void DefaultStatement();
void BreakStatement();
void ReturnStatement();
void ExprStatement();
void IntConstExpr();
void Expression();
void AssignExpr();
void LogicOrExpr();
void LogicAndExpr();
void EqualExpr();
void RelationExpr();
void AddMinusExpr();
void MultiDivideExpr();
void BaseExpr();
void ElementExpr();

#endif //PCC_H