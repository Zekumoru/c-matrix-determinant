#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Matrix
{
  double **elements;
  size_t row;
  size_t col;
} Matrix;

Matrix *createMatrix(int row, int col);
void loadMatrix(Matrix *matrix, const char data[]);
void printMatrix(Matrix *matrix);
void freeMatrix(Matrix *matrix);

void swapRowsMatrix(Matrix *matrix, int row1, int row2);
void rowEchelonMatrix(Matrix *matrix);
double determinantMatrix(Matrix *matrix);

int main()
{

  Matrix *matrix = createMatrix(4, 4);
  loadMatrix(matrix, "0, 1, 2, 3\n2, 2, 3, 4\n3, 4, 2, 4\n5, 1, 0, 3");
  printMatrix(matrix);
  double determinant = determinantMatrix(matrix);
  printf("Determinant: %.2lf\n", determinant);
  freeMatrix(matrix);
  return 0;
}

double determinantMatrix(Matrix *matrix)
{
  if (matrix->row != matrix->col)
    return NAN;

  rowEchelonMatrix(matrix);
  double determinant = 1;
  for (int i = 0; i < matrix->row; i++)
    determinant *= matrix->elements[i][i];
  return determinant;
}

void rowEchelonMatrix(Matrix *matrix)
{
  if (matrix->row != matrix->col)
    return;

  for (int col = 0; col < matrix->col; col++)
  {
    // check top-left if not 0
    // if 0 then find not 0 then swap
    for (int row = col; row < matrix->row; row++)
    {
      if (row == col && matrix->elements[row][col] != 0)
        break;
      if (row == col)
        row++;

      if (matrix->elements[row][col] != 0)
      {
        swapRowsMatrix(matrix, col, row);
        break;
      }
    }

    // perform Gaussian elimination
    for (int row = col + 1; row < matrix->row; row++)
    {
      // already zero
      if (matrix->elements[row][col] == 0)
        continue;

      // not zero so do the elimination
      double term = -(matrix->elements[row][col] / matrix->elements[col][col]);
      for (int i = 0; i < matrix->col; i++)
        matrix->elements[row][i] += term * matrix->elements[col][i];
    }
  }
}

void swapRowsMatrix(Matrix *matrix, int row1, int row2)
{
  if (row1 < 0 || row1 >= matrix->row)
    return;
  if (row2 < 0 || row2 >= matrix->row)
    return;

  bool toNegate = (row1 - row2) % 2 != 0;
  for (int i = 0; i < matrix->col; i++)
  {
    double temp = matrix->elements[row1][i];
    if (toNegate)
      temp = temp * (-1);
    matrix->elements[row1][i] = matrix->elements[row2][i];
    matrix->elements[row2][i] = temp;
  }
}

////////////////////////////////////////////////////////////////////////////////

Matrix *createMatrix(int row, int col)
{
  Matrix *matrix = malloc(sizeof(Matrix));
  matrix->row = row;
  matrix->col = col;
  matrix->elements = malloc(sizeof(double *) * row);
  for (int i = 0; i < row; i++)
    matrix->elements[i] = malloc(sizeof(double) * col);
  return matrix;
}

char **getlines(const char str[])
{
  size_t linesSize = 0;
  size_t linesAllocSize = 1;
  char **lines = malloc(linesAllocSize * sizeof(char *));

  for (size_t i = 0, startpos = 0, strlength = strlen(str); i < strlength; i++)
  {
    if (str[i] == '\n' || str[i] == '\r' || i == (strlength - 1))
    {
      // extract line
      int length = i - startpos;
      if (str[i] != '\n' && str[i] != '\r' && i == (strlength - 1)) // EOF
        length++;
      char *line = malloc(length + 1); // +1 for character terminator
      strncpy(line, str + startpos, length);
      startpos = i + 1; // +1 to ignore newline in the next line

      // reallocate lines
      lines[linesSize] = line;
      linesSize++;
      if (linesSize >= linesAllocSize)
      {
        linesAllocSize *= 2;
        lines = realloc(lines, linesAllocSize * sizeof(char *));
      }

      // ignore '\r' if '\n\r'
      if (i != (strlength - 1) && str[i + 1] == '\r')
      {
        i++;
        startpos++;
      }
    }
  }
  lines[linesSize] = NULL;

  return lines;
}

void loadMatrix(Matrix *matrix, const char data[])
{
  char **lines = getlines(data);
  char *line = lines[0];
  for (size_t i = 0; i < matrix->row && line != NULL; ++i, line = lines[i])
  {
    char *token = strtok(line, ", ");
    for (size_t j = 0; j < matrix->col && token != NULL; j++, token = strtok(NULL, ", "))
    {
      matrix->elements[i][j] = atoi(token);
    }
  }
  for (char **pline = lines; (*pline) != NULL; pline++)
    free(*pline);
  free(lines);
}

void printMatrix(Matrix *matrix)
{
  int row = matrix->row;
  int col = matrix->col;

  // print header
  printf("r/c | ");
  for (int i = 0; i < col; i++)
  {
    printf("%-7d", i);
    if (i != (col - 1))
    {
      printf(" | ");
    }
  }
  printf("\n");

  // print header divider
  int dividerLength = strlen("r/c | ") + (7 + strlen(" | ")) * col;
  for (int i = 0; i < dividerLength; i++)
  {
    printf("-");
  }
  printf("\n");

  // print matrix
  for (int i = 0; i < row; i++)
  {
    printf("%-3d | ", i);
    for (int j = 0; j < col; j++)
    {
      double element = matrix->elements[i][j];
      printf("%-7.1lf", (element == 0) ? abs(element) : element);
      if (j != (col - 1))
      {
        printf(" | ");
      }
    }
    printf("\n");
  }

  printf("\n");
}

void freeMatrix(Matrix *matrix)
{
  if (matrix == NULL)
    return;

  for (int i = 0; i < matrix->row; i++)
    free(matrix->elements[i]);
  free(matrix->elements);
  free(matrix);
}