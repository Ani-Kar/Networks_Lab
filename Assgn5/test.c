#include "mysocket.h"
#include <stdio.h>
int* test(){
  int * temp ;
  *temp = 5;
  return temp;
}
void main() 
{
  int * temp = test();
  printf("%d",*temp);
}