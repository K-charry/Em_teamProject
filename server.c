/* FPGA Text LCD Test Application
File : fpga_test_text_lcd.c*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <netinet/in.h>
#include <pthread.h>
#define PORT 8080

#define MAX_BUFF 32
#define LINE_BUFF 16
#define FPGA_TEXT_LCD_DEVICE "/dev/fpga_text_lcd"
#define FPGA_STEP_MOTOR_DEVICE "/dev/fpga_step_motor"

void read_plate();
void soc();

int main(void) {
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	int str_size;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed\n");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0)
	//    	{
	//        perror("accept");
	//        exit(EXIT_FAILURE);
	//    	}

	int i;
	int dev; // text lcd
	int dev2; //  motor

	int motor_action;
	int motor_direction = 1;
	int motor_speed = 15;

	unsigned char motor_state[3];

	FILE* fp = fopen("./car_number.txt", "r");
	if (fp == NULL) {
		printf("car_number.txt open error\n");
		return 1;
	}
	char result[32]; // 마지막줄 내용
	char* end_line; // 파일의 마지막줄 반환받을 포인터
	int count = 0;  // 개행수 세는 변수



	while (strcmp(buffer, "exit")) //while문  시작
	{
		int check=0;
		if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		valread = read(new_socket, buffer, 1024);
		memset(result, 0, sizeof(result));
		while (!feof(fp)) {
			fgets(result, 32, fp);
			if (strstr(result,buffer)!=NULL) {
				printf("%s\n", result);
				check = 1;
				break;
			}
		}
		fclose(fp);
		if (check != 1) {
			continue;
		}
		dev = open(FPGA_TEXT_LCD_DEVICE, O_WRONLY);
		if (dev < 0) {
			printf("Device open error : %s\n", FPGA_TEXT_LCD_DEVICE);
			return -1;
		}

		for (int i = 0; i < 16; i++) {

			buffer[i + 16] = buffer[i];
		}
		str_size = strlen(buffer);
		memset(buffer+str_size,' ',LINE_BUFF-str_size);
		memset(buffer+str_size+16,' ',LINE_BUFF-str_size);
		write(dev, buffer, MAX_BUFF);

		motor_state[0] = (unsigned char)motor_action;
		motor_state[1] = (unsigned char)motor_direction;;
		motor_state[2] = (unsigned char)motor_speed;

		dev2 = open(FPGA_STEP_MOTOR_DEVICE, O_WRONLY);
		if (dev2 < 0) {
			printf("Device open error : %s\n", FPGA_STEP_MOTOR_DEVICE);
			exit(1);
		}

		int endTime = (unsigned)time(NULL); // 끝나는 시간
		endTime += 5; //5초

		while (1) {
			motor_state[0] = 1;
			int startTime = (unsigned)time(NULL); // 현재시간(while문을 통해 점점 늘어나는 시간
			write(dev2, motor_state, 3);
			//printf("%d초\n", endTime - startTime); // 시간표시

			if (endTime - startTime == 0) { //시간이 0초면 종료
				motor_state[0] = 0;
				write(dev2, motor_state, 3);
				close(dev2);
				break;
			}

		}
		FILE* fp2 = fopen("./welcome.txt", "r");
		if (fp2 == NULL) {
			printf("welcome.txt open error\n");
			return 1;
		}
		while (!feof(fp2)) {
			if (fgets(result, 32, fp2) != NULL) {
				for (int i = 0; i < 15; i++)
				{
					result[i + 16] = result[i];
				}
				printf("%s\n", result);
			}
		}

		//dev = open(FPGA_TEXT_LCD_DEVICE, O_WRONLY);
		if (dev < 0) {
			printf("Device open error : %s\n", FPGA_TEXT_LCD_DEVICE);
			return -1;
		}
		str_size = strlen(result);
                memset(result+str_size,' ',LINE_BUFF-str_size);
		memset(result+str_size+16,' ',LINE_BUFF-str_size);

		write(dev, result, MAX_BUFF);
		
		close(dev);
		
		return 0;

	}//while문 닫는 부분
	usleep(3000000);
	return(0);
}
