/*
Functions made to manipulate files for projects of File Organization discipline taken at University of Sao Paulo - Sao Carlos, Brazil

@Authors: João Pedro Almeida Santos Secundino 10692054
		  Giovani Decico Lucafó 10288779
		  
*/
#include<ctype.h>
#include"fileManagement.h"
void replace(char* string, char oldChar, char newChar){
	int sz = strlen(string);
	for (int i = 0; i < sz; ++i){
		//if we find an oldChar, substitute it by newChar
		if(string[i] == oldChar){
			string[i] = newChar;
		}
	}
}

void fillWithChar(char *buffer, char newChar, int sz){
	//fill sz positions of buffer with newChar
	for( int i = 0; i < sz; i++ ){
		buffer[i] = newChar;
	}
}

void writeHeader(FILE *csv, FILE *binFile){

	char desCampo1[DESCRIPTION_SIZE] = {};
	char desCampo2[DESCRIPTION_SIZE] = {};
	char desCampo3[DESCRIPTION_SIZE] = {};
	char desCampo4[DESCRIPTION_SIZE] = {};
	char desCampo5[DESCRIPTION_SIZE] = {};
	
	//fills the fields descriptions with '@'s
	fillWithChar(desCampo1, '@', DESCRIPTION_SIZE);
	fillWithChar(desCampo2, '@', DESCRIPTION_SIZE);
	fillWithChar(desCampo3, '@', DESCRIPTION_SIZE);
	fillWithChar(desCampo4, '@', DESCRIPTION_SIZE);
	fillWithChar(desCampo5, '@', DESCRIPTION_SIZE);

	//reads the fields from first line of csv
	fscanf(csv,"%[^,],%[^,],%[^,],%[^,],%[^,\r\n]", desCampo1, desCampo2, desCampo3, desCampo4, desCampo5);

	char status = '0';
	long long int topoLista = -1;
	char tagCampo1 = 'i';
	char tagCampo2 = 's';
	char tagCampo3 = 't';
	char tagCampo4 = 'n';
	char tagCampo5 = 'c';

	//writes the header content in the binary file according to specification
	fwrite(&status, sizeof(char), 1, binFile);
	fwrite(&topoLista, sizeof(char), 8, binFile);
	fwrite(&tagCampo1, sizeof(char), 1, binFile);
	fwrite(&desCampo1, sizeof(char), DESCRIPTION_SIZE, binFile);
	fwrite(&tagCampo2, sizeof(char), 1, binFile);
	fwrite(&desCampo2, sizeof(char), DESCRIPTION_SIZE, binFile);
	fwrite(&tagCampo3, sizeof(char), 1, binFile);
	fwrite(&desCampo3, sizeof(char), DESCRIPTION_SIZE, binFile);
	fwrite(&tagCampo4, sizeof(char), 1, binFile);
	fwrite(&desCampo4, sizeof(char), DESCRIPTION_SIZE, binFile);
	fwrite(&tagCampo5, sizeof(char), 1, binFile);
	fwrite(&desCampo5, sizeof(char), DESCRIPTION_SIZE, binFile);

	char blankSpace[PAGE_SIZE] = {};
	fillWithChar(blankSpace, '@', PAGE_SIZE);

	//fills the rest of the page with '@'s
	fwrite(blankSpace, sizeof(char), PAGE_SIZE - HEADER_SIZE, binFile);

}

//deprecated
int findRegisterSize(EmployeeRegister *e){
	//if we have both fields
	if(e->indicadorTamanhoNome && e->indicadorTamanhoCargo){
		return findRegisterSizeByFields(e->nomeServidor, e->cargoServidor);
	}else if(e->indicadorTamanhoNome){ //if we have only the nameField
		e->cargoServidor[0] = '\0';
		return findRegisterSizeByFields(e->nomeServidor, e->cargoServidor);
	}else if(e->indicadorTamanhoCargo){//if we have only the postField
		e->nomeServidor[0] = '\0';
		return findRegisterSizeByFields(e->nomeServidor, e->cargoServidor);
	}else{//if we dont have any of these two fields
		e->cargoServidor[0] = '\0';
		e->nomeServidor[0] = '\0';
		return findRegisterSizeByFields(e->nomeServidor, e->cargoServidor);
	}
}

int hasCharacteristic(EmployeeRegister *e, Options o, int option ){
	switch(option){
		case 0:
			if(e->idServidor == o.id){
				return 1;
			}
			break;
		case 1:
			if(e->salarioServidor == o.salary){
				return 1;
			}
			break;
		case 2:
			if(!strcmp(e->telefoneServidor, o.phone)){
				return 1;
			}
			break;
		case 3:
			if(!strcmp(e->nomeServidor, o.name)){
				return 1;
			}
			break;
		case 4:
			if(!strcmp(e->cargoServidor, o.post)){
				return 1;
			}
			break;					
		default:
			return 0;
	}
	return 0;
}


int findRegisterSizeByFields(char *nomeServidor, char *cargoServidor){
	//fixed sizes:               8                  4                8               14
	int fixSizePositions = ENC_LISTA_SIZE + ID_SERVIDOR_SIZE + SAL_SERV_SIZE + TEL_SERV_SIZE;
	int endStringChar = 0;

	//if both fields are null, we need to decrease the fixSizePositions 
	//because we dont need to write in the file the two size indicator nor the two tags
	if(strlen(nomeServidor) == 0 && strlen(cargoServidor) == 0){
		endStringChar = 0;
	}else if(strlen(nomeServidor) == 0 || strlen(cargoServidor) == 0){//if only one is null, increase the size in one IND_TAM(4) and one TAG_SIZE(1)
		endStringChar = 1;
		fixSizePositions += (IND_TAM + TAG_SIZE);
	}else{// we dont have any null string, we have to count only the size of \0's and the two IND_TAM and TAG_SIZE
		endStringChar = 2; 
		fixSizePositions += 2*(IND_TAM + TAG_SIZE);
	}

	return fixSizePositions + strlen(nomeServidor) + strlen(cargoServidor)  + endStringChar; 

}

