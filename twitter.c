#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define vertex 5128
#define edge 21270
#define threshold 20000

typedef struct Node {
   struct node *next;
   int data;
}Node;
typedef struct List {
   Node *head;
}List;
List *adjlist[vertex] = { 0 };

int fwcount[vertex] = { 0 }; //follow count
int fecount[vertex] = { 0 }; //follower count
int adjmatrix[vertex][vertex] = { 0 };
int distance_adjmatrix[vertex][vertex] = { 0 };     // distance가 담긴 2차원 배열
int undirected_adjmatrix[vertex][vertex] = { 0 };
//variables for solution_4
int mutualmatrix[vertex][vertex] = { 0 };
int DFSvisit[vertex] = { 0 };
int DFSvisit2[vertex] = { 0 };

//sorting
void swap(int* a, int* b) {
   int tmp = *a;
   *a = *b;
   *b = tmp;
}
int compare(void *first, void *second) {
   if (*(int*)first > *(int*)second)
      return 1;
   else if (*(int*)first < *(int*)second)
      return -1;
   else
      return 0;
}
void quick_sort(int* arr1, int* arr2, int start, int end) {
   if (start >= end) return;
   int mid = (start + end) / 2;
   int pivot = arr1[mid];
   swap(&arr1[start], &arr1[mid]);
   swap(&arr2[start], &arr2[mid]);
   int p = start + 1, q = end;
   while (1) {
      while (arr1[p] <= pivot) { p++; }
      while (arr1[q] > pivot) { q--; }
      if (p > q) break;
      swap(&arr1[p], &arr1[q]);
      swap(&arr2[p], &arr2[q]);
   }
   swap(&arr1[start], &arr1[q]);
   swap(&arr2[start], &arr2[q]);
   quick_sort(arr1, arr2, start, q - 1);
   quick_sort(arr1, arr2, q + 1, end);
}
void addNode(int s, int d) //s = head index, d = connecting node
{
   Node *dest, *tmp, *src;
   if (adjlist[s]->head == NULL) {
      src = (Node *)malloc(sizeof(Node));
      src->data = s;
      src->next = NULL;
      adjlist[s]->head = src;
   }
   dest = (Node *)malloc(sizeof(Node));
   dest->data = d;
   dest->next = NULL;
   tmp = adjlist[s]->head;
   while (tmp->next != NULL)
      tmp = tmp->next;
   tmp->next = dest;
}
void showList()
{
   for (int i = 0; i < vertex; i++) {
      Node *p = adjlist[i]->head;
      printf("%d번째 list\n", i);
      while (p)
      {
         printf("%d -> ", p->data);
         p = p->next;
      }
      printf("\n");
   }
   printf("\n");
}

void make_userlist_array(int *arr1, int *arr2, int *user_list) {
   int temp[edge + 4218];  //temp stroage for make user_list
   int count = 0;
   temp[0] = arr1[0];
   for (int i = 1; i < edge; i++) {
      if (temp[count] != arr1[i]) {
         count++;
         temp[count] = arr1[i];
      }
   }
   count++;
   for (int i = 0; i < edge; i++) {
      temp[count] = arr2[i];
      count++;
   }
   qsort(temp, sizeof(temp) / sizeof(temp[0]), sizeof(int), compare);
   int check = 0;
   user_list[0] = temp[0];
   for (int i = 1; i < count; i++) {
      if (user_list[check] != temp[i]) {
         check++;
         user_list[check] = temp[i];
      }
   }
   //int k = 0;
   //for (int i = 0; i < 5128; i++) {
   //   printf("user_list: %d\n", user_list[i]);
   //   k++;
   //}
   //printf("size of list: %d\n", k);
}
void make_adjacencylist(int *arr1, int *arr2, int *user_list) {
   for (int i = 0; i < vertex; i++) {
      adjlist[i] = (List *)malloc(sizeof(List));
      adjlist[i]->head = NULL;
      addNode(i, user_list[i]);
      for (int j = 0; j < edge; j++) {
         if ((user_list[i] == arr1[j]) && (arr2[j - 1] != arr2[j])) { //add new user id
            addNode(i, arr2[j]);
            fwcount[i] += 1; //follow count
            for (int l = 0; l < vertex; l++) {
               if (user_list[l] == arr2[j]) {
                  adjmatrix[i][l] = 1;
                  distance_adjmatrix[i][l] = 1;
                  undirected_adjmatrix[i][l] = 1;
                  undirected_adjmatrix[l][i] = 1;
               }
            }
         }
      }
   }
   //for (int i = 0; i < 10; i++) {
   //   Node *p = adjlist[i]->head;
   //   p = p->next;
   //   printf("%d", p->data);
   //}

   //showList();
   //show adjmatrix
   //for (int i = 0; i < 10; i++)
   //{
   //   for (int j = 0; j < 10; j++) {
   //      printf("%d ", adjmatrix[i][j]);
   //   }
   //   printf("\n");
   //}

}

