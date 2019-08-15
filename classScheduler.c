/* Mason McDaniel UTAID: 1001456342 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct classroom
{
	char section[5];
	char subject[5];
	char courseNumber[5];
	char instructorName[31];
	char building[5];
	char room[5];
	char start[6];
	char end[6];
	char mon[2];
	char tues[2];	
};

struct classroom* readFile(FILE *fp,int *n)
{
	struct classroom *b;
	char* p;
	int lines = 0;
	char buffer[71], mon[2], tues[2], section[5], subject[5],
	courseNumber[5], instructorName[31], building[5], room[5],
	start[6], end[6];	
	b = calloc(1, sizeof(struct classroom));
	while(fgets(buffer, 71, fp) != NULL) 
	{
        char * pos; 
        struct classroom* temp;    
        if((pos=strchr(buffer, '\n')) != NULL)
        {
            *pos = '\0';
        } 
        if(lines <= 0)
        {
        	/* Do nothing this is the heading */
        }
        else 
        {
        	temp = realloc(b, ((lines) * sizeof(struct classroom)));
			if(temp != NULL)
			{
				b = temp;			
			}
			else
			{
				printf("unable to reallocate\n");
				free(temp);
				free(b);
				exit(1);
			}
        	strcpy(section, strtok(buffer, ","));
    		strcpy(subject, strtok(NULL, ","));
    		strcpy(courseNumber, strtok(NULL, ","));
    		strcpy(instructorName , strtok(NULL, ","));
    		strcpy(building, strtok(NULL, ","));
    		strcpy(room, strtok(NULL, ","));
    		strcpy(start, strtok(NULL, ","));
    		strcpy(end, strtok(NULL, ","));
    		strcpy(mon, strtok(NULL, ","));
    		strcpy(tues, strtok(NULL, ","));   
    		if(strcmp(mon, tues) != 0)
    		{
    			strcpy(b[lines-1].section, section);
    			strcpy(b[lines-1].subject, subject);
    			strcpy(b[lines-1].courseNumber, courseNumber);
    			strcpy(b[lines-1].instructorName , instructorName);
    			strcpy(b[lines-1].building, building);
    			strcpy(b[lines-1].room, room);
    			strcpy(b[lines-1].start, start);
    			strcpy(b[lines-1].end, end);
    			strcpy(b[lines-1].mon, mon);
    			strcpy(b[lines-1].tues, tues);
    		}
    		else
    		{
    			lines--;
    		}		
        }
        lines++; 
    }
    *n = lines-1;
    return b;
}
 
void bubbleSort(struct classroom* arr, int n) 
{ 
   int i, j; 
   struct classroom temp;
   /* Sort by class meeting date (M-T) */
   for(i = 0; i < n-1; i++)
   { 
   		for(j = 0; j < n-i-1; j++)  
   		{
       		if((strcmp(arr[j].mon, "Y") != 0) && (strcmp(arr[j+1].mon, "Y") == 0))
           	{
           		temp = arr[j];
           		arr[j] = arr[j+1];
           		arr[j+1] = temp;     				
           	}
           	else if(strcmp(arr[j].mon, arr[j].tues) == 0)
           	{
           		temp = arr[j];
           		arr[j] = arr[j+1];
           		arr[j+1] = temp;
           	}	
        }
   	}
   	/* Subsort by time (ascending) */ 	
    for(i=0; i<n-1; i++)
    {
    	for(j = 0; j < n-i-1; j++)  
   		{	
   			if((strcmp(arr[j].mon,"Y") == 0) && (strcmp(arr[j+1].mon,"N") != 0))
    		{
       			if(strcmp(arr[j].start, arr[j+1].start)>0)
    			{
    				temp = arr[j];
    				arr[j] = arr[j+1];
    				arr[j+1] = temp;
    			}
    		}
    		if((strcmp(arr[j+1].mon,"Y") != 0) && (strcmp(arr[j].tues,"Y") == 0))
    		{
       			if(strcmp(arr[j].start, arr[j+1].start)>0)
    			{
    				temp = arr[j];
    				arr[j] = arr[j+1];
    				arr[j+1] = temp;
    			}
    		}		
        } 			
    }

        	
}