void addEmployeeInFile(FILE *file, int regSize, EmployeeRegister *e){
	//counting the size with \0 and tagField size
	int fieldNameSize = strlen(e->nomeServidor) + END_STRING + TAG_SIZE;
	int fieldPostSize = strlen(e->cargoServidor) + END_STRING + TAG_SIZE;
	char removed = '-';
	long long int chainedList = -1;
	char nameTag = 'n';
	char postTag = 'c';
	
	//writes the employee information in the file
	fwrite(&removed, sizeof(char), 1, file);//1
	fwrite(&regSize, sizeof(int), 1, file);//4
	fwrite(&chainedList, sizeof(long long int), 1, file);//8
	fwrite(&(e->idServidor), sizeof(int), 1, file);//4
	fwrite(&(e->salarioServidor), sizeof(double), 1, file);//8
	fwrite(e->telefoneServidor, sizeof(char), 14, file);//14

	//if the name field isn't null
	if(fieldNameSize - END_STRING - TAG_SIZE != 0){
		//field size
		fwrite(&fieldNameSize, sizeof(int), 1, file);//4
		fwrite(&nameTag, sizeof(char), 1, file);//1
		fwrite(e->nomeServidor, sizeof(char), strlen(e->nomeServidor) + 1, file);
	}

	//if the post field isn't null
	if(fieldPostSize - END_STRING - TAG_SIZE != 0){
		//field size
		fwrite(&fieldPostSize, sizeof(int), 1, file);//4
		fwrite(&postTag, sizeof(char), 1, file);//1
		fwrite(e->cargoServidor, sizeof(char), strlen(e->cargoServidor) + 1, file);
	}
}	


void putCharOnPage(FILE *page, char c, int size){
	//put size chars c in page
	for( int i = 0; i < size; i++ ){
		fputc(c, page);
	}
}

HeaderRegister* createHeaderRegister(){
	HeaderRegister *h = (HeaderRegister *) calloc(1,sizeof(HeaderRegister));
	return h;
}		

void destroyHeaderRegister(HeaderRegister *h){
	free(h);
}

EmployeeRegister* createEmployeeRegister(){
	EmployeeRegister *e = (EmployeeRegister *)calloc(1, sizeof(EmployeeRegister));
	
	//initializes the register's variable size variables
	e->nomeServidor = (char *) calloc(1, sizeof(char) * MAX_BUFFER);
	e->cargoServidor = (char *) calloc(1, sizeof(char) * MAX_BUFFER);

	return e;
}

void destroyEmployeeRegister(EmployeeRegister *e){
	free(e->nomeServidor);
	free(e->cargoServidor);
	free(e);
}


void readHeaderPage(FILE *binFile, HeaderRegister *h){

	//reads the header from the binary file an places it into h structure
	fread(&(h->status), sizeof(char), 1, binFile);
	fread(&(h->topoLista), sizeof(long long int), 1, binFile);
	fread(&(h->tagCampo1), sizeof(char), 1, binFile);
	fread((h->desCampo1), sizeof(char), DESCRIPTION_SIZE, binFile);
	fread(&(h->tagCampo2), sizeof(char), TAG_SIZE, binFile);
	fread((h->desCampo2), sizeof(char), DESCRIPTION_SIZE, binFile);
	fread(&(h->tagCampo3), sizeof(char), TAG_SIZE, binFile);
	fread((h->desCampo3), sizeof(char), DESCRIPTION_SIZE, binFile);
	fread(&(h->tagCampo4), sizeof(char), TAG_SIZE, binFile);
	fread((h->desCampo4), sizeof(char), DESCRIPTION_SIZE, binFile);
	fread(&(h->tagCampo5), sizeof(char), TAG_SIZE, binFile);
	fread((h->desCampo5), sizeof(char), DESCRIPTION_SIZE, binFile);

	//changes the '@'s by blank spaces for printing
	replace(h->desCampo1, '@', ' ');
	replace(h->desCampo2, '@', ' ');
	replace(h->desCampo3, '@', ' ');
	replace(h->desCampo4, '@', ' ');
	replace(h->desCampo5, '@', ' ');

	//goes straight to the next page
	fseek(binFile, PAGE_SIZE - HEADER_SIZE, SEEK_CUR);
}

