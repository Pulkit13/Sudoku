#include "solve.h"
#include "count.h"
#include "gen.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#define UPPERLEFT(num) ((((num/9)*9)/(27)*(27))+((((num)-((num/9)*9))/3)*3)) 
typedef struct sudoku
{
	char array[81];
}sudoku;
typedef struct list
{
	int i;
	struct list* next;
}list;
char* board;
int found=0,count=0,gencount=0, genflag=0;
int flag_printall=0, flag_max=0, info_flag=0;
int globaltype=0,num_of_cells_covered=0;
list* output;
sudoku r;
int genarray[81];
void solve(FILE*, int);
void printboard(sudoku*);
void readboard(char* board, FILE* fp, int type);
int sudoku_solver(int loc, int num, sudoku board, int flag);
int next_empty_location(int loc, sudoku* board);
int allcheck(int loc, int num, sudoku* board);
void gen(FILE* fp);
void append(list** arr, int i);
int main(int argc, char* argv[])
{
	FILE* fp;
	char c;
	int type,i,num=0;
	//hw5 solve [file]
	if(argc<2)
	{
		fprintf(stderr,"%s\n","Invalid command line");
		exit(1);
	}
	if(strcmp(argv[1],"solve")==0)
	{
		if(argc>3)
		{
			fprintf(stderr, "%s\n","Correct usage: hw5 solve [file]");
			exit(1);
		}
		if(argc==3)
		{
			fp=fopen(argv[argc-1],"r");
			if(fp==NULL)
			{
				fprintf(stderr,"%s\n","Cannot access file!");
				exit(1);
			}
			if((c=fgetc(fp))=='+') type=1;
			else if(c=='.' || (c>='0' && c<='9')) type=0;
			else
			{
				fprintf(stderr,"%s\n","Invalid input!");
				exit(1);
			}
			fclose(fp);
			fopen(argv[argc-1],"r");
			
		}
		else
		{
			fp=stdin; type=2;
		}
		globaltype=1;
		solve(fp, type);
	}
	else if(strcmp(argv[1],"count")==0)
	{
		if(argc>5)
		{
			fprintf(stderr,"%s\n","Correct Usage: hw5 count [-printall] [-max=n] [file]");
			exit(1);
		}
		if(argc==2)
		{
			fp=stdin; type=2;
		}
		if(argc==3)
		{
			if(strcmp(argv[2],"-printall")==0)
			{
				fp=stdin;
				type=2;
				flag_printall=1;
			}
			else if(argv[2][0]=='-' && argv[2][1]=='m')
			{
				fp=stdin;
				type=2;
				i=5;
				while(argv[2][i]>='0' && argv[2][i]<='9')
				{
					num=num*10+ (argv[2][i]-'0');
					i++;
				}
				flag_max=num;
			}
			else
			{
				fp=fopen(argv[2],"r");
				if(fp==NULL)
				{
					fprintf(stderr,"%s\n","Cannot open file");
					exit(1);
				}
				if((c=fgetc(fp))=='+') type=1;
				else if(c=='.' || (c>='0' && c<='9')) type=0;
				else
				{
					fprintf(stderr,"%s\n","Invalid input!");
					exit(1);
				}
				fclose(fp);
				fp=fopen(argv[2],"r");
			}
		}
		if(argc==4)
		{
			if(strcmp(argv[2],"-printall")==0)
			{
				flag_printall=1;
				if(argv[3][0]=='-' && argv[3][1]=='m')
				{
					fp=stdin;
					type=2;
					i=5;
					if(argv[3][i]>='0' && argv[3][i]<='9')
					while(argv[3][i]>='0' && argv[3][i]<='9')
					{
						num=num*10+ (argv[3][i]-'0');
						i++;
					}
					else
					{
						fprintf(stderr,"%s\n","Invalid command!");
						exit(1);
					}
					flag_max=num;
				}
				else
				{
					fp=fopen(argv[3],"r");
					if(fp==NULL)
					{
						fprintf(stderr,"%s\n","Cannot open file!");
						exit(1);
					}
					if((c=fgetc(fp))=='+') type=1;
					else if(c=='.' || (c>='0' && c<='9')) type=0;
					else	
					{
						fprintf(stderr,"%s\n","Invalid input!");
						exit(1);
					}
					fclose(fp);
					fp=fopen(argv[3],"r");
				}
			}
			else if(argv[2][0]=='-' && argv[2][1]=='m')
			{
				i=5;
				while(argv[2][i]>='0' && argv[2][i]<='9')
				{
					num=num*10+ (argv[2][i]-'0');
					i++;
				}
				flag_max=num;
				if(strcmp(argv[3],"-printall")==0)
				{
					flag_printall=1;
					fp=stdin;
					type=2;
				}
				else
				{
					fp=fopen(argv[3],"r");
					if(fp==NULL)
					{
						fprintf(stderr,"%s\n","Cannot open file!");
						exit(1);
					}
					if((c=fgetc(fp))=='+') type=1;
					else if(c=='.' || (c>='0' && c<='9')) type=0;
					else
					{
						fprintf(stderr,"%s\n","Invalid input!");
						exit(1);
					}
					fclose(fp);
					fp=fopen(argv[3],"r");
				}
			}
		}
		if(argc==5)
		{
			if(strcmp(argv[2],"-printall")==0)
			{
				flag_printall=1;
				i=5;
				while(argv[3][i]>='0' && argv[3][i]<='9')
				{
					num=num*10+ (argv[3][i]-'0');
					i++;
				}
				flag_max=num;
				fp=fopen(argv[4],"r");
				if(fp==NULL)
				{
					fprintf(stderr,"%s\n","Cannot open file!");
					exit(1);
				}
				if((c=fgetc(fp))=='+') type=1;
				else if(c=='.' || (c>='0' && c<='9')) type=0;
				else
				{
					fprintf(stderr,"%s\n","Invalid input!");
					exit(1);
				}
				fclose(fp);
				fp=fopen(argv[4],"r");
			}
			else if(argv[2][0]=='-' && argv[2][1]=='m')
			{
				i=5;
				while(argv[2][i]>='0' && argv[2][i]<='9')
				{
					num=num*10+ (argv[2][i]-'0');
					i++;
				}
				flag_max=num;
				if(strcmp(argv[3],"-printall")==0)
				{
					flag_printall=1;
				}
				fp=fopen(argv[4],"r");
				if(fp==NULL)
				{
					fprintf(stderr,"%s\n","Cannot open file!");
					exit(1);
				}
				if((c=fgetc(fp))=='+') type=1;
				else if(c=='.' || (c>='0' && c<='9')) type=0;
				else
				{
					fprintf(stderr,"%s\n","Invalid input!");
					exit(1);
				}
				fclose(fp);
				fp=fopen(argv[4],"r");
			}
			else
			{
				fprintf(stderr,"%s\n","hw5 count [-printall] [-max=n] [file]");
				exit(1);
			}
		}
	solve(fp,type);
	}
	
	// hw5 gen [-info] [rndfile]
	else if(strcmp(argv[1],"gen")==0)
	{
		genflag=1;
		if(argc==2)
		{
			fp=stdin;
		}
		if(argc==3)
		{
			if(strcmp(argv[2],"-info")==0)
			{
				info_flag=1;
				fp=stdin;
			}
			else
			{
				fp=fopen(argv[2],"rb");
				if(fp==NULL)
				{
					fprintf(stderr,"%s\n","Cannot open file!");
					exit(2);
				}
			}
		}
		if(argc==4)
		{
			if(strcmp(argv[2],"-info")==0)
			{
				info_flag=1;
				fp=fopen(argv[3],"rb");
				if(fp==NULL)
				{
					fprintf(stderr,"%s\n","Cannot open file!");
					exit(2);
				}
			}
			else if(strcmp(argv[3],"-info")==0)
			{
				info_flag=1;
				fp=fopen(argv[2],"rb");
				if(fp==NULL)
				{
					fprintf(stderr,"%s\n","Cannot open file!");
					exit(2);
				}
			}
			else
			{
				fprintf(stderr,"%s\n","Correct Usage: hw5 gen [-info] [rndfile]");
				exit(3);
			}
		}
	gen(fp);
	}
	else 
	{
		fprintf(stderr,"%s\n","Invalid command line");
		exit(1);
	}
	return 1;
}

