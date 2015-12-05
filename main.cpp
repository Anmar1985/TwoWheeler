#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "MX28.h"
#include "LinuxCM730.h"
#include "PS3Controller.h"

#include "lidarLite.h"
#include <time.h>
#include <unistd.h>
#include <sys/timeb.h>

#define PROGRAM_VERSION		"v1.00"
#define MAXNUM_INPUTCHAR	(128)
using namespace std;
using namespace Robot;

LinuxCM730 linux_cm730("/dev/ttyUSB0");
CM730 cm730(&linux_cm730);

int gID = CM730::ID_CM;
void sighandler(int sig)
{
	exit(0);
}

int main(int argc,char *argv[])

{
	signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGQUIT, &sighandler);
	signal(SIGINT, &sighandler);
	char cmd[80],cm=0;
	char direction = 'N';
//	int GyroZ,GyroX,GyroY,error;
	int fd, res;
   	struct timeb t_start, t_current;
  	int t_diff;
	float degree = 0,distance=0,tmp,speed = 100.0/12 ,turn = 60.0;
	printf( "\n Two Wheels Control %s]\n", PROGRAM_VERSION);

	if(cm730.Connect() == true)
	{
		cm730.WriteWord(5, 24, 1, 0);
		cm730.WriteWord(6, 24, 1, 0);
		cm730.WriteWord(5, 6, 0, 0);
		cm730.WriteWord(6, 8, 0, 0);
		cm730.WriteWord(5, 6, 0, 0);
		cm730.WriteWord(6, 8, 0, 0);
		cout << "connected";
		while (PS3Controller_Start() == 0){

		cout << "Try again!!";
		}
		cout << "Controller Connected";
		ftime(&t_start);
		
		while(1)
			{
			if ((PS3.key.LJoyX-128 > 1) || (PS3.key.LJoyY-128 > 1 ))
				{
				cout << " X= " << PS3.key.LJoyX-182 << "  ";
				cout << " Y= " << PS3.key.LJoyY-128 << "  ";
				}
			if (PS3.key.Down == 1)
			{	
                                cm730.WriteWord(5, 32, 400, 0);
                                cm730.WriteWord(6, 32, 1424, 0);
				ftime(&t_start);
				direction = 'B';
			}
			else if (PS3.key.Up == 1)
			{
                                cm730.WriteWord(5, 32, 1424, 0);
                                cm730.WriteWord(6, 32, 400, 0);
				ftime(&t_start);
				direction = 'F';
			}
			else if (PS3.key.Left ==1)
			{
                                cm730.WriteWord(5, 32, 1424, 0);
                                cm730.WriteWord(6, 32, 1424, 0);
				ftime(&t_start);
				direction = 'L';
			}
			else if(PS3.key.Right == 1)
			{
                                cm730.WriteWord(5, 32, 400, 0);
                                cm730.WriteWord(6, 32, 400, 0);
				ftime(&t_start);
				direction = 'R';
			}
			else if(PS3.key.L1 == 1)
                        {
                                cm730.WriteWord(5, 32, 0, 0);
                                cm730.WriteWord(6, 32, 0, 0);
				direction = 'N';
                        }

                       // gyroz = cm730.ReadWord(200,38, &GyroZ, &error);
                       // gyrox = cm730.ReadByte(200,40, &GyroX, &error);
                       // gyroy = cm730.ReadWord(200,42, &GyroY, &error);
			if(strcmp(cmd, "exit") == 0)
				{
				cm730.WriteWord(5, 32, 0, 0);
				cm730.WriteWord(6, 32, 0, 0);
				break;}
			else if(strcmp(cmd, "f") == 0)
			{	cout << "Forward\n";
				cm730.WriteWord(5, 32, 400, 0);
				cm730.WriteWord(6, 32, 400, 0);

			}
			else if(strcmp(cmd, "b") == 0)
			{	cout << "Backword\n";
				cm730.WriteWord(5, 32, 1424, 0);
				cm730.WriteWord(6, 32, 1424, 0);

			}
			else if(strcmp(cmd, "l") == 0)
			{	cout << "Left\n";
				cm730.WriteWord(5, 32, 400, 0);
				cm730.WriteWord(6, 32, 1424, 0);

			}
			else if(strcmp(cmd, "r") == 0)
			{	cout <<"Right\n";
				cm730.WriteWord(5, 32, 1424, 0);
				cm730.WriteWord(6, 32, 400, 0);
			}
			else if (cm == 5)
			{
				cout << "Stop\n";
				cm730.WriteWord(5, 32, 0, 0);
				cm730.WriteWord(6, 32, 0, 0);
			}
			//if (argc > 1)
			//	del = atoi(argv[1]);
			//else del=10;
    			    fd = lidar_init(false);
    			if (fd == -1) {
       			 printf("initialization error\n");
        		}
    			else {
          			res = lidar_read(fd);
            			//st = lidar_status(fd);
            			printf(" LIDAR Distance =  %3.0d cm ", res);
				//printf( "Gyro Z = %d Gyro Y = %d  Gyro X = %d RightWheel = %d LeftWheel = %d " , GyroZ,GyroY,GyroX,RightWheel,LeftWheel);
            			//usleep(del);
			        ftime(&t_current);
				t_diff = (int) (1000.0 * (t_current.time - t_start.time)+ (t_current.millitm - t_start.millitm)); 
				tmp = t_diff/1000.0;
				if ((direction == 'R') || (direction =='L')){
				    if (t_diff>6000)
					ftime(&t_start);
				    degree = turn * tmp;
				   }
				else if (direction =='F'){
				 	distance = speed * tmp;
					}
				else if (direction =='B'){
                                        distance = -speed * tmp;
					}
				else if (direction =='N'){
					ftime(&t_start);
					distance = distance;
					}
				printf( "  Distance = %f         Degree = %f          ", distance, degree);
				printf("      \r ");
				fflush(stdout);

        		     }
			}
	}
	else
		printf("Failed to connect CM-730!");

	printf("\nTerminated DXL Manager.\n");
	return 0;
}
