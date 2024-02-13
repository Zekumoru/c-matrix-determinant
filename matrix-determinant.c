#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

typedef struct Matrix
{
  int **elements;
  size_t row;
  size_t col;
} Matrix;

Matrix *createMatrix(int row, int col);
void loadMatrix(Matrix *matrix, const char data[]);
void printMatrix(Matrix *matrix);
void freeMatrix(Matrix *matrix);

int main()
{

  Matrix *matrix = createMatrix(4, 4);
  loadMatrix(matrix, "0, 1, 2, 3\n2, 2, 3, 4\n3, 4, 2, 4\n5, 1, 0, 3");
  printMatrix(matrix);
  freeMatrix(matrix);
  return 0;
}

Matrix *createMatrix(int row, int col)
{
  Matrix *matrix = malloc(sizeof(Matrix));
  matrix->row = row;
  matrix->col = col;
  matrix->elements = malloc(sizeof(int *) * row);
  for (int i = 0; i < row; i++)
    matrix->elements[i] = malloc(sizeof(int) * col);
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
    printf("%-5d", i);
    if (i != (col - 1))
    {
      printf(" | ");
    }
  }
  printf("\n");

  // print header divider
  int dividerLength = strlen("r/c | ") + (5 + strlen(" | ")) * col;
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
      printf("%-5d", matrix->elements[i][j]);
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