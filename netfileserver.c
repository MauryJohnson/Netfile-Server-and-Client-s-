#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>
#include <math.h>

char* toString(char* C);

typedef struct ThreadRead_{

char * ARG;

int Socket;

char Permission;

int BYTES;

int FD;

char* Filename;

}ThreadRead;



typedef struct ThreadOpen_{

char * ARG;

int Socket;

char Permission;

char Mode;

char* Filename;

}ThreadOpen;


typedef struct ThreadClose_{

char* ARG;

int Socket;

int FD;


}ThreadClose;


typedef struct ThreadWrite_{

int Socket;

char Permission;

int BYTES;

int FD;

char* MESSAGE;

}ThreadWrite;


void openSendBack(char* op, int fdOrErrno, int sock);

void readSendBack(char* op, char* buff, int amountRead, int sock);

void writeSendBack(char* op, int ERRORFD,int sock);

void closeSendBack(char* op, int Errno,int sock);


void o(char* buff, int size, int socket);

void r(char* buff, int size,int socket);

void w(char* buff, int size, int socket);

void cl(char* buff, int size, int socket);


void * pthreadopen(void* args);

void * pthreadclose(void* args);

void * pthreadread(void* args);

void * pthreadaccept(void* args);

void * pthreadwrite(void* args);

void * pthreadlisten(void* args);



void Push(int fd, char* FileName, char Permission, char Mode);

void Delete(int fd);

void PrintNode();


//Searches for file descriptor and returns the file descriptor if found in list and 0 if not found in list
int SearchR (int fd);
//Searches for file descriptor and returns true if found in list and false if not found in list
bool SearchW (int fd);

//Searches for file name and returns true if found in list and false if not found in list
bool SearchO (char* FileName);

//Searches for given file descriptor and returns true if the file has read only or read write permissions and false if not.
bool PermissionCheck(int fd);

//Searches for given file descriptor and returns true if the file has write only or read write permissions and false if not.
bool PermissionCheck2(int fd);

//Searches for a given filename and returns true if the mode is transaction mode.
bool TransCheck(char* Filename);

//Searches for a given filename and returns true if the permission is write permission or read write permission.
bool CheckWrite(char* FileName);


//Checks if the file has write permission or read write permission and has exclusive mode and returns true if so. Returns false if otherwise.
bool ExclusiveWrite(char* FileName);

//Calls function transcheck and if transcheck is false then  checks if the permission is 0 and if so then returns false, then it calls checkwrite which returns true if checkwrite is true. Finally if the filename passes all these functions as false, it returns false.
bool CASE2(char* FileName, char Permission);

//Create a mutex which should be used to lock pthread
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Tree containing max number of threads
////Add a thread, delete a thread, sarch for a thread! of username type int

//List used to store file descriptors, filenames, permissions, socket and mode.
typedef struct ThreadList_{
int FD;

char* FileName;
//Must free FileName while freeing linked list.
int Permission;
//Must free Permissions while freeing linked list.
int SOCK;

int Mode;

struct ThreadList_*Next;
} Node;


#define MMAP_THRESHOLD (2*2*2*2*2*2*2*2*2*2)*128

Node* Head = NULL;

int ServerSock = 0;