int main()
{
	struct classroom *possibleClasses, *desiredClasses, *classes;
	int i, j, k, found, lines;
	int* p = &lines;
	char ptime[6] = "08:00";
	char buffer[12];
	FILE *fp, *eligible;
	desiredClasses = calloc(1, sizeof(struct classroom));
	classes = calloc(1, sizeof(struct classroom));
	fp = fopen("fall2019courses.csv", "r");
	eligible = fopen("eligible.txt", "r");
	i = 0;
	found = 0;
	if((fp == NULL) || (eligible == NULL))
	{
		printf("ERROR: Cant open file!");
		exit(1);
	}
	else
	{
		/* File successfully loaded */
		possibleClasses = readFile(fp, p);
		fclose(fp);
		bubbleSort(possibleClasses, lines);
		while(fgets(buffer, 11, eligible) != NULL) 
		{
			char * pos;
			struct classroom *temp;

			if((pos=strchr(buffer, '\n')) != NULL)
        	{
            	*pos = '\0';
        	} 
        	temp = realloc(desiredClasses, ((i+1) * sizeof(struct classroom)));
			if(temp != NULL)
			{
				desiredClasses = temp;			
			}
			else
			{
				printf("unable to reallocate\n");
				free(temp);
				exit(1);
			}

			strcpy(desiredClasses[i].subject, strtok(buffer, " "));
    		strcpy(desiredClasses[i].courseNumber, strtok(NULL, " "));
    		i++;
		}
		for(j=0;j<lines;j++)
		{
			for(k=0;k<i;k++)
			{
				if((strcmp(desiredClasses[k].subject, possibleClasses[j].subject) == 0) && (strcmp(desiredClasses[k].courseNumber, possibleClasses[j].courseNumber) == 0) 
					&& (strcmp(possibleClasses[j].mon, "Y")==0)){
					if((strcmp(possibleClasses[j].start, ptime)==0) || (strcmp(possibleClasses[j].start, ptime)>0))
					{
						struct classroom *temp;
						temp = realloc(classes, ((found+1) * sizeof(struct classroom)));
						if(temp != NULL)
						{
							classes = temp;			
						}
						else
						{
							printf("unable to reallocate\n");
							free(temp);
							exit(1);
						}
						classes[found] = possibleClasses[j];
						strcpy(desiredClasses[k].subject, "A"); 
						strcpy(ptime, possibleClasses[j].end);

						found++;
					}
				}
			}
		}
		strcpy(ptime,"08:00");
		for(j=0;j<lines;j++)
		{
			for(k=0;k<i;k++)
			{
				if((strcmp(desiredClasses[k].subject, possibleClasses[j].subject) == 0) && (strcmp(desiredClasses[k].courseNumber, possibleClasses[j].courseNumber) == 0) 
					&& (strcmp(possibleClasses[j].tues, "Y")==0)){
					if((strcmp(possibleClasses[j].start, ptime)==0) || (strcmp(possibleClasses[j].start, ptime)>0))
					{
						struct classroom *temp;
						temp = realloc(classes, ((found+1) * sizeof(struct classroom)));
						if(temp != NULL)
						{
							classes = temp;			
						}
						else
						{
							printf("unable to reallocate\n");
							free(temp);
							exit(1);
						}
						classes[found] = possibleClasses[j];
						strcpy(desiredClasses[k].subject, "A"); 
						strcpy(ptime, possibleClasses[j].end);

						found++;
					}
						
				}
			}
		}
		k=0;
		for(j=0;j<8;j++)
		{

			if(strcmp(classes[j].mon, "Y") == 0)
			{
				if(j==0)
				{
					printf("***  Monday schedule  ***\n");
				}
				printf("%s ", classes[j].start);
				printf("%5s ", classes[j].subject);
				printf("%s.", classes[j].courseNumber);
				printf("%s  ", classes[j].section);
				printf("%s--", classes[j].start);
				printf("%s  ", classes[j].end);
				printf("%s ", classes[j].building);
				printf("%s", classes[j].room);
				if(strlen(classes[j].building) == 3)
				{
					printf("  ");
				}
				else
				{
					printf("   ");
				}
				printf("%s\n", classes[j].instructorName);
			}
			if(strcmp(classes[j].mon, "N") == 0)
			{
				if(k==0)
				{
					printf("\n***  Tuesday schedule  ***\n");
				}
				printf("%s ", classes[j].start);
				printf("%5s ", classes[j].subject);
				printf("%s.", classes[j].courseNumber);
				printf("%s  ", classes[j].section);
				printf("%s--", classes[j].start);
				printf("%s  ", classes[j].end);
				printf("%s ", classes[j].building);
				printf("%s", classes[j].room);
				if(strlen(classes[j].building) == 3)
				{
					printf("  ");
				}
				else
				{
					printf("   ");
				}
				printf("%s\n", classes[j].instructorName);
				k++;
			}
		}
		free(classes);
		free(possibleClasses);
		free(desiredClasses);
	}
	return 0;
}
