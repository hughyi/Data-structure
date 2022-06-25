#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR_SIZE 10000000
#define MAX_PAT_SIZE 3000

char pat[MAX_PAT_SIZE];
char str[MAX_STR_SIZE];
int resultArr[MAX_STR_SIZE]; // 매치된 index 결과 담을 배열
int failure[MAX_PAT_SIZE];

void fail() // pat과 str은 전역변수로 선언하여 매개변수 전달x
{
  int i, j, n = strlen(pat);
  failure[0] = -1; // pat 첫 index의 값 -1로 설정
  for (j = 1; j < n; j++)
  {
    i = failure[j - 1]; // i 셋팅
    while ((pat[j] != pat[i + 1]) && (i >= 0))
      i = failure[i];
    if (pat[j] == pat[i + 1])
      failure[j] = i + 1;
    else
      failure[j] = -1;
  }
}

void KMP(int *cnt) // pat과 str은 전역변수로 선언하여 매개변수 전달x
{
  int i = 0, j = 0;
  int lens = strlen(str);
  int lenp = strlen(pat);
  while (i < lens && j < lenp)
  {
    if (str[i] == pat[j])
    {
      i++;
      j++;
    }
    else if (j == 0)
      i++;
    else
      j = failure[j - 1] + 1; // pat분석해서 j위치 재설정
    if (j == lenp)
    {
      resultArr[*cnt] = i - lenp;
      (*cnt)++;
      j = failure[j - 1] + 1; // pat분석해서 j위치 재설정
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

  FILE *p_file3 = fopen("result_kmp.txt", "w");
  if (NULL != p_file3)
  {
    fail();
    KMP(&cnt);
    fprintf(p_file3, "%d\n", cnt);
    for (i = 0; i < cnt; i++)
    {
      fprintf(p_file3, "%d ", resultArr[i]);
    }
  }
  else
  {
    fprintf(stderr, "result_kmp.txt does not exist\n");
    return 0;
  }

  printf("Program complete. Result saved to result_kmp.txt\n");

  fclose(p_file1);
  fclose(p_file2);
  fclose(p_file3);

  return 0;
}
