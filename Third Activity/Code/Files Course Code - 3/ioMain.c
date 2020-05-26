/*
Functions made for the first project of File Organization discipline taken at University of Sao Paulo - Sao Carlos, Brazil

@Authors: João Pedro Almeida Santos Secundino 10692054
		  Giovani Decico Lucafó 10288779
*/
#include "ioMain.h"
#include "fileManagement.h"

int readSpecificField(char *field_name, Options *o){
	int auxInt;
	char auxBuffer[MAX_BUFFER];

	int option = returnOptionByString(field_name);

	switch(option){
		case 0:
			scanf("%d", &auxInt);
			o->id = auxInt;
			break;
		case 1:
			scan_quote_string(auxBuffer);
			if(!strlen(auxBuffer)){
				o->salary = -1;
			}else{
				o->salary = atof(auxBuffer);
			}
			break;
		case 2:
			scan_quote_string(auxBuffer);
			
			if(!strlen(auxBuffer)){
				memcpy(o->phone, "\0@@@@@@@@@@@@@", 14);	
			}else{
				strcpy(o->phone, auxBuffer);
			}
			break;
		case 3:
			scan_quote_string(auxBuffer);
			strcpy(o->name, auxBuffer);
			break;
		case 4:
			scan_quote_string(auxBuffer);
			strcpy(o->post, auxBuffer);
			break;
	}

	return option;
}



/**
this function was made to encapsulate the program's first functionality
It reads the csv file, builds the Header page, and creates the binary file acording to what it read
@Arguments:
	- csv: pointer to a CSV file
	- binFile: pointer to an output binary file
@Return:
	Number of pages read
*/
int firstFunctionality(FILE *csv, FILE *binFile){
	int pageBytes = PAGE_SIZE;
	int pageCounter = 0;
	int prevSize = 0;
	//reads the header
	writeHeader(csv, binFile);
	
	//while we didn't reach the the end of file
	while(!feof(csv)){
		int endFlagFscanf = -1;
		char *endflagFgets = NULL;

		//create a new employee register
		EmployeeRegister *e = createEmployeeRegister();

		//initializes telefoneServidor field
		fillWithChar(e->telefoneServidor, '@', 14);
		e->telefoneServidor[0] = '\0'; 

		//reading the fields
		endFlagFscanf &= fscanf(csv, "%d %*c", &(e->idServidor));
		endFlagFscanf &= fscanf(csv, "%lf %*c", &(e->salarioServidor));
		endFlagFscanf &= fscanf(csv, "%[^,]s", e->telefoneServidor);
		endFlagFscanf &= fscanf(csv, "%*c %[^,]s", e->nomeServidor);
		endFlagFscanf &= fscanf(csv, "%*c");
		endflagFgets = fgets(e->cargoServidor, MAX_BUFFER, csv);
	
		// if we didn't get the the end of file after the readings
		if(!(endFlagFscanf == -1 && endflagFgets == NULL)){

			//changing \n by \0
			e->cargoServidor[(int)strlen(e->cargoServidor) - 1] = '\0';

			//find the register size
			int regSize = findRegisterSizeByFields(e->nomeServidor, e->cargoServidor);
			int totalRegSize = regSize + REMOVIDO_TAG_SIZE + IND_TAM_REG;
			
			//if our register doesn't fit in the actual page
			if(pageBytes  < totalRegSize){
				//return to previous register's begining
				fseek(binFile, -(prevSize + 4), SEEK_CUR);
				
				//change its size for more to count the end-of-page trash ('@')
				prevSize+=pageBytes;
				fwrite(&prevSize, sizeof(int), 1, binFile);
				prevSize-=pageBytes;

				//go to the end of the previous register
				fseek(binFile, prevSize , SEEK_CUR);

				//fill the remaining space of the page with '@'s
				putCharOnPage(binFile, '@', pageBytes);
				//increase the page number by 1
				pageCounter++;
				//put in count the new register's size
				pageBytes = PAGE_SIZE - regSize - REMOVIDO_TAG_SIZE -IND_TAM_REG ;
			}else{
				pageBytes -= regSize + REMOVIDO_TAG_SIZE + IND_TAM_REG;
			}
			

			//add the new register in the page
			addEmployeeInFile(binFile, regSize, e);

			//if the eof is reached, stop
			if(feof(csv))
				break;

			prevSize = regSize;
			
			destroyEmployeeRegister(e);
		}
	}

	//change the header's status
	int status = '1';
	fseek(binFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, binFile);

	return pageCounter;
}


