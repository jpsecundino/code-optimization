/**
Program made for the first project of File Organization discipline taken at University of Sao Paulo - Sao Carlos, Brazil.

The purpose of this program is to train the concepts of data organization in secondary memory
according to the theoretical concepts of Registers and Fields learned in class.

For the sake of clarity, many of the functions used in this project are declared in a second and third files, named "fileManagement.h" and "ioMain.h".

@Authors: João Pedro Almeida Santos Secundino 10692054
		  Giovani Decico Lucafó 10288779
*/

#include<stdio.h>
#include<string.h>
#include "ioMain.h"

/*
This is the main function. Here we take the inputs and set/unset the necessary variables to the perfect
functioning of our program. 
*/
int main(){
	int option;
	char fileName[FILE_NAME_SIZE];

	//reading the common input
	scanf("%d %s", &option, fileName);

	//opening the file and look for errors
	FILE *binFile =  fopen(fileName,"r+b");
	if(binFile == NULL){
		printf("Falha no processamento do arquivo.");
		return 0;
	}

	switch(option){
		case 1:
			setUnsetFunc1(binFile);
			break;
		case 2:
			secondFuncionality(binFile);
			break;
		case 3:
			setUnsetFunc3(binFile);
			break;
		case 4:
			setUnsetFunc4(binFile);
			break;
		case 5:
			setUnsetFunc5(binFile);
			break;
		case 6:
			setUnsetFunc6(binFile);
			break;
		case 7:
			setUnsetFunc7(binFile);
			break;
		case 8:
			setUnsetFuncs8And9(binFile, MERGE);
			break;
		case 9:
			setUnsetFuncs8And9(binFile, MATCH);
			break;
	}

	fclose(binFile);
	return 0;
}