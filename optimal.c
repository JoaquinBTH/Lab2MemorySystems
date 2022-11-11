#include <stdio.h>
#include <stdlib.h>

//Function to check for each page in our page table if the number is ever referenced again in the future or not
int predict(int page[], int physPages, int allNumbers[], int pos)
{
    int replaceIndex = 0; //Index of the most suitable number to replace.
    int furthestIndex = -1; //The index of the number that's referenced furthest into the future.

    for(int i = 0; i < physPages; i++) //Loop through all or pages
    {
        int j; //Declare variable outside of foor loop so we can check if it reached the end of its loop without finding a future reference.
        for(j = pos; j < 100000; j++) //Loop through all numbers after our current position in the list.
        {
            if(page[i] == allNumbers[j]) //It found a future reference of the number.
            {
                if(j > furthestIndex) //If the index of that future reference is after the current furthest known index of a reference, replace it.
                {
                    furthestIndex = j;
                    replaceIndex = i; //Update the index of what page that should be removed in case this is the furthest reference of it.
                }
                break;
            }
        }
        if(j == 100000) //No future reference was found, safe to replace the page on this index.
        {
            return i;
        }
    }

    return replaceIndex;
}


int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Correct usage: ./fifo <no_phys_pages> <page_size> <filename>\n");
        return 0;
    }

    //Get the values stored in each argument.
    int physPages = atoi(argv[1]);
    int pageSize = atoi(argv[2]);
    char *fileName = argv[3];

    printf("Physical pages = %d, Page size = %d, fileName = %s\n", physPages, pageSize, fileName);

    int pageFaults = 0; //Variable for storing number of page faults.

    //Create the page frames and initialize them to -1.
    int page[physPages];
    for(int i = 0; i < physPages; i++)
    {
        page[i] = -1;
    }

    //Open up the file.
    FILE *file = fopen(fileName, "r");
    if(file == NULL)
    {
        printf("Failed to open file\n");
        return 0;
    }

    char line[10]; //Variable to store what we read from the line.
    int first = 0; //Fill up the page table first using this variable.
    
    //Create a list with all numbers from the entire memory reference.
    int allNumbers[100000];
    int pos = 0;
    while (fgets(line, sizeof(line), file))
    {
        allNumbers[pos++] = atoi(line) / pageSize;
    }

    rewind(file); //Rewind the pointer to the start of the file.
    pos = 0; //Set pos to 0.

    while (fgets(line, sizeof(line), file)) //Reads each line of the file and stores it in "line".
    {
        int number = atoi(line) / pageSize; //Convert the line read to a number and divide it by pageSize.

        pos++; //Increment position variable.

        //Search if the number is currently in a page frame
        unsigned found = 0;
        for(int i = 0; i < physPages; i++)
        {
            if(page[i] == number)
            {
                found = 1;
                break;
            }
        }

        if(found == 0 && page[(physPages - 1)] == -1) //Fill up the page table first.
        {
            page[first++] = number;
            pageFaults++;
        }
        else if(found == 0) //Number wasn't found in any of the page frames.
        {
            page[predict(page, physPages, allNumbers, pos)] = number; //Replace the earliest element with the new number.
            pageFaults++; //Increment number of page faults.
        }

    }

    fclose(file); //Close the file.

    printf("Number of page faults = %d\n", pageFaults);

    return 0;
}