
#include "libnetfiles.h"

//Run Program, first argument has to be 0, read permission, 1, write permission, 2, read write permission.

int main( int argc, char** argv){
//Bottom=Top;

if(argc<3||argc>3){

printf("\n Error, Client must have arguments [HostName], [# 0 or 1 or 2]");

exit(-1);
}

printf("\n Argv[1] %s",argv[1]);
char* HostName = argv[1];

HOSTNAME=HostName;

printf("\n H_Errno: %d STR:%s",h_errno,hstrerror(h_errno));

if(atoi(argv[2])<0||atoi(argv[2])>2){

printf("\n Error, Client must have argument # 0,1,2");

exit(-2);
}

int E = netserverinit(HostName,atoi(argv[2]));

//////////////// INITIALIZE CLIENT SERVER CONNECTION GIVEN HOST AND PERMISSION TYPE!

//Can call functions netopen,netread,netwrite,netclose.
//Each function sets errno and h_errno.
//Must initialize server connection every time perform one command.
//

//Examples

//Read Test
/*
char b1[5000];

bzero(b1,5000);

b1[0]='H';
b1[1]='E';
b1[2]='L';
b1[3]='L';
b1[4]='O';

printf("STATUS READ:%d",netread(-7,b1,5000));

printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno,hstrerror(h_errno));
*/
///////


//Initialize test
//E = netserverinit(HostName,0);
/*
printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno, h_errno==500? "INVALID FILE MODE":hstrerror(h_errno));
*/
//Initialize test

//OPEN TEST
printf("\n\nSTATUS OPEN:%d\n",netopen("HUGE",0));

printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno,hstrerror(h_errno));
//OPEN TEST


/*
E = netserverinit(HostName,1);

printf("\n\nSTATUS OPEN:%d\n",netopen("B",1));

printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno,hstrerror(h_errno));

*/


/*

E = netserverinit(HostName,-992);

printf("\n\nSTATUS OPEN:%d\n",netopen("B",2));

printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno,hstrerror(h_errno));

*/

/*

E = netserverinit(HostName,2);

printf("\n\nSTATUS OPEN:%d\n",netopen("HUGE",1));

printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno,hstrerror(h_errno));




printf("STATUS READ:%d",netread(-12,b1,5));

printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno,hstrerror(h_errno));



printf("\n\nSTATUS OPEN:%d\n",netopen("HUGE",1));

printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno,hstrerror(h_errno));



printf("\n\nSTATUS OPEN:%d\n",netclose(-7));

printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno,hstrerror(h_errno));



printf("\nSTATUS WRITE:%d\n",netwrite(-9,b1,5));

printf("\n Errno: %d STR:%s\n",errno,strerror(errno));

printf("\n H_Errno: %d STR:%s\n\n",h_errno,hstrerror(h_errno));

*/
/*
//printf("STATUS READ:%d",netread(-12,b1,5));

printf("\nBUFFER1: %s\n",b1);

//netread(int fd, void* buff, size_t num);

//printf("STATUS WRITE:%d",netwrite(-9,b1,5));

printf("\n\nSTATUS OPEN:%d\n",netopen("ZZ",2));

printf("STATUS WRITE:%d",netwrite(-7,b1,5));

netserverinit(HostName,1);

printf("\n\nSTATUS OPEN:%d\n",netopen("HUGE",1));

printf("STATUS READ:%d",netread(-7,b1,5));

printf("\n\nSTATUS CLOSE:%d\n",netclose(-7));

*/

//netWrite("HELLO",6,5);

//printf("\n WRITE: %s",s);

//write(SOCKFD,"17,o,HELO.txt,0,0",17);

return 0;
}

//Push to the bottom the type of operation and the pointer to the operation's instructions.

////
//

