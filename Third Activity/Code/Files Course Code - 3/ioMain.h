/*
File made to simplify the main function visualization

@Authors: João Pedro Almeida Santos Secundino 10692054
		  Giovani Decico Lucafó 10288779
*/
#include <stdio.h>
#include <stdlib.h>
#include "fileManagement.h"
#ifndef IO_PROG_TRAB2
#define IO_PROG_TRAB2
#define MERGE 0
#define MATCH 1
/**
Reads a specific register field from stdin
@Arguments:
    - fieldName: string cointaining the name of the field to be read
    - o: union that will receive the content read by stdin
@Return:
    none
*/
int readSpecificField(char *fieldName, Options *o);

/**
this function was made to encapsulate the program's first functionality
It reads the csv file, builds the Header page, and creates the binary file acording to what it read
@Arguments:
	- csv: pointer to a CSV file
	- binFile: pointer to an output binary file
@Return:
	Number of pages read
*/
int firstFunctionality(FILE *csv, FILE *binFile);

/**
This function was made to encapsulate the program's second functionality
@Arguments:
	- binFile - pointer to a binary file
@Return:
	none
*/
void secondFuncionality( FILE *binFile );

/**
This function was made to encapsulate the program's third functionality
@Arguments:
	- binFile: pointer to a binary file
	- optionString:pointer to a string containing the fieldName to be searched for
@Return:
	none
*/
void thirdFunctionality(FILE *binFile, char *optionString);

/**
This function was made to encapsulate the program's fourth functionality
@Arguments:
	- fileRegister: pointer to a fileRegister
	- removedList: pointer to a list containing all the removed register from the file
    - data: array containing all queries to be made
    - dataToRemoveSize: size of data (number of queries)
@Return:
	none
*/
void fourthFunctionality(FileRegister *fileRegister, LIST *removedList, DataToRemove *data,int dataToRemoveSize);

/**
This function was made to encapsulate the program's fifth functionality
@Arguments:
	- fileRegister: pointer to a fileRegister
    - removedList: pointer to a list containing all the removed register from the file
@Return:
	none
*/
void fifthFunctionality(FileRegister *fileRegister, LIST *removedList);

/**
This function was made to encapsulate the program's sixth functionality
@Arguments:
	- fileRegister: pointer to a fileRegister
    - removedList: pointer to a list containing all the removed register from the file
@Return:
	none
*/
void sixthFunctionality(FileRegister *fileRegister, LIST *removedList);

/**
This function was made to encapsulate the program's eighth functionality: merge registers in two files by its ids
@Arguments:
	- firstFileRegister: pointer to the first file to be merged
	- secondFileRegister: pointer to the second file to be merged
	- outFile: pointer to a new created file that will contain the result of the two merged files
@Return:
	none
*/
void eighthFunctionality(FileRegister *firstFileRegister, FileRegister *secondFileRegister, FileRegister *outFileRegister);

/**
This function was made to encapsulate the program's ninth functionality: match registers in the two files by its ids
@Arguments:
	- firstFileRegister: pointer to the first file to be matched
	- secondFileRegister: pointer to the second file to be matched
	- outFile: pointer to a new created file that will contain the result of the two matched files
@Return:
	none
*/
void ninthFunctionality(FileRegister *firstFileRegister, FileRegister *secondFileRegister, FileRegister *outFileRegister);


/**
This function was made to prepare and clean all the needs of first functionality
@Arguments:
	- csv: pointer to a csv file
@Return:
	none
*/
void setUnsetFunc1(FILE *csv);

/**
This function was made to prepare and clean all the needs of third functionality
@Arguments:
	- binFile: pointer to a bin file
@Return:
	none
*/
void setUnsetFunc3(FILE *binFile);


/**
This function was made to prepare and clean all the needs of fourth functionality
@Arguments:
	- binFile: pointer to a bin file
@Return:
	none
*/
void setUnsetFunc4(FILE * binFile);

/**
This function was made to prepare and clean all the needs of fifth functionality
@Arguments:
	- binFile: pointer to a bin file
@Return:
	none
*/
void setUnsetFunc5(FILE *binFile);

/**
This function was made to prepare and clean all the needs of sixth functionality
@Arguments:
	- binFile: pointer to a bin file
@Return:
	none
*/
void setUnsetFunc6(FILE *binFile);

/**
This function was made to prepare and clean all the needs of seventh functionality
@Arguments:
	- binFile: pointer to a bin file
@Return:
	none
*/
void setUnsetFunc7(FILE *binFile);

/**
This function was made to prepare and clean all the needs of eighth and ninth functionalities
@Arguments:
	- binFile: pointer to a bin file
	- mode: indicates what functionality to use
		MERGE: will merge the files(will cal the eight functionality)
		MATCH: will match the registers in file (will call the ninth functionality)
@Return:
	none
*/
void setUnsetFuncs8And9(FILE *firstBinFile, const int mode);

#endif 