void gen(FILE* fp)
{
	FILE* fptr;
	list* input;
	list* temp;
	list* old;
	
	sudoku b;
	int i=0,j=0,k=-1,m=0,num;
	int x[4],R,L=9;
	unsigned int X=0;
	input=NULL;
	output=NULL;
	fptr=fp;
	for(i=0;i<9;i++)
	{
		num=1;
		for(j=i*9; j< (i*9)+9;j++)
		{
			append(&input,num);
			b.array[j]='.';
			num++;
		}
	}
	j=0;
	if(info_flag)
	printf("Random shuffle results:\n\n");
	while(j<81)
	{
		if(j%9==0) {if(info_flag) printf("\n"); L=9; k++;}
		for(i=0;i<4;i++)
		{
			x[i]=fgetc(fp);
			if(x[i]=='\n' || x[i]=='\0')
			{
				exit(1);
			}
			if(x[i]==EOF)
			{
				fprintf(stderr,"%s\n","Exhausted reading from file");
				exit(3);
			}
		}
		
		X=(x[0]<<24) + (x[1]<<16) + (x[2]<<8) + x[3];
		R=X%L;
		L--;
		temp=input;
		old=NULL;
		for(m=0;m<R;m++)
		{
			old=temp;
			temp=temp->next;
		}
		if(R==0)
		{
			input=input->next;
		}
		if(R!=0) old->next=temp->next;
		append(&output,temp->i);
		if(info_flag)
		printf("%d",temp->i);
		free(temp);
		j++;
	}
	printf("\n");
	found=0;
	temp=output; i=0;
	while(temp!=NULL)
	{
		genarray[i++]=(temp->i);
		temp=temp->next;
	}
	printf("Genarray :\n");
	for(i=0;i<9;i++)
	{
		for(j=i*9;j<(i*9)+9;j++)
		{
			printf("%d",genarray[j]);
		}
		printf("\n");
	}
	return ;
}
void solve(FILE* fp, int type)
{
	int i=0,j=0;
	char c;
	char* initial_board;
	sudoku gameboard;
	board = (char*)malloc(9 * 9);
	initial_board=(char*)malloc(9*sizeof(char));
	if(type==2)
	{
		//read first line in initial board
		while((c=fgetc(fp))!='\n')
		{
			initial_board[j++]=c;
		}
		if(initial_board[0]=='+') type=1;
		else if(initial_board[0]=='.' || (initial_board[0]>='0' && initial_board[0]<='9')) 
		{
			for(i=0;i<9;i++)
			{
				board[i]=initial_board[i];
			}
			type=0;
		}
		else
		{
			fprintf(stderr,"%s\n","Invalid input");
			exit(2);
		}
		
	}
	readboard(board,fp,type);
	//printboard(board);
	
	//so now board has the initial configuration like:
	/*
	9.8..6.2.
	..4..2.96
	...9..3..
	...2..85.
	...3.8...
	...75.2.9
	....1.7.2
	81...7...
	72...316.
	*/
	for(i=0;i<81;i++)
	{
		gameboard.array[i]=board[i];
	}
	
	sudoku_solver(0,1,gameboard,0);
	if(!found)
	{
		fprintf(stderr,"%s\n","No solutions!");
		exit(2);
	}
	if(!globaltype)
	{
		if(count>flag_max)
		{
			if(flag_max!=0)
			count=flag_max;
		}
		if(count>1)
		printf("%d solutions found\n",count);
		else
		printf("%d solution found\n",count);
		
	}
}

