/*
Functions made for the first project of File Organization discipline taken at University of Sao Paulo - Sao Carlos, Brazil

@Authors: João Pedro Almeida Santos Secundino 10692054
		  Giovani Decico Lucafó 10288779
		  
*/
#include "listaOrdenada.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#ifndef FILE_MANAGEMENT
#define FILE_MANAGEMENT 
#define ENTRY_SIZE 10003
#define FILE_NAME_SIZE 30
#define MAX_BUFFER 500
#define DESCRIPTION_SIZE 40
#define PHONE_SIZE 15
#define HEADER_SIZE 214
#define PAGE_SIZE 32000
#define REMOVIDO_TAG_SIZE 1
#define ID_SERVIDOR_SIZE 4
#define ENC_LISTA_SIZE 8
#define SAL_SERV_SIZE 8
#define TEL_SERV_SIZE 14
#define IND_TAM_REG 4
#define IND_TAM 4
#define TAG_SIZE 1
#define NUM_FIELDS 5
#define END_STRING 1
#define METADATA 13

/**
Union made to make third functionality easier to implement. 
It saves the five possible specific inputs of this functionality, one at time.
*/
typedef union _Options{
	int id;
	double salary;
	char phone[PHONE_SIZE];
	char name[MAX_BUFFER];
	char post[MAX_BUFFER];

} Options;

typedef enum _Fields{
	idServidor,
	salarioServidor,
	telefoneServidor,
	nomeServidor,
	cargoServidor
} Fields;

/**
Struct made to simplify functionality 4 and increase its performance
*/
typedef struct _DataToRemove{
	char field[MAX_BUFFER];
	Options o;
	int option;
} DataToRemove;


/**
Struct made to represend the HeaderRegister.
It contains the metadata of the file.
*/
typedef struct _HeaderRegister{
	char status;
	long long int topoLista;
	char tagCampo1;
	char desCampo1[DESCRIPTION_SIZE];
	char tagCampo2;
	char desCampo2[DESCRIPTION_SIZE];
	char tagCampo3;
	char desCampo3[DESCRIPTION_SIZE];
	char tagCampo4;
	char desCampo4[DESCRIPTION_SIZE];
	char tagCampo5;
	char desCampo5[DESCRIPTION_SIZE];
} HeaderRegister;

/**
Struct made to represent a data register.
It contains information about the employees and metadata about he register.
*/
typedef struct _EmployeeRegister{
	char removido;
	int tamanhoRegistro;
	long long int encadeamentoLista;
	int idServidor;
	double salarioServidor;
	char telefoneServidor[PHONE_SIZE];
	int indicadorTamanhoNome;
	char tagCampo4;
	char *nomeServidor;
	int indicadorTamanhoCargo;
	char tagCampo5;
	char *cargoServidor;
} EmployeeRegister;

/**
Struct made to represent the main contents of a file
*/
typedef struct _FileRegister{
	HeaderRegister *header;
	FILE *filePointer;
	long long int lastRegisterByteOffSet;
}FileRegister;

/**
 * Struct made to save, in RAM, registers from a file
*/
typedef struct _employeeArray{
	EmployeeRegister **array;
	int totalSize;
	int actualSize;
}EmployeeArray;

/**
Replaces oldChar by newChar in the specified string inplace.
@Arguments:
	- string: pointer to a string to which you want to change the characters
	- oldChar: char to be substituted
	- newChar: substitute char
@Return:
	none
*/
void replace(char* string, char oldChar, char newChar);

/**
Fills the specified buffer with sz newChars inplace
@Arguments:
	- buffer: pointer to a string to be filled
	- newChar: specified char that will fill the buffer
	- sz: number of char to put in the buffer
@Return:
	none
*/
void fillWithChar(char *buffer, char newChar, int sz);

/**
Fills a file page with char c by size inplace
@Arguments:
	- page: pointer to a file to be filled
	- c: char to fill the page
	- size: number of chars to be put in the page
@Return:
	none 
*/
void putCharOnPage	(FILE *page, char c, int size);

