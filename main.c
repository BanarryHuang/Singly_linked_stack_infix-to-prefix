#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int isp[]={0,19,12,12,13,13,0};//���|���u���v
int icp[]={20,19,12,12,13,13,0};//�i���|�u���v
char equation[200];
char postequa[200];
int postlength=0;
typedef enum {
    lparen,rparen,plus,minus,divide,times,eos,operand
} precedence;
typedef struct stack *stackPointer;//�B�zpostfix
typedef struct stack{
    int item;
    stackPointer link;
};
stackPointer top;
void push(int item){
    stackPointer temp;
    temp = (stackPointer)malloc(sizeof(stackPointer));
    temp->item = item;
    temp->link = top;
    top = temp;
}
int pop(){
    stackPointer temp=top;
    int item;
    if(temp==NULL) return 6;//6 �N�� precedence ���� eos
    item=temp->item;
    top=temp->link;
    free(temp);
    return item;
}
typedef struct stack2 *stackPointer2;//�B�z�B�I��eval()�B��
typedef struct stack2{
    double fpn;
    stackPointer2 link;
};
stackPointer2 top2;
void pushf(double fpn){
    stackPointer2 temp;
    temp = (stackPointer2)malloc(sizeof(stackPointer2));
    temp->fpn = fpn;
    temp->link = top2;
    top2 = temp;
}
double popf(){
    stackPointer2 temp=top2;
    double fpitem;
    if(temp==NULL) return 0;
    fpitem=temp->fpn;
    top2=temp->link;
    free(temp);
    return fpitem;
}
precedence getToken(char *symbol, int *n, int x)
{
     if(x) *symbol = equation[(*n)++];//�쥻infix���⦡
     else  *symbol = postequa[(*n)++];//�নpostfix���⦡
      switch (*symbol)  {
              case '(' : return lparen;
              case ')' : return rparen;
              case '+' : return plus;
              case '-' : return minus;
              case '/' : return divide;
              case '*' : return times;
              case '\0' : return eos;
              default  : return operand;
      };
}
void printToken(precedence d){
    switch (d)  {
              case plus :   postequa[postlength++]='+';
                            postequa[postlength++]=' ';
                            break;
              case minus :  postequa[postlength++]='-';
                            postequa[postlength++]=' ';
                            break;
              case divide : postequa[postlength++]='/';
                            postequa[postlength++]=' ';
                            break;
              case times :  postequa[postlength++]='*';
                            postequa[postlength++]=' ';
                            break;
    }
}
void postfix(void){
    char symbol;
    precedence token;
    int n=0, i;
    int first=10;
    int x=0;//first�O�_�s�J�B��l
    int dupsign=10;//�P�_�O�_���ƥX�{�P�@�B��l
    int fin=10;//�����̫�@�ӹB��l
    char num[100];//�Ʀr������
    int length=0;
    push(eos);
    for (token = getToken(&symbol, &n, 1); token != eos;token = getToken(&symbol, &n, 1)) {
        if (token == operand){
            if(dupsign==minus&&first==plus&&fin==minus){//dupsign�O�B�z�����ƹB��l���ȡA(+,-)&-
                pop();
                push(plus);
                num[length] = '-';
                length++;
            }
            else if(dupsign==plus&&first==minus&&fin==minus){//dupsign�O�B�z�����ƹB��l���ȡA(-,-)&+
                pop();
                push(minus);
                num[length] = '-';
                length++;
            }
            else if(dupsign==minus&&(first==times||first==divide)){//*-��/-���p
                pop();
                num[length] = '-';
                length++;
            }
            if(dupsign==plus&&(first==times||first==divide))//*+��/+���p
                pop();
            if(token == operand){
                x = 0;
                first = 10;
                dupsign = 10;
                fin = 10;
            }
            num[length] = symbol;
            length++;
        }
        else if (token == rparen){
            if(length>0){
                for(i=0;i<length;i++)
                    postequa[postlength++]=num[i];
                postequa[postlength++]=' ';
                num[0]='\0';
                length=0;
            }
            while (top->item != lparen)
                printToken(pop( ));
            pop( );
        }
        else{
            if(x==0){//�Ĥ@��token
                first=token;
                x=1;
            }
            fin=token;
            if(token==minus){
                if(dupsign==minus){//�t�t�ন��
                    pop();
                    push(plus);
                    dupsign=plus;
                    continue;
                }
                else if(dupsign==plus){//���t�ন�t
                    pop();
                    push(minus);
                    dupsign=minus;
                    continue;
                }
                else if(dupsign==times||dupsign==divide){//�t���e��������
                    push(minus);
                    dupsign=minus;
                    continue;
                }
            }
            if(dupsign==minus&&token==plus) continue;
            if(token==dupsign&&length==0) continue;//���F�t���e���t�~�B�z�A�ѤU��@���ƿ�J(���L)
            if(length>0&&num[0]!='-'){
                for(i=0;i<length;i++)
                    postequa[postlength++]=num[i];
                postequa[postlength++]=' ';
                num[0]='\0';
                length=0;
            }
            while( isp[top->item] >= icp[token] )
                printToken(pop( ));
            dupsign=token;
            push(token);
        }
    }
    if(length>0){
        for(i=0;i<length;i++)
            postequa[postlength++]=num[i];
        postequa[postlength++]=' ';
        num[0]='\0';
        length=0;
    }
    while ((token = pop ( )) != eos)
        printToken(token);
    postequa[postlength-1]='\0';
    postlength--;
}
void eval(void){
    precedence token;
    char symbol;
    int i;
    int n = 0;
    double temp = 0, op1, op2;
    char num[100];
    int length=0;
    token = getToken(&symbol, &n, 0);
    pushf(0);
    while (token != eos)  {
        if (token == operand||(token == minus && postequa[n*+1]!=' ')){
            num[length] = symbol;
            length++;
            num[length]=0;
            if (postequa[n*+1]==' '){
                temp=(double)atoi(num);
                pushf(temp);
                for(i=0;i<length;i++)
                    num[i]=0;
                length=0;
            }
        }
        else{
            op2 = popf ( );
            op1 = popf ( );
            switch(token) {
                case plus:   pushf(op1+op2);
                             break;
                case minus:  pushf(op1-op2);
                             break;
                case times:  pushf(op1*op2);
                             break;
                case divide: pushf(op1/op2);
            }
        }
    token = getToken (&symbol, &n, 0);
    }
    printf("    %.1f\n",popf());
}
int main()
{
    freopen("infix.txt","r",stdin);
    freopen("postfix.txt","w",stdout);
    int i;
    while(gets(equation)!=NULL){
	if(equation[strlen(equation)-1]==13)
    		equation[strlen(equation)-1]='\0';
    postfix();
    for(i=0;i<postlength;i++)
        printf("%c",postequa[i]);
    eval();
    for(i=0;i<postlength;i++)
        postequa[i]='\0';
    postlength=0;
    }
    return 0;
}