int readPerson(FILE *binFile, EmployeeRegister *e){
	int sizeField = 0;
	char tag = 'a';
	//count the difference betwen the real size (without the end-of-page trash) of the last register in a page and the actual size of this register
	int lastRegisterDiff = 0;	
	//checks the end-of-file
	int flagEOF = 1;

	//reads the fields and checks if we get the end-of-file
	flagEOF &= !fread(&(e->removido), sizeof(char), 1, binFile);
	flagEOF &= !fread(&(e->tamanhoRegistro), sizeof(int), 1, binFile);
	flagEOF &= !fread(&(e->encadeamentoLista), sizeof(long long int), 1, binFile);
	flagEOF &= !fread(&(e->idServidor), sizeof(int), 1, binFile);
	flagEOF &= !fread(&(e->salarioServidor), sizeof(double), 1, binFile);
	flagEOF &= !fread((e->telefoneServidor), sizeof(char), PHONE_SIZE -1, binFile);
	
	//if eof is reached
	if(flagEOF == 1){
		fgetc(binFile);
		return 0;
	}

	//if we have '@' in telefoneServidor field, change it for blank spaces
	replace(e->telefoneServidor, '@', ' ');
	//put a '\0' in the string's end to print it with printf
	e->telefoneServidor[PHONE_SIZE - 1] = '\0';

	//try to read the name/ post field
	fread(&(sizeField), sizeof(int), 1, binFile);
	fread(&(tag), sizeof(char), 1, binFile);
	
	//if the two variable fields are null (that means that we don't have any of these fields in the file)
	if(tag != 'n' && tag != 'c'){

		//return the file pointer to the previous position 
		fseek(binFile, -(sizeof(int) + sizeof(char)), SEEK_CUR);
		
		//these fields have size = 0
		e->indicadorTamanhoNome = 0;
		e->indicadorTamanhoCargo = 0;
		strcpy(e->nomeServidor, "\0");
		strcpy(e->cargoServidor, "\0");

	}else if(tag == 'n'){//if we found the name field
		//we dont put in the count of indicadorTamanhoNome the tag size nor the \0 size
		e->indicadorTamanhoNome = sizeField - TAG_SIZE - END_STRING;
		e->tagCampo4 = tag;
		fread((e->nomeServidor), sizeof(char), e->indicadorTamanhoNome + END_STRING, binFile);

		//trying to read the post field (same idea from above)
		fread(&(sizeField), sizeof(int), 1, binFile);
		fread(&(tag), sizeof(char), 1, binFile);

		//if we found it
		if(tag == 'c'){
			//we dont put in the count of indicadorTamanhoCargo the tag size nor the \0 size
			e->indicadorTamanhoCargo = sizeField - TAG_SIZE - END_STRING;
			e->tagCampo5 = tag;
			fread((e->cargoServidor), sizeof(char), e->indicadorTamanhoCargo + END_STRING, binFile);	
		}else{
			//if not, we return the file pointer to the previous position
			fseek(binFile, -(sizeof(int) + sizeof(char)), SEEK_CUR);
			//and we indicate the postSize as 0
			e->indicadorTamanhoCargo = 0;
			strcpy(e->cargoServidor, "\0");
		}

	}else if(tag == 'c'){//if we doesn't have a name field but we have a post field
		//the name field has size zero 
		e->indicadorTamanhoNome = 0;
		strcpy(e->nomeServidor, "\0");
		//we dont put in the count of indicadorTamanhoCargo the tag size nor the \0 size
		e->indicadorTamanhoCargo = sizeField - TAG_SIZE - END_STRING;
		e->tagCampo5 = tag;
		fread((e->cargoServidor), sizeof(char), e->indicadorTamanhoCargo + END_STRING, binFile);
	}

	//if the register size is greater than its real size, it means that it is the last register in the page
	//if we are dealing with the last register of the page, we need to go to the begining of the new page
	if(findRegisterSize(e) < e->tamanhoRegistro){
		lastRegisterDiff = e->tamanhoRegistro - findRegisterSize(e);
		fseek(binFile, lastRegisterDiff, SEEK_CUR);
	}

	//an employee was succesfully read
	return 1;

}

int myCeil(double num){

	int x = (int) num;
	//return (int)num if it is a whole number or the next nearest integer
	return (num == (double) x) ? x : x + 1;
}

void printBlank(int n){
	//prints n blank spaces on the screen
	for (int i = 0; i < n; ++i)
	{
		printf(" ");
	}
}

int returnOptionByString(char *option){
	const char optionsString[NUM_FIELDS][MAX_BUFFER] = {{"idServidor"}, {"salarioServidor"}, {"telefoneServidor"}, {"nomeServidor"}, {"cargoServidor"}};

	//return the index of string option
	for (int i = 0; i < NUM_FIELDS; ++i){
		if(!strcmp(option, optionsString[i])){
			return i;
		}
	}

	//if we not find the option
	return -1;
}

void printEmployeebyHeader(HeaderRegister *h, EmployeeRegister *e, FILE *outputStream){
	
	if(e->removido == '*')
		return;
	//each of the next five blocks has the same idea behind:
	//picks the field's metadata from HeaderRegister h and prints it with the content of that field present in EmployeeRegister,
	//always treating null values and garbage	
	fprintf(outputStream,"%s: ", h->desCampo1);
	if(e->idServidor != -1){
		fprintf(outputStream,"%d\n", e->idServidor );
	}else{
		fprintf(outputStream,"valor nao declarado\n");
	}

	fprintf(outputStream,"%s: ", h->desCampo2);
	if(e->salarioServidor != -1){
		fprintf(outputStream,"%.2lf\n", e->salarioServidor);
	}else{
		fprintf(outputStream,"valor nao declarado\n");
	}

	fprintf(outputStream,"%s: ", h->desCampo3);
	if(strlen(e->telefoneServidor)){
		fprintf(outputStream,"%s\n", e->telefoneServidor );
	}else{
		fprintf(outputStream,"valor nao declarado\n");
	}

	fprintf(outputStream,"%s: ", h->desCampo4);
	if(strlen(e->nomeServidor)){
		fprintf(outputStream,"%s\n", e->nomeServidor );
	}else{
		fprintf(outputStream,"valor nao declarado\n");
	}
	
	fprintf(outputStream,"%s: ", h->desCampo5);
	if(strlen(e->cargoServidor)){
		fprintf(outputStream,"%s\n", e->cargoServidor );
	}else{
		fprintf(outputStream,"valor nao declarado\n");
	}

	fprintf(outputStream,"\n");
	fflush(outputStream);
}