/**
Returns the ceil of a double.
@Arguments:
	- num: number to be rounded up
@Return:
	largest interger nearest to num
*/
int myCeil(double num);

/**
Print n blank spaces on the screen
*/
void printBlank(int n);

/**
Read the reader from csv and writes it in the binary file
@Arguments:
	- csv: pointer to csv file
	- binFile: pointer to a binary file
@Return:
	none
*/
void writeHeader(FILE *csv, FILE *binFile);

/**
Calculates the size of a register, given its variable fields
@Arguments:
	- nomeServidor: pointer to a string containing the employee's name
	- cargoServidor: pointer to a string containing the employee's post
@Return:
	The size of the register
*/
int findRegisterSizeByFields( char *nomeServidor, char *cargoServidor);

/**
Calculates the real size (without the end-of-page trash) of a register given a EmployeeRegister
@Arguments:
	- e: register which will gave its size calculated
@Return:
	Register's real size
*/
int findRegisterSize(EmployeeRegister *e);

/**
Adds the employee in the binary file
@Arguments:
	- regSize: size of the register
	- e: employee register containing the employee's info
@Return:
	none
*/
void addEmployeeInFile(FILE *file, int regSize, EmployeeRegister *e);

/**
Allocates a Header register
@Arguments:
	none
@Return:
	pointer to a new HeaderRegister
*/
HeaderRegister* createHeaderRegister();

/**
Deallocates a HeaderRegister
@Arguments:
	- h: pointer to a HeaderRegister to be deallocated
@Return:
	none
*/
void destroyHeaderRegister(HeaderRegister *h);

/**
Allocates an EmployeeRegister
@Arguments:
	none
@Return:
	pointer to a new EmployeeRegister
*/
EmployeeRegister* createEmployeeRegister();

/**
Deallocates a EmployeeRegister
@Arguments:
	- e: pointer to a EmployeeRegister to be deallocated
@Return:
*/
void destroyEmployeeRegister(EmployeeRegister *e);

/**
Reads a header metadata from a binary file
@Arguments:
	-binFile: pointer to a binary file
	-h: pointer to a HeaderRegister
@Return:
	none
*/
void readHeaderPage(FILE *binFile, HeaderRegister *h);

/**
Reads the information about an employee from a binary file
@Arguments:
	-binFile: pointer to a binary file
	-h: pointer to a EmployeeRegister
@Return:
*/
int readPerson(FILE *binFile, EmployeeRegister *e);

/**
Used on third functionality
Takes a string and returns the required option in a int
@Arguments:
	- option: pointer to a string containing the field to be searched
@Return:
	the required option
*/
int returnOptionByString(char *option);

/**
Takes the metadata from header and, based on it, prints on the screen the employee's information
@Arguments:
	- h: pointer to a HeaderRegister
	- e: pointer to a EmployeeRegister
@Return:
	none
*/
void printEmployeebyHeader(HeaderRegister *h, EmployeeRegister *e);

/**
Search for employees based on the field indicated by option an described in o.
@Arguments:
	- binFile: pointer to a binary file
	- o: union containing the specified field content
	- option: indicates the required field to be searched
@Return:
	none
*/
void readEmployeesFromFile(FILE* binFile, Options o, int option);

/**
Reads the file header and allocates a file register
@Arguments:
	- file: pointer to an file
@Return:
	pointer to a file register

*/
FileRegister *createFileRegister(FILE *file);

/**
Deallocates a file register
@Arguments:
	-f : pointer to a file
@Return:
	none
*/
void destroyFileRegister(FileRegister *f);

/**
Mark a register as removed in file and adds it to the removedList
@Arguments:
	- fileRegister: pointer to a file register
	- removedList: list containing the removed registers from file
	- o: union containing the specified field content
	- option: indicates the required field to be searched
@Return:
	none
*/
void virtualRemoveEmployee(FileRegister *fileRegister, LIST *removedList, Options o, int option);

