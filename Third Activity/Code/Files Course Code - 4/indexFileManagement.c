/*
Functions made to manipulate index files for projects of File Organization discipline taken at University of Sao Paulo - Sao Carlos, Brazil

@Authors: João Pedro Almeida Santos Secundino 10692054
		  Giovani Decico Lucafó 10288779
		  
*/

#include "indexFileManagement.h"

int initIndexFileRegister(FILE *indexFile, IndexFileRegister *indexFileRegister){
    //initializes the header 
    IndexHeader h = {'0', 0};//implicitly changing the file status to 0
    indexFileRegister->fileHeader = h;
    //associates an index file to indexFileRegister
    indexFileRegister->filePointer = indexFile;
    indexFileRegister->indexArray = createIndexRegisterArray(MAX_BUFFER_SIZE);

    if(indexFileRegister->indexArray == NULL ){
        return 0;
    }

    return 1;
}

IndexRegisterArray *createIndexRegisterArray( int size ){
    //malloc routine
    IndexRegisterArray *array = malloc(sizeof(IndexRegisterArray));
    if(array == NULL) return NULL;
    array->fileRegisters = malloc(sizeof(IndexRegister) * size);
    if(array ->fileRegisters == NULL) {
        free(array);
        return NULL;
    }
    array->actualSize = 0;
    array->totalSize = size;
    return array;
}

void reallocIndexRegisterArray(IndexFileRegister *indexFileRegister){
    //realloc routine
    IndexRegister *array = indexFileRegister->indexArray->fileRegisters;
    array = realloc(array, indexFileRegister->indexArray->actualSize * 5 * sizeof(IndexRegister));
    indexFileRegister->indexArray->fileRegisters = array;
    indexFileRegister->indexArray->totalSize *= 5;
}

void destroyIndexRegisterArray(IndexFileRegister *indexFileRegister){
    //dealloc routine
    free(indexFileRegister->indexArray->fileRegisters);
    free(indexFileRegister->indexArray);
}

IndexHeader readIndexHeaderFromIndexFile(FILE *indexFile){
    IndexHeader h;
    //goes to file begining
    fseek(indexFile, 0, SEEK_SET);
    //reads the fields
    fread(&h.status, sizeof(char), 1, indexFile);
    fread(&h.nroRegistros, sizeof(int), 1, indexFile);
    return h;
}

int readIndexRegisterFromIndexFile(FILE *file, IndexRegister *indexRegister){
    int tagSuccesfullRead =  0;
    //reads the fields
    tagSuccesfullRead += fread(&indexRegister->chaveBusca, sizeof(char), SIZE_CHAVE_BUSCA, file);
    tagSuccesfullRead += fread(&indexRegister->byteOffset, sizeof(long long int), 1, file);

    return tagSuccesfullRead;
}

/**
 *the index already exists in file, but not in ram
 */ 
int readIndexesFromIndexFile(IndexFileRegister * indexFileRegister){
    IndexRegister ir;
    FILE *file = indexFileRegister->filePointer;
    //goes to header page end
    fseek(file, INDEX_PAGE_SIZE, SEEK_SET );
    while(!feof(file)){
        if(readIndexRegisterFromIndexFile(file, &ir) ){
            addEmployeeInIndexArrayEnd(indexFileRegister, ir.chaveBusca, ir.byteOffset);
        }
    }

    return myCeil((double)ftell(file)/(double)INDEX_PAGE_SIZE) -1;
}

int readIndexFile(IndexFileRegister *indexFileRegister){
    //read the file's header
    indexFileRegister->fileHeader = readIndexHeaderFromIndexFile(indexFileRegister->filePointer);
    
    return readIndexesFromIndexFile(indexFileRegister) + 1;
}

/**
 * the index only exists in RAM
 */
void writeIndexFile(IndexFileRegister * indexFileRegister){
    IndexRegister ir;
    FILE *file = indexFileRegister->filePointer;
    //writes the header in disk
    writeIndexHeader(indexFileRegister);

    int numReg = 0;
    //iterates by the array, adding the register to file
    for (int i = 0; i < indexFileRegister->indexArray->actualSize; i++){
        ir = indexFileRegister->indexArray->fileRegisters[i];
        //check if the register was removed
        if(ir.removed != '*'){
            writeIndexRegisterInIndexFile(file, ir);
            numReg++;
        }
    }
    //update the new number of registers
    changeIndexFileNumReg(indexFileRegister, numReg);
}

int writeIndexRegisterInIndexFile(FILE *indexFile, IndexRegister ir){
    int flagSuccesful = 0;
    //write the info
    flagSuccesful += fwrite(&ir.chaveBusca, sizeof(char), SIZE_CHAVE_BUSCA, indexFile );
    flagSuccesful += fwrite(&ir.byteOffset, sizeof(long long int), 1, indexFile );
    return flagSuccesful;
}

void writeIndexHeader(IndexFileRegister *indexFileRegister){
    //goes to the file begining
    fseek(indexFileRegister->filePointer, 0, SEEK_SET);
    fwrite(&indexFileRegister->fileHeader.status, sizeof(char), 1, indexFileRegister->filePointer);
    fwrite(&indexFileRegister->fileHeader.nroRegistros, sizeof(int), 1, indexFileRegister->filePointer);
    putCharOnPage(indexFileRegister->filePointer, '@', INDEX_PAGE_SIZE - SIZE_NRO_REG - STATUS_TAG_SIZE);
}