int netserverinit(char* HostName, int FileMode){


HOSTINIT = -1;
h_errno=0;
errno=0;

if(FileMode==0){
MODE="0";
}
else if(FileMode==1){
MODE="1";
}
else if(FileMode==2){
MODE="2";
}
else{

h_errno = INVALID_FILE_MODE;

//printf("\n\n Error, Invalid file mode \n\n");

return -1;
}

int port;

//KeepConfig config = {60,5,5};

printf("\n Enter a port.\n");

scanf("%d",&port);

PORT=port;

if(port<=8000||port>=64000){

printf("\nError, port number is not within range");

exit(0);
}

int sock = socket(AF_INET,SOCK_STREAM,0);

//fcntl(sock,F_SETFL,O_NONBLOCK);

if(sock<0){
printf("SOCKET ERROR:%s",strerror(errno));

return errno;

}

struct hostent* server;

struct sockaddr_in serv_addr;

socklen_t servlen;

//char buffer[1000];
int n=0;

char buffer [5000];

int ReturnSock;

//char* Host = "adapter.cs.rutgers.edu";



printf("\n\n\n BEFORE GET HOST BY NAME:%d \n\n\n",h_errno);

server = gethostbyname(HostName);

SERVER = server;


if(server==NULL){

fprintf(stderr,"ERROR, no such host\n");

h_errno=HOST_NOT_FOUND;

return -1;

}

h_errno=0;

printf("\n\n\n AFTER GET HOST BY NAME:%d \n\n\n",h_errno);


///////////////////////////////////////////////////////

bzero((char*)&serv_addr,sizeof(serv_addr));
serv_addr.sin_family = AF_INET;

bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);serv_addr.sin_port=htons(port);

printf("SERVER ADDRESS %d",serv_addr);


SERVADDR=serv_addr;

printf("SERVER ADDRESS 222222%d",serv_addr);


HOSTINIT=1;

/////////////////////////////////////////stop

//If server not running, connection refused!

printf("\n\n\n HERROR:%d\n",h_errno);

printf("CONNECTED TO HOST! PORT: %d",port);

return 0;
}

/*
int SetKeepAlive(int sock){

int option = 1;


return setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,&option,sizeof(option));
return 0;
}
*/

/*
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>
#include <math.h>

#define unrestricted  "0"
#define exclusive     "1"
#define transaction   "2"
#define O_RDONLY      "0"
#define O_WRONLY      "1"
#define O_RDWR	      "2"
*/




char* readFromServer(int new_socket){		//takes a socket as input and reads from server the message, then it processes said message	
						//and returns the message excluding the size and comma delimiter for size on sucess. On error,
						//which happens b/c a time out or connect reset, returns "-1";


	errno=0;				//reseting errno

	
	int valread;
	char * size = (char*)malloc(sizeof(char) * 2);	//size is going to be the size of the message which is going to be extracted
	char * c = (char*)malloc(sizeof(char) * 2); 	//c is also used for the size
	
	if(size == NULL || c == NULL)
		exit(-1);

		
	//Starting to read the message the server sent back the client
	valread = read( new_socket , size, 1);     //reading 1 byte at a time to size b/c we dont know the current lenght of the size of the message
	
	if(valread==0){	    //checking for a time out

	errno = ETIMEDOUT; 
	free(size);
	free(c);
	
	return "0";
	}
	if(valread==-1){   //checking for a connect reset 
	
	errno = ECONNRESET;
	free(size);
	free(c);
	
	return "-1";

	}

	
	size[1] = '\0';
				//making size and c into actual "strings"
	c[1] = '\0';
	
	int i = 1;		//i is the loop counter to keep tracking of how many bytes we have read in the message so far
	
	
	while(1){		// This while loop reading 1 byte at a time from socket until the "," is read which indicates the end
				//of the size part of the message

	valread = read( new_socket , c, 1);	

	
	if(valread==0){		//checking for time out error
	errno = ETIMEDOUT; 
	free(size);
	free(c);
	
	return "0";	
	
	}	

	if(valread==-1){		//checking for a connect reset error

	errno = ECONNRESET;
	free(size);
	free(c);
	
	return "-1";

	}


	if( c[0] == ',')		//checking if the current read char was an ',' since that is the delimiter for the size part
			break;
	
	//NewSize is going to be the total size so far, so it has to cat the old size which was size and the new char that was read or c

	char* NewSize = (char*)malloc( strlen(c) + strlen(size) + 1);


	if(NewSize == NULL)
		exit(-1);
	
	strcpy(NewSize,size);
	
	strcat(NewSize,c);

	free(size);		//freeing the old size since now longer need it

	size = NewSize;		//now pointing size to newSize, so now size now is the total size so far again
	
	i++;	
	
	}
	free(c);
	
	long long intSize = atoi(size);		//intSize is the int representation of the size of the message

						//Now we have have the total bytes of the message, we just have to calculate the rest of the bytes
						//of the message excluding the size part, to do this we need to see how many bytes the size of the
						//message was taking

	
	long long int getBytesForSize =  floor(  (log(intSize)/ log(10) ) )  + 2; //calculating the bytes for the size and the comma
	long long int restOfStuff = (intSize - getBytesForSize);		  //restOfSest is the bytes of the rest of the message now

	char * buffer = (char*)malloc( restOfStuff +1 );
		if(buffer == NULL)			
			exit(-1);					        //creating a char* buffer to use to read in the rest of the message,
	valread = read( new_socket , buffer, restOfStuff );			//since now we have the bytes of the rest of the message


	
	if(valread==0){			//checking for time out error
	errno = ETIMEDOUT; 
	free(size);
	free(buffer);
	return "0";	
	
	}	
	
	if(valread==-1){		//checking for connect reset error

	errno = ECONNRESET;
	free(size);
	free(buffer);
	return "-1";

	}

	buffer[restOfStuff] = '\0';
	
	free(size);
	
	return buffer;							//returning the rest of the message
}



