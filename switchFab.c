#include <fcntl.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
static void __attribute__ ((constructor)) sF_Init(void);
//static void __attribute__ ((destructor)) sF_save(void);
void *sFswitch=0;
void *function=0;
void *offset=0;

void sF_Init(){
    //struct stat sb;
    int fd;
    uid_t eid;
    gid_t gid;

    eid=geteuid();
    gid=getegid();

    seteuid(1000);
    setegid(1000);

    fd=shm_open("/switchFab",O_RDWR | O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
/*    if(fd==-1){return errno;}

    fstat(fd,&sb);
    if(sb.st_mode!=(S_IFREG|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)){
	printf("Mode: %o.\n",sb.st_mode);
	//shm_unlink("/switchFab");
        close(fd);
	return 500;
    }*/

    ftruncate(fd,sizeof(bool));
    sFswitch=mmap(NULL,sizeof(bool),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);

    fd=shm_open("/function",O_RDWR | O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    ftruncate(fd,1073741824);
    function=mmap(NULL,1073741824,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);

    fd=shm_open("/offset",O_RDWR | O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    ftruncate(fd,sizeof(long));
    offset=mmap(NULL,sizeof(long),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);

    seteuid(eid);
    setegid(gid);/*
    if(sFswitch==-1){return errno;}
    else{
	*(bool*)sFswitch=false;
	return 0;
    }*/
}

bool sF_Switch(){
    /*int i=0;
    if(sFswitch==-1||sFswitch==0){
	i=sF_Init();
    }*/
    if(sFswitch==-1||sFswitch==0){return false;}
    else{return *(bool*)sFswitch;}
}

void sF_turnOn(){
    /*int i=0;
    if(sFswitch==-1||sFswitch==0){
	i=sF_Init();
        if(i!=0){
	if(i==500){printf("Mismatched Mode:?");}
	else{printf("Errno: %s.\n",strerror(i));}
	}
    }
    else{*/*(bool*)sFswitch=true;//}
}

void sF_turnOff(){
    /*int i=0;
    if(sFswitch==-1||sFswitch==0){
	i=sF_Init();
        if(i!=0){
	if(i==500){printf("Mismatched Mode:?");}
	else{printf("Errno: %s.\n",strerror(i));}
	}
    }
    else{*/*(bool*)sFswitch=false;//}
    int i;
    FILE * fi;
    fi = fopen("/home/mesa/function.log","a");
    printf("File: %d.\n",fi);
    for(i=0;i<1073741824;i++){
	if(*((char*)function+i)!=0){fputc(*((char*)function+i),fi);}
	else{break;}
    }
    fclose(fi);
    
}

void sF_write(char *c, int i){
    int j=0;
    if(((*(long*)offset)+i)>1073741824){return;}
    //printf("Writing %i chars.\n",i);
    for(j=0;j<i;j++){
	*((char*)function+(*(long*)offset))=*(c+j);
	*((long*)offset)=*((long*)offset)+1;
    }
    *((char*)function+(*(long*)offset))='\n';
    *((long*)offset)=*((long*)offset)+1;
    //printf("Offset: %ld.\n",*((long*)offset)); 
}
