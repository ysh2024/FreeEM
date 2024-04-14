#include <sys/time.h>
#include <math.h>
#include <immintrin.h> //__m512i
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10	 // how many pending connections queue will hold

__m512i addr[8][4096];
long long val;
int cnt;

int head[32]={1,2,3,4,5,6,7,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int data[2562];
int chip[128]={1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,1,4,4,4,4,4,4,4,1,5,5,5,5,5,5,5,1,6,6,6,6,6,6,6,1,7,7,7,7,7,7,7,1,8,8,8,8,8,8,8,2,1,2,3,4,5,6,7,2,2,1,4,3,6,5,8,2,3,4,1,2,7,8,5,2,4,3,2,1,8,7,6,2,5,6,7,8,1,2,3,2,6,5,8,7,2,1,4,2,7,8,5,6,3,4,1,2,8,7,6,5,4,3,2};


struct timeval curTime;
long int thres;
long int dur;
long long start;
long long finish;
long long rdLen;
long long sbDur[8];
double SBDUR;
double temp;
long long dataLen=sizeof(data)/sizeof(data[0]);
long long headLen=sizeof(head)/sizeof(head[0]);
int waitus,sleepus;
long int startTime, endTime;

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int server(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	int cnt=1, fourClient[4];

	while(1) {
		if (cnt==1){
			send(fourClient[0], "begin", 5, 0);
			break;
		}
	}
	return 0;
}

void WriteRAM(int array[], long long arrayLen){
for (rdLen=0;rdLen<arrayLen;rdLen++) {
		//printf("%d\n",a[rdLen]);

		switch(array[rdLen]){

			case 0:
				for (val=1;val<SBDUR;val++) {
				    asm volatile(			    
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					: 
					: "r" (addr[0]), "r" (addr[1]), "r" (addr[2]), "r" (addr[3]), "r" (addr[4]), "r" (addr[5]), "r" (addr[6]), "r" (addr[7])
					: "%ymm0", "%ymm1", "%ymm2", "%ymm3");
				}
			break;
			
			case 1:
				for (val=1;val<sbDur[0];val++) {
				    asm volatile(			    
					" clflush (%0) \n"//clear reg
					" clflush (%1) \n"//clear reg
					" clflush (%2) \n"//clear reg
					" clflush (%3) \n"//clear reg
					" clflush (%4) \n"//clear reg
					" clflush (%5) \n"//clear reg
					" clflush (%6) \n"//clear reg
					" clflush (%7) \n"//clear reg
					" vmovdqa %%ymm0, (%0) \n"//read from RAM
					" vmovdqa %%ymm0, (%1) \n"//read from RAM
					" vmovdqa %%ymm0, (%2) \n"//read from RAM
					" vmovdqa %%ymm0, (%3) \n"//read from RAM
					" vmovdqa %%ymm0, (%4) \n"//read from RAM
					" vmovdqa %%ymm0, (%5) \n"//read from RAM
					" vmovdqa %%ymm0, (%6) \n"//read from RAM
					" vmovdqa %%ymm0, (%7) \n"//read from RAM
					: 
					: "r" (addr[0]), "r" (addr[1]), "r" (addr[2]), "r" (addr[3]), "r" (addr[4]), "r" (addr[5]), "r" (addr[6]), "r" (addr[7])
					: "%ymm0", "%ymm1", "%ymm2", "%ymm3");
				}
			break;

			case 2:
				//gettimeofday(&tv_begin,NULL);
				for (val=1;val<sbDur[1];val++) {
					asm volatile(
					" clflush (%0) \n"//clear reg
					" clflush (%1) \n"//clear reg
					" clflush (%2) \n"//clear reg
					" clflush (%3) \n"//clear reg
					" clflush (%4) \n"//clear reg
					" clflush (%5) \n"//clear reg
					" clflush (%6) \n"//clear reg
					" clflush (%7) \n"//clear reg
					" vmovdqa %%ymm0, (%0) \n"//read from RAM
					" vmovdqa %%ymm0, (%1) \n"//read from RAM
					" vmovdqa %%ymm0, (%2) \n"//read from RAM
					" vmovdqa %%ymm0, (%3) \n"//read from RAM
					" vmovdqa %%ymm0, (%4) \n"//read from RAM
					" vmovdqa %%ymm0, (%5) \n"//read from RAM
					" vmovdqa %%ymm0, (%6) \n"//read from RAM
					" vmovdqa %%ymm0, (%7) \n"//read from RAM
					" mfence \n"
					" mfence \n"
					" mfence \n"
					:		
					: "r" (addr[0]), "r" (addr[1]), "r" (addr[2]), "r" (addr[3]), "r" (addr[4]), "r" (addr[5]), "r" (addr[6]), "r" (addr[7])
					: "%ymm0", "%ymm1", "%ymm2", "%ymm3");
					//gettimeofday(&tv_end,NULL);
					//dur=tv_end.tv_usec-tv_begin.tv_usec;
					//if (dur>500000)
						//break;
				}
				//printf("%lld\n",val);
				//printf("S:%ld, E:%ld, dur:%ld\n",tv_begin.tv_usec,tv_end.tv_usec,dur);
			break;

			case 3:
				for (val=1;val<sbDur[2];val++) {
					asm volatile(
					" clflush (%0) \n"//clear reg
					" clflush (%1) \n"//clear reg
					" clflush (%2) \n"//clear reg
					" clflush (%3) \n"//clear reg
					" clflush (%4) \n"//clear reg
					" clflush (%5) \n"//clear reg
					" clflush (%6) \n"//clear reg
					" clflush (%7) \n"//clear reg
					" vmovdqa %%ymm0, (%0) \n"//read from RAM
					" vmovdqa %%ymm0, (%1) \n"//read from RAM
					" vmovdqa %%ymm0, (%2) \n"//read from RAM
					" vmovdqa %%ymm0, (%3) \n"//read from RAM
					" vmovdqa %%ymm0, (%4) \n"//read from RAM
					" vmovdqa %%ymm0, (%5) \n"//read from RAM
					" vmovdqa %%ymm0, (%6) \n"//read from RAM
					" vmovdqa %%ymm0, (%7) \n"//read from RAM
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					: 
					: "r" (addr[0]), "r" (addr[1]), "r" (addr[2]), "r" (addr[3]), "r" (addr[4]), "r" (addr[5]), "r" (addr[6]), "r" (addr[7])
					: "%ymm0", "%ymm1", "%ymm2", "%ymm3");
				}
			break;

			case 4:
				for (val=1;val<sbDur[3];val++) {
					asm volatile(
					" clflush (%0) \n"//clear reg
					" clflush (%1) \n"//clear reg
					" clflush (%2) \n"//clear reg
					" clflush (%3) \n"//clear reg
					" clflush (%4) \n"//clear reg
					" clflush (%5) \n"//clear reg
					" clflush (%6) \n"//clear reg
					" clflush (%7) \n"//clear reg
					" vmovdqa %%ymm0, (%0) \n"//read from RAM
					" vmovdqa %%ymm0, (%1) \n"//read from RAM
					" vmovdqa %%ymm0, (%2) \n"//read from RAM
					" vmovdqa %%ymm0, (%3) \n"//read from RAM
					" vmovdqa %%ymm0, (%4) \n"//read from RAM
					" vmovdqa %%ymm0, (%5) \n"//read from RAM
					" vmovdqa %%ymm0, (%6) \n"//read from RAM
					" vmovdqa %%ymm0, (%7) \n"//read from RAM
					" mfence \n"//11
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					: 
					: "r" (addr[0]), "r" (addr[1]), "r" (addr[2]), "r" (addr[3]), "r" (addr[4]), "r" (addr[5]), "r" (addr[6]), "r" (addr[7])
					: "%ymm0", "%ymm1", "%ymm2", "%ymm3");
				}
			break;

			case 5:
				for (val=1;val<sbDur[4];val++) {
					asm volatile(
					" clflush (%0) \n"//clear reg
					" clflush (%1) \n"//clear reg
					" clflush (%2) \n"//clear reg
					" clflush (%3) \n"//clear reg
					" clflush (%4) \n"//clear reg
					" clflush (%5) \n"//clear reg
					" clflush (%6) \n"//clear reg
					" clflush (%7) \n"//clear reg
					" vmovdqa %%ymm0, (%0) \n"//read from RAM
					" vmovdqa %%ymm0, (%1) \n"//read from RAM
					" vmovdqa %%ymm0, (%2) \n"//read from RAM
					" vmovdqa %%ymm0, (%3) \n"//read from RAM
					" vmovdqa %%ymm0, (%4) \n"//read from RAM
					" vmovdqa %%ymm0, (%5) \n"//read from RAM
					" vmovdqa %%ymm0, (%6) \n"//read from RAM
					" vmovdqa %%ymm0, (%7) \n"//read from RAM
					" mfence \n"//16
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					: 
					: "r" (addr[0]), "r" (addr[1]), "r" (addr[2]), "r" (addr[3]), "r" (addr[4]), "r" (addr[5]), "r" (addr[6]), "r" (addr[7])
					: "%ymm0", "%ymm1", "%ymm2", "%ymm3");
				}
			break;

			case 6:
				for (val=1;val<sbDur[5];val++) {
					asm volatile(
					" clflush (%0) \n"//clear reg
					" clflush (%1) \n"//clear reg
					" clflush (%2) \n"//clear reg
					" clflush (%3) \n"//clear reg
					" clflush (%4) \n"//clear reg
					" clflush (%5) \n"//clear reg
					" clflush (%6) \n"//clear reg
					" clflush (%7) \n"//clear reg
					" vmovdqa %%ymm0, (%0) \n"//read from RAM
					" vmovdqa %%ymm0, (%1) \n"//read from RAM
					" vmovdqa %%ymm0, (%2) \n"//read from RAM
					" vmovdqa %%ymm0, (%3) \n"//read from RAM
					" vmovdqa %%ymm0, (%4) \n"//read from RAM
					" vmovdqa %%ymm0, (%5) \n"//read from RAM
					" vmovdqa %%ymm0, (%6) \n"//read from RAM
					" vmovdqa %%ymm0, (%7) \n"//read from RAM
					" mfence \n"//23
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					: 
					: "r" (addr[0]), "r" (addr[1]), "r" (addr[2]), "r" (addr[3]), "r" (addr[4]), "r" (addr[5]), "r" (addr[6]), "r" (addr[7])
					: "%ymm0", "%ymm1", "%ymm2", "%ymm3");
				}
			break;

			case 7:
				for (val=1;val<sbDur[6];val++) {
					asm volatile(
					" clflush (%0) \n"//clear reg
					" clflush (%1) \n"//clear reg
					" clflush (%2) \n"//clear reg
					" clflush (%3) \n"//clear reg
					" clflush (%4) \n"//clear reg
					" clflush (%5) \n"//clear reg
					" clflush (%6) \n"//clear reg
					" clflush (%7) \n"//clear reg
					" vmovdqa %%ymm0, (%0) \n"//read from RAM
					" vmovdqa %%ymm0, (%1) \n"//read from RAM
					" vmovdqa %%ymm0, (%2) \n"//read from RAM
					" vmovdqa %%ymm0, (%3) \n"//read from RAM
					" vmovdqa %%ymm0, (%4) \n"//read from RAM
					" vmovdqa %%ymm0, (%5) \n"//read from RAM
					" vmovdqa %%ymm0, (%6) \n"//read from RAM
					" vmovdqa %%ymm0, (%7) \n"//read from RAM
					" mfence \n"//33
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"

					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					: 
					: "r" (addr[0]), "r" (addr[1]), "r" (addr[2]), "r" (addr[3]), "r" (addr[4]), "r" (addr[5]), "r" (addr[6]), "r" (addr[7])
					: "%ymm0", "%ymm1", "%ymm2", "%ymm3");
				}
			break;

			case 8:
				for (val=1;val<sbDur[7];val++) {
					asm volatile(		
					" clflush (%0) \n"//clear reg
					" clflush (%1) \n"//clear reg
					" clflush (%2) \n"//clear reg
					" clflush (%3) \n"//clear reg
					" clflush (%4) \n"//clear reg
					" clflush (%5) \n"//clear reg
					" clflush (%6) \n"//clear reg
					" clflush (%7) \n"//clear reg
					" vmovdqa %%ymm0, (%0) \n"//read from RAM
					" vmovdqa %%ymm0, (%1) \n"//read from RAM
					" vmovdqa %%ymm0, (%2) \n"//read from RAM
					" vmovdqa %%ymm0, (%3) \n"//read from RAM
					" vmovdqa %%ymm0, (%4) \n"//read from RAM
					" vmovdqa %%ymm0, (%5) \n"//read from RAM
					" vmovdqa %%ymm0, (%6) \n"//read from RAM
					" vmovdqa %%ymm0, (%7) \n"//read from RAM
					" mfence \n"//46
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"
					" mfence \n"	
					" mfence \n"
					" mfence \n"		
					: 
					: "r" (addr[0]), "r" (addr[1]), "r" (addr[2]), "r" (addr[3]), "r" (addr[4]), "r" (addr[5]), "r" (addr[6]), "r" (addr[7])
					: "%ymm0", "%ymm1", "%ymm2", "%ymm3");
				}
			break;


			default:
			printf("nothing\n");

			break;
		}
		gettimeofday( &curTime,NULL );
		endTime=1000000*curTime.tv_sec+curTime.tv_usec;
		if ((endTime-startTime)>waitus){
			printf("chip:%d,dur:%ld\n",array[rdLen],endTime-startTime);
		}
		while ((endTime-startTime)<waitus){
			gettimeofday( &curTime,NULL );
			endTime=1000000*curTime.tv_sec+curTime.tv_usec;
		}
		startTime=startTime+waitus;
	}
}


void main(){
	int i,j;
	data[0]=0;
	data[dataLen-1]=0;
	for (i=1;i<=dataLen-2;i=i+sizeof(chip)/sizeof(chip[0]))
		for (j=i;j<i+sizeof(chip)/sizeof(chip[0]);j++)
			data[j]=chip[j-i];

	printf("data length:%lld\n",dataLen);

	input:
	printf("input	1:head	2:data	3:quit ");
	scanf("%d", &cnt);
	if (cnt==1){

	}
	else if (cnt==2){
		SBDUR=8000;//120=1024
		sbDur[0]=(int)2290;//2550
		sbDur[1]=(int)2090;//2400
		sbDur[2]=(int)1960;//2200
		sbDur[3]=(int)1800;//2050
		sbDur[4]=(int)1590;//1850
		sbDur[5]=(int)1420;//1650
		sbDur[6]=(int)1200;//1450
		sbDur[7]=(int)1080;//1250
		waitus=1000;
		sleepus=150;
	}
	else if (cnt==3)
		exit(0);
	else{
		printf("input error!\n");
		goto input;
	}

	//server();
	//usleep (sleepus);
	gettimeofday(&curTime,NULL);
	startTime=1000000 * curTime.tv_sec + curTime.tv_usec;
	//printf("startTime:%ld\n",startTime);
	WriteRAM(data,dataLen);

	SBDUR=1070000;
	sbDur[0]=(int)255000;
	sbDur[1]=(int)240000;
	sbDur[2]=(int)220000;
	sbDur[3]=(int)205000;
	sbDur[4]=(int)185000;
	sbDur[5]=(int)165000;
	sbDur[6]=(int)145000;
	sbDur[7]=(int)125000;
	waitus=100000;
	
	WriteRAM(head,headLen);


	//printf("\nfinish sending");
}