void DFS(int start, int N) {
   int i;

   DFSvisit[start] = 1;
   for (i = 0; i < N; i++) {
      if (mutualmatrix[start][i] == 1 && mutualmatrix[i][start] == 1
         && DFSvisit[i] == 0) {
         DFS(i, N);
      }
   }
   return;
}

void DFS2(int start, int N) {
   int i;

   DFSvisit2[start] = 1;
   for (i = 0; i <= N; i++) {
      if (undirected_adjmatrix[start][i] == 1 && DFSvisit2[i] == 0) {
         DFS2(i, N);
      }
   }
   return;
}

// solution_1 알고리즘
void solution_1(int *arr1, int *arr2, int *user_list) {
   int k = 0;
   printf("[sol1] \n");
   for (int i = 0; i < vertex; i++) {
      for (int j = 0; j < edge; j++) {
         if (arr2[j] == user_list[i]) fecount[i] += 1;
      }
      printf("id:%d, #n of follow:%d, #n of followee:%d \n", user_list[i], fwcount[i], fecount[i]);
   }
}
// solution_2
void solution_2(int *user_list){
  int max = 0;  // check the maximum distance
  int x, y;     // remember the user and follow
  for (int k = 0 ; k < vertex ; k++){
    for (int i = 0 ; i < vertex ; i++){
      if(k != i && distance_adjmatrix[i][k] >= 1){    // i == k 이거나, distance == 0 이면 볼 필요 없다.
        for (int j = 0 ; j < vertex ; j++){
          if(i != j && distance_adjmatrix[i][j] == 0 && distance_adjmatrix[k][j] >= 1){         // 직접 연결되어 있지 않으며 가는 거리가 설정되어 있지 않으면 무조건 연결
            distance_adjmatrix[i][j] = distance_adjmatrix[i][k] + distance_adjmatrix[k][j];
          }
          else if(i != j && distance_adjmatrix[i][j] > 1 && distance_adjmatrix[k][j] >= 1){     // 직접 연결되어 있지 않으며 가는 거리가 설정되어 있으면 최단거리 연결
            if(distance_adjmatrix[i][j] > distance_adjmatrix[i][k] + distance_adjmatrix[k][j])
              distance_adjmatrix[i][j] = distance_adjmatrix[i][k] + distance_adjmatrix[k][j];
          }
        }
      }
    }
  }
  for (int i = 0; i < vertex; i++) {
     for (int j = 0; j < vertex; j++) {
        if (distance_adjmatrix[i][j] > max) {
           max = distance_adjmatrix[i][j];
           x = i;
           y = j;
        }
     }
  }
  printf("Maximum distance: %d, with %d and %d\n", max, user_list[x], user_list[y]);
}
//solution_3
void solution_3() {
   int cntComponent = 0;
   for (int i = 1; i < vertex; i++) {
      if (DFSvisit2[i] == 0) {
         DFS2(i, vertex);
         cntComponent++;
      }
   }
   printf("\n[sol3] every does not be connect with another one\n");
   printf("because number of partition : %d\n", cntComponent);
}
//solution_4
void solution_4() {
   // make mutualmatrix
   for (int i = 0; i < vertex; i++) {
      for (int j = 0; j < vertex; j++) {
         if (adjmatrix[i][j] == 1 && adjmatrix[j][i] == 1)
            mutualmatrix[i][j] = 1;
      }
      mutualmatrix[i][i] = 1;
   }
   /*
   //show mutualmatrix
   printf("\n");
   for (int i = 0; i < 20; i++)
    {
      for (int j = 0; j < 20; j++) {
         printf("%d ", mutualmatrix[i][j]);
      }
      printf("\n");
    }
    */
    // start
   int cntComponent = 0;
   for (int i = 0; i < vertex; i++) {
      if (DFSvisit[i] == 0) {
         DFS(i, vertex);
         cntComponent++;
      }
   }
   printf("\n[sol4] number of friend partition : %d\n", cntComponent);
}
//solution_5
void solution_5(int *user_list) {
   Node *randomSurfer;
   int random = 0; //random
   int magic = 0; //probability
   int index = 0; //index reference
   int count = 0; //total number (random surfer -> node visit)
   int usercount[vertex] = { 0 }; //count++ when random surfer visit each user
   int userflag[vertex] = { 0 };
   int userrankindex[20] = { 0 };
   int rank[20] = { 0 };
   int rank_index = 0;
   srand(time(NULL));
   //rand run well test(o)
   //int countnum[10] = { 0 };
   //for (int i = 0; i < 10000; i++) {
   //   random = rand() % 10;
   //   countnum[random] += 1;
   //}
   //for (int i = 0; i < 10; i++) {
   //   printf("%d ", countnum[i]);
   //}
   //printf("\n");

   rank_index = 0;
   random = rand() % vertex; //first node position
   index = random;
   randomSurfer = adjlist[index]->head;
   randomSurfer = randomSurfer->next;
   count += 1;
   usercount[index] += 1;

   //이 랜덤값이 진짜 랜덤하게 assign되는지 array 생성해서 확인할것(o)
   while (rank_index < 20) {
      magic = rand() % 10;
      if ((randomSurfer->next == NULL) || magic == 0) { //10% 확률이거나 해당 user의 follow가 없을 경우, 랜덤한 노드로 이동
         random = rand() % vertex;
         index = random;
         randomSurfer = adjlist[index]->head;
         randomSurfer = randomSurfer->next;
         count += 1;
         usercount[index] += 1;
      }
      else { //90%의 확률로 해당 user의 follow 중 하나로 이동
         magic = rand() % fwcount[index]; //해당 user의 follow 리스트 안에서 랜덤하게 이동하기 위해
         Node *p = randomSurfer;
         for (int i = 0; i < magic + 1; i++) {
            p = p->next;
         }
         for (int i = 0; i < vertex; i++) {
            if (user_list[i] == p->data) {
               randomSurfer = adjlist[i]->head;
               randomSurfer = randomSurfer->next;
               index = i;
            }
         }
         count += 1;
         usercount[index] += 1;
      }
      if ((usercount[index] >= threshold) && (userflag[index] == 0)) { //threshold
         rank[rank_index] = user_list[index];
         userrankindex[rank_index] = index;
         rank_index += 1;
         userflag[index] = 1;
      }
   }
   printf("\n[sol5] Pagerank with threshold %d: \n", threshold);
   for (int i = 0; i < 20; i++) {
      printf("rank %d - user%d percentage: %f \n", i+1, rank[i], (double)usercount[userrankindex[i]]/count);
   }
   //for (int i = 0; i < 20; i++) {
   //   printf("user%d percentage: %f \n", user_list[i], (double)usercount[i] / count);
   //}
   //printf("Number of times a random surfer has moved the network: %d", count);
}

//main
int main(void) {
   int arr1[edge]; // A
   int arr2[edge]; // B
   int user_list[vertex];

   //read file
   FILE *fp = fopen("twitter.txt", "r");
   int fcount = 0;
   int num = 0;
   int user[5128] = { 0 };
   while (!feof(fp)) {
      fcount++;
      fscanf(fp, "%d", &num);
      if (fcount % 2 == 1) arr1[fcount / 2] = num;
      else arr2[fcount / 2 - 1] = num;
   }
   fclose(fp);

   //sort
   quick_sort(arr1, arr2, 0, 21269);

   //make userlist array
   make_userlist_array(arr1, arr2, user_list);

   //make adjacencylist
   make_adjacencylist(arr1, arr2, user_list);

   //solution_1
   solution_1(arr1, arr2, user_list);

   //solution_2
   solution_2(user_list);

   //solution_3
   solution_3();

   //solution_4
   solution_4();

   //solution_5
   solution_5(user_list);
}
