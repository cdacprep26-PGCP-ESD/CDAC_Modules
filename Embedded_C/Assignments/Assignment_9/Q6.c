//Q6. Creating an anonymous structure, creating some variables from this (with & without typedef)

#include<stdio.h>

//Anonymous structure 
struct
{
	int number;
	char grade;
}student1,student2;

//Anonymous structure using typedef
typedef struct
{
	int number;
	char grade;
}student;

int main()
{
	student student3;
	student student4;
	
	student1.number = 1;
	student1.grade = 'A';
	
	student2.number = 2;
	student2.grade= 'B';
	
	student3.number = 3;
	student3.grade = 'A';
	
	student4.number = 4;
	student4.grade = 'C';
	
	printf("Student1 number=%d and grade=%c\n",student1.number,student1.grade);
	printf("Student2 number=%d and grade=%c\n",student2.number,student2.grade);
	
	printf("Student3 number=%d and grade=%c\n",student3.number,student3.grade);
	printf("Student4 number=%d and grade=%c\n",student4.number,student4.grade);
	
	return 0;
}//main
