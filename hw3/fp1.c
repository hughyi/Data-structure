#include <time.h> // clock_t, clock(), CLOCKS_PER_SEC
#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTICES 10000
#define MAX_EDGES 50000001 // minHeap[0]은 reserved되었으므로 +1
#define HEAP_FULL(n) (n == MAX_EDGES - 1)

typedef struct
{
  int u;      // 정점1
  int v;      // 정점2
  int weight; // weight(가중치)
} element;
element minHeap[MAX_EDGES]; // edge들의 weight를 저장하므로 MAX_EDGES만큼
int n = 0;                  // min heap의 현재 크기를 저장하는 값
int parent[MAX_VERTICES];   // disjoint set의 부모 배열
FILE *outfile;              // 출력 파일 포인터 kruskalAlgo()에서 사용하도록 전역변수 선언

// 함수의 원형 선언
void insertMinHeap(element item, int *n);
element deleteMinHeap(int *n);
int Find(int i);
int weightedUnion(int i, int j);
void kruskalAlgo(int vertexNum, int edgeNum);

int main(int argc, char *argv[])
{
  int vertexNum, edgeNum;
  int i, u, v, weight;
  element node;

  // 인자가 1개가 아닌 경우 예외 처리
  if (argc != 2)
  {
    fprintf(stderr, "usage: ./fp1 input_filename");
    return 1;
  }

  clock_t begin = clock();
  FILE *infile = fopen(argv[1], "r");
  if (NULL == infile) // 파일 없는 경우 예외 처리
  {
    fprintf(stderr, "The input file does not exist.\n");
    return 1;
  }

  else
  {
    fscanf(infile, "%d ", &vertexNum);
    fscanf(infile, "%d ", &edgeNum);
    for (i = 0; i < vertexNum; i++) // 부모 값 -1 로 disjoint set 초기화
      parent[i] = -1;
    for (i = 0; i < edgeNum; i++) // input 파일 읽어서 min heap으로 weight 오름차순 정렬
    {
      fscanf(infile, "%d %d %d ", &u, &v, &weight);
      node.u = u;
      node.v = v;
      node.weight = weight;
      insertMinHeap(node, &n); // n은 전역변수이므로 주소로 넘겨줌
    }
    outfile = fopen("fp1_result.txt", "w"); // outfile w 모드로 열어서 생성
    kruskalAlgo(vertexNum, edgeNum);
  }
  fclose(infile);
  fclose(outfile);
  clock_t end = clock();

  printf("output written to fp1_result.txt.\n");
  printf("running time: %f seconds\n", (double)(end - begin) / CLOCKS_PER_SEC); // clock으로 run time 계산

  return 0;
}

void insertMinHeap(element item, int *n)
{
  int i;
  if (HEAP_FULL(*n))
  {
    fprintf(stderr, "The heap is full.\n");
    exit(1);
  }
  i = ++(*n); // 삽입되는 위치 바라보도록
  // min heap 유지되도록 부모가 크기 더 작도록
  while ((i != 1) && (item.weight < minHeap[i / 2].weight))
  {
    minHeap[i] = minHeap[i / 2];
    i /= 2;
  }
  minHeap[i] = item;
}

element deleteMinHeap(int *n)
{
  int parent, child;
  element item, temp;
  item = minHeap[1];      // 가장 작은 weight를 갖는 element값 저장. minHeap의 [0]은 비어있음
  temp = minHeap[(*n)--]; // heap의 가장 마지막 element값 저장 후 n의 크기 1 감소시킴
  parent = 1;
  child = 2;
  while (child <= *n)
  {
    // 현재 부모의 자식 중 더 작은 자식으로 업데이트
    if ((child < *n) && (minHeap[child].weight > minHeap[child + 1].weight))
      child++;
    // n번째 노드(deleted node)가 더 작으면 반복문 종료
    if (temp.weight <= minHeap[child].weight)
      break;
    // 자식 element로 부모 element 업데이트 시키고, 더 아랫쪽 노드 바라보도록
    minHeap[parent] = minHeap[child];
    parent = child;
    child *= 2;
  }
  minHeap[parent] = temp;
  return item;
}

int Find(int i)
{
  for (; parent[i] >= 0; i = parent[i])
    ;
  return i;
}

int weightedUnion(int i, int j)
{
  int temp = parent[i] + parent[j]; // 합쳐진 부모에 전달할 count field
  if (parent[i] > parent[j])        // parent의 각 값은 음수이므로 부호 반대
  {
    parent[i] = j;
    parent[j] = temp;
  }
  else
  {
    parent[j] = i;
    parent[i] = temp;
  }
}

void kruskalAlgo(int vertexNum, int edgeNum)
{
  int edgeCount = 0, totalCost = 0;
  int set1, set2;
  element node;
  while (edgeCount < vertexNum - 1) // vertexNum - 1까지만 선택되도록 union
  {
    node = deleteMinHeap(&n);
    set1 = Find(node.u);
    set2 = Find(node.v);
    if (set1 != set2) // cycle 안만들면
    {
      weightedUnion(set1, set2); // 추가
      fprintf(outfile, "%d %d %d\n", node.u, node.v, node.weight);
      totalCost += node.weight;
      edgeCount++;
    }
    if (edgeCount == edgeNum) // edgeNum이 vertexNum - 1보다 작은 경우 예외처리
      break;
  }
  fprintf(outfile, "%d\n", totalCost);
  if (edgeCount < vertexNum - 1)
    fprintf(outfile, "DISCONNECTED\n");
  else
    fprintf(outfile, "CONNECTED\n");
}