ssize_t netclose(int fd){

	int sock2 = socket(AF_INET,SOCK_STREAM,0);

	if(sock2<0){
	printf("SOCKET ERROR:%s",strerror(errno));

	return errno;

	}

	SOCKFD=sock2;

	if(HOSTINIT<0){
	
	h_errno=HOST_NOT_FOUND;
	
	printf("\nW11111\n");

	printf("\nHOSTINIT:%d\n",HOSTINIT);

	return -1;

	}

	bzero((char*)&SERVADDR,sizeof(SERVADDR));
	SERVADDR.sin_family = AF_INET;

	bcopy((char*)SERVER->h_addr,(char*)&SERVADDR.sin_addr.s_addr,SERVER->h_length);SERVADDR.sin_port=htons(PORT);
	
	if(connect(SOCKFD,(struct sockaddr *)&SERVADDR,sizeof(SERVADDR))<0){

	//printf("\nERROR, [%s]\n%s",HostName,strerror(errno));

	//Set Hp_ERRNO to HOST NOT FOUND

	printf("\nW1234\n");

	return -1;


	}

	
	if(fd>=0){	//checking for valid fds
			
	errno = 9;	

	return -1;

	}	


	if(SOCKFD==-1){		//checking if init was setup correctly, if so the global SOCKFD should not be -1

	h_errno=HOST_NOT_FOUND;

	return -1;	

	}

	int sock = SOCKFD;

		//this part of the code calcs the size of the message by determining the size in bytes of all the args

	long int posFd = fd * -1;					//positive version of fd since need to do log on it
	long int bytesForFd = floor( ( (log(posFd))/ (log(10)) ) ) + 2; //have to calculate how many bytes needed for the fd and also the negative sign

	long int total = 2 + bytesForFd;				//adding two to comensate for the comma and the operation bytes
	long int bytesNeedForTotal = floor( ( (log(total))/ (log(10)) ) ) + 1;

	long int bytesForTotalMess = floor( ( (log(bytesNeedForTotal + total))/ (log(10)) ) ) + 1;	//bytesForTotalMess now has the size
													//in bytes needed for the size portion of the
													//message. So now we can malloc the message 
													//we have the bytes for the rest of the message
													//in total and bytes for the size in 														//bytesForTMesss
	
	char* message = (char*)malloc(sizeof(char)* ( bytesForTotalMess   + 1 + total));	//message will be used to send to the server 
												//now just need to fill with correct contexts 					

	if(message == NULL)
		exit(-1);  //ran out of memory
	
	snprintf(message, bytesForTotalMess + 1, "%d",bytesForTotalMess + total);		//writing the size portion of the message to message

	strncat(message,",",1);									//adding the delimiter and operation
	strncat(message,"c",1);
	
	char* fdString = (char*)malloc(sizeof(char*) * (bytesForFd + 1));			//need to now add the fd to message. To do this
												//we need to create the string fdString and
												//strncat that to message
	if(fdString == NULL)
		exit(-1);  //ran out of memory

	snprintf(fdString, bytesForFd + 1, "%d", fd);						//writing the fd to fdString
	strncat(message,fdString,bytesForFd);							//now adding the fd portion to message
	
												//at this point we have now finished the message
												//and are ready to send to the server

	int checkSend = 0;

	checkSend = send(sock , message , strlen(message) , 0 );				//sending the message tot he server now


	if(checkSend==0){					//checking for a time out error
		free(message);
		free(fdString);
		errno = ETIMEDOUT;
	
		SOCKFD=-1;

		return -1;
	}


	if(checkSend == -1){					//checking for a connect reset error
		free(message);
		free(fdString);
		errno = ECONNRESET;
		
		SOCKFD=-1;

		return -1;
	}


	char* readMess = readFromServer(sock); 	//this server now has the message and should process the message, we can now try to read 
 						//the servers response which readFromServer() does. It will return the buffer back excluding
						//the size and delimiter for size

	if(strcmp(readMess,"-1")==0 ){ //checking if readFromServer had a connect reset error
	
	free(message);
	free(fdString);

	SOCKFD=-1;

	return -1;

	}
	if(strcmp(readMess,"0")==0 ){ //checking if readFromServer had a time out error
	
	free(message);
	free(fdString);

	SOCKFD=-1;

	return -1;

	}


	

	int Messlen = strlen(readMess);

	char* errnoOrFd = (char*)malloc( sizeof(char)  *  ( Messlen -1 ) );
		if(errnoOrFd == NULL)
			exit(-1);						 //errnoOrFd is going to be the string of the fd closed or errno value
	strncpy(errnoOrFd, readMess + 2, Messlen - 2 );				//writing the corresponding value to errnoOrFd
	
	
	free(message);
	free(fdString);


	if(readMess[0] == '1' ){ //the front of readMess was 1, then it is an error, so errnoOrFd is errno which we have to set
		
		free(readMess);
		int err = atoi(errnoOrFd);
		
		free(errnoOrFd);
		
		errno = err;
		return -1;
				}

	else{	 //the front of readMess was 0 so the task was sucessful so errnoOrFd was the closed fd, so we can just return 0 now
		free(readMess);
		free(errnoOrFd);
		return 0;
		
	

	}
	








	
}