int readEmployeesFromFile(FILE* binFile, Options o, int option, const int checkStatus, FILE *outputStream){

	int numPages;
	int bytesAccessedTotal = 0;
	HeaderRegister *h = createHeaderRegister();
	EmployeeRegister *e = createEmployeeRegister();
	//flag that indicates that we found at least one employee that fits in the given constraints
	int flagExist = 0;
	//flag that indicates that we found a new employee  that fits in the given constraints
	int flagFound = 0;
	//counter that counts the number of bytes acccessed during the search
	int bytesAccessed = 0;
	fseek(binFile, 0, SEEK_SET);
	//reads the header and puts its info on h
	readHeaderPage(binFile, h);

	//if we have an inconsistent file, abort
	if(h->status == '0' && checkStatus == _TRUE){
		fprintf(outputStream,"Falha no processamento do arquivo.");
		return 1;
	}

	//while we didn't reach the eof
	while(!feof(binFile)){
		//if a employee was succesfully read
		if(readPerson(binFile, e)){

			//according to the specified option, we compare the fields passed trough Option o
			switch(option){
				case 0:
					if(e->idServidor == o.id){
						flagExist = 1;
						printEmployeebyHeader(h, e, outputStream);
						flagFound = 1;
					}
					break;
				case 1:
					if(e->salarioServidor == o.salary){
						flagExist = 1;
						flagFound = 1;
						printEmployeebyHeader(h, e, outputStream);
					}				
					break;
				case 2:
					if(!strcmp(e->telefoneServidor, o.phone)){
						flagExist = 1;
						flagFound = 1;
						printEmployeebyHeader(h, e, outputStream);
					}
					break;
				case 3:
					if(!strcmp(e->nomeServidor, o.name)){
						flagExist = 1;
						flagFound = 1;
						printEmployeebyHeader(h, e, outputStream);
					}
					break;
				case 4:
					if(!strcmp(e->cargoServidor, o.post)){
						flagExist = 1;
						flagFound = 1;
						printEmployeebyHeader(h, e, outputStream);
					}
					break;
					
			}
			
			//refreshes the total number of bytes accessed
			bytesAccessedTotal += e->tamanhoRegistro + REMOVIDO_TAG_SIZE + IND_TAM_REG;
			//if we found a new employee that fits in the constraints, we refresh the bytes acessed until this discovery
			if(flagFound){
				bytesAccessed = bytesAccessedTotal;
				flagFound = 0;
			}
		}	

	}

	destroyHeaderRegister(h);
	destroyEmployeeRegister(e);

	if(option == 0){
		//calculates the number os pages acessed until our last employee found, counting with the Header Page ( + 1 )
		numPages = myCeil(((double) bytesAccessed/ (double) PAGE_SIZE)) + 1 ;
	}else{
		//calculates the number os pages acessed in total, counting with the Header Page ( + 1 )
		numPages = myCeil(((double) bytesAccessedTotal/ (double) PAGE_SIZE)) + 1 ;
	}

	//if we didn't find any employee
	if(!flagExist){
		fprintf(outputStream,"Registro inexistente.\n");
	}

	return numPages;


}

FileRegister *createFileRegister(FILE *file){
	
	HeaderRegister *h = (HeaderRegister*) calloc(1,sizeof(HeaderRegister));
	if (h == NULL){
		return NULL;
	}

	readHeaderPage(file, h);

	FileRegister *fileRegister= (FileRegister *) calloc(1, sizeof(FileRegister));
	if (fileRegister == NULL){
		return NULL;
	}

	fileRegister->header = h;
	fileRegister->filePointer = file; 
	findLastRegister(fileRegister, PAGE_SIZE);
	fseek(fileRegister->filePointer, PAGE_SIZE - HEADER_SIZE, SEEK_CUR);
	return fileRegister;
}

void destroyFileRegister(FileRegister *f){
	free(f->header);
	free(f);
}

