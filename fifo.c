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
    int first = 0; //Value to keep track of the earliest added element.

    while (fgets(line, sizeof(line), file)) //Reads each line of the file and stores it in "line".
    {
        int number = atoi(line) / pageSize; //Convert the line read to a number and divide it by pageSize.

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

        if(found == 0) //Number wasn't found in any of the page frames.
        {
            page[first] = number; //Replace the earliest element with the new number.
            pageFaults++; //Increment number of page faults.
            first = (first + 1) % physPages; //Move to the position of the next earliest element in a circular manner.
        }

    }

    fclose(file); //Close the file.

    printf("Number of page faults = %d\n", pageFaults);

    return 0;
}