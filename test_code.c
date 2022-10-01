#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include"MyStl.h"


typedef struct a {
    int age;
    char* name;
}A;

int cmp(A* a, A* b) {

	if (a->age == b->age && strcmp(a->name, b->name) == 0) {
		return 0;
	}
    if (a->age <= b->age) return -1;

	return 1;
}

long long hashFun(A* a) {
    return llabs((a->age + (long long)a->name)%100);
}

void printFunForMap(char* key, A* a) {
    printf("%s==A{age=%d, name=%s}", key, a->age, a->name);
}

void printFun(A* a) {
    printf("A{age=%d, name=%s}", a->age, a->name);
}

void printInt(int a, int b) {
    printf("key = %d, val = %d", a, b);
}

int main() {

    srand((unsigned int)time(NULL));

    A arr[20] = {{10, "d"},
                {20, "f"},
                {30, "a"},
                {20,"c"},
                {10,"b"},
                {5, "d"},
                {26, "f"},
                {39, "a"},
                {2,"c"},
                {100,"b"},
                {75, "d"},
                {62, "f"},
                {81, "a"},
                {91,"c"},
                {32,"b"},
                {50, "d"},
                {20, "f"},
                {70, "a"},
                {20,"c"},
                {110,"b"}};

    A* arrPtr[5] = {arr, arr+1, arr+2, arr+3, arr+4};

    char* arrStr[5] = {arr[0].name, arr[1].name, arr[2].name, arr[3].name, arr[4].name};

   TreeMap* queue = iniTreeMap();

   int i;
   for (i = 0; i < 50; i++) {
       queue->put(queue, rand(), i + 1);

   }

   queue->printAll(queue);


   Iterator * iterator = queue->getIterator(queue);

    while (iterator->hasNext(iterator)) {
        EntryOfTree en = iterator->next(iterator);
        printf("%d == %d\n", en->key, en->ele);
    }





//   Iterator* iterator = queue->getIterator(queue);
//
//
//    while (iterator->hasNext(iterator)) {
//        EntryOfTree a = iterator->next(iterator);
//        queue->print(queue, a->key, a->ele);
//        printf("\n");
//    }


	return 0;
	
}
