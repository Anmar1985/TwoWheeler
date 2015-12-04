#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "MX28.h"
#include "LinuxCM730.h"
//#include "cmd_process.h"
#include "PS3Controller.h"

#include "lidarLite.h"
#include <time.h>
#include <unistd.h>

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
	//CM730 *m_cm730;
	//m_CM730 = m_cm730; 
	char input[MAXNUM_INPUTCHAR];
	char *token;
	int input_len;
	char cmd[80];
	char param[20][30];
	int num_param;
	int iparam[20];	
        int gyroz,gyrox,gyroy;
	int GyroZ,GyroX,GyroY,error;
	int fd, res, i, del;
	unsigned char st, ver;


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
		while(1)
			{
			gets(input);
			fflush(stdin);
			input_len = strlen(input);
			if(input_len == 0)
				continue;
			token = strtok( input, " " );
			if(token == 0)
				continue;

			strcpy( cmd, token );
			token = strtok( 0, " " );
			num_param = 0;
			while(token != 0)
			{
				strcpy( param[num_param++], token );
				token = strtok( 0, " " );
			}

			if ((PS3.key.LJoyX-128 > 1) || (PS3.key.LJoyY-128 > 1 ))
				{
				cout << "X= " << PS3.key.LJoyX-182;
				cout << " Y= " << PS3.key.LJoyY-128 << endl;
				}
			if (PS3.key.Up == 1)
			{	cout << "Up";
                                cm730.WriteWord(5, 32, 400, 0);
                                cm730.WriteWord(6, 32, 1424, 0);
			}
			else if (PS3.key.Down == 1)
			{
				cout << "Down";
                                cm730.WriteWord(5, 32, 1424, 0);
                                cm730.WriteWord(6, 32, 400, 0);
			}
			else if (PS3.key.Left ==1)
			{
                                cm730.WriteWord(5, 32, 1424, 0);
                                cm730.WriteWord(6, 32, 1424, 0);
			}
			else if(PS3.key.Right == 1)
			{
                                cm730.WriteWord(5, 32, 400, 0);
                                cm730.WriteWord(6, 32, 400, 0);
			}
			else if(PS3.key.L1 == 1)
                        {
                                cm730.WriteWord(5, 32, 0, 0);
                                cm730.WriteWord(6, 32, 0, 0);
                        }
			//gyroz = cm730.m_BulkReadData[200].ReadWord(38);
			//gyroz = cm730.m_BulkReadData[200].ReadWord(40);
			//gyroz = cm730.m_BulkReadData[200].ReadWord(42);
                        gyroz = cm730.ReadWord(200,38, &GyroZ, &error);
                        gyrox = cm730.ReadByte(200,40, &GyroX, &error);
                        gyroy = cm730.ReadWord(200,42, &GyroY, &error);

			printf( " Gyro Z = %d \n Gyro Y = %d \n Gyro X = %d \n \r" , GyroZ,GyroY,GyroX);
			//printf( "Gyro Y =\r %d \n" , gyroy);
			//printf( "Gyro X =\r %d \1n" , gyrox);
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
			{	cout << "Lef\n";
				cm730.WriteWord(5, 32, 400, 0);
				cm730.WriteWord(6, 32, 1424, 0);

			}
			else if(strcmp(cmd, "r") == 0)
			{	cout <<"Right\n";
				cm730.WriteWord(5, 32, 1424, 0);
				cm730.WriteWord(6, 32, 400, 0);

			}
			else if (strcmp(cmd, "s") == 0)
			{
				cout << "Stop\n";
				cm730.WriteWord(5, 32, 0, 0);
				cm730.WriteWord(6, 32, 0, 0);
			}
			if (argc > 1)
				del = atoi(argv[1]);
			else del=10;
    			    fd = lidar_init(false);
    			if (fd == -1) {
       			 printf("initialization error\n");
        		}
    			else {

            			res = lidar_read(fd);
            			st = lidar_status(fd);
		                //ver = lidar_version(fd);
            			printf("%3.0d cm \n", res);
            		 	//lidar_status_print(st);
            			usleep(del);
        		     }
			}
	}
	else
		printf("Failed to connect CM-730!");

	printf("\nTerminated DXL Manager.\n");
	return 0;
}
