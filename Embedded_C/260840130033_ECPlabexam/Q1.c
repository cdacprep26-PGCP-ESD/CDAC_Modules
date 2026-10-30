#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct 
{
    char item[20];
    int quantity;
    int is_low;
} Box;


Box** b = NULL;
int n = 0; // Number of Items
int g_row, g_column;

//Dynamiic Memory Allocation 2D array shelve

void create_grid() 
{

    printf("Enter Number of Rows in Grid : ");
    scanf("%d", &g_row);
    printf("Enter Number of Columns in Grid : ");
    scanf("%d", &g_column);

    b = (Box**)malloc(g_row * sizeof(Box*));
    if (b == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i < g_row; i++)
     {
        b[i] = (Box*)malloc(g_column * sizeof(Box));
        if (b[i] == NULL)
         {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
}


void free_grid() 
{
    for (int i = 0; i < g_row; i++) 
    {
        free(b[i]);
    }
    free(b);
}

//Add Item
void add_item() 
{
    if (n == g_row * g_column) 
    {
        return;
    }

    int row, column;
    scanf("%d %d", &row, &column);

    if (row < 0 || row >= g_row || column < 0 || column >= g_column) 
    {
        printf("Invalid Shelve ID\n");
        return;
    }


    scanf("%s", b[row][column].item);


    int quantity;
    scanf("%d", &quantity);
    if (quantity < 0) 
    {
        printf("Invalid Quantity\n");
        return;
    }
    
    b[row][column].quantity = quantity;

    if (b[row][column].quantity < 10) 
    {
        b[row][column].is_low = 1;
    } 
    else 
    {
        b[row][column].is_low = 0;
    }

    n++;
}



//Modified Quantity
void mod_quantity() 
{
    int row, column;
    printf("Enter the Shelve ID: ");
	scanf("%d %d", &row, &column);

    if (row < 0 || row >= g_row || column < 0 || column >= g_column) 
    {
        printf("Invalid Shelve ID\n");
        return;
    }


    printf("Enter New Quantity: ");
    int new_quantity;
    scanf("%d", &new_quantity);
    if (new_quantity < 0) 
    {
        printf("Invalid Quantity\n");
        return;
    }
    
    b[row][column].quantity = new_quantity;

    if (b[row][column].quantity < 10) {
        b[row][column].is_low = 1;
    } else {
        b[row][column].is_low = 0;
    }
}

//Diplay Elements
void display() 
{

    printf("Displaying All Items\n");
    for (int i = 0; i < g_row; i++) 
    {
        for (int j = 0; j < g_column; j++) 
        {
            if (b[i][j].quantity > 0) 
            { // Only print non-empty boxes
                printf("Item Name : %s\n", b[i][j].item);
                printf("Quantity : %d\n", b[i][j].quantity);
                
                if (b[i][j].is_low == 1) 
                {
                    printf("Quantity of Item %s : low\n", b[i][j].item);
                } else 
                {
                    printf("Quantity of Item %s : high\n", b[i][j].item);
                }
            }
        }
    }
}

//Search 
void search() 
{
    char temp_name[20];
    printf("Enter Item Name: ");
    scanf("%s", temp_name);

    int found = 0;
    for (int i = 0; i < g_row; i++) 
    {
        for (int j = 0; j < g_column; j++) 
        {
            if ((strcmp(temp_name, b[i][j].item) == 0))
             {
                printf("Item is at Shelve_ID %d,%d\n", i, j);
                found = 1;
                break;
            }
        }
        if (found) 
        {
            break;
        }
    }

    if (!found)
     {

        printf("Not Found\n");
    }
}

int main() 
{
    int choice, flag = 0;

    while (1) 
    {

        printf("\nMenu:\n");
        printf("1. Add Item\n");
        printf("2. Modify Quantity\n");
        printf("3. Display All the items\n");
        printf("4. Search Item by Name\n");
        printf("5. Exit\n");
        printf("Enter Choice : ");
        scanf("%d", &choice);
        switch (choice) 
        {
            case 1:
                if (flag == 0) 
                {
                    create_grid();
                    printf("Shelve Grid Created\n");
                    flag = 1;
                } 
                else
                 {
                    add_item();
                }
                break;
            case 2:
                mod_quantity();
                break;
            case 3:
                display();
                break;
            case 4:
                search();
                break;
            case 5:

                printf("Exiting\n");

                free_grid();
                return 0;
            default:

                printf("Invalid Input!!\n");

                break;
        }
    }

    return 0;
}//main
		
			
			
			 	
	
	
			
				
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	 
