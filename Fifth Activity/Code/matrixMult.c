#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<x86intrin.h>

#define ROW_SIZE 10
#define COLUMN_SIZE 10
#define DEBUG false
#define DATATYPE float
#define TILE_SIZE 32

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)
#define LARGEST_CACHE_SZ (3 * MB)

static unsigned char dummy_buffer[LARGEST_CACHE_SZ];

void cache_clean()
{
    unsigned long long i;
    
    for (i=0; i<LARGEST_CACHE_SZ; i++)
        dummy_buffer[i] += 1;
}

typedef struct 
{
    DATATYPE **matrix;
    int rowSize,
        columnSize;
} Matrix;

Matrix *allocate_matrix(int num_row, int num_collumn, bool isZeroed)
{

    Matrix *M = malloc(sizeof(Matrix *));
    
    M->matrix = malloc(num_row * sizeof(DATATYPE*));

    for (int i = 0; i < num_row; i++)
    {
        M->matrix[i] = malloc(num_collumn * sizeof(DATATYPE));
        
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
            printf("%lf ", M->matrix[i][j]);
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

Matrix *blockMultMatrixNormal(Matrix *M1, Matrix *M2)
{

    Matrix *result = allocate_matrix(M1->rowSize, M2->columnSize, true);
    int size = M1->columnSize;
    int x,y,z,i,j,k;
    for(x = 0; x < size; x += TILE_SIZE){

        for(y = 0; y < size; y += TILE_SIZE){

            for(z = 0; z < size; z += TILE_SIZE){

                for(i = x; i < (x + TILE_SIZE); i++){

                    for(k = z; k < (z + TILE_SIZE); k++){
                        
                        for(j = y; j < (y + TILE_SIZE); j++){

                            result->matrix[i][j] += M1->matrix[i][k] * M2->matrix[k][j];  
                        }
                    }
                }
            }
        }
    }


    return result;

}

Matrix *blockMultMatrix(Matrix *M1, Matrix *M2)
{

    Matrix *result = allocate_matrix(M1->rowSize, M2->columnSize, true);
    int size = M1->columnSize;
    int x,y,z,i,j,k;
    for(x = 0; x < size; x += TILE_SIZE){

        for(y = 0; y < size; y += TILE_SIZE){

            for(z = 0; z < size; z += TILE_SIZE){

                for(i = x; i < (x + TILE_SIZE); i++){

                    for(k = z; k < (z + TILE_SIZE); k+=4){
                        
                        for(j = y; j < (y + TILE_SIZE); j++){

                            result->matrix[i][j] += M1->matrix[i][k] * M2->matrix[k][j]; 
                            result->matrix[i][j] += M1->matrix[i][k+1] * M2->matrix[k+1][j]; 
                            result->matrix[i][j] += M1->matrix[i][k+2] * M2->matrix[k+2][j]; 
                            result->matrix[i][j] += M1->matrix[i][k+3] * M2->matrix[k+3][j]; 
                        }
                    }
                }
            }
        }
    }


    return result;

}

Matrix *blockMultMatrixVec(Matrix *M1, Matrix *M2)
{

    Matrix *result = allocate_matrix(M1->rowSize, M2->columnSize, true);
    __m128 A, B, C, prod;
    
    int size = M1->columnSize;
    int x,y,z,i,j,k;

    for(x = 0; x < size; x += TILE_SIZE){
        for(y = 0; y < size; y += TILE_SIZE){
            for(z = 0; z < size; z += TILE_SIZE){
                for(i = x; i < x + TILE_SIZE; i++){
                    for(k = z; k < z + TILE_SIZE; k++){
                        
                        A = _mm_set1_ps(M1->matrix[i][k]);
                        
                        for(j = y; j < y + TILE_SIZE; j+=4){
                            B = _mm_load_ps(&(M2->matrix[k][j]));//load B row
                            C = _mm_load_ps(&(result->matrix[i][j]));//Load actual C row values
                            prod = _mm_mul_ps(A, B);// Calculates the product between A item by B 
                            prod = _mm_add_ps(prod,C);// Calculates the product between A item by B row
                            _mm_store_ps(&(result->matrix[i][j]), prod);//store the new values to C, summing it to the last product
                        }
                    }
                }
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

Matrix *UnrIntMultMatrix(Matrix *M1, Matrix *M2)
{

    Matrix *result = allocate_matrix(M1->rowSize, M2->columnSize, true);

    for (int i = 0; i < M1->rowSize; i++)
    {
        for (int k = 0; k < M1->columnSize; k+=4)
        {
            for (int j = 0; j < M2->columnSize; j++)
            {
                result->matrix[i][j] += M1->matrix[i][k] * M2->matrix[k][j]; 
                result->matrix[i][j] += M1->matrix[i][k+1] * M2->matrix[k+1][j]; 
                result->matrix[i][j] += M1->matrix[i][k+2] * M2->matrix[k+2][j]; 
                result->matrix[i][j] += M1->matrix[i][k+3] * M2->matrix[k+3][j]; 
            }   
        }        
    }
    
    return result;

}

//based on normal multiplication
Matrix *multMatrix_intrinsics_v1(Matrix *M1, Matrix *M2)
{

    Matrix *result = allocate_matrix(M1->rowSize, M2->columnSize, true);
    
    __m128 m1, m2, prod, sum;
    
    float res, res_partial;
    
    for (int i = 0; i < M1->rowSize; i++)
    {
        for (int j = 0; j < M2->columnSize; j++)
        {
            res = 0;
            for (int k = 0; k < M1->columnSize; k += 4)
            { 
                m1 = _mm_load_ps(&(M1->matrix[i][k]));
                m2 = _mm_set_ps(M2->matrix[k+3][j], M2->matrix[k + 2][j], M2->matrix[k + 1][j], M2->matrix[k][j]);
                prod  = _mm_mul_ps(m1, m2);
                sum = _mm_hadd_ps(prod,prod);
                sum = _mm_hadd_ps(sum,sum);
                _mm_store_ss(&res_partial, sum);
                
                res += res_partial; 

            }   
            result->matrix[i][j] = res;
        }        
    }
    
    return result;

}

//C = A x B
//based on interchange
Matrix *multMatrix_intrinsics_v2(Matrix *M1, Matrix *M2)
{

    Matrix *result = allocate_matrix(M1->rowSize, M2->columnSize, true);

    __m128 A, B, C, prod;

    for (int i = 0; i < M1->rowSize; i++)
    {
        for (int k = 0; k < M1->columnSize; k++)
        {

            A = _mm_set1_ps(M1->matrix[i][k]);

            for (int j = 0; j < M2->columnSize; j+=4)
            {
                B = _mm_load_ps(&(M2->matrix[k][j]));//load B row
                C = _mm_load_ps(&(result->matrix[i][j]));//Load actual C row values
                prod = _mm_mul_ps(A, B);// Calculates the product between A item by B 
                prod = _mm_add_ps(prod,C);// Calculates the product between A item by B row
                _mm_store_ps(&(result->matrix[i][j]), prod);//store the new values to C, summing it to the last product

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

    cache_clean();

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
        case 3:
            M3 = multMatrix_intrinsics_v1(M1, M2);
            break;
        case 4:
            M3 = multMatrix_intrinsics_v2(M1, M2);
            break;
        case 5:
            M3 = UnrIntMultMatrix(M1, M2);
            break;
        case 6:
            M3 = blockMultMatrix(M1, M2);
            break;
        case 7:
            M3 = blockMultMatrixVec(M1, M2);
            break;
        case 8:
            M3 = blockMultMatrixNormal(M1, M2);
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