#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

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


//�ж϶�����ַ�������
#define is_end_of_input(ch) ((ch)== -1)
#define is_lc_letter(ch) ('a'<=(ch) && (ch)<='Z')
#define is_uc_letter(ch) ('A'<=(ch) && (ch)<='Z')
#define is_letter(ch) ('a'<=(ch) && (ch)<='z' || 'A'<=(ch) && (ch)<='Z')
#define is_digit(ch) ('0'<=(ch) && (ch)<='9')
#define is_letter_or_digit(ch) (is_letter(ch) && is_digit(ch))
#define is_operator(ch) ((ch)=='+' || (ch)=='-' || (ch)=='*')
#define is_layout(ch) (!is_end_of_input(ch) && (ch)<=' ')  //��ʽ��

#define Error(STR) printf("%s",STR), exit(1)

typedef struct Token_Node * PtrToNode;
typedef PtrToNode Token;
typedef PtrToNode Position;

struct Token_Node {
	int class;  //���ʱ���
	char * seman;   //����
	Position  last;
	Position  next;
};

char ch;  //����ȫ���ַ�����
FILE *fp;  //����ȫ���ļ�ָ�����
Position P;  //����ȫ��tokenָ��
Position P_old; //�����﷨���������еĵ��ʻ���

Token init_token() {
	Token T;
	T = malloc(sizeof(struct Token_Node));
	T->next = NULL;
	T->last = NULL;
	P = T;
	return T;
}

//�������͵Ķ��ַ����������˸�ʽ��
void next_Char() {
	ch = fgetc(fp);
}
void next_avail_char() {
	next_Char();
	while (is_layout(ch)) {
		next_Char();
	}
}

void back() {
	fseek(fp, -1, 1); //���ļ�ָ��ӵ�ǰλ����ǰ�ƶ�һ���ֽ�
}

//�������������������ַ������뵽����
void recognize_number() {
	int N = 0;
	while (is_digit(ch)) {
		N = N * 10 + (ch)-'0';
		next_Char();
	}
	Position Tmp;
	Tmp = malloc(sizeof(struct Token_Node));
	Tmp->class = NUMB;
	Tmp->seman = malloc(sizeof(char) + 1);
	Tmp->seman[0] = N;  //������������ֱ�Ӵ����ַ������У��ַ�����ĵ�һ��Ԫ�ؼ�ΪASCII��ΪN��Ӧ���ַ�
	Tmp->seman[1] = '\0';
	P->next = Tmp;
	Tmp->last = P;
	P = Tmp;
	back();
}

//������ʶ��
void recognize_name() {
	char name[10];
	int np = 0;
	name[np++] = ch;
	next_Char();
	while (is_digit(ch) || is_letter(ch)) {
		name[np++] = ch;
		next_Char();
	}
	name[np] = '\0';
	if (strcmp(name, "begin") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = BEGIN;
		Tmp->seman = "begin";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else if (strcmp(name, "end") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = END;
		Tmp->seman = "end";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		return; //end�����ֲ���ʹ��back()������Ӧ��ֱ�ӷ��أ�
	}
	else if (strcmp(name, "read") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = READ;
		Tmp->seman = "read";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else if (strcmp(name, "write") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = WRITE;
		Tmp->seman = "write";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = IDEN;
		Tmp->seman = malloc(sizeof(char) * strlen(name));
		strcpy(Tmp->seman, name);
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	back();
}

//����һ�����ʵĴʷ�������
void next_token(void) {

	next_avail_char();
	switch (ch) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': recognize_number(); break;

	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
	case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v':
	case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K':
	case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
	case 'W': case 'X': case 'Y': case 'Z': recognize_name(); break;

	case '+': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = PLUS;
		Tmp->seman = "+";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL; 
		break;
	}
	case '*': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = MULT;
		Tmp->seman = "*";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ':': {
		next_Char(); 
		if (ch != '=') Error("Error: invalid identifier!\n");
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = ASS;
		Tmp->seman = ":=";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ';': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = SEMI;
		Tmp->seman = ";";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case '(': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = OPEN;
		Tmp->seman = "(";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ')': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = CLOSE;
		Tmp->seman = ")";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case  EOF: {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = EOF;
		Tmp->seman = "eof";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	default: break;
	}
}

//�﷨����������
void token(int class) {
	if (P->class != class) {
		printf("%d,%d\n", P->class, class);
		Error("Syntax Error1!\n");
	}
	P = P->next;
}

void expr_parser() {
loop:	if (P->class != IDEN && P->class != NUMB) {
			Error("Syntax Error2!\n");
		}
		P = P->next;
		if (P->class == PLUS || P->class == MULT) {
			P = P->next;
			goto loop;
		}
		return;
}

void prog_parser() {
	token(BEGIN);
	while (P->class != END) {
		switch (P->class) {
		case READ: P = P->next; token(OPEN); token(IDEN); token(CLOSE); token(SEMI); break;
		case WRITE: P = P->next; token(OPEN); expr_parser(); token(CLOSE); token(SEMI); break;
		case IDEN: P = P->next; token(ASS); expr_parser(); token(SEMI); break;
		default: Error("Syntax Error3!\n"); break;
		}
	}
	P = P->next;
	token(EOF);

	return;
}

//�������ģ��
//�����洢�ռ�洢����
struct {
	char *iden_name;
	int val;
}Memory[100];
int mp; //��ʾMemory�б����ĸ���

//���ݱ�������Memory����ö�Ӧval;
int fetch(char *name) {
	int i = 0;
	while (strcmp(Memory[i].iden_name, name) != 0 && i < mp) {
		++i;
	}
	if (i == mp)
		Error("Variable does not exist.!\n");
	else
		return Memory[i].val;
}
//�ӿ���̨�������
int read() {
	int tmp;
	scanf("%d", &tmp);
	return tmp;
}
//����������Memory��
void updata(char * name, int a) {
	Memory[mp].iden_name = name;
	Memory[mp].val = a;
}
//���ʽ��ֵ
int expr_val() {

}

void interpreter() {
	token(BEGIN);
	while (P->class != END) {
		switch (P->class) {
		case READ: P = P->next; token(OPEN); updata(P->seman, read()); token(CLOSE); token(SEMI); break;
		case WRITE: P = P->next; token(OPEN); expr��ֵ->print; token(CLOSE); token(SEMI); break;
		case IDEN: P = P->next; token(ASS); expr��ֵ->x->����; token(SEMI); break;
		default: Error("Syntax Error!\n");
		} 
	}
}


int main() {
	Token T;
	T = init_token();
	fp = fopen("C:\\1.txt", "r");

	//�ʷ�����
	while(P->class != EOF){
		next_token();
	}
	P = T->next;
	//�﷨����
	prog_parser();
	P = T->next;
	//�������

	

	fclose(fp);

	return 0;
}