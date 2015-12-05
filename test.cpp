#include <stdio.h>
#include <iostream>
#include <sys/timeb.h>                                        /* 3 */
#include <unistd.h>                                                              /* 4 */
int main(void)                                               /* 5 */
{                                                             /* 6 */
   struct timeb t_start, t_current;                           /* 7 */
   int t_diff;
   float tmp;                                                /* 8 */
   ftime(&t_start);                                           /* 9 */
	usleep(540000);
      ftime(&t_current);                                      /* 13 */
      t_diff = (int) (1000.0 * (t_current.time - t_start.time)/* 14 */
        + (t_current.millitm - t_start.millitm));             /* 15 */

	std::cout<< t_diff << std::endl;

	tmp = 128.0 / 1000;
	printf("tmp = %f",tmp);

	return 0;                                      /* 17 */


}                                                             /* 18 */


