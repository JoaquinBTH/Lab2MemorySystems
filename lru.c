#include <stdio.h>
#include <stdlib.h>

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

    int page[physPages]; //Create the page frames
    int lastAccessed[physPages]; //Create a list of when a page frame was last accessed
    for(int i = 0; i < physPages; i++) //Initialize all elements in both arrays to -1.
    {
        page[i] = -1;
        lastAccessed[i] = -1;
    }
    //Open up the file.
    FILE *file = fopen(fileName, "r");
    if(file == NULL)
    {
        printf("Failed to open file\n");
        return 0;
    }

    char line[10]; //Variable to store what we read from the line.
    int least = 0; //Value to store the position of the least accessed element.
    int accessID = -1; //ID to give to a page frame whenever it gets accessed. Lower ID means less recently used.

    while (fgets(line, sizeof(line), file)) //Reads each line of the file and stores it in "line".
    {
        accessID++;
        int number = atoi(line) / pageSize; //Convert the line read to a number and divide it by pageSize.

        //Search if the number is currently in a page frame
        unsigned found = 0;
        for(int i = 0; i < physPages; i++)
        {
            if(page[i] == number)
            {
                found = 1;
                lastAccessed[i] = accessID; //Set the element found to the most recently accessed.
                break;
            }
            else if(lastAccessed[i] < lastAccessed[least]) //Determines what the least accessed element is.
            {
                least = i;
            }
        }

        if(found == 0) //Number wasn't found in any of the page frames.
        {
            page[least] = number; //Replace the least accessed element with the new number.
            lastAccessed[least] = accessID; //Set the element changed to be the most recently accessed.
            pageFaults++; //Increment number of page faults.
        }

    }

    fclose(file); //Close the file.

    printf("Number of page faults = %d\n", pageFaults);

    return 0;
}