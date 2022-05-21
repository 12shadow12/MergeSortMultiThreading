#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <vector>

int *arr; // global dynamic array
int split;
int size, limit, p;
pthread_mutex_t myMutex;

void *sort_thread_func(void *arg) {

 size_t i = (size_t)arg;
 int index  = i * split;
 int s;

 if (i == p - 1)
 {
   s = size -((p - 1) * split);
 }
 else
 {
   s = split;
 }

 std::sort(&arr[index], &arr[index + s]);

 pthread_mutex_lock(&myMutex);
 std::cout << "Sorted floor(n/p) numbers: \n";
 for(int i = 0; i < s; i++)
       std::cout << arr[index + i] << " ";
       std::cout << std::endl;
 pthread_mutex_unlock(&myMutex);

 pthread_exit(NULL);
}

void *merge_thread_func(void *arg) {

  size_t i = (size_t)arg;
  if(i % 2 == 1)
   {
     int index  = i * split;
     int s;
     int *temp = new int[size];
     temp = arr;

     if (i == p - 1)
     {
       s = size -((p - 1) * split);
     }
     else
     {
       s = split;
     }

     int previousSegment = index - split;
     std::vector<int> v(split + s);

      std::merge(&arr[previousSegment], &arr[previousSegment + split], &temp[index], &temp[index + s], v.begin());

      pthread_mutex_lock(&myMutex);
      std::cout << "Merged floor(n/p) and floor(n/p) numbers: \n";
      for(std::vector<int>::iterator it = v.begin();  it != v.end(); ++it)
            std::cout << *it << " ";
            std::cout << std::endl;
      pthread_mutex_unlock(&myMutex);
   }

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  size = atoi(argv[1]); // total number of random numbers
  limit = atoi(argv[2]); // range of valid numbers to use
  p = atoi(argv[3]);     // number of splits we can have

  if (limit < 0 || limit > 100)
  {
    std::cout << "Please enter a random number ranging 0-100" << std::endl;
    return 0;
  }
  else if (size < 0 || p < 0)
  {
    std::cout << "Please enter a number greater than 0" << std::endl;
    return 0;
  }

  arr = new int[size];

  srand(time(0));
  std::cout << "Original Array: ";
  for (int i = 0; i < size; i++) {
    arr[i] = rand() % limit;
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  split = floor(size / p);

  /* Initialize the mutex */
  pthread_mutex_init(&myMutex, NULL);

  pthread_t sortThreads[p];
  int thread;

  // Sort Threads
  for (size_t i = 0; i < p; i++){
    thread = pthread_create(&sortThreads[i], NULL, sort_thread_func, (void*)i);

    if(thread)
    {
      std::cout << "System Error: Unable to create a thread. \n";
      exit(-1);
    }
  }

  for (int i = 0; i < p; i++) {
    pthread_join(sortThreads[i], NULL);
  }

  // Merge Threads
  pthread_t mergeThreads[p];

  for (size_t i = 0; i < p; i++)
  {
    thread = pthread_create(&mergeThreads[i], NULL, merge_thread_func, (void*)i);


    if(thread)
    {
      std::cout << "System Error: Unable to create a thread. \n";
      exit(-1);
    }
  }

  for (int i = 0; i < p; i++) {
    pthread_join(mergeThreads[i], NULL);
  }


  std::cout << "Sorted and Merge Array: ";
  for (int i = 0; i < size; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
}
