#include <stdio.h>

//Token--���ʵı���
#define BEGIN 1
#define NUMB 2
#define IDEN 3
#define PLUS 4
#define MULT 5
#define ASS 6 //���塮:=����ֵ�ĺ���
#define READ 7
#define WRITE 8
#define SEMI 9
#define OPEN 10
#define CLOSE 11
#define END 12
#define EOF 13

//�ж϶�����ַ�������
#define is_end_of_input(ch) ((ch)== '\0')
#define is_lc_letter(ch) ('a'<=(ch) && (ch)<='Z')
#define is_uc_letter(ch) ('A'<=(ch) && (ch)<='Z')
#define is_letter(ch) ('a'<=(ch) && (ch)<='z' || 'A'<=(ch) && (ch)<='Z')
#define is_digit(ch) ( '0'<(ch) && (ch)<='9')
#define is_letter_or_digit(ch) (is_letter(ch) && is_digit(ch))
#define is_operator(ch) ((ch)=='+' || (ch)=='-' || (ch)=='*')
#define is_layout(ch) (!is_end_of_input(ch) && (ch)<=' ')  //��ʽ��

typedef struct {
	int class;  //���ʱ���
	char seman[];   //����
					//position_in_File pos;  //��ǰ�����ڳ����ļ��е�λ��
}Token_type;

typedef struct {
	char * file_name;  //ָ�򵥴�λ�õ��ļ�ָ��
	int line_number;  //�����ڳ����ı��е��к�
	int token_number; //���������е����
}position_in_File;

//�������͵Ķ��ַ����������˸�ʽ��
void next_Char() {
	ch = prog_file[++fp];
}
void next_avail_char() {
	next_Char();
	while (is_layout(ch)) {
		next_Char();
	}
}

//�������������������ַ������뵽����
void recognize_number(ch) {
	int N = 0;
	while (is_digit(ch)) {
		N = N * 10 + (ch)-'0';
		next_Char();
	}
	Token.class = NUMB;
	Token.seman = N;
	back();
}

//������ʶ��
void recognize_name(ch) {
	char name[10];
	np = 0;
	name[np++] = ch;
	next_Char();
	while (is_digit(ch) || is_letter(ch)) {
		name[np++] = ch;
		next_Char();
	}
	name[np] = '\0';
	switch (ch) {
	case "begin": Token.class = BEGIN; Token.seman = "begin"; break;
	case "end": Token.class = END; Token.seman = "end"; break;
	case "read": Token.class = READ; Token.seman = "read"; break;
	case "write": Token.class = WRITE; Token.seman = "write";  break;
	default: Token.class = IDEN; Token.seman = name; break;
	}
	back();
}

//����һ�����ʵĴʷ�������
void next_token(void) {
	next_avail_char();
	switch (ch) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': recognize_number(ch); break;

	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
	case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v':
	case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K':
	case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
	case 'W': case 'X': case 'Y': case 'Z': recognize_name(ch); break;

	case '+': Token.class = PLUS; break;
	case '*': Token.class = MULT; break;
	case ':': next_char(); if (ch != '=') error();
		Token.class = ASS;  break;
	case ';': Token.class = SEMI; break;
	case '(': Token.class = OPEN; break;
	case ')': Token.class = CLOSE; break;
	case '\0': Token.class = EOF;  break;
	default: error();
	}
}

//�﷨����ģ��
void next_token(Token_type tk) {
	old_fp = fp;
	/*  ...  */
}
void back_token() {
	if (fp != old_fp)
		fp = fp - 1;
}
void token(int CLASS) {
	next_token(tk);
	if (tk.class != CLASS)
		error();
}
//���ʽ����
void  expr_parser() {
	1000	next_token(tk);
	if (tk.class != NUMB && tk.class != IDEN) error();
	2000  next_token(tk);
	if (tk.class == PLUS || tk.calss == MULT) goto 1000;
	3000  back_token();
	return;
}
//�﷨����
void prog_parser() {
	token(BEGIN);
	while (tk.class != END) {
		next_token(tk);
		switch (tk.class) {
		case READ: token(OPEN); token(IDEN);   token(CLOSE); break;
		case WRITE: token(OPEN); expr_parser(); token(CLOSE); break;
		case IDEN: token(ASS);  expr_parser(); break;
		default: error();
		}
		token(SEMI); //�����ĺ���ַ�
	}
	token(EOF);
	return;
}

//����洢��:��ű�����ֵ
typedef struct {
	char *name[10];
	int val;
}Memory[100];
int fetch(char *id[]) {  //��Memory��ñ���id��ֵ��idΪ��ʶ��
	i = 0;
	while (!strcmp(Memory[i].name, id) && i <= mp) ++i; //mpΪȫ�ֱ�����Memory[mp]��ʾ���һ��Ԫ��
	if (i > mp) error();
	else return Memory[i].val;
}
void updata(char * id[], int n) {
	i = 0;
	while (!strcmp(Memory[i].name, id) && i <= mp) ++i;
	if (i <= mp)
		Memory[i].val = n;
	else {
		Memory[i].name = id;
		Memory[mp].val = n;
	}
}


int main() {
	return 0;
}