int sudoku_solver(int loc, int num, sudoku b, int flag)
{
	if(num>9) 
	return -1;
	if(flag==1)
	{
		loc++;
	}
	if(flag==2)
	{
		loc--;
	}
	loc=next_empty_location(loc, &b); 

    if (loc == -1) {
		count++;
		if(genflag)
		{
			if(count==2)
			gencount=1;
		}
		if(flag_max)
		{
			if(count<=flag_max)
			{
				if(flag_printall)
				{
					printboard(&b);
					printf("\n");
				}
			}
		}
		else if(flag_printall)
		{
			printboard(&b);
			printf("\n");
		}
		else if(globaltype)
		{
			printboard(&b);
		}
		else if(!flag_max && !flag_printall)
		{
			found=1;
		}
        return -1; 
    } 
	if(allcheck(loc,num,&b))
	{
		b.array[loc]=(num+'0');
		sudoku_solver(loc,1,b,1);
		if(flag==0)
		b.array[loc]=num+'0';
		b.array[loc]='.';
		sudoku_solver(loc+1,num+1,b,2);
	}
	else
	{
		sudoku_solver(loc+1,num+1,b,2);
	}
	return 1;
}

int next_empty_location(int loc, sudoku* b)
{
	while(loc<81)
	{
		if(b->array[loc]!='.')
		loc++;
		else break;
	}
	if(loc<81)
	return loc;
	
	else
	return -1;
}

