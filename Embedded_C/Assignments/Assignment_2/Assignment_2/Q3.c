// Grade of students marks based on marks
#include <stdio.h>

int main(){
	int Science_Marks, Math_Marks, English_Marks, Marathi_Marks, Percentage; //variables declared

	printf("Enter your Science marks out of 100 :");
	scanf("%d",&Science_Marks);

	printf("Enter your Math marks out of 100 :");
        scanf("%d",&Math_Marks);

	printf("Enter your English marks out of 100 :");
        scanf("%d",&English_Marks);

	printf("Enter your Marathi marks out of 100 :");
        scanf("%d",&Marathi_Marks);

	//Percentage formula
	Percentage = (Science_Marks + Math_Marks + English_Marks + Marathi_Marks)/4;
        printf("Your Percentage is : %d\n",Percentage);

	//for second class students
	if(Percentage<=60 && Percentage>=50){
		printf("You got Second Class");
	}

	//for first class students
	else if(Percentage>60 && Percentage<=75){
		printf("You got First class");
		}

	else if(Percentage>75 && Percentage<=100){
                printf("You got First class with Distinction");
                }

	else if(Percentage<50){
                printf("Sorry you have Failed");
                }//else if
	else
		printf("Invalid marks");

	return 0;
}//main


