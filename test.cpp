#include<stdio.h>
#include <iostream>

using namespace std;
int main ()
{
   // for loop execution
    for( int a = 1; a < 250; a = a + 1 )
     {
      printf("\r[%3d%%]",a);
      printf("\r");
     }
   return 0;
}
