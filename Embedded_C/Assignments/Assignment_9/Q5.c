/*Q5. Whats wrong in the following code, suggest a fix for this.
struct A
{
int x;
char *str; // (or) char str[20];
};
struct A a1 = { 101, “abc” } , a2; a1.x=10; a1.str=”hello”; 
//works? scanf(“%d%s”,&a1.x,a1.str); 
//works? a2 = a1; //shallow copy or deep copy?
What if str is declared as an array instead of pointer, i.e. char str[20]
*/

#include<stdio.h>
#include<string.h>
struct A
{
int x;
//char *str; Modifying str Will give Segmentation Fault As it writes to read only section of the memory
char str[20];
};

int main()
{
	struct A a1 = { 101, "abc" } , a2; 
	printf("a1.x %d\n,a1.str %s\n",a1.x,a1.str);
	a1.x=10; 
	strcpy(a1.str, "hello");
	printf("a1.x %d\n,a1.str %s\n",a1.x,a1.str);
	
	//For char*str; gives Segmentation Fault
	scanf("%d%s",&a1.x,a1.str); 
	printf("a1.x %d\n,a1.str %s\n",a1.x,a1.str);
	
	// a2 = a1  Shallow or deep copy ?
	a2 = a1;
	printf("Copying a1 to a2\n");
	printf("a2.x %d \n a2.name %s\n", a2.x, a2.str);

	//Modifying a1.str
	printf("Modifying a1.str\n");
	strcpy(a1.str,"pqr");
	printf("Shallow copy a2.str %s\n", a2.str); //shallow copy a2.str value changed

	return 0;
}//main
