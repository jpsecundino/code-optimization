/*
Functions made to manipulate index files for projects of File Organization discipline taken at University of Sao Paulo - Sao Carlos, Brazil

@Authors: João Pedro Almeida Santos Secundino 10692054
		  Giovani Decico Lucafó 10288779
		  
*/
#ifndef INDEX_FILE_MANAGEMENT
#define INDEX_FILE_MANAGEMENT
#include "fileManagement.h"
#define SIZE_CHAVE_BUSCA 120
#define SIZE_NRO_REG 4
#define NUM_REG_OFFSET 1
#define MAX_BUFFER_SIZE 250
#define INDEX_PAGE_SIZE 32000
#define STATUS_TAG_SIZE 1
#define END_STRING_TOKEN 1

typedef struct _indexFileHeader{
    char status;
    int nroRegistros;
}IndexHeader;

typedef struct _indexRegister{
    char chaveBusca[SIZE_CHAVE_BUSCA];
    long long int byteOffset;
    char removed;
}IndexRegister;

typedef struct _indexRegisterArray{
    IndexRegister *fileRegisters;
    int actualSize;
    int totalSize;
}IndexRegisterArray;

typedef struct _indexFileRegister{
   IndexHeader fileHeader;
   IndexRegisterArray *indexArray;
   FILE *filePointer;
}IndexFileRegister;

/**
 * @brief  allocates a register array
 * @note the array need to be deallocated after use (see destroyIndexRegisterArray)  
 * @param  size: size of array if index registers 
 * @retval pointer to a recent allocated IndexRegisterArray or null, if doesn't have enough memory
 */
IndexRegisterArray *createIndexRegisterArray( int size );

/**
 * @brief  Realoc a indexRegisterArray 
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @retval None
 */
void reallocIndexRegisterArray(IndexFileRegister *indexFileRegister);

/**
 * @brief  Deallocates an IndexRegisterArray
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @retval None
 */
void destroyIndexRegisterArray(IndexFileRegister *indexFileRegister);

/**
 * @brief  Reads a indexHeaderRegister from an existent Index File
 * @param  *indexFile: pointer to an index file 
 * @retval an indexHeader
 */
IndexHeader readIndexHeaderFromIndexFile(FILE *indexFile);

/**
 * @brief Writes an indexHeader in an IndexFile 
 * @note  If there was another header page, it will be overwritten 
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @retval None
 */
void writeIndexHeader(IndexFileRegister *indexFileRegister);

/**
 * @brief  Reads a index register from an index file
 * @note   the read register will be placed in indexRegister
 * @param  *indexFile: pointer to an Index FIile
 * @param  *indexRegister: pointer to an indexRegister that will receive the register read from file 
 * @retval 0 if it was unccessfull read, any number otherwise
 */
int readIndexRegisterFromIndexFile(FILE *indexFile, IndexRegister *indexRegister);

/**
 * @brief reads all registers from index File and puts it in an IndexRegisterArray in indexFileRegister
 * @note   all the elements read can be accessed in indexFileRegister
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file  
 * @retval number of file pages accessed
 */
int readIndexesFromIndexFile(IndexFileRegister *indexFileRegister);

/**
 * @brief  Writes an indexFile on disk
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file  
 * @retval None
 */
void writeIndexFile(IndexFileRegister *indexFileRegister);

/**
 * @brief  Writes only one register on indexFile
 * @param  *indexFile: pointer to an index file 
 * @param  ir: register to be written
 * @retval 0 is it was usuccessful, any number othwerwise
 */
int writeIndexRegisterInIndexFile(FILE *indexFile, IndexRegister ir);

/**
 * @brief  Creates an index file in RAM from an employee file in DISK
 * @note   all the index File info will be placed in indexFileRegister
 * @param  *employeeFile: pointer to an file full of employee information
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @retval None
 */
void makeIndexFromEmployeeFile(FILE *employeeFile, IndexFileRegister *indexFileRegister);

/**
 * @brief Compares two index registers by its names
 * @note used in sortIndexes
 * @param  *a: first register
 * @param  *b: second register
 * @retval if a <= b (lexicographically), return -1. 1 otherwise 
 */
int compareIndexRegisters(const void *a, const void *b);

/**
 * @brief  Sort an index register array
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @retval None
 */
void sortIndexes(IndexFileRegister *indexFileRegister);

/**
 * @brief  initializes an indexFileRegister
 * @param  *indexFile: pointer to an index file
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @retval 1 if it was successful initialized. 0 otherwise
 */
int initIndexFileRegister(FILE *indexFile, IndexFileRegister *indexFileRegister);

/**
 * @brief  Changes an Index File status
 * @note   
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @param  newStatus: char that contains the new status of file ( '0' or '1', for example)
 * @retval 0 in unsuccess. Any number otherwise
 */
int changeIndexFileStatus(IndexFileRegister *indexFileRegister, char newStatus);

/**
 * @brief  Change the number of register of an Register Array
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @retval None
 */
void setNumberOfRegisters(IndexFileRegister *indexFileRegister);

/**
 * @brief  Reads an existing Index File from DISK
 * @note   The read content is placed in
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @retval number of pages accessed
 */
int readIndexFile(IndexFileRegister *indexFileRegister);

/**
 * @brief  binary search an array of Index Registers
 * @param  *array: array of indexRegisters that contain all the records of index file 
 * @param  beg: min interval to query 
 * @param  size: actual size of array
 * @param  *key: string that contains the key to be searched in array
 * @retval the searched element (or the begining of a block of repeated elements) position.
 *         -1 if the element doesn't exists
 */
int indexBinarySearch(IndexRegister *array, int beg, int size, char *key);

/**
 * @brief  Check if an Index File is consistent
 * @param  *header: pointer to an initialized index header
 * @retval 1 if it is. 0 otherwise
 */
int isIndexConsistent(IndexHeader *header );

/**
 * @brief  
 * @note   
 * @param  *array: array of indexRegisters that contain all the records of index file 
 * @param  initialPos: first position to be searched for 
 * @param  *employeeName: string that contains an employee name to be searched for
 * @retval 
 */
int findRepeatedNamesBlockBegin(IndexRegister *array,int initialPos, char *employeeName);

/**
 * @brief  Removes an element (or a set of elements) from an Index Registers Array
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @param  *employeeName: string that contains an employee name to be searched for
 * @retval None
 */
void removeFromIndexFile(IndexFileRegister *indexFileRegister, char *employeeName );

/**
 * @brief  Change the number of register field of an Index File
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @param  numReg: new field value
 * @retval None
 */
void changeIndexFileNumReg(IndexFileRegister *indexFileRegister, int numReg);

/**
 * @brief  Adds a new Index Register at the end of an IndexRegisterArray
 * @param  *indexFileRegister: pointer to a index file register that contains a pointer to an index file 
 * @param  *employeeName: string that contains an employee name to be searched for
 * @param  byteOffset: employee's byte offset
 * @retval None
 */
void addEmployeeInIndexArrayEnd(IndexFileRegister *indexFileRegister, char *employeeName , long long int byteOffset);
#endif