void virtualRemoveEmployee(FileRegister *fileRegister, LIST *removedList, Options o, int option){

	char removed = '*';
	//go to the first data page
	fseek(fileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	//while we didn't reach the eof
	while(!feof(fileRegister->filePointer)){

		EmployeeRegister *e = createEmployeeRegister();
		//find register's offset
		long long int regOffset= ftell(fileRegister->filePointer);
		
		if(readPerson(fileRegister->filePointer, e)){
			long long int nextRegOffSet = ftell(fileRegister->filePointer);
			//according to the specified option, we compare the fields passed trough Option o
			if(hasCharacteristic(e, o, option) && e->removido != '*'){				
				e->removido = removed;
				insertOrderedList(removedList,e, regOffset, 0);

				//mark it as removed in file
				fseek(fileRegister->filePointer, regOffset, SEEK_SET);
				fwrite(&removed, sizeof(char), 1, fileRegister->filePointer);
				fseek(fileRegister->filePointer, nextRegOffSet, SEEK_SET);

				//if it is a unique characteristic
				if(option == idServidor){
					//return to HEADER PAGE end
					fseek(fileRegister->filePointer, PAGE_SIZE, SEEK_SET);
					return;
				}	
			}else{
				destroyEmployeeRegister(e);
			}

		}else{
			destroyEmployeeRegister(e);
		}
	}	
	fseek(fileRegister->filePointer, PAGE_SIZE, SEEK_SET);
}

void realRemoveEmployee(FileRegister *fileRegister, LIST *removedList ){
	int littleMetaData = REMOVIDO_TAG_SIZE + IND_TAM_REG;
	long long int listTop = 1;
	if(!isListEmpty(removedList)){
		NODE *it = removedList->begin;

		//go to header's "topoLista" field
		fseek(fileRegister->filePointer, listTop, SEEK_SET);
		
		//put there the byte offset of first element of removedList
		fwrite(&(it->employeeInfo.byteOffSet), sizeof(long long int), 1, fileRegister->filePointer);	
		
		while(it!=NULL){
			//go to register's byteOffet
			fseek(fileRegister->filePointer, it->employeeInfo.byteOffSet, SEEK_SET);
			
			//fill the register with @'s
			putCharOnPage(fileRegister->filePointer, '@', it->employeeInfo.employeeRegister->tamanhoRegistro + littleMetaData );
			
			//go back to register's beginning
			fseek(fileRegister->filePointer, it->employeeInfo.byteOffSet, SEEK_SET);
			
			//write the updated info in the file
			fwrite(&(it->employeeInfo.employeeRegister->removido), sizeof(char), 1, fileRegister->filePointer);
			fwrite(&(it->employeeInfo.employeeRegister->tamanhoRegistro), sizeof(int), 1, fileRegister->filePointer);
			fwrite(&(it->employeeInfo.employeeRegister->encadeamentoLista), sizeof(long long int), 1, fileRegister->filePointer);

			it = it->next;
		}
	}
}

long long int insertEndFile(FileRegister *fileRegister, EmployeeRegister *e){
	fseek(fileRegister->filePointer,0,SEEK_END);
	long long int lastOffSet = ftell(fileRegister->filePointer);
	int lastRegSize,
		sizeDifference;
	if (strcmp(e->telefoneServidor, "(27)99187-8778")==0){
		lastRegSize = lastRegSize;
	}
	//if the new register doesn't fit in page
	if((lastOffSet%PAGE_SIZE + e->tamanhoRegistro + REMOVIDO_TAG_SIZE + IND_TAM) > PAGE_SIZE  ){
		//go to last register in page
		fseek(fileRegister->filePointer, fileRegister->lastRegisterByteOffSet + REMOVIDO_TAG_SIZE, SEEK_SET );
		//read the last register size
		fread(&lastRegSize, sizeof(int), 1, fileRegister->filePointer);
		fseek(fileRegister->filePointer, -IND_TAM, SEEK_CUR );
		//change its size to complete the page
		sizeDifference = PAGE_SIZE -  lastOffSet%PAGE_SIZE;
		lastRegSize += sizeDifference;
		fwrite(&lastRegSize, sizeof(int), 1, fileRegister->filePointer);
		fseek(fileRegister->filePointer, lastRegSize - sizeDifference, SEEK_CUR);
		//fill the rest of page with '@'
		putCharOnPage(fileRegister->filePointer, '@', sizeDifference);
	}
	fileRegister->lastRegisterByteOffSet = ftell(fileRegister->filePointer);
	addEmployeeInFile(fileRegister->filePointer, e->tamanhoRegistro, e);
	return fileRegister->lastRegisterByteOffSet;
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

void readEmployeeFromInput(EmployeeRegister *e){
	char buffer[NUM_FIELDS][MAX_BUFFER];
	char nullStr[] = {"NULO"};


	for(int i = 0; i < NUM_FIELDS; i++){
		if(i<2){
			scanf("%s", buffer[i]);
			switch(i){
				case 0:
					if(strcmp(buffer[i], nullStr) != 0){
						e->idServidor = atoi(buffer[i]);
					}else{
						e->idServidor = -1;
					}
					break;
			
				case 1:
					if(strcmp(buffer[i], nullStr) != 0){
						e->salarioServidor = atof(buffer[i]);
					}else{
						e->salarioServidor = -1;
					}

					break;
			}
		}else{
			scan_quote_string(buffer[i]);
			switch (i){
				case 2:
					if(strlen(buffer[i])){
						strcpy(e->telefoneServidor, buffer[i]);
					}else{
						memcpy(e->telefoneServidor, "\0@@@@@@@@@@@@@", 14);
					}
					break;
				case 3:
					if(strlen(buffer[i])){
						strcpy(e->nomeServidor, buffer[i]);
						e->indicadorTamanhoNome = (int) strlen(buffer[i]) + TAG_SIZE + END_STRING;
					}else{
						strcpy(e->nomeServidor, "");
						e->indicadorTamanhoCargo = 0;
					}
					break;
				case 4:
					if(strlen(buffer[i])){
						strcpy(e->cargoServidor, buffer[i]);
						e->indicadorTamanhoCargo = (int) strlen(buffer[i]) + TAG_SIZE + END_STRING; 
					}else{
						strcpy(e->cargoServidor, "");
						
						e->indicadorTamanhoCargo = 0;
					}
					break;

			}
		}
	}

		e->removido = '-';
		e->tamanhoRegistro = findRegisterSizeByFields(e->nomeServidor, e->cargoServidor);
		e->encadeamentoLista = -1;
		e->tagCampo4 = 'n';
		e->tagCampo5 = 'c';

}

void copyRegisterContent(EmployeeRegister *destiny, EmployeeRegister *font){
	destiny->removido = font->removido;
	destiny->tamanhoRegistro = font->tamanhoRegistro;
	destiny->encadeamentoLista = font->encadeamentoLista;
	destiny->idServidor = font->idServidor;
	destiny->salarioServidor = font->salarioServidor;
	memcpy(destiny->telefoneServidor, font->telefoneServidor, 14);
	destiny->indicadorTamanhoNome = font->indicadorTamanhoNome;
	destiny->tagCampo4 = font->tagCampo4;
	strcpy(destiny->nomeServidor, font->nomeServidor);
	destiny->indicadorTamanhoCargo = font->indicadorTamanhoCargo;
	destiny->tagCampo5 = font->tagCampo5;
	strcpy(destiny->cargoServidor, font->cargoServidor);
}

void binarioNaTela1(FILE *ponteiroArquivoBinario) {

	/* Escolha essa função se você ainda tem o ponteiro de arquivo 'FILE *' aberto.
	*  Lembrando que você tem que ter aberto ele no fopen para leitura também pra funcionar (exemplo: rb, rb+, wb+, ...) */

	unsigned char *mb;
	unsigned long i;
	size_t fl;
	
	fseek(ponteiroArquivoBinario, 0, SEEK_END);
	
	fl = ftell(ponteiroArquivoBinario);
	
	fseek(ponteiroArquivoBinario, 0, SEEK_SET);
	
	mb = (unsigned char *) malloc(fl);
	fread(mb, sizeof(char), fl, ponteiroArquivoBinario);
	
	for(i = 0; i < fl; i += sizeof(unsigned char)) {
		printf("%02X ", mb[i]);
		if((i + 1) % 16 == 0)	printf("\n");
	}
	
	free(mb);
}

void fileToRemovedList(FileRegister *fileRegister, LIST *removedList){
	fseek(fileRegister->filePointer, 1, SEEK_SET);
	FILE *binFile = fileRegister->filePointer;
	EmployeeRegister *e;
	long long byteOffSet, nextByteOffSet;
	//iterates by the file removed list
	fread(&byteOffSet,sizeof(long long int), 1, binFile);
	while(byteOffSet != -1){//while we didn's reached the lists' end
		e = createEmployeeRegister();
		fseek(binFile, byteOffSet, SEEK_SET);
		readPerson(binFile, e);
		nextByteOffSet = e->encadeamentoLista;
		insertEndList(removedList,e, byteOffSet);
		byteOffSet = nextByteOffSet;
	}
}

NODE* findNextNotReplaced(NODE *it){

	while(it != NULL && it->employeeInfo.employeeRegister->removido == '-'){
			it = it->next;

	}

	return it;
	
}

NODE * findNextReplaced(NODE *it){

	while(it != NULL && it->employeeInfo.employeeRegister->removido == '*'){
			it = it->next;
	}

	return it;
}

void updateRemovedListOnFile(FileRegister *fileRegister, LIST *removedList){
	if(isListEmpty(removedList)){
		return;
	}

	NODE *actual = findNextNotReplaced(removedList->begin);
	NODE *it;
	long long int endList = -1;
	long long auxByteOffSet;

	if(actual != NULL){
		auxByteOffSet = actual->employeeInfo.byteOffSet;
	}else{
		auxByteOffSet = -1; 
	}

	//if the first element of the removedList has changed, change it in file
	if(auxByteOffSet != fileRegister->header->topoLista){
		actual = findNextNotReplaced(actual);

		fseek(fileRegister->filePointer, 1, SEEK_SET);
		if(actual == NULL){
			fwrite(&(endList), sizeof(long long int), 1, fileRegister->filePointer);
			return;
		}

		fwrite(&(actual->employeeInfo.byteOffSet), sizeof(long long int), 1, fileRegister->filePointer);
		fseek(fileRegister->filePointer, actual->employeeInfo.byteOffSet, SEEK_SET);
	}

	//rearranging the removed list in file
	it = actual;
	while(it!=NULL){
		it = findNextNotReplaced(it->next);
		
		if(it != NULL){
			actual->employeeInfo.employeeRegister->encadeamentoLista = it->employeeInfo.byteOffSet;
			
			//go directly to previous removed register encadeamentoLista field
			fseek(fileRegister->filePointer, actual->employeeInfo.byteOffSet + REMOVIDO_TAG_SIZE + IND_TAM_REG, SEEK_SET );
			//change it's value to next register's byteOffSet
			fwrite(&(it->employeeInfo.byteOffSet), sizeof(long long int), 1, fileRegister->filePointer); 
			
			actual = it;
			
		}
	}

	//add -1 to list's end
	if(actual != NULL){
		actual->employeeInfo.employeeRegister->encadeamentoLista = -1;
		fseek(fileRegister->filePointer, actual->employeeInfo.byteOffSet + REMOVIDO_TAG_SIZE + IND_TAM_REG, SEEK_SET );
		fwrite(&(endList), sizeof(long long int), 1, fileRegister->filePointer); 
	}


}

void addReplacedRegistersOnFile(FileRegister *fileRegister, LIST *removedList){
	if(isListEmpty(removedList)){
		return;
	}

	NODE *it = findNextReplaced(removedList->begin);

	while(it != NULL){//iterate the list only by the replaced registers
		fseek(fileRegister->filePointer, it->employeeInfo.byteOffSet, SEEK_SET);
		int regSize = it->employeeInfo.employeeRegister->tamanhoRegistro;
		addEmployeeInFile(fileRegister->filePointer,regSize, it->employeeInfo.employeeRegister);
		it = findNextReplaced(it->next);
	}

}

void changeFileStatus(FileRegister *f, char c){
	long long int originalOffset = ftell(f->filePointer);
	fseek(f->filePointer,0, SEEK_SET);
	fwrite(&c,sizeof(char), 1, f->filePointer);
	fseek(f->filePointer, originalOffset, SEEK_SET);
}

int isConsistent(HeaderRegister *h){
	return h->status == '0' ? 0 : 1; 
}

LIST *findEmployees(FileRegister *fileRegister, Options o, int option){
	LIST *found = createList();

	//go to the first data page
	fseek(fileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	//while we didn't reach the eof
	while(!feof(fileRegister->filePointer)){

		EmployeeRegister *e = createEmployeeRegister();
		
		//find register's offset
		long long int regOffset= ftell(fileRegister->filePointer);
		
		if(readPerson(fileRegister->filePointer, e)){
			//according to the specified option, we compare the fields passed trough Option o
			if(hasCharacteristic(e, o, option) && e->removido != '*'){				
				insertEndList(found,e, regOffset);

				//if it is a unique characteristic
				if(option == idServidor){
					//return to HEADER PAGE end
					fseek(fileRegister->filePointer, PAGE_SIZE, SEEK_SET);
					break;
				}	
			}else{
				destroyEmployeeRegister(e);
			}

		}else{
			destroyEmployeeRegister(e);
		}
	}	
	fseek(fileRegister->filePointer, PAGE_SIZE, SEEK_SET);
	
	return found;
}


int updateRegister(FileRegister *fileRegister, NODE *n , Options o, int option){
	EmployeeRegister *e = n->employeeInfo.employeeRegister;
	long long int regByteOffSet = n->employeeInfo.byteOffSet;
	//if we are updating the fixed size fields, no problem, just update
	if( option < 3  ){
		if(option == idServidor ){
			fseek(fileRegister->filePointer, regByteOffSet + METADATA, SEEK_SET );
			fwrite(&o.id, sizeof(int), 1, fileRegister->filePointer );
		}else if(option == salarioServidor ){
			fseek(fileRegister->filePointer, regByteOffSet + METADATA + ID_SERVIDOR_SIZE, SEEK_SET );
			fwrite(&o.salary, sizeof(long long int), 1, fileRegister->filePointer );
		}else if(option == telefoneServidor){
			fseek(fileRegister->filePointer, regByteOffSet + METADATA + ID_SERVIDOR_SIZE + SAL_SERV_SIZE, SEEK_SET );
			fwrite(&o.phone, sizeof(char), 14, fileRegister->filePointer );
		}
	}else{// if we're updating dinamic size fields
		if(option == nomeServidor){
			return changeEmployeeNameOnFile(fileRegister, e, regByteOffSet, o.name);
		}else if(option == cargoServidor) {
			return changeEmployeePostOnFile(fileRegister, e, regByteOffSet, o.post);
		}
	}

	return 1;
}

int changeEmployeeNameOnFile(FileRegister *fileRegister, EmployeeRegister *e, long long int regOffSet, char *name){
	
	if(e->indicadorTamanhoNome == 0 && strlen(name) == 0){
		return 1;
	}

	int regSize = e->tamanhoRegistro;
	long long int nameOffSet = METADATA + ID_SERVIDOR_SIZE + SAL_SERV_SIZE + TEL_SERV_SIZE;
	int newRegSize = findRegisterSizeByFields(name, e->cargoServidor);
	int newFieldNameSize = strlen(name) + TAG_SIZE + REMOVIDO_TAG_SIZE;
	char nameTag = 'n';
	char postTag = 'c';

	if( newRegSize > regSize ){//if the new name doesn't fit
		return 0;
	}else{
		//check if a new name fits in the register
		fseek(fileRegister->filePointer, regOffSet + nameOffSet, SEEK_SET);
		if(strlen(name) != 0){
			fwrite(&(newFieldNameSize), sizeof(int), 1, fileRegister->filePointer);
			fwrite(&(nameTag), sizeof(char), 1, fileRegister->filePointer);
			fwrite(&(name), sizeof(char), strlen(name) + END_STRING, fileRegister->filePointer);
		}

		if(e->indicadorTamanhoCargo != 0){
			fwrite(&(e->indicadorTamanhoCargo), sizeof(int), 1, fileRegister->filePointer);
			fwrite(&(postTag), sizeof(char), 1, fileRegister->filePointer);
			fwrite(&(e->cargoServidor), sizeof(char), strlen(e->cargoServidor) + END_STRING, fileRegister->filePointer);
		}

		putCharOnPage(fileRegister->filePointer, '@', regSize - newRegSize );
	}

	return 1;
}

int changeEmployeePostOnFile(FileRegister *fileRegister, EmployeeRegister *e, long long int regOffSet, char *post){
	
		
	if(e->indicadorTamanhoNome == 0 && strlen(post) == 0){
		return 1;
	}

	int regSize = e->tamanhoRegistro;
	long long int nameOffSet = METADATA + ID_SERVIDOR_SIZE + SAL_SERV_SIZE + TEL_SERV_SIZE;
	int newRegSize = findRegisterSizeByFields(e->nomeServidor, post);
	int newFieldPostSize = strlen(post) + TAG_SIZE + REMOVIDO_TAG_SIZE;
	char postTag = 'c';
	
	if( newRegSize > regSize ){//if the new post doesn't fit
		return 0;
	}else{
		//check if a new post fits in the register
		fseek(fileRegister->filePointer, regOffSet + nameOffSet, SEEK_SET);
		//check if a name exists
		if(e->indicadorTamanhoNome != 0){
			fseek(fileRegister->filePointer, e->indicadorTamanhoNome, SEEK_CUR);
		}
		//write the new post in file
		if(strlen(post) != 0){
			fwrite(&(newFieldPostSize), sizeof(int), 1, fileRegister->filePointer);
			fwrite(&(postTag), sizeof(char), 1, fileRegister->filePointer);
			fwrite(&(post), sizeof(char), strlen(post) + END_STRING, fileRegister->filePointer);
		}

		putCharOnPage(fileRegister->filePointer, '@', regSize - newRegSize );
	}

	return 1;
}

long long int addNewRegisterInFile(FileRegister *fileRegister, IndexFileRegister *indexFileRegister,LIST *removedList, EmployeeRegister *e){
	NODE* correctPlace = greaterEqualRegisterInList(removedList, e);
	long long int newByteOffset = 0;
	if(!correctPlace){
		newByteOffset = insertEndFile(fileRegister, e);
	}else{
		newByteOffset = replaceInfoInList(correctPlace, e);
	}

	if(indexFileRegister != NULL && e->indicadorTamanhoNome != 0){
			addEmployeeInIndexArrayEnd(indexFileRegister,e->nomeServidor,newByteOffset);
	}

	return newByteOffset;
}

void changeEmployeeName(EmployeeRegister *e, char *name){
	if(!strlen(name)){
		strcpy(e->nomeServidor, "");
		e->indicadorTamanhoNome = 0;
	}else{
		strcpy(e->nomeServidor, name);
		e->tagCampo4 = 'n';
		e->indicadorTamanhoNome = strlen(name) + END_STRING + TAG_SIZE;
	}

	e->tamanhoRegistro = findRegisterSizeByFields(e->nomeServidor, e->cargoServidor);
}

void changeEmployeePost(EmployeeRegister *e, char *post){
	if(!strlen(post)){
		strcpy(e->cargoServidor, "");
		e->indicadorTamanhoCargo = 0;
	}else{
		strcpy(e->cargoServidor, post);
		e->tagCampo5 = 'c';
		e->indicadorTamanhoCargo = strlen(post) + END_STRING + TAG_SIZE;
	}

	e->tamanhoRegistro = findRegisterSizeByFields(e->nomeServidor, e->cargoServidor);
}

void removeEmployeeOnFile(FileRegister *fileRegister,EmployeeRegister *e, long long int regOffSet){
	char removedTag = '*';
	fseek(fileRegister->filePointer, regOffSet, SEEK_SET);//go to register
	fwrite( &(removedTag), sizeof(char), 1, fileRegister->filePointer );//put * in removed field
	fseek(fileRegister->filePointer, regOffSet + IND_TAM + REMOVIDO_TAG_SIZE + ENC_LISTA_SIZE, SEEK_SET);//go to after encadeamentoLista field
	putCharOnPage(fileRegister->filePointer, '@', e->tamanhoRegistro - (ENC_LISTA_SIZE));//fill to the register's end with '@'
}

void findLastRegister(FileRegister *fileRegister, long long int pageSize){
	
	//goes to the end
	fseek(fileRegister->filePointer, 0, SEEK_END);

	long long int lastByteOffSet = ftell(fileRegister->filePointer);
	long long int lastPageOffSet = lastByteOffSet - lastByteOffSet%PAGE_SIZE;
	
	fseek(fileRegister->filePointer, lastPageOffSet, SEEK_SET);

	EmployeeRegister *aux = createEmployeeRegister();
	long long int lastRegisterByteOffset = ftell(fileRegister->filePointer);
	
	//while we didn't reach the eof
	while(ftell(fileRegister->filePointer) != lastByteOffSet){
		lastRegisterByteOffset = ftell(fileRegister->filePointer);
		readPerson(fileRegister->filePointer, aux);
	}

	destroyEmployeeRegister(aux);

	fileRegister->lastRegisterByteOffSet = lastRegisterByteOffset;

}

void fileToRAM(FILE *binFile, EmployeeArray *employeeArray){

	int arrayIterator = 0;
	int arraySize = employeeArray->totalSize;
	EmployeeRegister *aux = createEmployeeRegister();
	fseek(binFile, PAGE_SIZE, SEEK_SET);
	
	//iterating through the file
	while(!feof(binFile)){
		//if a employee was succesfully read
		if(readPerson(binFile, aux) && aux->removido != '*'){		
			//realloc if necessary
			if(arrayIterator == arraySize){
				arraySize *= 2;
				employeeArray->totalSize = arraySize;
				employeeArray->array = realloc(employeeArray->array, arraySize*sizeof(EmployeeRegister*));
			}
			//add employee to array
			employeeArray->array[arrayIterator] = createEmployeeRegister();
			copyRegisterContent(employeeArray->array[arrayIterator], aux);

			arrayIterator++;
			employeeArray->actualSize++;
		}
	}
}

void mergeSortById(EmployeeArray *employeeArray, int beg, int end){
	//base case
	if(beg >= end) return;
	//setting variables
	int middle = (beg + end)/2,
		itFirstHalf = beg,
		itSecondHalf = middle+1,
		itAux = 0;
		
	//calling recursions
	mergeSortById(employeeArray, beg, middle);
	mergeSortById(employeeArray, middle+1, end);
	
	//allocating the auxiliary array
	EmployeeRegister **aux = (EmployeeRegister **) calloc( end - beg + 1, sizeof( EmployeeRegister *));

	if(aux == NULL) {
		printf("Insuficient Memory");
		return;
	}
	
	//merging
	while(itFirstHalf <= middle && itSecondHalf <= end){
		int idReg1 = employeeArray->array[itFirstHalf]->idServidor,
			idReg2 = employeeArray->array[itSecondHalf]->idServidor;

		if( idReg1 <= idReg2 ){
			aux[itAux++] = employeeArray->array[itFirstHalf++];
		}else{
			aux[itAux++] = employeeArray->array[itSecondHalf++];
		}
	}

	while(itFirstHalf <= middle){
		aux[itAux++] = employeeArray->array[itFirstHalf++];
	}

	while (itSecondHalf <= end){
		aux[itAux++] = employeeArray->array[itSecondHalf++];
	}
	
	//passing the auxiliary content to our array
	for (int i = 0, j = beg; j <= end; j++, i++){
		employeeArray->array[j] = aux[i];
	}

	free(aux);	
}

void RAMToFile(FileRegister *fileRegister, EmployeeArray *employeeArray){
	EmployeeRegister *aux = createEmployeeRegister();
	fseek(fileRegister->filePointer, PAGE_SIZE, SEEK_SET);	
	//iterating through the array
	for (int i = 0; i < employeeArray->actualSize; i++){
		aux = employeeArray->array[i];
		//finding the registers's real size (ignoring the end-of-page trash of old page)
		aux->tamanhoRegistro = findRegisterSizeByFields(aux->nomeServidor, aux->cargoServidor);
		insertEndFile(fileRegister, aux);
	}

	free(aux);
}

EmployeeArray *createEmployeeArray(int size){
	//allocating the array and its components
	EmployeeRegister **aux = (EmployeeRegister **)malloc(size * sizeof(EmployeeRegister *));
	EmployeeArray *e = (EmployeeArray*) malloc(sizeof(EmployeeArray));
	//initializing the array
	if(e != NULL){
		e->actualSize = 0;
		e->array = aux;
		e->totalSize = size;
	}

	return e;
}

void destroyEmployeeArray(EmployeeArray *employeeArray){
	//array-free routine
	for (int i = 0; i < employeeArray->actualSize; i++){
		destroyEmployeeRegister(employeeArray->array[i]);
	}

	free(employeeArray);
}

void writeHeaderFromRam(FILE *file, HeaderRegister *h){
	
	//write the header's content on page
	fwrite(&h->status, sizeof(char), 1, file);
	fwrite(&h->topoLista, sizeof(char), 8, file);
	fwrite(&h->tagCampo1, sizeof(char), 1, file);
	fwrite(&h->desCampo1, sizeof(char), DESCRIPTION_SIZE, file);
	fwrite(&h->tagCampo2, sizeof(char), 1, file);
	fwrite(&h->desCampo2, sizeof(char), DESCRIPTION_SIZE, file);
	fwrite(&h->tagCampo3, sizeof(char), 1, file);
	fwrite(&h->desCampo3, sizeof(char), DESCRIPTION_SIZE, file);
	fwrite(&h->tagCampo4, sizeof(char), 1, file);
	fwrite(&h->desCampo4, sizeof(char), DESCRIPTION_SIZE, file);
	fwrite(&h->tagCampo5, sizeof(char), 1, file);
	fwrite(&h->desCampo5, sizeof(char), DESCRIPTION_SIZE, file);

	//fills the rest of the page with '@'s
	putCharOnPage(file,'@', PAGE_SIZE - HEADER_SIZE);

}

int isSamePage(long long int byteOffsetA, long long int byteOffsetB){
	long long int bigger = byteOffsetA <= byteOffsetB ? byteOffsetB : byteOffsetA;
	long long int smaller = byteOffsetA <=byteOffsetB ? byteOffsetA : byteOffsetB;
	
	//check if there is an multiple of PAGE_SIZE in [smaller, bigger]
	if((bigger/PAGE_SIZE ) * PAGE_SIZE > smaller ){
		return _FALSE;
	}

	return _TRUE;
}
