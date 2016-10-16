#ifndef  TOKEN_H
#define TOKEN_H

typedef struct Token_Node * PtrToNode;
typedef PtrToNode Token;
typedef PtrToNode Position;

//Token--���ʵı���
//�Ѿ����ö��� EOF (-1)
#define BEGIN 0
#define NUMB 1
#define IDEN 2
#define PLUS 3
#define MULT 4
#define ASS 5 //���塮:=����ֵ�ĺ���
#define READ 6
#define WRITE 7
#define SEMI 8
#define OPEN 9
#define CLOSE 10
#define END 11

struct Token_Node
{
	int _class;
	char * seman;
	Position last;
	Position next;
};

Token init_token();

//�������͵Ķ��ַ����������˸�ʽ��
void next_Char();
void next_avail_char();


#endif // ! TOKEN_H