/**
Fills the removed registers in removedList with '@' in file
@Arguments:
	- fileRegister: pointer to a file register
	- removedList: list containing the removed registers from file
@Return:
	none
*/
void realRemoveEmployee(FileRegister *fileRegister, LIST *removedList );

/**
Transcribes, to the file, a the list of removed registers 
@Arguments:
	- fileRegister: pointer to a file register
	- removedList: list containing the removed registers from file
@Return:
	none
*/
void updateRemovedListOnFile(FileRegister *fileRegister, LIST *removedList);

/**
Transcribes, to the file, a the registers in removedList that were replaced (marked with '-') 
@Arguments:
	- fileRegister: pointer to a file register
	- removedList: list containing the removed registers from file
@Return:
	none
*/
void addReplacedRegistersOnFile(FileRegister *fileRegister, LIST *removedList);

/**
Check if a register has one specific property
@Arguments:
	- e: register to be checked
	- o: union containing the specified field content
	- option: indicates the required field to be compared (name, post...)
@Return:
	1 - it has the specified characteristic
	0 - otherwise
*/
int hasCharacteristic(EmployeeRegister *e, Options o, int option );

/**
Insert a register in the file's end
@Arguments:
	- e: pointer to employee register that will be inserted
	- fileRegister: pointer to a file register
@Return:
	none
*/
void insertEndFile(FileRegister *fileRegister, EmployeeRegister *e);

/**
Prints the file content as hexadecimal in stdout
@Arguments:
	- pointer to file to be printed
@Return:
	none
*/
void binarioNaTela1(FILE *ponteiroArquivoBinario);

/**
Fill the register with the content read from stdin
@Arguments:
	- e: pointer to employee register that will be filled
@Return:
	none
*/
void readEmployeeFromInput(EmployeeRegister *e);

/**
Copies the content of font to destiny 
*/
void copyRegisterContent(EmployeeRegister *destiny, EmployeeRegister *font);

/**
Brings the removed list from file to RAM in  removedList
@Arguments:
	- fileRegister: pointer to a file register
	- removedList: list containing the removed registers from file
@Return:
	none
*/
void fileToRemovedList(FileRegister *fileRegister, LIST *removedList);

/**
Changes the file status
@Arguments:
	- fileRegister: pointer to a file register containing a pointer to the file that will pass by a status change
	- c: new status
@Return:
	none
*/
void changeFileStatus(FileRegister *fileRegister, char c);

/**
Check if a file is consistent
@Arguments:
	- h: pointer to a file HeaderRegister
@Return:

*/
int isConsistent(HeaderRegister *h);

/**
Reads content from stdin
@Arguments:
	- str: pointer to a string that will receive the read content
@Return:
	none
*/
void scan_quote_string(char *str);

/**
Search by an employee that has one specific characteristic
@Arguments:
	- fileRegister: pointer to a file register
	- o: union containing the specified field content
	- option: indicates the required field to be compared (name, post...)
@Return:
	a LIST containing all occurences of this characteristic in file
	NULL if this characteristic doesn't appears in the list
*/
LIST *findEmployees(FileRegister *fileRegister, Options o, int option);

/**
Tries to update, in file, a specific field in a register
@Arguments:
	- fileRegister: pointer to a file register
	- o: union containing the specified field content to be updated
	- option: indicates the required field to be updated (name, post...)
@Return:
	1 - the register was succesfully updated 
	0 - otherwise
*/
int updateRegister(FileRegister *fileRegister, NODE *n , Options o, int option);

/**
Tries to change a name of a employee in file
@Arguments:
	- fileRegister: pointer to a file register
	- e: pointer to employee register that will be updated
	- regOffSet: byte offset of the employee in register
	- name: new name
@Return:
	1 - the register was succesfully updated 
	0 - otherwise
*/
int changeEmployeeNameOnFile(FileRegister *fileRegister, EmployeeRegister *e, long long int regOffSet, char *name);

/**
Tries to change a post of a employee in file
@Arguments:
	- fileRegister: pointer to a file register
	- e: pointer to employee register that will be updated
	- regOffSet: byte offset of the employee in file
	- name: new name
@Return:
	1 - the register was succesfully updated 
	0 - otherwise
*/
int changeEmployeePostOnFile(FileRegister *fileRegister, EmployeeRegister *e, long long int regOffSet, char *post);

