#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_SIZE 10000000
#define MAX_PAT_SIZE 3000

char pat[MAX_PAT_SIZE];
char str[MAX_STR_SIZE];
int resultArr[MAX_STR_SIZE]; // 매치된 index 결과 담을 배열

void naive(int *cnt)
{
  int i, j, start = 0;
  int lasts = strlen(str) - 1;
  int lastp = strlen(pat) - 1;
  int endmatch = lastp;
  for (i = 0; endmatch <= lasts; endmatch++, start++) // pat의 endmatch가 str의 마지막 글자를 넘는 경우는 고려 x
  {
    if (str[endmatch] == pat[lastp]) // str마지막 글자와 pat의 마지막 글자가 같은 경우에만 따지도록
      for (j = 0, i = start; j < lastp && str[i] == pat[j]; i++, j++)
        ;
    if (j == lastp)
    {
      resultArr[*cnt] = start;
      (*cnt)++;
    }
  }
}

int main()
{

  int i, cnt = 0;
  FILE *p_file1 = fopen("string.txt", "r");
  if (NULL != p_file1)
    fgets(str, MAX_STR_SIZE, p_file1);
  else
  {
    fprintf(stderr, "string.txt does not exist.\n");
    return 0;
  }

  FILE *p_file2 = fopen("pattern.txt", "r");
  if (NULL != p_file2)
    fgets(pat, MAX_PAT_SIZE, p_file2);
  else
  {
    fprintf(stderr, "pattern.txt does not exist.\n");
    return 0;
  }

  FILE *p_file3 = fopen("result_naive.txt", "w");
  if (NULL != p_file3)
  {
    naive(&cnt);
    fprintf(p_file3, "%d\n", cnt);
    for (i = 0; i < cnt; i++)
    {
      fprintf(p_file3, "%d ", resultArr[i]);
    }
  }
  else
  {
    fprintf(stderr, "result_naive.txt does not exist\n");
    return 0;
  }

  printf("Program complete. Result saved to result_naive.txt\n");

  fclose(p_file1);
  fclose(p_file2);
  fclose(p_file3);

  return 0;
}