int allcheck(int loc, int num, sudoku* b)
{
	int i,j,k;
	i= (loc/9)*9;
	for(j=0;j<9;j++)
	{
		if((b->array[i+j])==(num+'0'))
		return 0;
	}
	i=(loc)%9;
	for(j=0;j<9;j++)
	{
		if((b->array[i+(j*9)])==(num+'0'))
		return 0;	
	}
	i=UPPERLEFT(loc);
	for(j=0;j<3;j++)
	{
		for(k=0;k<3;k++)
		{
			if((b->array[i+(j*9)+k])==(num+'0'))
			return 0;
		}
	}
	return 1;
}

void printboard(sudoku* ch)
{
	int i,count1=0;
	if(genflag==0){
	for(i=0;i<81;i++)
	{
		if(board[i]!='.')
		{
			ch->array[i]='.';
		}
	}
	}
	printf("+---+---+---+");
	for(i=0;i<81;i++)
	{
			printf("\n");
			if(i==27 || i==54)
			printf("+---+---+---+\n");
			while(count1<9)
			{
					if(count1%3==0)
					printf("|%c",ch->array[i++]);
					else
					printf("%c",ch->array[i++]);
					count1++;
			}
			printf("|");
			i--;
			count1=0;
	}
	printf("\n+---+---+---+\n");
	found=1;
}
void readboard(char* board, FILE* fp, int type)
{
	int i=0,j=0,k=0;
	char buf[15];
	char ch[10][14];
	char ch1[8][9];
	int c=0;
	if(type==1)
	{
		while(fgets(buf,15,fp)!=NULL)  // +---+---+---+
		{
			c++;
			if((*buf)=='\n')
			break;
			
			if(buf[0] == '+') 
			{
			continue;
			}
			while(j<13) 
			{
				ch[i][j]=buf[j]; //ch[0][0] till ch[0][12], ch[1][0], .... ch[8][0]
				j++;
			}
			i++;
			j=0;
			
		}
		
		if(c==14)
		{
			fprintf(stderr,"%s\n","Invalid sudoku!");
			exit(2);
		}
		for(i=0;i<9;i++)
		{
			for(j=0;j<13;j++)
			{
				if(j%4==0)
				if(ch[i][j]!='|')
				{
					fprintf(stderr,"%s\n","Invalid input!");
					exit(1);
				}
					
				
				if(j%4!=0)
				{
					if(ch[i][j]!='.' && !(ch[i][j]>='0' && ch[i][j]<='9'))
					{
						fprintf(stderr,"%s\n","Invalid input!");
						exit(1);
					}
					board[k]=ch[i][j];
					k++;
				}
			}
		}
		
	}
	
	else if(type==0)
	{
		while(fgets(buf,11,fp)!=NULL)
		{
			if((*buf)=='\n') 
			break;
			while(j<9)
			{
				ch1[i][j]=buf[j];
				j++;
			} 
			i++;
			j=0;
		}
		k=9;
		for(i=0;i<8;i++)
		{
			for(j=0;j<9;j++)
			{
				board[k]=ch1[i][j];
				k++;
			}
		}
	}
	
}
void append(list** arr, int i)
{
	list* temp;
	list* r;
	if(*arr==NULL)
	{
		temp=(list*)malloc(sizeof(list));
		temp->i=i;
		temp->next=NULL;
		*arr=temp;
	}
	else
	{
		temp=*arr;
		while(temp->next!=NULL) temp=temp->next;
		r=(list*)malloc(sizeof(list));
		r->i=i;
		r->next=NULL;
		temp->next=r;
	}
}
