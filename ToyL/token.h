#ifndef  TOKEN_H
#define TOKEN_H

typedef struct Token_Node * PtrToNode;
typedef PtrToNode Token;
typedef PtrToNode Position;

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
