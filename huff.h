#define MAX 16
#define LEAF 0
#define INTERNAL 1
#define ASCII_SIZE 256
char padding;
unsigned char N;

typedef struct coder
{ char alpha;
  char code[MAX];
}coder;


typedef struct node
{
	char alpha;
	char *code;
	int freq;
	int type;
	struct node *next;
	struct node *left; 
	struct node *right;
}node;

node *head,*root;

void generateC(node * a,char* code);
void printll();
char ext[]=".huff";
void writeB(int b,FILE *f);
char dext[]=".unhuff";
void makeTree();
void insert(node *a,node *b);
void addSym(char c);
void writeC(char ch,FILE *f);
int fileError(FILE * fp;);
char *getC(char ch);
coder *symlist;
void writeH(FILE *f);
int match(char a[],char b[],int limit);
int n;
char *decodeBuffer(char buffer);
char *intstring(int n);
