#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include "huff.h"

node* newNode(char c)
{
	node * q=(node *)malloc(sizeof(node));
	q->alpha=c;
	q->type=LEAF;
	q->freq=1;
	q->next=NULL;
	q->left=NULL;
	q->right=NULL;
	return q;
}


void printll()
{
node *a;
a=head;
 while(a!=NULL)
 {
   a=a->next;
 }
}


void writeH(FILE *f)
{
coder record;
node *a = head;
int temp = 0;
int i = 0;
while(a!=NULL)
{
	temp+=(strlen(a->code)) * (a->freq);
	if(strlen(a->code) > MAX)
	temp%=8;
	i++;
	a=a->next;
}

if(i==256)
	N=0;
else 
	N=i;

fwrite(&N,sizeof(unsigned char),1,f);
a = head;
while(a != NULL)
{
	record.alpha=a->alpha;
	strcpy(record.code,a->code);
	fwrite(&record,sizeof(coder),1,f);
	a = a->next;
}
padding=8-(char)temp;
fwrite(&padding,sizeof(char),1,f);
for(i=0;i<padding;i++)
	writeB(0,f);
}

void writeC(char ch,FILE *f)
{
char *code;
code=getC(ch);
	while(*code!='\0')
	{
		if(*code=='1')
			writeB(1,f);
		else
			writeB(0,f);
	code++;
	}
	return;
}

void writeB(int b,FILE *f)
{
	static char byte;
	static int count;
	char temp;
	if(b==1)
	{	temp = 1;
		temp = temp<<(7-count);
		byte = byte | temp;
	}
	count++;
	
	if(count==8)
	{
		fwrite(&byte,sizeof(char),1,f);
		count=0;
		byte=0;	
		return;
	}
	return;
}


void insert(node *a,node *b)
{
	if(b->next==NULL)
	{  
		b->next=a;
		return;
	}
	while(b->next->freq < a->freq)
	{  
		b=b->next;
	  if(b->next==NULL)
	    { 
			b->next=a;
			return;
		}
	}
  a->next=b->next;
  b->next=a;
}
void generateC(node * a,char* code)
{
char *lcode,*rcode;
static node *s;
static int flag;
if(a!=NULL)
{
	if(a->type==LEAF)
	{	if(flag==0) 
		{
			flag=1; 
			head = a;
		}
		else
		s->next=a;

		a->next=NULL;
		s=a;
	}
	a->code=code;
	lcode=(char *)malloc(strlen(code)+2);
	rcode=(char *)malloc(strlen(code)+2);
	sprintf(lcode,"%s0",code);
	sprintf(rcode,"%s1",code);
	generateC(a->left,lcode);
	generateC(a->right,rcode);
}
}

void addSym(char c)
{
node *a,*b,*e;

if(head==NULL)
{	
	head=newNode(c);
	return;
}
	a=head;
	b=NULL;
if(a->alpha==c)
{
	a->freq+=1;
	if(a->next==NULL)
		return;
	if(a->freq > a->next->freq)
	{
		head = a->next;
		a->next=NULL;
		insert(a,head);
	}
	return;
}

while(a->next!=NULL && a->alpha!=c)
{
	b=a;
	a=a->next;
}

if(a->alpha==c)
{
	a->freq+=1;
        if(a->next==NULL)
		return;	
	if(a->freq > a->next->freq)
	{
		e=a->next;
		b->next=a->next;
		a->next=NULL;
		insert(a,head);
	}
}
else 
{	
	b=newNode(c);
	b->next=head;  
	head = b;
}
}

char *getC(char ch)
{
node *a= head;
	while(a!=NULL)
	{
	    if(a->alpha==ch)
		  return a->code;
          a=a->next;
	}
	return NULL;
}
void makeTree()
{
node  *a,*b;
a = head;
b = NULL;
	while(a!=NULL)
	{
		b=newNode('+');
		b->type=INTERNAL;
		b->left=a;		
		b->freq=a->freq;
		if(a->next!=NULL)
		{
			a=a->next;
			b->right=a;	
			b->freq += a->freq;
		}
		a=a->next;	
		if(a==NULL)
			break;
		if(b->freq <= a->freq)
		{
			b->next=a;
			a=b;
		}
		else
			insert(b,a);
	}
	root = b;
}

int main(int argc, char *argv[])
{
head=NULL;
root=NULL;
FILE *fp,*fp1;
char ch;
if(argc<=2)
{
	if(argc==2)
	{
		argv[2]=(char *)malloc(sizeof(char)*(strlen(argv[1])+strlen(ext)+1));
		strcpy(argv[2],argv[1]);
		strcat(argv[2],ext);
		argc++;
	}
	else
		return 0;
}
fp=fopen(argv[1],"rb");
if(fp==NULL)
{
	printf("File Cannot be Opened.\n");
	return 0;
}
	while(fread(&ch,sizeof(char),1,fp)!=0)
		addSym(ch);
	fclose(fp);
makeTree();
generateC(root,"\0");
fp=fopen(argv[1],"r");
if(fp==NULL)
{
	printf("File Cannot be Opened.\n");
	return 0;
}
fp1=fopen(argv[2],"wb");
if(fp1==NULL)
{
	printf("File Cannot be Opened.\n");
	return 0;
}
	writeH(fp1);
	while(fread(&ch,sizeof(char),1,fp)!=0)
	{	
		writeC(ch,fp1);
	}
fclose(fp);
fclose(fp1);
return 0;
}
