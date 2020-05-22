#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

#define ROW_SIZE 10
#define COLUMN_SIZE 10
#define DEBUG false

typedef struct 
{
    int **matrix;
    int rowSize,
        columnSize;
} Matrix;

Matrix *allocate_matrix(int num_row, int num_collumn, bool isZeroed)
{

    Matrix *M = malloc(sizeof(Matrix *));
    
    M->matrix = malloc(num_row * sizeof(int*));

    for (int i = 0; i < num_row; i++)
    {
        M->matrix[i] = malloc(num_collumn * sizeof(int));
        
        for (int k = 0; k < num_collumn; k++)   
        {
            if(isZeroed == true)
            {
                M->matrix[i][k] = 0;
            }else
            {
                M->matrix[i][k] = rand() % 5;
            }
        }
        
    }

    M->rowSize = num_row;
    M->columnSize = num_collumn;

    return M; 

}

void deallocate_matrix(Matrix *M)
{

    for (int i = 0; i < M->rowSize; i++)
    {
        free(M->matrix[i]);
    }

    free(M->matrix);
    

}


void print_matrix(Matrix *M)
{
    for (int i = 0; i < M->rowSize; i++)
    {
        for (int j = 0; j < M->columnSize; j++)
        {
            printf("%d ", M->matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    printf("\n");
    
}

Matrix *multMatrix(Matrix *M1, Matrix *M2)
{

    Matrix *result = allocate_matrix(M1->rowSize, M2->columnSize, true);

    for (int i = 0; i < M1->rowSize; i++)
    {
        for (int j = 0; j < M2->columnSize; j++)
        {
            for (int k = 0; k < M1->columnSize; k++)
            {
                result->matrix[i][j] += M1->matrix[i][k] * M2->matrix[k][j]; 
            }   
        }        
    }
    
    return result;

}

Matrix *interchangeMultMatrix(Matrix *M1, Matrix *M2)
{

    Matrix *result = allocate_matrix(M1->rowSize, M2->columnSize, true);

    for (int i = 0; i < M1->rowSize; i++)
    {
        for (int k = 0; k < M1->columnSize; k++)
        {
            for (int j = 0; j < M2->columnSize; j++)
            {
                result->matrix[i][j] += M1->matrix[i][k] * M2->matrix[k][j]; 
            }   
        }        
    }
    
    return result;

}

Matrix *UnrollingMultMatrix(Matrix *M1, Matrix *M2)
{

    Matrix *result = allocate_matrix(M1->rowSize, M2->columnSize, true);

    for (int i = 0; i < M1->rowSize; i++)
    {
        for (int j = 0; j < M2->columnSize; j++)
        {
            for (int k = 0; k < M1->columnSize; k+=2)
            {
                result->matrix[i][j] += M1->matrix[i][k] * M2->matrix[k][j]; 
                result->matrix[i][j] += M1->matrix[i][k+1] * M2->matrix[k+1][j]; 
            }   
        }        
    }
    
    return result;

}




int main(int argc, char *argv[]) {
    
    Matrix *M1, *M2, *M3;
    char *p;
    
    if(argc < 2){
        printf("Select the multiplication mode. ./program mode\n");
        printf("If you want to set the size of matrices try send arguments by command line ./program mode M1_num_rows M1_num_columns M2_num_rows M2_num_columns\n");
        return 0;
    }

    if(argc == 6)
    {
        int M1r = strtol(argv[2],NULL,10),
            M1c = strtol(argv[3],NULL,10),
            M2r = strtol(argv[4],NULL,10),
            M2c = strtol(argv[5],NULL,10);

        M1 = allocate_matrix(M1r, M1c, false);
        M2 = allocate_matrix(M2r, M2c, false);
        
        printf("Using custom params M1[%s][%s] and M2[%s][%s]\n", argv[2], argv[3], argv[4], argv[5]);
    }else
    {
        M1 = allocate_matrix(ROW_SIZE, COLUMN_SIZE, false);
        M2 = allocate_matrix(ROW_SIZE, COLUMN_SIZE, false);
        printf("Using default params: M1[%d][%d] and M2[%d][%d]\n", ROW_SIZE, COLUMN_SIZE, ROW_SIZE, COLUMN_SIZE);
    }

    switch (strtol(argv[1], NULL, 10))
    {
        case 0:
            M3 = multMatrix(M1, M2);
            break;
        case 1:
            M3 = interchangeMultMatrix(M1, M2);
            break;
        case 2:
            M3 = UnrollingMultMatrix(M1, M2);
            break;
        default:
            break;
    }
    
    if(DEBUG)
    {
        print_matrix(M1);
        print_matrix(M2);
    }

    
    if (DEBUG)
    {
        print_matrix(M3);
    }
    
    deallocate_matrix(M1);
    deallocate_matrix(M2);
    deallocate_matrix(M3);

    return 0;
}