/**
Fills one employee register with '@' in file
@Arguments:
	- fileRegister: pointer to a file register
	- e: pointer to employee register that will be removed
	- regOffSet: byte offset of the employee in file
@Return:
	none
*/
void removeEmployeeOnFile(FileRegister *fileRegister,EmployeeRegister *e, long long int regOffSet);

/**
Change a EmployeeRegister's post
@Arguments:
	- e: pointer to employee register that will be updated
	- post: new post
@Return:
	none
*/
void changeEmployeePost(EmployeeRegister *e, char *post);

/**
Change a EmployeeRegister's name
@Arguments:
	- e: pointer to employee register that will be updated
	- name: new post
@Return:
	none
*/
void changeEmployeeName(EmployeeRegister *e, char *name);

/**
Adds a new register in file
@Arguments:
	- fileRegister: pointer to a file register
	- removedList: pointer to a that ontains the file removed registers
	- e: pointer to employee register that will be updated
@Return:
	none
*/
void addNewRegisterInFile(FileRegister *fileRegister, LIST *removedList, EmployeeRegister *e);

/**
Search for the last element of the whole file and save its byteoffset in fileRegister
@Arguments:
	- fileRegister: pointer to a file register
	- pageSize: the size of one file page
@Return:
	none
*/
void findLastRegister(FileRegister *fileRegister, long long int pageSize);

/**
Find the next element not replaced in list
@Arguments:
	- it: pointer to a list node
@Return:
	- NULL: if the next not replaced doesn't exist
	- a pointer to the next element not replaced
*/
NODE* findNextNotReplaced(NODE *it);

/**
Find the next element that was replaced in list
@Arguments:
	- it: pointer to a list node
@Return:
	- NULL: if the next replaced doesn't exist
	- a pointer to the next element not replaced
*/
NODE * findNextReplaced(NODE *it);

/**
 * Passes all the registers in a file to an array of registers in RAM 
 * @Arguments:
 * 		- binFile: pointer to a file that will have its register passed to RAM
 * 		- employeeArray: pointer to an EmployeeArray that will receive the registers
 * @Return:
 * 		none, but all the files' registers will be in employeeArray
*/
void fileToRAM(FILE *binFile, EmployeeArray *employeeArray);

/**
 * Sort the registers in employeeArray by its ids
 * @Arguments:
 * 		- employeeArray: pointer to an EmployeeArray that contains all the register to be sorted
 * 		- beg: first array's position (0 based)
 * 		- end: end array's position (0 based | arraySize - 1)
 * @None:
 * 		none
*/
void mergeSortById(EmployeeArray *employeeArray, int beg, int end);

/**
 * Passes all the registers in employeeArray to a file
 * @Arguments:
 * 		- fileRegister: fileRegister of the file that will receive the registers
 * 		- employeeArray: pointer to an EmployeeArray that will contain the register to be recorded in file
 * @Return:
 * 		none, but the file indicated in fileRegister will contain all the registers of employeeArray
*/
void RAMToFile(FileRegister *fileRegister, EmployeeArray *employeeArray);

/**
 * Deallocates an EmployeeArray
 * @Arguments:
 * 		- employeeArray: pointer to an employeeArray
 * @Return:
 * 		none
*/
void destroyEmployeeArray(EmployeeArray *employeeArray);

/**
 * Allocates an EmployeeArray
 * @Arguments
 * 		-size: size of the array
 * @Return:
 * 		pointer to an initialized EmployeeArray if it was succesful
 * 		NULL otherwise
*/
EmployeeArray *createEmployeeArray(int size);

/**
 * Takes a headerPage in RAM and writes it in a file
 * @Arguments
 * 		- file: file that will receive the header page
 * 		- h: pointer to a headerRegister
 * @Return:
 * 		none 	
*/
void writeHeaderFromRam(FILE* file, HeaderRegister *h);

#endif