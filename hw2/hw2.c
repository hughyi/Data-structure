#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
  struct node *prev;
  int data;
  struct node *next;
} NODE;
typedef NODE *listPointer;
listPointer head = NULL;

// 함수의 원형 선언
void print_forward(listPointer list);
void print_reverse(listPointer list);
void Insert(int key);
void Delete(int key);

int main(int argc, char *argv[])
{
  char line[1024];
  char *token;
  int key;

  // 인자가 1개가 아닌 경우 예외 처리
  if (argc != 2)
  {
    fprintf(stderr, "usage: ./hw2 input_filename");
    return 1;
  }

  FILE *infile = fopen(argv[1], "r");
  if (NULL == infile) // 파일 없는 경우 예외 처리
  {
    fprintf(stderr, "The input file does not exist.\n");
    return 1;
  }
  else
  {
    // outfile w 모드로 열어서 생성하고 바로 닫기 - 이후에 사용
    FILE *outfile = fopen("hw2_result.txt", "w");
    fclose(outfile);
    while (feof(infile) == 0) // 파일 끝까지 반복하도록
    {
      fgets(line, sizeof(line), infile); // 첫줄부터 한줄씩 읽어서 line에 저장
      token = strtok(line, " \n");       //첫 공백문자 전까지 문자열 token에 저장
      if (strcmp(token, "INSERT") == 0)
      {
        token = strtok(NULL, " \n");
        key = atoi(token); // atoi 함수로 문자열 정수로 바꿔주기
        Insert(key);
      }
      else if (strcmp(token, "DELETE") == 0)
      {
        token = strtok(NULL, " \n");
        key = atoi(token);
        Delete(key);
      }
      else if (strcmp(token, "ASCEND") == 0)
        print_forward(head);
      else if (strcmp(token, "DESCEND") == 0)
        print_reverse(head);
    }
  }
  return 0;
}

void Insert(int key)
{
  listPointer newnode = (listPointer)malloc(sizeof(NODE));
  newnode->data = key;
  newnode->prev = newnode;
  newnode->next = newnode;
  listPointer curr = head;
  listPointer checker = head;

  if (head == NULL) // 노드없을 때는 새 노드를 head로
  {
    head = newnode;
    return;
  }
  else
  {
    do // linked list 내에 duplicate 있는지 체크
    {
      if (checker->data == key)
      {
        free(newnode);
        return;
      }
      checker = checker->next;
    } while (checker != head);

    if (head->data > newnode->data) // head가 새 노드보다 클 때는 새 노드를 head로
    {
      newnode->next = head;
      newnode->prev = head->prev;
      head->prev->next = newnode;
      head->prev = newnode;
      head = newnode;
    }
    else // 새 노드가 기존의 노드들보다 큰 경우 알맞은 위치에 삽입
    {
      while (curr->next != head && curr->next->data < newnode->data)
        curr = curr->next;
      newnode->next = curr->next;
      newnode->next->prev = newnode;
      curr->next = newnode;
      newnode->prev = curr;
    }
  }
}

void Delete(int key)
{
  listPointer curr = head;
  if (head == NULL) // 노드가 비어있으면 리턴
    return;
  else
  {
    do // 모든 노드 탐색하면서 노드에 값이 key와 같은 경우에만 삭제
    {
      if (curr->data == key)
      {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        free(curr);
      }
      curr = curr->next;
    } while (curr != head);
  }
}

void print_forward(listPointer list)
{
  listPointer curr;
  FILE *outfile;
  outfile = fopen("hw2_result.txt", "a");
  if (list)
  {
    curr = list;
    while (1)
    {
      fprintf(outfile, "%d ", curr->data);
      printf("%d ", curr->data);
      curr = curr->next;
      if (curr == list)
        break;
    }
  }
  fprintf(outfile, "\n");
  printf("\n");
  fclose(outfile);
}

void print_reverse(listPointer list)
{
  listPointer curr;
  FILE *outfile;
  outfile = fopen("hw2_result.txt", "a");
  if (list)
  {
    curr = list->prev;
    while (curr != list)
    {
      fprintf(outfile, "%d ", curr->data);
      printf("%d ", curr->data);
      curr = curr->prev;
    }
    fprintf(outfile, "%d ", curr->data);
    printf("%d ", curr->data);
  }
  fprintf(outfile, "\n");
  printf("\n");
  fclose(outfile);
}