/**
This function was made to encapsulate the program's second functionality
@Arguments:
	binFile - pointer to a binary file
@Return:
	none
*/
void secondFuncionality( FILE *binFile ){
	
	int numPages;
	int bytesAccessedTotal = 0;
	HeaderRegister *h = createHeaderRegister();
	EmployeeRegister *e = createEmployeeRegister();
	int employeeFoundFlag = 0;
	//read the Header and put it's contents in HeaderRegister h
	readHeaderPage(binFile, h);

	//if we have an inconsistent file, abort
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.");
		return;
	}

	//while we don't reach the EOF
	while(!feof(binFile)){

		//if a Employee was read
		if(readPerson(binFile, e)){
			
			bytesAccessedTotal += e->tamanhoRegistro + REMOVIDO_TAG_SIZE + IND_TAM_REG;
			if(e->removido == '*'){
				continue;
			}
			//we found at least one employee
			employeeFoundFlag = 1;
			
			//treating null fields
			//each of the next five blocks are doing the same thing: if the field is null, doesn't print it/ print '@'s

			if(e->idServidor != -1){
				printf("%d", e->idServidor );
			}else{
				printBlank(sizeof(int) + 1);
			}

			if(e->salarioServidor != -1){
				printf(" %.2lf", e->salarioServidor);
			}else{
				printBlank(sizeof(double) + 1 );
			}

			if(strlen(e->telefoneServidor)){
				printf(" %s", e->telefoneServidor);
			}else{
				printBlank(PHONE_SIZE);
			}
			
			if(e->indicadorTamanhoNome != 0){
				printf(" %d %s", e->indicadorTamanhoNome, e->nomeServidor);
			}
			
			if(e->indicadorTamanhoCargo != 0){
				printf(" %d %s", e->indicadorTamanhoCargo, e->cargoServidor );
			}

			printf("\r\n");

			//refresh the total number of bytes accessed
		}	

	}

	destroyHeaderRegister(h);
	destroyEmployeeRegister(e);

	//if we found at least one employee
	if(employeeFoundFlag){
		//calculates the number os pages acessed, counting with the Header Page
		numPages = myCeil(((double) bytesAccessedTotal/ (double) PAGE_SIZE)) + 1;
		printf("Número de páginas de disco acessadas: %d\n", numPages );
	}else{//if not
		printf("Registro inexistente.");
	}
}

/**
This function was made to encapsulate the program's third functionality
@Arguments:
	binFile: pointer to a binary file
	optionString:pointer to a string containing the fieldName to be searched for
@Return:
	none
*/
void thirdFunctionality(FILE *binFile, char *optionString){
	//picks the option's index from string
	int option = returnOptionByString(optionString) ;

	if(option == -1){
		printf("Falha no processamento do arquivo.");
		return;
	}

	int auxInt;
	double auxDouble;
	char auxBuffer[MAX_BUFFER];
	//union described in fileManagement.h
	Options o;
	
	//according to the required option, we read the right field of the functionality's fourth argument
	switch(option){
		case 0:
			scanf("%d", &auxInt);
			o.id = auxInt;
			break;
		case 1:
			scanf("%lf", &auxDouble);
			o.salary = auxDouble;
			break;
		case 2:
			scanf("%*c %[^\r\n]", auxBuffer);
			strcpy(o.phone, auxBuffer);
			break;
		case 3:
			scanf("%*c %[^\r\n]", auxBuffer);
			strcpy(o.name, auxBuffer);
			break;
		case 4:
			scanf("%*c %[^\r\n]", auxBuffer);
			strcpy(o.post, auxBuffer);
			break;
	}

	//search by employee
	readEmployeesFromFile(binFile, o, option);

}