int main( int argc, char** argv){

int port;

printf("\nEnter a port number from 8001-63999\n");

scanf("%d",&port);

//Prompt user for port number and check if port is not within range. 

if(port<=8000||port>=64000){
printf("\nError, port number is not within the range!\n");
return -1;
}


//Create socket under TCP protocol.
int sock = socket(AF_INET,SOCK_STREAM,0);


//Set global socket for server
ServerSock=sock;


//If socket creation failed, error for socket creation.
if(sock<0){
printf("SOCKET ERROR:%s",strerror(errno));

return errno;

}


//Structure for server creation and binding to a port number.
struct sockaddr_in serv_addr;

serv_addr.sin_family=AF_INET;


serv_addr.sin_port=htons(port);


serv_addr.sin_addr.s_addr = INADDR_ANY;

printf("\nSERVER FAMILY: %d, SERVER PORT(Converted to network byte order): %d, SERVER IP: %d\n",serv_addr.sin_family,serv_addr.sin_port,serv_addr.sin_addr.s_addr);

if(bind(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){

printf("SOCKET BINDING ERROR:%s",strerror(errno));

return errno;

}


printf("\nBIND OF PORT:%d SUCCESSFUL!\n",port);


printf("\nLISTENING...\n");


while(1){

pthread_t thread1;

pthread_t* threadhandle0=&thread1;

pthread_create(threadhandle0,NULL,pthreadlisten,(void*)(ServerSock));

pthread_join(thread1,NULL);

}


printf("\nEND ALL LISTENING, SHOULD NEVER HAPPEN\n");

printf("???");

FreeL();

return 0;
}

//Push to the bottom the type of operation and the pointer to the operation's instructions.

void Push(int fd, char* FileName, char Permissions, char Mode){
//Pointer should be the last

printf("\n FILENAME PUSH: %s\n",FileName);

//If there is no node, add a node with filename, permission, file descriptor, and mode.
if(Head==NULL){

(Head) = malloc(sizeof(Node));

(Head)->FD = (fd);
//printf("\n FILENAME PUSH: %s\n",FileName);
(Head)->FileName = FileName;
(Head)->Permission = Permissions;
(Head)->Mode =Mode;

(Head)->Next=NULL;


}
else{
//Push to top, so push every other pointer down.
Node* Q2 = malloc(sizeof(Node));

Q2->FD=(fd);
Q2->FileName = FileName;
Q2->Permission=Permissions;
Q2->Mode=Mode;

Q2->Next=(Head);

Head=Q2;

}

printf("SUCCESSSSS\n\n");
return;
}


//Thread open 

void* pthreadopen(void* argb){

//Lock the thread for open

pthread_mutex_lock(&mutex);

pthread_t ER;

pthread_t* ERR = &ER;

//Set errno to 0.

errno = 0;

//Set all parameters from thread open structure

int SOCK = (int)(((ThreadOpen*)argb)->Socket);

char* FileName = (char*)(((ThreadOpen*)argb)->Filename);

char Permission = (char)(((ThreadOpen*)argb)->Permission);

char Mode = (char)(((ThreadOpen*)argb)->Mode);


printf("\n PERMISSION: %c",Permission);

int fd = 0;


//Before opening, check trans check

if(Mode=='2'){
printf("\n\nMODE 2 BEFEOR SEARCH");
//File Already opened
if(SearchO(FileName)){
printf("\nTransaction Mode Error! File Already Opened\n");



errno = EBADF;

fd=-1;

}

printf("\n\nMODE 2 AFTER SEARCH");

}

//Check CASE2
else if(Mode=='1'){

if(CASE2(FileName,Permission)==true){

printf("\nExclusive Mode Error! File Already Opened With Write Permission\n");

errno = EBADF;

fd=-1;

}

}

//Mode is 0, unrestricted, check trans check
else{

if(TransCheck(FileName)){

printf("\nExclusive Mode Error! File Already Opened With Write Permission\n");

errno = EBADF;

fd=-1;

}

if(Permission=='1'||Permission=='2'){
if(ExclusiveWrite(FileName)){

printf("\nExclusive Mode Error! File Already Opened With Write Permission\n");

errno = EBADF;

fd = -1;

}

}
}

//If there is an error, call open send back
if(fd==-1){

openSendBack("1",errno,SOCK);

pthread_mutex_unlock(&mutex);

pthread_exit((void*)0);

}

//If there is not an error, check permissions and open with appropriate permissions
else{

if( Permission == '0'){
fd = open(FileName,O_RDONLY);
}

else if(Permission == '1'){
fd=open(FileName,O_WRONLY);
}

else if(Permission == '2'){
fd=open(FileName,O_RDWR);
}


//If opening failed, call open send back with errno set
if(fd==-1){

openSendBack("1",errno,SOCK);

pthread_mutex_unlock(&mutex);

pthread_exit((void*)0);

}


//Upon successful open, push node onto list.
Push(fd, FileName, Permission, Mode);

fd = (fd*-1)-1;

//printf("PRINTNODES");

PrintNode();

//printf("\nMADE IT PRINTNODE");

//Send back negative fd.

openSendBack("0",fd,SOCK);

}

//Unlock locked pthread open
pthread_mutex_unlock(&mutex);

//To get back to original file descriptor,
pthread_exit((void*)((fd+1)*-1));

}

//Explained above main
int SearchR(int fd){
Node* Q = Head;

while(Q!=NULL){
Node*Q1=Q->Next;

if(Q->FD==fd){
return fd;
}

Q=Q1;

}

return 0;
}


//Stated above main.
void Delete(int fd){

//Deletes node containing fd after user closes file.

if(Head->FD == fd){

	Node* tmp = Head;
	Head = Head->Next;
	free(tmp);
	return;
}

Node* Q;

for(Q = Head; Q!=NULL; Q=Q->Next){
if(Q->Next==NULL){
break;
}
if(Q->Next->FD==fd){
//free(Q->Next);

Node* Q2 = Q->Next;

Q->Next = Q2->Next;

free(Q2);

break;

}

}

return;
}



//Prints the list.
void PrintNode(){
Node* Q = Head;

printf("\nPrinting\n");

if(Q==NULL){
printf("\nNULL\n");
}


while((Q)!=NULL){

Node*Q1= (Q)->Next;

printf("\nFile Descriptor: %d",(Q)->FD);

printf("\nFileName: %s",(Q)->FileName);

printf("\nPersmission: %c",(Q)->Permission);

printf("\nMode: %c",(Q)->Mode);

Q=Q1;

}

return;
}


//Pthread close function
void* pthreadclose(void* argb){


//Locks for close
pthread_mutex_lock(&mutex);

int E = 0;

int SOCK = ((ThreadClose*)argb)->Socket;

int f1 = ((ThreadClose*)argb)->FD;

printf("\n SOCK:%d FD:%d\n",SOCK,f1);


//Convert negative fd back to positive.
f1=(f1*-1)-1;


//Search read file descriptor.
if(SearchR(f1)==0){

printf("\n Error, CANT FIND FD TO CLOSE WITH\n");

E=9;

closeSendBack("1",E,SOCK);

pthread_mutex_unlock(&mutex);

pthread_exit((void*)(EBADF));
}


int f2 = close(f1);

E=errno;

//Check if close failed.
if(f2<0){
printf("\nERROR CLOSING\n");

closeSendBack("1",E,SOCK);

pthread_mutex_unlock(&mutex);

pthread_exit((void*)(f2));
}

printf("\nDELETE: %d\n",f1);

Delete(f1);


printf("\n\n\n\n\n");

PrintNode();

//Call close send back
closeSendBack("0",1,SOCK);


//Unlock mutex for close
pthread_mutex_unlock(&mutex);


//Exit pthread close
pthread_exit((void*)(0));

}


//Pthread read
void* pthreadread(void* args){

pthread_t ER;

pthread_t* ERR = &ER;

errno = 0;

int f2;


//Set all parameters for threadread list.
int SOCK = (int)(((ThreadRead*)args)->Socket);

int fd = (int)(((ThreadRead*)args)->FD);

int BYTES = (int)(((ThreadRead*)args)->BYTES);


int A1 = fd;


int A3=0;


int length1=0;


int length2=0;

printf("\nSOCK:%d, FD:%d BYTES: %d",SOCK,fd,BYTES);

printf("\n SEARCH, File Descriptor:%d HOW MANY: %d\n",A1,A3);

//Convert FD
A1=(A1*-1)-1;

//Search for fd in list
if(SearchR(A1)==0){
printf("\n File Does not exist \n");

errno = 9;

readSendBack("1", "", errno, SOCK);

pthread_exit((void*)(0));


}
//Check for permission for FD
if(PermissionCheck(A1)){
//SET error not permission.
printf("\nError, not correct permission");

errno=9;

//Send back func
readSendBack("1", "",errno , SOCK);

//Exit
pthread_exit((void*)(0));


}

//Get bytes for reading from file.
char B[(BYTES+1)*sizeof(char)];
bzero(B,BYTES+1);

printf("\nSIZE: %d",BYTES);

int FD=0;

int F = 0;


//Read from file bytes

F= read(A1,(B+FD),BYTES);

FD += F;

//IF read failed
if(F<0){

printf("\n Error: %s",strerror(errno));

//Send back func to client
readSendBack("1", B, strlen(B), SOCK);
//Exit
pthread_exit((void*)(EBADF));
}

BYTES-=F;
//Successful read, send back to client state
readSendBack("0",toString(B), strlen(B), SOCK);

//Exit pthreadread
pthread_exit((void*)(FD));

}

//Explained above main
bool PermissionCheck(int fd){

Node* T = Head;

while(T!=NULL){

if(T->FD==fd&&(T->Permission=='0'||T->Permission=='2')){
return false;
}

Node*  T1 = T->Next;
T=T1;
}

return true;
}


//Explanied above main
bool PermissionCheck2(int fd){

Node* T = Head;

while(T!=NULL){

if(T->FD==fd&&(T->Permission=='1'||T->Permission=='2')){
return false;
}

Node*  T1 = T->Next;
T=T1;
}

return true;
}


	void* pthreadaccept(void* ARG){


	printf("\nPTHREAD ACCEPT\n");

	int new_socket = ((int)ARG);


	printf("\nNEW SOCKET:%d\n",new_socket);

	
	
	char * size = (char*)malloc(sizeof(char) * 2);
	char * c = (char*)malloc(sizeof(char) * 2);
	
	int valread = read( new_socket , size, 1);

	if (valread<=0){

	free(size);
	free(c);
	
	printf("\n ERROR, CONNECTION DROPPED");

	pthread_exit((void*)0);


	}


	size[1] = '\0';

	c[1] = '\0';
	//size[0] = '\0';
	int i = 1;

        printf("\nBEFORE\n");

	//printf("w1\n");
	while(1){			//getting the size//

	valread = read( new_socket , c, 1);

	
	if (valread<=0){

	free(size);
	free(c);
	
	printf("\n ERROR, CONNECTION DROPPED");

	pthread_exit((void*)0);


	}


	if( c[0] == ',')
			break;
	//c[0] = output[i];
	//printf("w2\n");
	char* NewSize = (char*)malloc( strlen(c) + strlen(size) + 1);
	//size[i +1] = '\0';
	
	
	//printf("w3\n");
	strcpy(NewSize,size);
	//strncat(size,c,1);
	strcat(NewSize,c);

	free(size);

	size = NewSize;
	//printf("w4\n");
	i++;	
	
	}
printf("\nPAFTER\n");

	free(c);
	//printf("w5\n");
	//size[i] = '\0';
	long long intSize = atoi(size);
	long long int bytesForSize = floor(  (log(intSize)/ log(10) ) )  + 2; //for the comma
	long long int bytesForRest = intSize - bytesForSize;
	
	printf("size: %s bytesForSize: %d bytesForRestOfMessage: %d\n",size,bytesForSize,bytesForRest);

	long long int getBytesForSize =  floor(  (log(intSize)/ log(10) ) )  + 2; //for comma aswell
	long long int restOfStuff = (intSize - getBytesForSize);

	char * buffer = (char*)malloc( restOfStuff +1 );
	valread = read( new_socket , buffer, restOfStuff );
	
	if (valread<=0){

	free(size);

	printf("\n ERROR, CONNECTION DROPPED");

	pthread_exit((void*)0);


	}


	buffer[restOfStuff] = '\0';
	
	free(size);

	
	char command = buffer[0];
	printf("%s\n",buffer );

	printf("it worked\n");
		
	if(command=='o')
	 o(buffer,restOfStuff,new_socket);
	//pthread_exit((void*)0);	
	else if(command=='r')
	  r(buffer,restOfStuff, new_socket);	
	else if(command=='w')
	  w(buffer,restOfStuff, new_socket);
	else if(command=='c')
	   cl(buffer,restOfStuff, new_socket);
	// openSendBack("1",90);
	//closeSendBack("1",123456789);
	//writeSendBack("1", 123456789);
	//readSendBack("1", "helloY",6, new_socket);// on error send errno as amountRead instead

	free(buffer);

	pthread_exit((void*)0);

	}




void* pthreadlisten(void* args){
//Create Time Stamp
//
time_t RAW;

time(&RAW);

printf("\nCurrent Time: %s",ctime(&RAW));

//Time 

pthread_t thread00;

pthread_t * THandle = &thread00;

//time(&RAW);

int sock = (int)args;

int Listen=1;

socklen_t clilen;

int newsock;

struct sockaddr_in cli_addr;

int n=0;
int SockNum=0;
//Listen for the same port numbers, use socket which is now binded to port.
printf("LISTENING\n");

//Create Thread bool TimeTrack(Curr Time)

//Only listen until reach 10 socket connections!
//pthread_create(&thread00,NULL,TrackTime,(void*)RAW);

//pthread_join(thread00,NULL);

Listen = listen(sock,0);
	
	//If time track = false, p thread exit

	printf("\n\n\nStatus: %s\n\n\n",strerror(Listen));
	
	if(Listen>=0){
	//Found a connection??

	//clilen = sizeof(cli_addr);
	//cli_addr_size = sizeof(serv_addr);
	
	clilen=sizeof(cli_addr);

	printf("\nWAITING TO ACCEPT A SOCKET\n");

	newsock = accept(sock,(struct sockaddr*) &cli_addr,&clilen);

        time_t Y;
	time(&Y);
	printf("\nNew Time: %s",ctime(&Y));


	printf("\n DIFFERENCE TIME: %lf",(difftime(Y,RAW)));
       
	//Accept error.
	if(newsock<0){
	printf("\nACCEPT ERROR:%s\n",strerror(errno));
	return errno;
	}

	//Found a socket connection!
	printf("CLIENT ADDRESS: %d",&cli_addr);

	pthread_t thread2;

	pthread_t* threadHandle0 = &thread2;

	//Create a thread for accept if found a socket to handle this socket, then loops again to listen
	pthread_create(&thread2,NULL,pthreadaccept,(void*)(newsock));

	SockNum++;

	}


if(errno!=0){
printf("LISTEN ERROR:%s",strerror(errno));
//pthread_exit((void*)errno);
}

pthread_exit((void*)errno);
}
		

//Pthread write

void* pthreadwrite(void* argb){

pthread_t ER;

pthread_t* ERR = &ER;


//Set all parameters for pthread write
int SOCK = ((ThreadWrite*)argb)->Socket;

int fd = ((ThreadWrite*)argb)->FD;

char* Str3 = ((ThreadWrite*)argb)->MESSAGE;

int BYTES = ((ThreadWrite*)argb)->BYTES;

//Convert FD back

int A2 = (fd*-1)-1;

printf("\nSOCK:%d, FD:%d BYTES: %d",SOCK,fd,BYTES);

printf("\n SEARCH, File Descriptor:%d HOW MANY: %d\n",A2,BYTES);

errno = 0;


//Search write permissions
if(SearchW(A2)){

printf("\nError, File does not exist\n");

errno = EBADF;


//Write send back func.
writeSendBack("1",errno, SOCK);

//Exit
pthread_exit((void*)(EBADF));

}
//Permission check 2.
if(PermissionCheck2(A2)){
//SET error not permission.
printf("\nError, not correct permission");

errno=EBADF;

//Write send back func
writeSendBack("1",errno, SOCK);


//Exit
pthread_exit((void*)(EBADF));

}
//Success


int I = write(A2,Str3,strlen(Str3));

//If write failed
if(I<0){

//printf("\nError,:%s",I==errno ? strerror(errno):strerror(I));

printf("\n ERROR: %s",strerror(errno));

errno=EBADF;

//Write send back to client

writeSendBack("1",errno, SOCK);

pthread_exit((void*)(EBADF));

}

//Successful write, send back how much wrote.
writeSendBack("0",I, SOCK);


//Exit.
pthread_exit((void*)0);
}



//Explained above main
bool SearchW(int fd){
Node* Q = Head;



while(Q!=NULL){
Node*Q1=Q->Next;

if(Q->FD==fd){
return false;
}

printf("\n FD: %d\n",Q->FD);

Q=Q1;

}


return true;


}


//Explained above main
bool TransCheck(char* FileName){
Node* H1 = Head;

while(H1!=NULL){
Node* H2 = H1->Next;

if(strcmp(H1->FileName,FileName)==0&&H1->Mode=='2'){
return true;
}

H1=H2;
}

return false;
}


//Creates copy of the string given
char* toString(char* C){
if(C==NULL){
return NULL;
}
char* v = malloc((strlen(C)+1)*sizeof(char));
bzero(v,strlen(C)+1);
//
int i=0;
char u = C[i];
while(u!='\0'){
v[i]=u;
i++;
u=C[i];
}
return v;
}

//Explained above main
bool CheckWrite(char* FileName){
	Node* ptr;
	for(ptr = Head; ptr != NULL; ptr=ptr->Next){
	
		if(  (strcmp(ptr->FileName,FileName) == 0 ) && ((ptr->Permission == '1') || (ptr->Permission == '2') ) ) 
			return true;
	}
	return false;
}

bool ExclusiveWrite(char* FileName){
        Node* ptr;
        for(ptr = Head; ptr != NULL; ptr=ptr->Next){

                if(  (strcmp(ptr->FileName,FileName) == 0 ) && ((ptr->Permission == '1') || (ptr->Permission == '2') ) && ptr->Mode=='1')
                        return true;
        }
        return false;
}



//Explained above main
bool CASE2(char* FileName, char Permission){	

if((TransCheck(FileName))){
return true;

}

if(Permission=='0'){
return false;
}
//(Permission==1||Permission==2)
if(CheckWrite(FileName)){
return true;
}

return false;
}

bool SearchO(char* FileName){
Node* H1 = Head;

while(H1!=NULL){

Node* H2 = H1->Next;

if(strcmp(H1->FileName,FileName)==0){
return true;
}

H1=H2;
}


return false;

}



void o(char* buff, int size, int socket){		//o() accepts a buff, which is part of the message sent from the client on a netopen(), and 								//size of
							//the buffer. It then processes said message and extracts the attributes from it to make
							//a open command
	
	char perm = buff[1];		//we can extract the permission and mode of the open already, which are the 2nd and third position in the 
					//buffer respectively
	char mode = buff[2];		
	
	char* fileName = (char*)malloc( sizeof(char) * (size -2));	//we have to malloc a buffer for extracting the file name. we know that
									//the buffer begins with 0 followed by perm and mode, so the size of 
									//the file name is everything else
		if(fileName == NULL)
			exit(-1);
	
	strncpy(fileName, buff + 3, size - 3);			//extracting file name from buff to fileName
	
						
	ThreadOpen* T = malloc(sizeof(ThreadOpen));	
		if(T == NULL)
			exit(-1);

	pthread_t thread1;				//we are creating the open thread now to handle the open command since we have all the info
							//extracted from the buffer
	pthread_t* threadhandle = &thread1;


	T->Socket=socket;
	T->Mode=mode;					//setting the T struct attributes to the extracted info, since we have to send this to
	T->Filename=fileName;				//the open thread to further process this command
	T->Permission = perm;

		
	pthread_create(&thread1,NULL,pthreadopen,(void*)T);	
	
	pthread_join(thread1,NULL);			//run the open thread and now we have to wait till it has finished


	free(T);
	T=NULL;


	
}


void r(char* buff, int size,int socket){	//r() accepts a buff, which is part of the message sent from the client. Using the sent in size, r()
						//processes said buff and extracts the needed info in order to do a read command



	int count = 1; // this is the count since we need to first extract the number of bytes to read which is the first thing in this part of
			//message. From counts size, we can call strncpy() on the buff and write the number of bytes to numOfBytesToRead
	char* numOfBytesToRead;
	char* fd;

	while(1){
	
		if(buff[count] == ','){		//hit a delimiter, so finished scanning through the number of bytes part of the message
			numOfBytesToRead = (char*)malloc((sizeof(char)) * count);
				if(numOfBytesToRead == NULL)
					exit(-1);
			strncpy(numOfBytesToRead,buff + 1, (count -1 ) );	//we can extract said number
			break;
			}
		count++;
	}
	
	fd = (char*)malloc( (sizeof(char)) * (size - count) );	//we can now extract the fd portion since count tells us the size of the number portion
								//if we subtract that from the total size(size), this will give us the needed space for
								//the fd

		if(fd == NULL)
			exit(-1);

	strncpy(fd, buff + count + 1, (size - count - 1) );	//we are now writing the corresponding fd value to fd which starts at count +1.
								//have to add +1 too since have to skip over that delimiter that divides the number
								//of bytes and fd
	

		//at this portion, we have extracted all the needed info from the buffer
	ThreadRead* T = malloc(sizeof(ThreadRead));
		if(T == NULL)
			exit(-1);
	T->Socket=socket;
	int FD = atoi(fd);
	int BYTES = atoi(numOfBytesToRead);		//setting the T struct with said extracted info in order to send it to the read thread 
							//which will then process the read command further
	T->FD=FD;
	T->BYTES=BYTES;

	pthread_t thread1;
	pthread_t* threadhandle0 = &thread1;

	pthread_create(&thread1,NULL,pthreadread,(void*)T);	//starting the read thread and now we have to wait for it to finish
	
	pthread_join(thread1,NULL);


	free(fd);
	free(numOfBytesToRead);
	

}

void w(char* buff, int size, int socket){	//w() accepts a buff, which is part of the message the client sent to the server on a write command.
						//w() will now process this part of the message and extract all needed info in order to further process
						//the write command

	

	int count = 1;
	char* numOfBytes;
	char* fd;
	while(1){		//we now have to extract the number of bytes written portion in the buff and will write that to numOfBytes.
				//this loop looks at each char of buff one by one until a delimiter is reached( the delimiter that sepearats the number
				//and fd. Once we have that size in bytes of the number of bytes to be written( denoted by count), we can then
				//cpy that value into numOfBytes

		if(buff[count] == ','){	//delimiter is found, so the number of bytes written portion is now over
			
			numOfBytes = (char*)malloc( sizeof(char) * count );	
				if(numOfBytes == NULL)
					exit(-1);		//we can now write the number of bytes written into numOfBytes

			strncpy(numOfBytes, buff + 1, (count - 1) );
			
			break;
			}
		count += 1;
	}
	
	int count2 = count + 1;

	while(1){  //we now have to extract the fd portion of the message, which is right after the number of bytes written. We have to create count2
		   //and add one to it in order to skip over the comma delimiter


		if(buff[count2] == ','){ //the delimiter is found, so the fd portion is over

		fd = (char*)malloc(sizeof(char) * (count2 - count) );	//the size of the fd would be the differnece of the count and count2 
			if(fd == NULL)
				exit(-1);

		strncpy( fd, buff + count + 1, (count2 - count - 1) );	//writing the corresponding  fd value into fd
		
		break;
			}
		count2 += 1;
		}
	
	
	//Now that we have count2, which marks the end of fd portion or the start of the buffer to be written. We can now extract said buffer from the
		//mess and write that to mess
	char* mess = (char*)malloc( sizeof(char) * (size - count2) );
		if(mess == NULL)
			exit(-1);
	strncpy( mess, buff + count2 + 1, size - count2 - 1);	//writing the message portion into mess
	
	//at this point, we have all the needed info
	int BYTES= atoi(numOfBytes);
	int FD = atoi(fd);		//setting the extracted info into there int forms since this info must be plugged into the threadWrite struct
	char * M = toString(mess);

	ThreadWrite* T = malloc(sizeof(ThreadWrite));
	
	if(T == NULL)
		exit(-1);

	T->FD=FD;
					//created the threadWrite stuct and now setting associated attributes with the int forms of the extracted
					//info
	T->BYTES=BYTES;
	
	T->MESSAGE=M;

	T->Socket=socket;


	pthread_t thread1;
	pthread_t* threadhandle0 = &thread1;
					//created the write thread which will take the threadWrite stuct and process the write command further.
					//we have to wait for the thread to end before returning out of function
	pthread_create(&thread1,NULL,pthreadwrite,(void*)T);


	pthread_join(thread1,NULL);




	free(fd);
	free(numOfBytes);
	free(mess);
}
			

void cl(char* buff, int size, int socket){  //cl() takes in part of the message, or buff, sent by the client in a netclose, and size of said portion of
						//the message. It then will process this buff and extract all the info needed to perform a close
						//command. It will then call close thread, which will accept a struct containing all extracted info and
						//further process this command


	char* fd = (char*)malloc( (sizeof(char)) * size ); //the only info inside the buff is the fd portion, so we can extract this and write the
							   //value into fd. the size of this is just size, since the buff just contains 1 delimiter
							   //and the fd
		if(fd == NULL)
			exit(-1);
		
	strncpy(fd, buff + 1, size - 1);		//writing the fd value from the buff into fd
	
	
	//At this portion, we have all the needed info extracted from the buff inorder to further process the close command
	pthread_t thread1;
	pthread_t * threadhandle0 = &thread1;

	ThreadClose* T = malloc(sizeof(ThreadClose));

		if(T == NULL)			//creating the threadClose struct since we need to plug the needed info into this struct and
						//send this to the close thread, which will further process the close command
			exit(-1);	
		
	T->Socket=socket;

	T->FD = atoi(fd);			//fd needs to be in its int form for the close thread

	pthread_create(&thread1,NULL,pthreadclose,(void*)T);

							//we then start and have to wait for the close thread to finsih
	pthread_join(thread1,NULL);

	free(fd);

}
		
void openSendBack(char* op, int fdOrErrno, int sock){  //sends back a correctly formatted message back the client base on the arg values
							//op = "0" on sucess, or "1" on error
							//fdOrErrno = the fd value is op is "0" or the errno value if op is "1"


	//have to get the total size of the message based on the bytes taken by each of the args first

	long int posFd = fdOrErrno * -1;		//getting a positive version of the fd
	
	long int bytesForFd = 0;
								 
	if(strcmp(op,"0") == 0){  //means sending back a fd instead of errno. Have to do this b/c fdOrErrno might be - on sucess.
		bytesForFd = floor( ( (log(posFd))/ (log(10)) ) ) + 2; //getting the bytes taken by the fd. Also have to comensate for the negative
		}

	else{        //So op was "1", so error, so cant use posFd b/c that value will be negative and no longer need to comensate for the negative sign
		bytesForFd = floor( ( (log(fdOrErrno))/ (log(10)) ) ) + 1;
		}

	
	long long int total = 3 + bytesForFd; //total now contains the total amount of bytes the message needs, excluding the size of the message
						//portion. Have to add by 3 to comensate for the op and delimiters.
		
	long long int bytesForTotal = floor( ( (log(total))/ (log(10)) ) ) + 1;	//geting bytes taken by total
	long long int bytesNeedForTMess = floor( ( (log(bytesForTotal + total))/ (log(10)) ) ) + 1; //getting bytes need for the size portion of the
												    //message		


					   
		//now we have the correct size of the message, now we can malloc the message
	char* message = (char*)malloc( sizeof(char) * ( bytesNeedForTMess + total + 1) );

		if(message == NULL)
			exit(-1);

	snprintf(message, bytesNeedForTMess + 1, "%d",total + bytesNeedForTMess);	//writing the size in bytes of the message to the front of the
											//message
	strncat(message,",",1);
									//now we can add delimiters and op to the message
	strncat(message,op,1);
	strncat(message,",",1);
	
	char* fdString = (char*)malloc(sizeof(char*) * (bytesForFd + 1));	//to add fdOrErrno, we first have to malloc a buffer to the correct
										//size of fdOrErrno which fdString points to 
	
	
	if(fdString == NULL)
		exit(-1);  //ran out of memory


	snprintf(fdString, bytesForFd + 1, "%d", fdOrErrno);		//writing associate value to fdString and adding that to the back of message
	
	strncat(message,fdString,bytesForFd);
	
	
	send(sock,message,strlen(message),0);	//At this point, we have sucessfully formated the message, we can now send it back to the client
	
	
	shutdown(sock,2);

	close(sock);			//killing the connection, since our job is done

	free(fdString);

	free(message);
}





void readSendBack(char* op, char* buff, int amountRead, int sock){ // on error send errno as amountRead instead
			
								//function formats a message to be sent back to the client based on args.
								//op = "0" on sucess, so amountRead will be the acutal amountRead.
								//op = "1" on failure, so amountRead will be the associated errno value

	char* message;
	
	if( strcmp(op, "1" )  == 0 ){ //op is "1" so failure. amountRead is the associated errno value
	
		//to format the message correctly, we need to know the amount of bytes each arg is taking

		long long int bytesForBuff = floor( ( (log(amountRead))/ (log(10)) ) ) + 1; //bytesForBuff is the amount of bytes the buff is taking
		long long int total = bytesForBuff + 3 ; //total is the total amount of bytes the message needs, excluding the size portion on the
							//front of the message. Adding by 3 to comensate for delimiters and op

		long long int bytesForTotal = floor( ( (log(total))/ (log(10)) ) ) + 1;
		long long int bytesNeedForTMess = floor( ( (log(bytesForTotal + total))/ (log(10)) ) ) + 1; //bytesForTotal is the number of bytes
													//total is taking. While bytesNeedForTMess
													//is the bytes the size portion of the message
													//is taking
		

			//at this point we know the correct size of the message, so we can now malloc message

		message = (char*)malloc( sizeof(char) * ( bytesNeedForTMess + total + 1) );	

			if(message == NULL)
				exit(-1);


		snprintf(message, bytesNeedForTMess + 1, "%d",total + bytesNeedForTMess);  //writing the size portion of the message to the front
											   //of the message

		strncat(message,",",1);
		strncat(message,op,1);			//adding delimiters and op to message
		strncat(message,",",1);
		
		char* ErrnoString = (char*)malloc(sizeof(char*) * (amountRead + 1)); //amountRead is errno. so to add errno to the message
										//have to malloc a buffer and strncat that to message	
		if(ErrnoString == NULL)
			exit(-1);  //ran out of memory

		snprintf(ErrnoString, bytesForBuff + 1, "%d", amountRead);
		strncat(message,ErrnoString,bytesForBuff);		//writing the correct errno value to ErrnoString and adding that to message
		free(ErrnoString);
	
		}

	else{		//op is "0" so success. Code below is setup the same as the if() case above. But this time, amountRead is the actual amountRead
		

		

		long long int total = amountRead + 3 ; //total is the total size in bytes of the message, including delimiters,op, and the size of the
							//buff
	
		long long int bytesForTotal = floor( ( (log(total))/ (log(10)) ) ) + 1;
		long long int bytesNeedForTMess = floor( ( (log(bytesForTotal + total))/ (log(10)) ) ) + 1;
		message = (char*)malloc( sizeof(char) * ( bytesNeedForTMess + total + 1) );

			if(message == NULL)
				exit(-1);

		snprintf(message, bytesNeedForTMess + 1, "%d",total + bytesNeedForTMess);
		strncat(message,",",1);

		strncat(message,op,1);
		strncat(message,",",1);
		
		strncat(message, buff, amountRead);

		}
	//At this point we have a correctly formatted message ready to send back to the client
	send(sock , message , strlen(message) , 0 );


	shutdown(sock,2);
				//we kill the connection since we are done with it
	close(sock);


	free(message);



}


void writeSendBack(char* op, int ErrnoOrBytesWritten, int sock){ // op is "0" on sucess. So ErrnoOrBytesWritten is the bytes written. when
								//op is "1", or failure, ErrnoOrBytesWritten is the associated errno value

	//to correctly format the message. We first have to determine the size in bytes of each of the args


	long long int bytesForErrno = floor( ( (log(ErrnoOrBytesWritten))/ (log(10)) ) ) + 1;	//bytesForErrno is the space needed 
												//for ErrnoOrBytesWritten
	long long int total = bytesForErrno + 3 ; //total is the total amount of bytes for the message including the delimiters and op, but excluding
						//the size portion of the message located on the front of the message
	
	long long int bytesForTotal = floor( ( (log(total))/ (log(10)) ) ) + 1;	//bytesForTotal is the bytes needed by the int value of total
	long long int bytesNeedForTMess = floor( ( (log(bytesForTotal + total))/ (log(10)) ) ) + 1;// bytesNeedForTMess is the bytes needed for the
													//size portion of the message

	//at this point we have the correct amount of bytes needed byt the message, we can now malloc message

	char* message = (char*)malloc( sizeof(char) * ( bytesNeedForTMess + total + 1) );
		if(message == NULL)
			exit(-1);

	snprintf(message, bytesNeedForTMess + 1, "%d",total + bytesNeedForTMess); //writing the correct size of the message to the front of the message

	strncat(message,",",1);
						//adding delimiters and op to message
	strncat(message,op,1);
	strncat(message,",",1);

	

		char* ErrnoString = (char*)malloc(sizeof(char*) * (bytesForErrno + 1));	//To add the ErrnoOrBytesWritten portion of the message,
											//first have to malloc a buffer and write the value to it
	
	


		if(ErrnoString == NULL)
			exit(-1);  //ran out of memory

		snprintf(ErrnoString, bytesForErrno + 1, "%d", ErrnoOrBytesWritten);
								//writing ErrnoOrBytesWritten to ErrnoString and then adding it to the
							        //back of the message
		strncat(message,ErrnoString,bytesForErrno);
		free(ErrnoString);
	

	//at this point, we have the correctly formatted message ready to send to the client
	send(sock , message , strlen(message) , 0 );		//sending the message to the client

	shutdown(sock,2);

	close(sock);				//we are now killing the connection since we no longer need it anylonger


	free(message);




}


void closeSendBack(char* op, int Errno,int sock){  //2nd argu has to be 0 on sucess
						//op is "0" on sucess, so Errno will be set to 1. when op is "1", this means error. So Errno is
						//the associated errno value
	
	//to correctly format the message, we first need to determine the size in bytes of each of the args

	long long int bytesForErrno = floor( ( (log(Errno))/ (log(10)) ) ) + 1;	//bytesForErrno now contains the bytes taken by Errno
	long long int total = 3+bytesForErrno;				//total now contains the total amount of bytes need by the message, including
									//op and delimiters, but excluding the size portion located on the front of 
									//the message


	
	long long int bytesForTotal = floor( ( (log(total))/ (log(10)) ) ) + 1;//bytesForTotal contains the bytes taken by int value of total


	long long int bytesNeedForTMess = floor( ( (log(bytesForTotal + total))/ (log(10)) ) ) + 1;//bytesNeedForTMess is the bytes taken by the size
												//portion of the message

	//at this point, we have the right amount of bytes needed for the message. We can now malloc the message	


	char* message = (char*)malloc( sizeof(char) * ( bytesNeedForTMess + total + 1) );
	
	if(message==NULL){

	exit(-1);

	}
	
	snprintf(message, bytesNeedForTMess + 1, "%d",total + bytesNeedForTMess);	//writing the correct size portion of the message to the
											//front of the message

	strncat(message,",",1);
	
	strncat(message,op,1);		//adding delimiters and op to the message
	strncat(message,",",1);
	
	

	if( strcmp(op,"0" ) == 0 ){ //when op is "0" on sucess, we can just add "0" to the back of the message
	strncat(message,"0",1);
	}

	else{	//when op is "1: on failure, we have to add the corresponding errno value to the back of the message

		char* ErrnoString = (char*)malloc(sizeof(char*) * (bytesForErrno + 1));	// to do this we have to malloc a buffer first
	
	
		if(ErrnoString == NULL)
			exit(-1);  //ran out of memory

		snprintf(ErrnoString, bytesForErrno + 1, "%d", Errno);
								//writing the corresponding errno value to ErrnoString and then adding ErrnoString
								//to the back of the message
		strncat(message,ErrnoString,bytesForErrno);		
		free(ErrnoString);
	}


	//at this point, we have the correctly formatted message ready to send to the client
	send(sock , message , strlen(message) , 0 );		//sending the message to the client

	shutdown(sock,2);

	close(sock);				//we are now killing the connection since we no longer need it anylonger


	free(message);

}








