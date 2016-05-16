#include "stdafx.h"
#include "optlist.h"
#include "arcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *RemovePath(char *fullPath);
int main(int argc, char *argv[])
{
    option_t *optList, *thisOpt;
    FILE *inFile, *outFile; /* input & output files */
    char encode;            /* encode/decode */
    char staticModel;       /* static/adaptive model*/

    /* initialize data */
    inFile = NULL;
    outFile = NULL;
    encode = TRUE;
    staticModel = TRUE;

    /* parse command line */
    optList = GetOptList(argc, argv, "acdi:o:h?");
    thisOpt = optList;

    while (thisOpt != NULL)
    {
        switch(thisOpt->option)
        {
            case 'a':       /* adaptive model vs. static */
                staticModel = FALSE;
                break;

            case 'c':       /* compression mode */
                encode = TRUE;
                break;

            case 'd':       /* decompression mode */
                encode = FALSE;
                break;

            case 'i':       /* input file name */
                if (inFile != NULL)
                {
                    fprintf(stderr, "Multiple input files not allowed.\n");
                    fclose(inFile);

                    if (outFile != NULL)
                    {
                        fclose(outFile);
                    }

                    FreeOptList(optList);
                    exit(EXIT_FAILURE);
                }
                else if ((inFile = fopen(thisOpt->argument, "rb")) == NULL)
                {
                    perror("Opening Input File");

                    if (outFile != NULL)
                    {
                        fclose(outFile);
                    }

                    FreeOptList(optList);
                    exit(EXIT_FAILURE);
                }

                break;

            case 'o':       /* output file name */
                if (outFile != NULL)
                {
                    fprintf(stderr, "Multiple output files not allowed.\n");
                    fclose(outFile);

                    if (inFile != NULL)
                    {
                        fclose(inFile);
                    }

                    FreeOptList(optList);
                    exit(EXIT_FAILURE);
                }
                else if ((outFile = fopen(thisOpt->argument, "wb")) == NULL)
                {
                    perror("Opening Output File");

                    if (inFile != NULL)
                    {
                        fclose(inFile);
                    }

                    FreeOptList(optList);
                    exit(EXIT_FAILURE);
                }

                break;

            case 'h':
            case '?':
                printf("Usage: %s <options>\n\n", RemovePath(argv[0]));
                printf("options:\n");
                printf("  -c : Encode input file to output file.\n");
                printf("  -d : Decode input file to output file.\n");
                printf("  -i <filename> : Name of input file.\n");
                printf("  -o <filename> : Name of output file.\n");
                printf("  -a : Use adaptive model instead of static.\n");
                printf("  -h | ?  : Print out command line options.\n\n");
                printf("Default: %s -c\n", RemovePath(argv[0]));

                FreeOptList(optList);
                return(EXIT_SUCCESS);
        }

        optList = thisOpt->next;
        free(thisOpt);
        thisOpt = optList;
    }

    /* validate command line */
    if (NULL == inFile)
    {
        fprintf(stderr, "Input file must be provided\n");
        fprintf(stderr, "Enter \"%s -?\" for help.\n", RemovePath(argv[0]));

        if (outFile != NULL)
        {
            fclose(outFile);
        }

        exit (EXIT_FAILURE);
    }
    else if (NULL == outFile)
    {
        fprintf(stderr, "Output file must be provided\n");
        fprintf(stderr, "Enter \"%s -?\" for help.\n", RemovePath(argv[0]));

        if (inFile != NULL)
        {
            fclose(inFile);
        }

        exit (EXIT_FAILURE);
    }

    /* we have valid parameters encode or decode */
    if (encode)
    {
        ArEncodeFile(inFile, outFile, staticModel);
    }
    else
    {
        ArDecodeFile(inFile, outFile, staticModel);
    }

    fclose(inFile);
    fclose(outFile);
    return EXIT_SUCCESS;
}

char *RemovePath(char *fullPath)
{
    int i;
    char *start, *tmp;                          /* start of file name */
    const char delim[3] = {'\\', '/', ':'};     /* path deliminators */

    start = fullPath;

    /* find the first character after all file path delimiters */
    for (i = 0; i < 3; i++)
    {
        tmp = strrchr(start, delim[i]);

        if (tmp != NULL)
        {
            start = tmp + 1;
        }
    }

    return start;
}
