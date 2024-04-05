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
//4 or 3 senders, use chip[130]; 2 senders, use data[258]; 130=0+128+0; 258=0+256+0; 0 is used as guard.

int head[32]={1,2,3,4,5,6,7,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int data[2562];

int chip[128]={1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,1,4,4,4,4,4,4,4,1,5,5,5,5,5,5,5,1,6,6,6,6,6,6,6,1,7,7,7,7,7,7,7,1,8,8,8,8,8,8,8,2,1,2,3,4,5,6,7,2,2,1,4,3,6,5,8,2,3,4,1,2,7,8,5,2,4,3,2,1,8,7,6,2,5,6,7,8,1,2,3,2,6,5,8,7,2,1,4,2,7,8,5,6,3,4,1,2,8,7,6,5,4,3,2};
//int chip[128]={3,1,3,5,7,4,2,8,3,2,4,6,8,3,1,7,3,3,1,7,5,2,4,6,3,4,2,8,6,1,3,5,3,5,7,1,3,8,6,4,3,6,8,2,4,7,5,3,3,7,5,3,1,6,8,2,3,8,6,4,2,5,7,1,4,1,4,7,6,8,5,2,4,2,3,8,5,7,6,1,4,3,2,5,8,6,7,4,4,4,1,6,7,5,8,3,4,5,8,3,2,4,1,6,4,6,7,4,1,3,2,5,4,7,6,1,4,2,3,8,4,8,5,2,3,1,4,7};
//int chip[128]={5,1,5,4,8,7,3,6,5,2,6,3,7,8,4,5,5,3,7,2,6,5,1,8,5,4,8,1,5,6,2,7,5,5,1,8,4,3,7,2,5,6,2,7,3,4,8,1,5,7,3,6,2,1,5,4,5,8,4,5,1,2,6,3,6,1,6,2,5,3,8,4,6,2,5,1,6,4,7,3,6,3,8,4,7,1,6,2,6,4,7,3,8,2,5,1,6,5,2,6,1,7,4,8,6,6,1,5,2,8,3,7,6,7,4,8,3,5,2,6,6,8,3,7,4,6,1,5};
//int chip[128]={7,1,7,8,2,6,4,3,7,2,8,7,1,5,3,4,7,3,5,6,4,8,2,1,7,4,6,5,3,7,1,2,7,5,3,4,6,2,8,7,7,6,4,3,5,1,7,8,7,7,1,2,8,4,6,5,7,8,2,1,7,3,5,6,8,1,8,6,3,2,7,5,8,2,7,5,4,1,8,6,8,3,6,8,1,4,5,7,8,4,5,7,2,3,6,8,8,5,4,2,7,6,3,1,8,6,3,1,8,5,4,2,8,7,2,4,5,8,1,3,8,8,1,3,6,7,2,4};

//int chip[256]={1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,1,4,4,4,4,4,4,4,1,5,5,5,5,5,5,5,1,6,6,6,6,6,6,6,1,7,7,7,7,7,7,7,1,8,8,8,8,8,8,8,2,1,2,3,4,5,6,7,2,2,1,4,3,6,5,8,2,3,4,1,2,7,8,5,2,4,3,2,1,8,7,6,2,5,6,7,8,1,2,3,2,6,5,8,7,2,1,4,2,7,8,5,6,3,4,1,2,8,7,6,5,4,3,2,3,1,3,5,7,4,2,8,3,2,4,6,8,3,1,7,3,3,1,7,5,2,4,6,3,4,2,8,6,1,3,5,3,5,7,1,3,8,6,4,3,6,8,2,4,7,5,3,3,7,5,3,1,6,8,2,3,8,6,4,2,5,7,1,4,1,4,7,6,8,5,2,4,2,3,8,5,7,6,1,4,3,2,5,8,6,7,4,4,4,1,6,7,5,8,3,4,5,8,3,2,4,1,6,4,6,7,4,1,3,2,5,4,7,6,1,4,2,3,8,4,8,5,2,3,1,4,7};
//int chip[256]={5,1,5,4,8,7,3,6,5,2,6,3,7,8,4,5,5,3,7,2,6,5,1,8,5,4,8,1,5,6,2,7,5,5,1,8,4,3,7,2,5,6,2,7,3,4,8,1,5,7,3,6,2,1,5,4,5,8,4,5,1,2,6,3,6,1,6,2,5,3,8,4,6,2,5,1,6,4,7,3,6,3,8,4,7,1,6,2,6,4,7,3,8,2,5,1,6,5,2,6,1,7,4,8,6,6,1,5,2,8,3,7,6,7,4,8,3,5,2,6,6,8,3,7,4,6,1,5,7,1,7,8,2,6,4,3,7,2,8,7,1,5,3,4,7,3,5,6,4,8,2,1,7,4,6,5,3,7,1,2,7,5,3,4,6,2,8,7,7,6,4,3,5,1,7,8,7,7,1,2,8,4,6,5,7,8,2,1,7,3,5,6,8,1,8,6,3,2,7,5,8,2,7,5,4,1,8,6,8,3,6,8,1,4,5,7,8,4,5,7,2,3,6,8,8,5,4,2,7,6,3,1,8,6,3,1,8,5,4,2,8,7,2,4,5,8,1,3,8,8,1,3,6,7,2,4};

//int chip[512]={1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,1,4,4,4,4,4,4,4,1,5,5,5,5,5,5,5,1,6,6,6,6,6,6,6,1,7,7,7,7,7,7,7,1,8,8,8,8,8,8,8,2,1,2,3,4,5,6,7,2,2,1,4,3,6,5,8,2,3,4,1,2,7,8,5,2,4,3,2,1,8,7,6,2,5,6,7,8,1,2,3,2,6,5,8,7,2,1,4,2,7,8,5,6,3,4,1,2,8,7,6,5,4,3,2,3,1,3,5,7,4,2,8,3,2,4,6,8,3,1,7,3,3,1,7,5,2,4,6,3,4,2,8,6,1,3,5,3,5,7,1,3,8,6,4,3,6,8,2,4,7,5,3,3,7,5,3,1,6,8,2,3,8,6,4,2,5,7,1,4,1,4,7,6,8,5,2,4,2,3,8,5,7,6,1,4,3,2,5,8,6,7,4,4,4,1,6,7,5,8,3,4,5,8,3,2,4,1,6,4,6,7,4,1,3,2,5,4,7,6,1,4,2,3,8,4,8,5,2,3,1,4,7,5,1,5,4,8,7,3,6,5,2,6,3,7,8,4,5,5,3,7,2,6,5,1,8,5,4,8,1,5,6,2,7,5,5,1,8,4,3,7,2,5,6,2,7,3,4,8,1,5,7,3,6,2,1,5,4,5,8,4,5,1,2,6,3,6,1,6,2,5,3,8,4,6,2,5,1,6,4,7,3,6,3,8,4,7,1,6,2,6,4,7,3,8,2,5,1,6,5,2,6,1,7,4,8,6,6,1,5,2,8,3,7,6,7,4,8,3,5,2,6,6,8,3,7,4,6,1,5,7,1,7,8,2,6,4,3,7,2,8,7,1,5,3,4,7,3,5,6,4,8,2,1,7,4,6,5,3,7,1,2,7,5,3,4,6,2,8,7,7,6,4,3,5,1,7,8,7,7,1,2,8,4,6,5,7,8,2,1,7,3,5,6,8,1,8,6,3,2,7,5,8,2,7,5,4,1,8,6,8,3,6,8,1,4,5,7,8,4,5,7,2,3,6,8,8,5,4,2,7,6,3,1,8,6,3,1,8,5,4,2,8,7,2,4,5,8,1,3,8,8,1,3,6,7,2,4};



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
	/*char content[20];
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");*/

	while(1) {  // main accept() loop
		/*sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);
		printf("new_fd: %d\n", new_fd);
		fourClient[cnt]=new_fd;
		cnt++;*/
		if (cnt==1){
			//gettimeofday( &curTime,NULL );
			//sendTime =curTime.tv_sec;
			//sprintf(content,"%ld",sendTime);
			send(fourClient[0], "begin", 5, 0);
			//send(fourClient[1], "begin", 5, 0);
			//send(fourClient[2], "begin", 5, 0);
			//close(fourClient[0]);
			//close(fourClient[1]);
			//close(fourClient[2]);
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
	/*for (i=0;i<dataLen;i=i+1)
		printf("%d,",data[i]);*/

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
	/*gettimeofday(&tv_begin,NULL);
	thres=tv_begin.tv_usec/100000*100000+100000;
	printf("thres:%ld\n",thres);
	gettimeofday(&tv_end,NULL);
	while (tv_end.tv_usec<thres) {
		gettimeofday(&tv_end,NULL);
		//printf("cur:%ld\n",tv_end.tv_usec);
	}*/
	server();
	usleep (sleepus);
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
