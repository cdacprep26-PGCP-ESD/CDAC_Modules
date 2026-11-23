#include<linux/module.h>
#include<linux/init.h>
#include<linux/moduleparam.h>
#include<linux/kernel.h>
#include<linux/errno.h>

static int marks[5];
static int count = 0;
static int passing_marks = 40;
static int pased_students = 0;


static int __init marks_analyzer_init(void)
{
	if(count >= 1)
	{
		int total_marks = 0;
		int high = marks[0], low = marks[0];
		for(int i = 0; i<count; i++)
		{	
			if(marks[i]>100 || marks[i]<0)
			{
				pr_err("Marks Range: 0 to 100\n");
				return EINVAL;
			}
			else
			{

				if(marks[i]>high)
				{
					high = marks[i];
				}
				else if(marks[i]<low)
				{
					low = marks[i];
				}

				total_marks += marks[i];

				if(marks[i]>passing_marks)
				{
					pased_students++;
				}
			}
		}

		int avg = total_marks / count;
		pr_info("Marks Collected: %d Highest Marks: %d Lowest Marks: %d Total Mark: %d Avg: %d Passed  Students\n", count, high, low, total_marks, avg, passed_student);

	}
	return 0;
}

static void __exit marks_analyzer_exit(void)	
{
	pr_info("Marks_Analyzer Exiting\n");
}

module_param_array(marks, int, &count, 0444);
module_init(marks_analyzer_init);
module_exit(marks_analyzer_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Marks Analyzer");