ssize_t netopen(const char* fileName, size_t intPerm){
	
	
	int sock2 = socket(AF_INET,SOCK_STREAM,0);

	if(sock2<0){
	printf("SOCKET ERROR:%s",strerror(errno));

	return errno;

	}

	SOCKFD=sock2;

	if(HOSTINIT<0){
	
	h_errno=HOST_NOT_FOUND;
	
	printf("\nW11111\n");

	printf("\nHOSTINIT:%d\n",HOSTINIT);

	return -1;

	}

	bzero((char*)&SERVADDR,sizeof(SERVADDR));
	SERVADDR.sin_family = AF_INET;

	bcopy((char*)SERVER->h_addr,(char*)&SERVADDR.sin_addr.s_addr,SERVER->h_length);SERVADDR.sin_port=htons(PORT);
	
	if(connect(SOCKFD,(struct sockaddr *)&SERVADDR,sizeof(SERVADDR))<0){

	//printf("\nERROR, [%s]\n%s",HostName,strerror(errno));

	//Set Hp_ERRNO to HOST NOT FOUND

	printf("\nW1234\n");

	return -1;


	}


	if(SOCKFD==-1){		//checking if init was setup correctly, if so the global SOCKFD should not be -1

	h_errno=HOST_NOT_FOUND;

	return -1;	

	}



	if(strcmp(MODE,"-1")==0){	//checking if init() was setup correctly

	h_errno = INVALID_FILE_MODE;

	return -1;

	}

	if(SOCKFD==-1){			//checking if init() was setup correctly

	h_errno=HOST_NOT_FOUND;

	return -1;	


	}

	int sock = SOCKFD;

	char* perm = "-1";	//netopen has to process the permission as a string so need to get the string equivalent of size_t intPerm to perm

	char* mode = MODE;	//need to set the the mode to open the file in


	if(intPerm==0){			//the below 3 if and else ifs set perm to correct value
	
	perm = "0";
	
	}
	else if(intPerm==1){
	
	perm = "1";

	}
	else if(intPerm==2){
	
	perm = "2";	

	}
	else{			//the user set in a permission int that we cant process so error

	
	
	errno = 13;
		
	return -1;
	}
	
	
	errno=0;


	//have to find the the bytes taken each of the attributes to malloc a message that is the correct length long

	long long int bytesNeedTotal = strlen(fileName) + 4; // bytesNeedTotal is the total bytes needed for the message excluding the size of the
							     //size of the message. Adding by 4 to comensate for the operation, delimiter, permission,
							     //and mode to be send in which all take 1 byte
	
	long long int bytesForSize =   floor( ( (log(bytesNeedTotal))/ (log(10)) ) ) + 1; //getting the bytes take up from bytesNeedTotal

	long long int bytesForSizeForbytes = (floor( ( (log(bytesForSize + bytesNeedTotal ))/ (log(10)) ) ) + 1) ;//getting the bytes taken up by
													          //the size portion of the message

	char* message = (char*)malloc( sizeof(char) * (bytesForSizeForbytes + 1 + bytesNeedTotal) );	//now we have the correct size of the message
													//so now we can malloc the message
	if(message == NULL)
		exit(-1);  //ran out of memory


	long long int newTotal = bytesNeedTotal + bytesForSizeForbytes;
									//writing newTotal, or the size in bytes of the message to the front of the 
									//message
	snprintf(message,bytesForSizeForbytes +1, "%d",newTotal);

	strncat(message,",",1);
	strncat(message,"o",1);		//adding delimiter, operation, permission,mode, and the name of the file to message
	strncat(message,perm,1);
	strncat(message,mode,1);
	strncat(message,fileName,strlen(fileName));

	
	int checkSend = 0;				//at this point we have correctly formatted message

	checkSend = send(sock , message , strlen(message), 0);		//sending the message to the server
	
	
	if(checkSend==0){				//checking for a time out error
		free(message);
		errno = ETIMEDOUT;

		SOCKFD=-1;

		MODE = "-1";
		  	
		return -1;
	}


	if(checkSend == -1){				//checking for a time out error
		free(message);
		errno = ECONNRESET;

		SOCKFD=-1;

		MODE = "-1";
	
		return -1;
	}

	char* readMess = readFromServer(sock); 		//readMess now points to the buffer that was read back from the server
		
	if(strcmp(readMess,"-1")==0){		//checking for a connect reset error

	free(message);
	
	SOCKFD=-1;

	MODE = "-1";
	
	return -1;

	}

	if(strcmp(readMess,"0")==0 ){ 		//checking for a time out error
	
	free(message);

	SOCKFD=-1;

	MODE = "-1";

	return -1;

	}
	
	
	int Messlen = strlen(readMess);		//MessLen is the length of the message

	char* errnoOrFd = (char*)malloc( sizeof(char)  *  ( Messlen -1 ) ); //errnoOrFd now contains the errno value if the command failed
									    //or the associated fd if the command was a sucess
		
		if(errnoOrFd == NULL)
			exit(-1);


	strncpy(errnoOrFd, readMess + 2, Messlen - 2 );				//errnoOrFd now has it corresponding value
	
	free(message);
	
	if(readMess[0] == '1' ){ //the front of readMess was a 1, so error, so errnoOrFd is errno. Sets errno and returns -1
		
		free(readMess);
		int err = atoi(errnoOrFd);		//grabing the corresponding errno value
		
		free(errnoOrFd);
		
		errno = err;

		SOCKFD=-1;

		MODE = "-1";

		return -1;
				}

	else{ 		//the front of readMess was a 0, so sucess, so errnoOrFd is the fd. Now just return the fd
		free(readMess);
		SOCKFD=-1;
		MODE = "-1";
		return atoi(errnoOrFd);	//returns the corresponding fd in int form
		

	}
}