void fourthFunctionality(FileRegister *fileRegister, LIST *removedList, DataToRemove *data,int dataToRemoveSize){
	EmployeeRegister *e;
	long long int regByteOffSet;
	fseek(fileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	//iterates the registers in file
	
	e = createEmployeeRegister();
	
	while(!feof(fileRegister->filePointer)){
		
		regByteOffSet = ftell(fileRegister->filePointer);
		
		//reads the employee 
		if(readPerson(fileRegister->filePointer, e) && e->removido != '*'){
			
			//runs all queries to throught actual register
			for (int i = 0; i < dataToRemoveSize; i++){
				
				//if this register has the characteristic specified by the querie i:
				if(hasCharacteristic(e,data[i].o, data[i].option )){
					e->removido = '*';
			
					insertOrderedList(removedList, e, regByteOffSet, i);
					removeEmployeeOnFile(fileRegister,e,regByteOffSet);

					e = createEmployeeRegister();
		
					break;
				}
			}
		}
	}

	destroyEmployeeRegister(e);

}

void fifthFunctionality(FileRegister *fileRegister, LIST *removedList){
	EmployeeRegister *e = createEmployeeRegister();
	readEmployeeFromInput(e);
	addNewRegisterInFile(fileRegister, removedList, e);
	destroyEmployeeRegister(e);
}

void sixthFunctionality(FileRegister *fileRegister, LIST *removedList){
	char auxBuffer[MAX_BUFFER] = {};
	Options o;
	
	//reads the search field
	scanf("%s", auxBuffer);
	int option = readSpecificField(auxBuffer, &o);
	
	//return all registers with that characteristic
	LIST *found = findEmployees(fileRegister, o, option);
	
	//reads the update field
	scanf("%s", auxBuffer);
	option = readSpecificField(auxBuffer, &o);

	NODE *it = found->begin;
	while(it != NULL){
		if(updateRegister(fileRegister, it, o, option) == 0){
			
			EmployeeRegister *e = createEmployeeRegister();
			copyRegisterContent( e, it->employeeInfo.employeeRegister);			
			
			insertOrderedList(removedList, e, it->employeeInfo.byteOffSet, 0);
			removeEmployeeOnFile(fileRegister, it->employeeInfo.employeeRegister, it->employeeInfo.byteOffSet);

			if(option == nomeServidor){
				changeEmployeeName(it->employeeInfo.employeeRegister, o.name);
			}else{
				changeEmployeePost(it->employeeInfo.employeeRegister, o.post);
			}
			
			addNewRegisterInFile(fileRegister, removedList, it->employeeInfo.employeeRegister);
		}
		it = it->next;
	}

	destroyList(found);
	
}

void seventhFuncionality(FileRegister *fileRegisterIn, FileRegister *fileRegisterOut, EmployeeArray *employeeArray ){
	//passing all employees from file to RAM
	fileToRAM(fileRegisterIn->filePointer, employeeArray);

	//sorting the employees by id
	mergeSortById(employeeArray, 0, employeeArray->actualSize - 1);
	
	//passing all the employees from RAM to file
	RAMToFile(fileRegisterOut, employeeArray);
}

void eightFunctionality(FileRegister *firstFileRegister, FileRegister *secondFileRegister, FileRegister *outFileRegister){
	//setting the file pointer to correct places
	fseek(firstFileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	fseek(secondFileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	fseek(outFileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	EmployeeRegister *employeeOne = createEmployeeRegister(),
					 *employeeTwo = createEmployeeRegister();
	FILE *firstFile = firstFileRegister->filePointer,
		 *secondFile = secondFileRegister->filePointer;
	
	//merging the files
	readPerson(firstFile, employeeOne);
	readPerson(secondFile, employeeTwo);
	while(!feof(firstFile) && !feof(secondFile)){
			
			//calculating the real size of registers (discounting the end-of-page trash)
			employeeOne->tamanhoRegistro = findRegisterSizeByFields(employeeOne->nomeServidor, employeeOne->cargoServidor);
			employeeTwo->tamanhoRegistro = findRegisterSizeByFields(employeeTwo->nomeServidor, employeeTwo->cargoServidor);
			
			//merging by ids
			if(employeeOne->idServidor < employeeTwo->idServidor){
				insertEndFile(outFileRegister, employeeOne);
				readPerson(firstFile, employeeOne);
			}else if(employeeOne->idServidor > employeeTwo->idServidor){
				insertEndFile(outFileRegister, employeeTwo);
				readPerson(secondFile, employeeTwo);
			}else{
				insertEndFile(outFileRegister, employeeOne);
				readPerson(firstFile, employeeOne);
				readPerson(secondFile, employeeTwo);
			}
	}

	//complete the merge
	while(!feof(firstFile)){
		insertEndFile(outFileRegister,employeeOne);
		readPerson(firstFile, employeeOne);
	}

	while(!feof(secondFile)){
		insertEndFile(outFileRegister,employeeTwo);
		readPerson(firstFile, employeeTwo);
	}

	destroyEmployeeRegister(employeeOne);
	destroyEmployeeRegister(employeeTwo);
}

void ninthFunctionality(FileRegister *firstFileRegister, FileRegister *secondFileRegister, FileRegister *outFileRegister){
	//setting the file pointer to correct places
	fseek(firstFileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	fseek(secondFileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	fseek(outFileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	EmployeeRegister *employeeOne = createEmployeeRegister(),
					 *employeeTwo = createEmployeeRegister();
	FILE *firstFile = firstFileRegister->filePointer,
		 *secondFile = secondFileRegister->filePointer;
	
	//matching the files
	readPerson(firstFile, employeeOne);
	readPerson(secondFile, employeeTwo);
	while(!feof(firstFile) && !feof(secondFile)){
			
			//calculating the real size of registers (discounting the end-of-page trash)
			employeeOne->tamanhoRegistro = findRegisterSizeByFields(employeeOne->nomeServidor, employeeOne->cargoServidor);
			employeeTwo->tamanhoRegistro = findRegisterSizeByFields(employeeTwo->nomeServidor, employeeTwo->cargoServidor);
			
			if(employeeOne->idServidor < employeeTwo->idServidor){
				readPerson(firstFile, employeeOne);
			}else if(employeeOne->idServidor > employeeTwo->idServidor){
				readPerson(secondFile, employeeTwo);
			}else{
				insertEndFile(outFileRegister, employeeOne);
				readPerson(firstFile, employeeOne);
				readPerson(secondFile, employeeTwo);
			}
	}
	
	destroyEmployeeRegister(employeeOne);
	destroyEmployeeRegister(employeeTwo);
}
void setUnsetFunc1(FILE *csv){
	FILE *binFile;

	//opening binary out file and looking for errors
	binFile = fopen("arquivoTrab1.bin", "wb");
	if(binFile == NULL){
		fclose(csv);
		printf("Falha no carregamento do arquivo.");
		return;
	}
	
	//calling the function of the first functionality
	firstFunctionality(csv, binFile);
	printf("arquivoTrab1.bin");
	
	//closing the opened files
	fclose(binFile);
}


void setUnsetFunc3(FILE* binFile){
	char optionString[MAX_BUFFER] = {};
	//reading the specific input (third argument)
	scanf("%s", optionString);

	//calling the function of the third functionality
	thirdFunctionality(binFile, optionString);
}

void setUnsetFunc4(FILE * binFile){
	int n;
	FileRegister *fileRegister = createFileRegister(binFile);
	
	//check if the file is consistent
	if(!isConsistent(fileRegister->header)){
		printf("Falha no processamento do arquivo.");
		destroyFileRegister(fileRegister);
		return;
	}

	changeFileStatus(fileRegister, '0');
	
	LIST *removedList = createList();
	DataToRemove *data = (DataToRemove *)malloc(ENTRY_SIZE*sizeof(DataToRemove));
	
	//bring the removed list from file to RAM
	fileToRemovedList(fileRegister, removedList);
	
	scanf("%d", &n);
	
	//read all the queries an save it in data array
	for( int i = 0; i < n; i++ ){
		scanf("%s", data[i].field);
		readSpecificField(data[i].field, &data[i].o);
		data[i].option = returnOptionByString(data[i].field);
	}

	//search for and remove the employees
	fourthFunctionality(fileRegister, removedList, data, n);

	//after all remotions, update the new removedList in file
	updateRemovedListOnFile(fileRegister, removedList);
	
	changeFileStatus(fileRegister, '1');
	
	//print the file
	binarioNaTela1(fileRegister->filePointer);
	
	//destroy used data structures
	free(data);
	destroyList(removedList);
	destroyFileRegister(fileRegister);
}

void setUnsetFunc5(FILE *binFile){
	int n;
	//checking if the file is
	FileRegister *fileRegister = createFileRegister(binFile);		
	if(!isConsistent(fileRegister->header)){
		printf("Falha no processamento do arquivo.");
		destroyFileRegister(fileRegister);
		return;
	}

	LIST *removedList = createList();

	changeFileStatus(fileRegister, '0');			

	//bring the removed list from file to RAM
	fileToRemovedList(fileRegister, removedList);

	scanf("%d", &n);
	for(int i = 0; i < n; i++){
		fifthFunctionality(fileRegister, removedList);
	}

	//after all done, update the new removedList in file
	updateRemovedListOnFile(fileRegister, removedList);
	//update the changed registers in file
	addReplacedRegistersOnFile(fileRegister, removedList);

	changeFileStatus(fileRegister, '1');

	//print the file
	binarioNaTela1(fileRegister->filePointer);

	destroyList(removedList);
	destroyFileRegister(fileRegister);
}

void setUnsetFunc6(FILE *binFile){
	int n;
	FileRegister *fileRegister = createFileRegister(binFile);
	if(!isConsistent(fileRegister->header)){
		printf("Falha no processamento do arquivo.");
		destroyFileRegister(fileRegister);
		return;
	}

	EmployeeRegister *e = createEmployeeRegister();
	LIST *removedList = createList();

	changeFileStatus(fileRegister, '0');

	//bring the removed list from file to RAM
	fileToRemovedList(fileRegister, removedList);

	scanf("%d", &n);
	for(int i = 0; i < n; i++){
		sixthFunctionality(fileRegister, removedList);
	}

	//after all done, update the new removedList in file
	updateRemovedListOnFile(fileRegister, removedList);
	//update the changed registers in file
	addReplacedRegistersOnFile(fileRegister, removedList);

	changeFileStatus(fileRegister, '1');

	//print the file
	binarioNaTela1(fileRegister->filePointer);

	destroyList(removedList);
	destroyFileRegister(fileRegister);
	destroyEmployeeRegister(e);
}

void setUnsetFunc7(FILE *binFile){
	//creating fileRegister to binFile
	FileRegister *fileRegisterIn = createFileRegister(binFile); 
	fileRegisterIn->header->topoLista = -1;
	
	//checking if the file is consistent
	if(isConsistent(fileRegisterIn->header) != 1){
		printf("Falha no processamento do arquivo.");
		destroyFileRegister(fileRegisterIn);
		return;
	}

	//opening out file
	char outFileName[MAX_BUFFER] = {};
	scanf("%s", outFileName);
	FILE *outBin = fopen(outFileName, "w+b");
	if(outBin == NULL){
		printf("Falha no processamento do arquivo.");
		return;
	}
	
	//writing outFile header
	writeHeaderFromRam(outBin, fileRegisterIn->header);
	
	//creating a fileRegister to outFile
	FileRegister *fileRegisterOut = createFileRegister(outBin);

	//changing file' status
	changeFileStatus(fileRegisterOut, '0');

	//creating an array of employeeRegisters to maanage all the employees from file
	EmployeeArray *employeeArray = createEmployeeArray(ENTRY_SIZE);
	
	//calling the functionality
	seventhFuncionality(fileRegisterIn, fileRegisterOut, employeeArray);
	
	//changing file' status
	changeFileStatus(fileRegisterOut, '1');

	binarioNaTela1(outBin);

	//destroying used data structures
	destroyFileRegister(fileRegisterIn);
	destroyFileRegister(fileRegisterOut);
	destroyEmployeeArray(employeeArray);
	fclose(outBin);
}

void setUnsetFuncs8And9(FILE *firstBinFile, const int mode){
	FILE *secondBinFile, *outBinFile;
	FileRegister *firstFileRegister, *secondFileRegister, *outFileRegister;
	
	//opening the second file
	char secondFileName[MAX_BUFFER], outFileName[MAX_BUFFER];
	scanf("%s %s", secondFileName, outFileName);
	secondBinFile = fopen(secondFileName, "r+b");
	outBinFile = fopen(outFileName, "w+b");

	//dealing with errors
	if(secondBinFile == NULL || outBinFile == NULL){
		printf("Falha no processamento do arquivo.");
		if(secondBinFile != NULL) fclose(secondBinFile);
		if(outBinFile != NULL) fclose(outBinFile);
		return;
	}

	firstFileRegister = createFileRegister(firstBinFile);
	secondFileRegister = createFileRegister(secondBinFile);
	if(isConsistent(firstFileRegister->header) != 1 || isConsistent(secondFileRegister->header) != 1){
		printf("Falha no processamento do arquivo.");
		destroyFileRegister(firstFileRegister);
		destroyFileRegister(secondFileRegister);

		if(secondBinFile != NULL) fclose(secondBinFile);
		if(outBinFile != NULL) fclose(outBinFile);
		
		return;
	}

	//initializing the output File (it will receive the modified first file header)
	firstFileRegister->header->topoLista = -1;
	firstFileRegister->header->status = '0'; //implicit change of file status to '0'
	writeHeaderFromRam(outBinFile, firstFileRegister->header);
	outFileRegister = createFileRegister(outBinFile);
	
	//selecting the mode to be used
	switch (mode){
		case MERGE:
			eightFunctionality(firstFileRegister, secondFileRegister, outFileRegister);
			break;
		case MATCH:
			ninthFunctionality(firstFileRegister, secondFileRegister, outFileRegister);
			break;
	}
	
	//changing the file status to 1
	changeFileStatus(outFileRegister, '1');
	
	//print the file
	binarioNaTela1(outFileRegister->filePointer);
	
	//destroy used data structures
	destroyFileRegister(firstFileRegister);
	destroyFileRegister(secondFileRegister);
	destroyFileRegister(outFileRegister);
	
	//close files
	fclose(secondBinFile);
	fclose(outBinFile);

}