/**
 * the index doesn't exist in any place
 */
void makeIndexFromEmployeeFile(FILE *employeeFile, IndexFileRegister *indexFileRegister){
    EmployeeRegister *e = createEmployeeRegister();
    long long int employeeByteOffset;
    //iterate by the employeeFile, reading all the register and addin they on the indexFileRegister
    fseek(employeeFile, INDEX_PAGE_SIZE, SEEK_SET);
    while(!feof(employeeFile)){
        employeeByteOffset = ftell(employeeFile);
        if(readPerson(employeeFile,e)){
            //check for a valid name and remotion conditions
            if(e->indicadorTamanhoNome != 0 && e->removido != '*'){
                addEmployeeInIndexArrayEnd(indexFileRegister, e->nomeServidor, employeeByteOffset);
            }
        }
    }

    //update the number of registers
    setNumberOfRegisters(indexFileRegister);
    
    //sort the indexes array
    sortIndexes(indexFileRegister);
    
    destroyEmployeeRegister(e);
    
}

int compareIndexRegisters(const void *a, const void *b){
    char *aString = (char *)a;
    char *bString = (char *)b;
    return strcmp(aString,bString) <= 0 ? -1 : 1;
}

void sortIndexes(IndexFileRegister *indexFileRegister){
    IndexRegister *array = indexFileRegister->indexArray->fileRegisters;
    int arraySize = indexFileRegister->indexArray->actualSize;
    qsort(array, arraySize, sizeof(IndexRegister), compareIndexRegisters );
}

int changeIndexFileStatus(IndexFileRegister *indexFileRegister, char newStatus){
    int flagSuccesful = 0;
    //go to file begining
    fseek(indexFileRegister->filePointer, 0 , SEEK_SET);
    //change status
    flagSuccesful += fwrite(&newStatus, sizeof(char), 1, indexFileRegister->filePointer);

    return flagSuccesful;
}

void setNumberOfRegisters(IndexFileRegister *indexFileRegister){
    indexFileRegister->fileHeader.nroRegistros = indexFileRegister->indexArray->actualSize;
}

int indexBinarySearch(IndexRegister *array, int inf, int sup , char *key){
    //common binary search
    if(inf > sup) return -1;

    int mid = ( inf + sup ) /2;

    int res = strcmp(array[mid].chaveBusca, key);
    
    if( res == 0 ) {
        return mid;   
    }else if( res < 0 ){
        return indexBinarySearch(array, mid + 1, sup, key);
    }else{
        return indexBinarySearch(array, inf, mid - 1, key);
    }
}

int isIndexConsistent(IndexHeader *header ){
    return header->status == '1' ? 1 : 0; 
}

int findRepeatedNamesBlockBegin(IndexRegister *array,int initialPos, char *employeeName){
    
    //find the beginning of repeated names block
	while(initialPos > 0 && strcmp(array[initialPos - 1].chaveBusca, employeeName) == 0 ){
		initialPos --;
	}
    //return the blocks' begining
    return initialPos;
}

void removeFromIndexFile(IndexFileRegister *indexFileRegister, char *employeeName ){
    IndexRegister *array = indexFileRegister->indexArray->fileRegisters;
    int arraySize = indexFileRegister->indexArray->actualSize;
    int pos = indexBinarySearch(array, 0, arraySize, employeeName);
    //if we didn't find any register that matches
    if(pos != -1){
        pos = findRepeatedNamesBlockBegin(array, pos, employeeName);
    }
    //logically remove all the registers that matches
    while(pos < arraySize && strcmp(array[pos].chaveBusca, employeeName) == 0){
        array[pos].removed = '*';			
        pos++;					
    }
}

void changeIndexFileNumReg(IndexFileRegister *indexFileRegister, int numReg){
    //goes to numReg field
    fseek(indexFileRegister->filePointer, NUM_REG_OFFSET, SEEK_SET );
    //change its values
    fwrite(&numReg, sizeof(int), 1, indexFileRegister->filePointer);
}

void addEmployeeInIndexArrayEnd(IndexFileRegister *indexFileRegister, char *employeeName, long long int employeeByteOffset){
    int arrayIt = 0,
    bufferSize = 0;
    char auxBuffer[MAX_BUFFER_SIZE] = {};
    
    //preparing the key
    strcpy(auxBuffer, employeeName);
    bufferSize = strlen(auxBuffer) + END_STRING_TOKEN;
    //filling it with '@'
    fillWithChar(auxBuffer + bufferSize, '@', SIZE_CHAVE_BUSCA - bufferSize);
    //realloc if necessary
    if(indexFileRegister->indexArray->actualSize == indexFileRegister->indexArray->totalSize){
        reallocIndexRegisterArray(indexFileRegister);
    }
    
    //add the info to array
    arrayIt = indexFileRegister->indexArray->actualSize;
    indexFileRegister->indexArray->fileRegisters[arrayIt].byteOffset = employeeByteOffset;
    indexFileRegister->indexArray->fileRegisters[arrayIt].removed = '-';
    memcpy(indexFileRegister->indexArray->fileRegisters[arrayIt].chaveBusca, auxBuffer, SIZE_CHAVE_BUSCA*sizeof(char));
    
    //update its size
    indexFileRegister->indexArray->actualSize++;
}