ssize_t netread(int fd, void* buff, size_t num){

	errno=0;


	
	int sock2 = socket(AF_INET,SOCK_STREAM,0);

	if(sock2<0){
	printf("SOCKET ERROR:%s",strerror(errno));

	return errno;

	}

	SOCKFD=sock2;

	if(HOSTINIT<0){
	
	h_errno=HOST_NOT_FOUND;
	
	printf("\nW11111\n");

	printf("\nHOSTINIT:%d\n",HOSTINIT);

	return -1;

	}

	bzero((char*)&SERVADDR,sizeof(SERVADDR));
	SERVADDR.sin_family = AF_INET;

	bcopy((char*)SERVER->h_addr,(char*)&SERVADDR.sin_addr.s_addr,SERVER->h_length);SERVADDR.sin_port=htons(PORT);
	
	if(connect(SOCKFD,(struct sockaddr *)&SERVADDR,sizeof(SERVADDR))<0){

	//printf("\nERROR, [%s]\n%s",HostName,strerror(errno));

	//Set Hp_ERRNO to HOST NOT FOUND

	printf("\nW1234\n");

	return -1;


	}

	
	if(fd>=0){	//checking for valid fds
			
	errno = 9;	

	return -1;

	}	


	if(SOCKFD==-1){		//checking if init was setup correctly, if so the global SOCKFD should not be -1

	h_errno=HOST_NOT_FOUND;

	return -1;	

	}


	
	
	if(fd>=0){

	errno = 9;

	return -1;

	}

	if( num <= 0){
		errno = 9;
		return -1;
	}

	if(SOCKFD ==-1){

	h_errno=HOST_NOT_FOUND;

	return -1;

	}

	int sock = SOCKFD;

	char* buffToBeWritten=(char*)buff;		//have to type cast buff to a char* 


	//have to find the size in bytes of each of the args so we can malloc a buffer to a correct size


	long long int posFd = fd * -1;	//just like in netclose(), have to set the fd to positive
	long long int bytesForFd = floor( ( (log(posFd))/ (log(10)) ) ) + 2; //bytesForFd now has the required bytes for the fd
	long long int bytesForNum = floor( ( (log(num))/ (log(10)) ) ) + 1;//geting the bytes that the bytes to be read takes up
	
	long long int total = bytesForFd + bytesForNum + 3; //Total now has the bytes of the message excluding the size of the message and
							    //comensates for the operations and delimiters by adding 3

	long long int bytesForTotal = floor( ( (log(total))/ (log(10)) ) ) + 1;	//need to see how many bytes total takes up
	long long int bytesForSize = floor( ( (log(bytesForTotal + total))/ (log(10)) ) ) + 1; //need to see how many bytes the size of the message
												//takes up
	
	char* message = (char*)malloc(sizeof(char)* ( bytesForSize   + 1 + total));	//now we have the correct size of the message and can now
											//malloc the message

	if(message == NULL)
		exit(-1);  //ran out of memory


	
	snprintf(message, bytesForSize + 1, "%d",total + bytesForSize);		//writing the size of the total message to message
	
	strncat(message,",",1);
	strncat(message,"r",1);							//writing a delimiter and operation to message

	char * numString = (char*)malloc((sizeof(char)) * (bytesForNum + 1));	//have to write the bytes to be read in, but need to malloc
										//a buffer (numString) and write num to it and then strcat message
										//and numString


	if(numString == NULL)
		exit(-1);  //ran out of memory


	snprintf(numString, bytesForNum + 1, "%d",num);			//writing the number of bytes to be read to numString
	strncat(message,numString,bytesForNum);				//now adding numString to the end of the message
	
	strncat(message,",",1);						//adding a delimiter
	
	char* fdString = (char*)malloc(sizeof(char*) * (bytesForFd + 1) );	//have to add the fd to message, but just like in number of bytes to be
										//read, need to malloc a buffer and write to said buff and strcat that
	if(fdString == NULL)							//to message
		exit(-1);  //ran out of memory
	
	snprintf(fdString, bytesForFd + 1, "%d", fd);		
	strncat(message,fdString,bytesForFd);			//at this point we have finished formatting message


	int checkSend = 0;

	checkSend = send(sock , message , strlen(message),0);	//sending the message to the server
	
	
	if(checkSend==0){					//checking for a time out error
		free(message);
		free(fdString);
		free(numString);

		SOCKFD=-1;
		errno= ETIMEDOUT;  	
		return -1;
	}


	if(checkSend == -1){					//checking for a connect reset error
		free(message);
		free(fdString);
		free(numString);
		
		SOCKFD=-1;
		errno = ECONNRESET;
		return -1;
	}


	


	char* readMess = readFromServer(sock);			//Now readMess is the server's response

	if(strcmp(readMess,"-1")==0){				//checking for connect reset error

	free(message);
	free(fdString);
	free(numString);
	
	SOCKFD=-1;
	return -1;

	}

	if(strcmp(readMess,"0")==0){			//checking for a time out error
	free(message);
	free(fdString);
	free(numString);
	
	SOCKFD=-1;

	return -1;

	}


	
	int Messlen = strlen(readMess);			//MessLen is the size of read in message from the server

	char* errnoOrBuff = (char*)malloc( sizeof(char)  *  ( Messlen -1 ) );	//errnoOrBuff is the either the errno statment on error or the stuff
										//that was read on sucess
		if(errnoOrBuff == NULL)						//copying the correct stuff to errnoOrBuff
			exit(-1);
			
	strncpy(errnoOrBuff, readMess + 2, Messlen - 2 );			
	


	
	free(message);
	free(fdString);
	free(numString);

	if(readMess[0] == '1' ){ //Message has 1 on front, thus error so errnoOrBuff is the corresponding errno value
		
		free(readMess);
		int err = atoi(errnoOrBuff);	//setting the correct errno number
		free(errnoOrBuff);
		errno = err;
		return -1;
				}

	else{		//Message has 0 on front, thus sucess, so errnoOrBuff is the buff that was read
		free(readMess);
		strncpy(buffToBeWritten, errnoOrBuff, Messlen-2 );	//writing errnoOrBuff to the input buffer

			return Messlen -2;				//returning the number of bytes to be read. sub by 2 to compensate
									//for the "0" and delimiter

	}
			

	
	}


