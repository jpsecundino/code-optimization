#ifndef LISTA_ORDENADA
#define LISTA_ORDENADA

typedef struct _EmployeeRegister EmployeeRegister;
typedef union _Options Options;
typedef struct{
	EmployeeRegister *employeeRegister;
	long long int byteOffSet;
}Info;

typedef struct node{
	Info employeeInfo;
	struct node *next;
	struct node *prev;
	int discovery;
} NODE;

typedef struct{
	NODE *begin, *end;
	int sz;
} LIST;

/**
Allocates a list
@Return:
	pointer to a list
*/
LIST* createList();

/**
Inserts a element in an ordered way
@Arguments:
	-l: pointer to a list
@Return:
	none
*/
void insertOrderedList(LIST *l, EmployeeRegister *x, long long int byteOffSet, int discovery);

/**
Insert elements in the list's end
@Arguments:
	-l: pointer to a list
	- x:pointer to an employee register to be added in list
	- byteOffSet: register's byte offset in file
@Return:
	none
*/
void insertEndList(LIST *l, EmployeeRegister *x, long long int byteOffSet);

/**
Search by elements that contain the specified property
@Arguments:
	-l: pointer to a list
	- o: union containing the specific content to be searched
	- option: indicates the field to be searched for
@Return:
	NULL - this property doesn't appears in the list
	LIST* - A pointer to a list with the elements that have this property otherwise

*/
LIST* findInList(LIST *l, Options o, int option);

/**
Search for a element that is greater or equal to another element
@Arguments:
	-l: pointer to a list
	-e: element to be compared with
@Return:
*/
NODE *greaterEqualRegisterInList(LIST *l, EmployeeRegister *e);

/**
Updates a element
@Arguments:
	- correctPlace: the node to be updated
	- e: the register that will fill the element
@Return:
	return the new register's byte offset
*/
long long int replaceInfoInList(NODE *correctPlace, EmployeeRegister *e);

/**
Remove a node from list
@Arguments:
	-l: pointer to a list
	- x: node to be removed
@Return:
*/
int removeElemByRef(LIST *l, NODE* x);

/**
@Arguments:
	-l: pointer to a list
@Return:
	1 - the list is empty
	0 - otherwise
*/
int isListEmpty(LIST *l);

/**
Deallocs a list
@Arguments:
	-l: pointer to a list
@Return:
	none
*/
void destroyList(LIST *l);

/**
Print list in stdout
@Arguments:
	-l: pointer to a list
@Return:
	none
*/
void showList(LIST *l);

#endif