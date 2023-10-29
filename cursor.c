#include <stdio.h>

int main()
{
	char pass[8]={'1',' ','2',' ','3',' ','4'}; //pass = 1 2 3 4
	
	char *p1,*p2,*p3,*p4;
	p1 = &pass[0];
	p2 = &pass[2];
	p3 = &pass[4];
	p4 = &pass[6];
	
	printf("abcd = %s\n",pass);
	printf("a b c d = %c %c %c %c\n",pass[0],pass[2],pass[4],pass[6]);
//	printf("p1 = %d\np2 = %d\np3 = %d\np4 = %d\n",p1,p2,p3,p4);
	printf("*p1 = %c\n*p2 = %c\n*p3 = %c\n*p4 = %c\n",*p1,*p2,*p3,*p4);	
	
	
	printf("Nhap gia tri cho a : ");
	scanf("%s",&*p1);
	printf("a = %c\n", *p1);
	
	printf("Nhap gia tri cho b : ");
	scanf("%s",&*p2);
	printf("b = %c\n", *p2);
	
	printf("Nhap gia tri cho c : ");
	scanf("%s",&*p3);
	printf("c = %c\n", *p3);
	
	printf("Nhap gia tri cho a : ");
	scanf("%s",&*p4);
	printf("d = %c\n", *p4);
	
	printf("Vay abcd = %s\n",pass);
	printf("abcd = %c %c %c %c",pass[0],pass[2],pass[4],pass[6]);
}