ssize_t netwrite(int fd,const void* buffer, size_t num){
		
	errno=0;
			
	
	int sock2 = socket(AF_INET,SOCK_STREAM,0);

	if(sock2<0){
	printf("SOCKET ERROR:%s",strerror(errno));

	return errno;

	}

	SOCKFD=sock2;

	if(HOSTINIT<0){
	
	h_errno=HOST_NOT_FOUND;
	
	printf("\nW11111\n");

	printf("\nHOSTINIT:%d\n",HOSTINIT);

	return -1;

	}

	bzero((char*)&SERVADDR,sizeof(SERVADDR));
	SERVADDR.sin_family = AF_INET;

	bcopy((char*)SERVER->h_addr,(char*)&SERVADDR.sin_addr.s_addr,SERVER->h_length);SERVADDR.sin_port=htons(PORT);
	
	if(connect(SOCKFD,(struct sockaddr *)&SERVADDR,sizeof(SERVADDR))<0){

	//printf("\nERROR, [%s]\n%s",HostName,strerror(errno));

	//Set Hp_ERRNO to HOST NOT FOUND

	printf("\nW1234\n");

	return -1;


	}

	
	if(fd>=0){	//checking for valid fds
			
	errno = 9;	

	return -1;

	}	


	if(SOCKFD==-1){		//checking if init was setup correctly, if so the global SOCKFD should not be -1

	h_errno=HOST_NOT_FOUND;

	return -1;	

	}



	if(fd>=0){     //checking if the user has sent in a valid fd since fds have to be negative

	errno = 9;

	return -1;

	}

		if(num <= 0){		//checking if the user has sent in a valid number of bytes to write
			errno = 9;
			return -1;
			}
	if(SOCKFD==-1){			//checking if init() has been setup correctly

	h_errno=HOST_NOT_FOUND;

	return -1;

	}

	int sock = SOCKFD;
			
	char* newBuffer = (char *)malloc(sizeof(char) * (num + 1));	//creating a newBuffer, since we need this buffer to be null terminated
									//Now we just cpy the old buffer in to the newBuffer, but only to the amount
									//of bytes to be written, since we dont care about any additional stuff on the
									//input buffer if it is not going to be written.

	if(newBuffer == NULL)
		exit(-1);  //ran out of memory
	int i;
	for(i = 0; i < num; i ++){
		newBuffer[i] = ((char*)buffer)[i];
		}
	newBuffer[num] = '\0';
	
	


	//Have to determine the size in bytes of the args so we can malloc a message to the correct size

	long long int posFd = fd * -1;		//getting the postive version of the fd, since we need to apply the below log() functions to it
	long long int bytesForFd = floor( ( (log(posFd))/ (log(10)) ) ) + 2; //now we have the size in bytes the fd is taken, with the negative sign
									    //to compensate->adding 2 instead of adding 1
	long long int bytesForNum = floor( ( (log(num))/ (log(10)) ) ) + 1;
	
	long long int total = bytesForFd + bytesForNum + num + 4; //total now contains the size of the message, including the bytes the fd takes,
								  //the buffer (noted by num) and operation and delimiters( noted by + 4)

	long long int bytesForTotal = floor( ( (log(total))/ (log(10)) ) ) + 1;	//need to determine the bytes the total is taken

	long long int bytesForSize = floor( ( (log(bytesForTotal + total))/ (log(10)) ) ) + 1;	// need to determine the bytes the size of the
												//message portion will take

					//at this part, we have the correct size of the message, now we can malloc the message

	char* message = (char*)malloc(sizeof(char)* ( bytesForSize   + 1 + total));

	if(message == NULL)
		exit(-1);  //ran out of memory
		

	snprintf(message, bytesForSize + 1, "%d",total + bytesForSize);	//writing in the correct size of the message in the front of the message

	strncat(message,",",1);				//then we are writing a corresponding delimiter and operation
	strncat(message,"w",1);
	
	
	char * numString = (char*)malloc((sizeof(char)) * (bytesForNum + 1));	//to add the number of bytes to be written, we have to malloc numString
										//.Write the num to it, then strcat that to the message

	if(numString == NULL)
		exit(-1);  //ran out of memory
	
	snprintf(numString, bytesForNum + 1, "%d",num);
	
	strncat(message,numString,bytesForNum);		//adding the num of bytes to be written and corresponding delimiter to message
	strncat(message,",",1);
	
	char* fdString = (char*)malloc(sizeof(char*) * (bytesForFd + 1) );		//similar to what we did for numString, have to malloc a 												//buffer and strncat that buffer to message to add the fd 
											//portion of the message


	if(fdString == NULL)
		exit(-1);  //ran out of memory


	
	snprintf(fdString, bytesForFd + 1, "%d", fd);		//writing the correct fd value to fdString
	strncat(message,fdString,bytesForFd);
	strncat(message,",",1);					//we are now adding the fd, delimiter, and the buffer to be writtern from(newBuffer)
								//to message
	strncat(message,newBuffer,num);

								//at this point, we have correctly formatted the message 
	int checkSend = 0;

	checkSend = send(sock , message , strlen(message), 0);		//now we can send the message to the server

	
	if(checkSend==0){					//checking for a time out error
		free(message);
		free(fdString);
		free(numString);
		
		errno = ETIMEDOUT;	  	
		return -1;
	}


	if(checkSend == -1){					//checking for connect reset error
		free(message);
		free(fdString);
		free(numString);
		
		errno = ECONNRESET;
		return -1;
	}

	

	char* readMess = readFromServer(sock);			//now we can read the response from the server

	if(strcmp(readMess,"-1")==0){				//checking for a connect reset error	

	free(message);
	free(fdString);
	free(numString);
	free(newBuffer);	

	return -1;

	}

	
	if(strcmp(readMess,"0")==0){			//checking for a time out error

	free(message);
	free(fdString);
	free(numString);
	free(newBuffer);	

	return -1;

	}

	
	int Messlen = strlen(readMess);			//MessLen is the size of the message

	char* errnoOrNum = (char*)malloc( sizeof(char)  *  ( Messlen -1 ) );	//errnoOrNum is the corresponding errno number on failure or the
										//number of bytes written on sucess

		if(errnoOrNum == NULL)
			exit(-1);
	strncpy(errnoOrNum, readMess + 2, Messlen - 2 );
	
	
	free(message);
	free(fdString);
	free(numString);
	free(newBuffer);


	if(readMess[0] == '1' ){ //The front of readMess was 1, so command was an error, so errnoOrNum is the corresponding errno value
		
		free(readMess);
		int err = atoi(errnoOrNum);	//err now its the corresponding errno value
		
		free(errnoOrNum);
		errno = err;
		return -1;
				}

	else{			//the front of readMess was 0, so command was a sucess, so errnoOrNum is the corresponding number of bytes written
		free(readMess);
		
		int n = atoi(errnoOrNum);	//n is the corresponding number of bytes written, we can just return this value
		free(errnoOrNum);
		return n;
		

	}
	



}







