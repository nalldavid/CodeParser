/* 
 * File:   main.c
 * Author: chrome
 *
 * Created on April 30, 2013, 11:13 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

/*
 * 
 */
  #define MAX_PATH 1024


 // typedef struct {       /* portable directory entry */
 //      long ino;                  /* inode number */
 //      char name[NAME_MAX+1];     /* name + '\0' terminator */
 //  } Dirent;*/
    FILE * parseLog;
void *parse(char *);
void callParse(char * name);
void dirwalk(char *, void (*fcn)(char *));
int main(int argc,char *argv[]) {
    parseLog=fopen("/home/mesa/parse.log","a");
    callParse(argv[1]);    
    fprintf(stderr,"done");
    fclose(parseLog);
    return (EXIT_SUCCESS);
}

void callParse(char * name){          //fsize equivalent
    char n[MAX_PATH];
    char * nameStart;
    struct stat stbuf;
    int j;

    nameStart=name;
    for(j=0;j<MAX_PATH;j++){n[j]=0;}
    for(j=0;j<MAX_PATH;j++){
        if(*name==0){break;}
        n[j]=*name;
        name++;
    }
    name=nameStart;
    fprintf(parseLog,"Parsing %s.\n",n);
    printf("Parsing %s.\n",n);
    if (stat(n, &stbuf) == -1) {
           fprintf(stderr, "callParse: can't access %s\n", n);
           return;
    }
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR){dirwalk(name, callParse);}
    else{parse(name);}
    //printf("%8ld %s\n", stbuf.st_size, name);
}

void dirwalk(char *dir, void (*fcn)(char *)){
       char name[MAX_PATH];
       char n[MAX_PATH];
       struct dirent *dp;
       DIR *dfd;
       int j;
       
       for(j=0;j<MAX_PATH;j++){n[j]=0;}
       for(j=0;j<MAX_PATH;j++){
               if(*dir==0){break;}
               n[j]=*dir;
               dir++;
       }

       if ((dfd = opendir(n)) == NULL) {
           fprintf(parseLog, "dirwalk: can't open %s\n", n);
           return;
       }
       while ((dp = readdir(dfd)) != NULL) {
           if (strcmp(dp->d_name, ".") == 0
               || strcmp(dp->d_name, "..") == 0){
               fprintf(parseLog,"%s.\n",dp->d_name);
               continue;    /* skip self and parent */
           }
           if (strlen(n)+strlen(dp->d_name)+2 > sizeof(name))
               fprintf(parseLog, "dirwalk: name %s %s too long\n",
                   n, dp->d_name);
           else {
               sprintf(name, "%s/%s", n, dp->d_name);
               (*fcn)(name);
           }
       }
       closedir(dfd);
   }

void *parse(char * name){
  char nameString[MAX_PATH];
  char nameWString[MAX_PATH];
  char * nameStart;
  FILE * readFile;
  FILE * writeFile;
  int j,i,k;
  char line[500];
  char funcName[500];
  char tmpLine[500];
  int numParen;
  int nameLength;
  int functionLength;

  bool foundASpace;
  bool itsAFunc;
  bool foundParen;
  bool firstAlpha;
  bool foundFirstChar;
  bool funcCall;
  bool EndOfFile;
  bool funcPrototype;
  bool multiLine;
  bool cFile;
  bool foundComment;
  bool endComment;
  bool singleLine;
  bool endParen;
  bool keywordStruct;
  bool foundEquals;
  bool foundComma;
  bool foundSpace;
  bool putusing;
  bool foundstructdefinition;

  int funcStartPos;
  int funcEndPos;
  int noParen;
  int lineLen;
  int firstChar;
  int firstCharPos;
  int lineNum;
  int fint;
  int stop;
  int noComment;
  int noBeginComments;
  int numBrackets;
  int lastInclude;
  int numbeginparen;
  int numendparen;


  noParen=-1;
  firstAlpha=-1;
  firstChar=0;
  foundASpace = false;
  itsAFunc = false;
  foundFirstChar=false;
  funcCall=false;
  funcStartPos=0;
  funcEndPos=0;
  lineNum=1;
  EndOfFile=false;
  foundParen=false;
  funcPrototype=false;
  multiLine=false;
  cFile=false;
  nameStart=name;
  noComment=0;
  foundComment=false;
  endComment=false;
  singleLine=false;
  endParen=false;
  numParen=0;
  keywordStruct=false;
  foundEquals=false;
  numBrackets=0;
  foundComma=false;
  foundSpace=false;
  putusing=true;
  foundstructdefinition=false;
  numbeginparen=0;
  numendparen=0;

  fprintf(parseLog,"In parse");

  char *prefix="   if(sF_Switch()){\n   char c[300];\n	struct timeval tv;\n	gettimeofday(&tv, NULL);\n	sprintf(c, \"[%ld.%ld] ";
  char *suffix="tv.tv_sec, tv.tv_usec);\n	sF_write(c,strlen(c));\n	}\n";
  char *tmp;
  char writeArg[1024];
  //char writeArgCount[3];
 
  for(j=0;j<MAX_PATH;j++){nameString[j]=0;}  //clearString
    for(j=0;j<MAX_PATH;j++){nameWString[j]=0;}  //clearString
  tmp=name;
  for(j=0;j<MAX_PATH;j++){
       if(*name==0){
           if(j==(MAX_PATH-2)){printf("Path too long.\n");return;}
           name--;
           if(*name=='p'){
               name--;
               if(*name=='p'){
               name--;
               if(*name=='c'){
               name--;
               if(*name=='.'){
                   cFile=true;
                   name=nameStart;
                   stop = j-4;
                   for(j=0;j<stop;j++){
                       nameWString[j]=*name;
                       name++;
                   }
                   nameWString[stop]='5';
                   nameWString[stop+1]='.';
                   nameWString[stop+2]='c';
                   nameWString[stop+3]='p';
                   nameWString[stop+4]='p';
               }}}
           }
           break;
       }
       nameString[j]=*name;
       name++;
  }
  if(!cFile){return;}
  name=tmp;
  for(j=0;j<500;j++){line[j]='\n';};
  for(j=0;j<500;j++){tmpLine[j]='\n';};
  for(j=0;j<500;j++){funcName[j]=0;};
  readFile=fopen (nameString,"r");
  
  char c;
  char switchh[23]={'#','i','n','c','l','u','d','e',' ','<','s','w','i','t','c','h','F','a','b','.','h','>','\n'};
  char stringh[20]={'#','i','n','c','l','u','d','e',' ','<','s','t','r','i','n','g','.','h','>','\n'};
  char timeh[22]={'#','i','n','c','l','u','d','e',' ','<','s','y','s','/','t','i','m','e','.','h','>','\n'};
  char hold[10]={0};
  char holdf[9]={0};
  bool stdIncluded=false;
  bool stdfIncluded=false;

/*  char namespace[21]={'u','s','i','n','g',' ','n','a','m','e','s','p','a','c','e',' ','s','t','d',';','\n'};

  do{
      c=fgetc(readFile);
      hold[9]=c;
      for(j=0;j<10;j++){if(hold[j]!=std[j]){break;}else{if(j==9){stdIncluded=true;}}}
      if(stdIncluded){break;}
      for(j=0;j<10;j++){hold[j]=hold[j+1];}
  }while(c!=EOF);
  fclose(readFile);
  readFile=fopen(nameString,"r");

  do{
	for(j=0;j<500;j++){
		line[j] = fgetc (readFile);
		if(line[j]=='\n'||line[j]==EOF)
		{
			if(line[j]==EOF){EndOfFile=true;}
			lineLen=j+1;
			break;
		}
	}					//load line
	if(line[0]=='#'&&line[1]=='i'&&line[2]=='n'){lastInclude=lineNum+1;}
	for(j=0;j<500;j++){line[j]='\n';}
        lineNum++;
  }while(!EndOfFile);
  EndOfFile=false;
  lineNum=1;
  fclose(readFile);
  readFile=fopen(nameString,"r");

  do{
      c=fgetc(readFile);
      holdf[8]=c;
      for(j=0;j<9;j++){if(holdf[j]!=stdf[j]){break;}else{if(j==8){stdfIncluded=true;}}}
      if(stdfIncluded){break;}
      for(j=0;j<9;j++){holdf[j]=holdf[j+1];}
  }while(c!=EOF);
  fclose(readFile);
  readFile=fopen(nameString,"r");*/
  
  fint = open(nameWString,O_CREAT,0666);  //create file to write to
  close(fint);                            //close it for reopening in write mode
  writeFile=fopen (nameWString,"w");  /*
  if(!stdIncluded){
      for(j=0;j<20;j++){fputc(stdWhole[j],writeFile);}
  }
  if(!stdfIncluded){
      for(j=0;j<19;j++){fputc(stdfWhole[j],writeFile);}
  }*/
  for(j=0;j<23;j++){fputc(switchh[j],writeFile);}
  for(j=0;j<20;j++){fputc(stringh[j],writeFile);}
  for(j=0;j<22;j++){fputc(timeh[j],writeFile);}
  //for(j=0;j<21;j++){fputc(namespace[j],writeFile);}
  if (readFile==NULL) perror ("Error opening file");
  else
  {
    do {
	fprintf(parseLog,"Line number:%i.\n",lineNum);
	for(j=0;j<500;j++){
		line[j] = fgetc (readFile);
		if(line[j]=='\n'||line[j]==EOF)
		{
			if(line[j]==EOF){EndOfFile=true;}
			lineLen=j+1;
			break;
		}
	}					//load line

	for(j=0;j<500;j++){
		if(line[j]==EOF){EndOfFile=true;break;}
		fputc(line[j],writeFile);
		if(line[j]=='\n'||line[j]==EOF){break;}
	}				//write line to file
        
        for(j=0;j<lineLen;j++){
		if(line[j]=='/'&&line[j+1]=='/'){
			noComment=j;
			foundComment=true;
			break;
		}
	}			//search for single-line comments
        
        if(foundComment){
            for(j=noComment;j>=0;j--){
                if((64<line[j]&&line[j]<91)||(line[j]==95)||(96<line[j]&&line[j]<122)){lineLen=noComment-1;foundComment=false;}
            }
        }
        
        if(!foundComment){
	    noBeginComments=0;
            for(j=0;j<lineLen;j++){
        		if(line[j]=='/'&&line[j+1]=='*'){
        			noComment=j;
                                noBeginComments++;
        		}
                        if(line[j]=='*'&&line[j+1]=='/'&&(noBeginComments>0)){noBeginComments=0;foundComment=true;}
        	}			//search for multi-line comments
            
            if(noBeginComments>0){
                do{
                        lineNum++;
                        for(j=0;j<500;j++){
                         	line[j] = fgetc (readFile);
                         	if(line[j]=='\n'||line[j]==EOF)
                        	{
                        		if(line[j]==EOF){EndOfFile=true;}
                        		lineLen=j+1;
                        		break;
                        	}
                        }					//load line

                        for(j=0;j<500;j++){
                        	if(line[j]==EOF){EndOfFile=true;break;}
                        	fputc(line[j],writeFile);
                        	if(line[j]=='\n'||line[j]==EOF){break;}
                        }				//write line to file
        
                        for(j=0;j<lineLen;j++){
                                if(line[j]=='*'&&line[j+1]=='/'){noBeginComments--;}
                        }			//resolve comment counts
                        for(j=0;j<500;j++){line[j]='\n';}//clear line
                }while((noBeginComments>0)&&!EndOfFile);
            }
        }

        if(!foundComment){
        	for(j=0;j<lineLen;j++){
        		if(line[j]=='('){
        			noParen=j;
        			foundParen=true;
        			break;
        		}
        	}			//search for parentheses
        }
        
        for(j=0;j<lineLen;j++){
            if(line[j]=='s'&&line[j+1]=='t'&&line[j+2]=='r'&&line[j+3]=='u'&&line[j+4]=='c'&&line[j+5]=='t'){
                for(k=j+6;k<lineLen;k++){
                    if(line[k]=='('){foundParen=true;break;}
                    if(line[k]=='{'){foundParen=false;break;}
					if(line[k]=='='){foundstructdefinition=true;foundParen=false;break;}
                }
            }
            if(foundParen){break;}
        }

		if(foundstructdefinition){
			for(j=0;j<lineLen;j++){
				if(line[j]=='{'){numbeginparen++;}
				if(line[j]=='}'){numendparen++;}
			}
			if(numbeginparen!=numendparen){
			                do{
                        lineNum++;
                        for(j=0;j<500;j++){
                         	line[j] = fgetc (readFile);
                         	if(line[j]=='\n'||line[j]==EOF)
                        	{
                        		if(line[j]==EOF){EndOfFile=true;}
                        		lineLen=j+1;
                        		break;
                        	}
                        }					//load line

                        for(j=0;j<500;j++){
                        	if(line[j]==EOF){EndOfFile=true;break;}
                        	fputc(line[j],writeFile);
                        	if(line[j]=='\n'||line[j]==EOF){break;}
                        }				//write line to file
        
                        for(j=0;j<lineLen;j++){
                                if(line[j]=='{'){numbeginparen++;}
								if(line[j]=='}'){numendparen++;}
                        }			//resolve struct parenthesis
                        for(j=0;j<500;j++){line[j]='\n';}//clear line
                }while((numbeginparen!=numendparen)&&!EndOfFile);
			}
			numbeginparen=0;
			numendparen=0;

		}				//found struct definition

	if(line[0]=='#'&&line[1]=='d'){
		foundParen=false;
		for(j=0;j<500;j++){
			if(line[j]==92&&line[j+1]=='\n'){
			multiLine=true;
			for(j=0;j<500;j++){line[j]='\n';}
			do{
				lineNum++;
				fprintf(parseLog,"Counting define extensions in line number: %i.\n",lineNum);
                      		for(j=0;j<500;j++){
			        line[j] = fgetc (readFile);
  				if(line[j]=='\n'||line[j]==EOF)
			        {
                        		if(line[j]==EOF){EndOfFile=true;}
                        		lineLen=j+1;
                        		break;
	                        	}
	                        }					//load line

	                       for(j=0;j<500;j++){
		                       	if(line[j]==EOF){EndOfFile=true;break;}
	                        	fputc(line[j],writeFile);
	                        	if(line[j]=='\n'||line[j]==EOF){break;}
	                       }				//write line to file
				for(j=0;j<500;j++){
				if(line[j+1]=='\n'){
					if(line[j]!=92){multiLine=false;}
					if(line[j]==92){break;}
				}
				}
				for(j=0;j<500;j++){line[j]='\n';}
			}while(multiLine);
			}
			
		}
	}
        
        if(line[0]=='#'){foundParen=false;}


	    for(j=0;j<lineLen;j++){
            if(line[j]=='{'){numbeginparen++;}
			if(line[j]=='}'){numendparen++;}
        }
		if(numbeginparen==1&&numendparen==1){foundParen=false;numbeginparen=0;numendparen=0;} //check for single line empty function definition

	
	if(foundParen)
	{
  		for(j=0;j<noParen;j++)		//check for if, for or do statements
		{
			if(!foundFirstChar)
			{
				if((64<line[j]&&line[j]<91)||(line[j]==95)||(96<line[j]&&line[j]<123)||line[j+1]=='(')	//alphabet or underscore
				{
					foundFirstChar=true;
					firstCharPos=j;
					if(line[j]==','){
						foundParen=false;
						for(j=0;j<500;j++){line[j]='\n';}
						break;
					}
					if(line[j+1]=='('){
						foundParen=false;
						for(j=0;j<500;j++){line[j]='\n';}
						break;
					}
					if((line[j]=='i')&&(line[j+1]=='f')&&((line[j+2]==' ')||(line[j+2]=='(')))
					{
						numParen=0;
						numBrackets=0;
						fprintf(parseLog,"Found an if statement in line:%i.\n",lineNum);
						for(k=j;k<500;k++){
							if(line[k]=='('){numParen++;}
							if(line[k]==')'){numParen--;}
							if(line[k]=='{'){numBrackets++;}
							if(line[k]=='}'){numBrackets--;}
						}
						if(numParen>0){
						for(j=0;j<500;j++){line[j]='\n';}
						do{
							lineNum++;
							fprintf(parseLog,"Line number: %i.\n",lineNum);
                       					for(j=0;j<500;j++){
			                         	line[j] = fgetc (readFile);
  				                       	if(line[j]=='\n'||line[j]==EOF)
			                        	{
			                        		if(line[j]==EOF){EndOfFile=true;}
			                        		lineLen=j+1;
			                        		break;
				                        	}
				                        }					//load line

 				                       for(j=0;j<500;j++){
 				                       	if(line[j]==EOF){EndOfFile=true;break;}
				                        	fputc(line[j],writeFile);
				                        	if(line[j]=='\n'||line[j]==EOF){break;}
 				                       }				//write line to file
							for(j=0;j<500;j++){
								if(line[j]=='('){numParen++;}
								if(line[j]==')'){numParen--;}
								if(line[j]=='{'){numBrackets++;}
								if(line[j]=='}'){numBrackets--;}
							}					//resolve numParen
							for(j=0;j<500;j++){line[j]='\n';}
						}while(numParen>0&&!EndOfFile);
						}
						if(numBrackets>0){
						for(j=0;j<500;j++){line[j]='\n';}
						do{
							lineNum++;
							fprintf(parseLog,"Line number: %i.\n",lineNum);
                       					for(j=0;j<500;j++){
			                         	line[j] = fgetc (readFile);
  				                       	if(line[j]=='\n'||line[j]==EOF)
			                        	{
			                        		if(line[j]==EOF){EndOfFile=true;}
			                        		lineLen=j+1;
			                        		break;
				                        	}
				                        }					//load line

 				                       for(j=0;j<500;j++){
 				                       	if(line[j]==EOF){EndOfFile=true;break;}
				                        	fputc(line[j],writeFile);
				                        	if(line[j]=='\n'||line[j]==EOF){break;}
 				                       }				//write line to file
							for(j=0;j<500;j++){
								if(line[j]=='{'){numBrackets++;}
								if(line[j]=='}'){numBrackets--;}
							}					//resolve numParen
							for(j=0;j<500;j++){line[j]='\n';}
						}while(numBrackets>0&&!EndOfFile);
						}
						for(j=0;j<500;j++){line[j]='\n';}			//if statement | clear line and move on
						foundParen=false;
						break;
					}
					if((line[j]=='f')&&(line[j+1]=='o')&&(line[j+2]=='r')&&((line[j+3]==' ')||(line[j+3]=='(')))
					{
						fprintf(parseLog,"Found a for statement in line:%i.\n",lineNum);
						for(j=0;j<500;j++){line[j]='\n';}			//for statement | clear line and move on
						foundParen=false;
						break;
					}
					if((line[j]=='d')&&(line[j+1]=='o')&&((line[j+2]==' ')||(line[j+2]=='(')))
					{
						fprintf(parseLog,"Found a do statement in line:%i.\n",lineNum);
						for(j=0;j<500;j++){line[j]='\n';}			//do statement | clear line and move on
						foundParen=false;
						break;
					}
                                        if((line[j]=='w')&&(line[j+1]=='h')&&(line[j+2]=='i')&&(line[j+3]=='l')&&(line[j+4]=='e')&&((line[j+5]==' ')||line[j+5]=='('))
					{
						fprintf(parseLog,"Found a while statement in line:%i.\n",lineNum);
						for(j=0;j<500;j++){line[j]='\n';}			//while statement | clear line and move on
						foundParen=false;
						break;
					}
                                        if((line[j]=='s')&&(line[j+1]=='w')&&(line[j+2]=='i')&&(line[j+3]=='t')&&(line[j+4]=='c')&&(line[j+5]=='h')&&((line[j+6]==' ')||line[j+6]=='('))
					{
						fprintf(parseLog,"Found a switch statement in line:%i.\n",lineNum);
						for(j=0;j<500;j++){line[j]='\n';}			//switch statement | clear line and move on
						foundParen=false;
						break;
					}
					if((line[j]=='e')&&(line[j+1]=='l')&&(line[j+2]=='s')&&(line[j+3]=='e')&&(line[j+4]==' ')&&(line[j+5]=='i')&&(line[j+6]=='f')&&((line[j+7]==' ')||(line[j+7]=='(')))
					{
						numParen=0;
						fprintf(parseLog,"Found an else if statement in line:%i.\n",lineNum);
						for(k=j;k<500;k++){
							if(line[k]=='('){numParen++;}
							if(line[k]==')'){numParen--;}
						}
						if(numParen>0){
						do{
							lineNum++;
							fprintf(parseLog,"Line number: %i.\n",lineNum);
                       					for(j=0;j<500;j++){
			                         	line[j] = fgetc (readFile);
  				                       	if(line[j]=='\n'||line[j]==EOF)
			                        	{
			                        		if(line[j]==EOF){EndOfFile=true;}
			                        		lineLen=j+1;
			                        		break;
				                        	}
				                        }					//load line

 				                       for(j=0;j<500;j++){
 				                       	if(line[j]==EOF){EndOfFile=true;break;}
				                        	fputc(line[j],writeFile);
				                        	if(line[j]=='\n'||line[j]==EOF){break;}
 				                       }				//write line to file
							for(j=0;j<500;j++){
								if(line[j]=='('){numParen++;}
								if(line[j]==')'){numParen--;}
							}					//resolve numParen
						}while(numParen>0&&!EndOfFile);
						}
						for(j=0;j<500;j++){line[j]='\n';}			//if statement | clear line and move on
						foundParen=false;
						break;
					}
                                        if((line[j]=='e')&&(line[j+1]=='l')&&(line[j+2]=='s')&&(line[j+3]=='e')&&((line[j+4]==' ')||line[j+4]=='(')||line[j+4]=='\n')
					{
						fprintf(parseLog,"Found an else statement in line:%i.\n",lineNum);
						for(j=0;j<500;j++){line[j]='\n';}			//switch statement | clear line and move on
						foundParen=false;
						break;
					}

				}
				if(line[j+1]=='('){
				for(i=j+2;i<500;i++)
					{
						if(line[i]==' '){break;}
						if(line[i]==')'){foundParen=false;
							fprintf(parseLog,"Found a type cast in line number: %i.\n",lineNum);
							break;
						}
					}
				}
			}
		}

        for(j=0;j<lineLen;j++){
            	if(line[j]=='='){
			foundParen=false;
			foundEquals=true;
			fprintf(parseLog,"Found = in line number: %i.\n",lineNum);
			if(!(((64<line[j-1]&&line[j-1]<91)||(line[j-1]==95)||(96<line[j-1]&&line[j-1]<123)||line[j-1]==' ')&&
			((64<line[j+1]&&line[j+1]<91)||(line[j+1]==95)||(96<line[j+1]&&line[j+1]<123)||line[j+1]==' '||line[j+1]=='\n'))){foundEquals=false;}
		}

		if(foundEquals){
			if(line[j]==';'){break;}
			if(j==lineLen-1){
				multiLine=true;
				do{
					lineNum++;
					fprintf(parseLog,"Line number: %i.\n",lineNum);
		                       for(j=0;j<500;j++){
		                         	line[j] = fgetc (readFile);
               		          		if(line[j]=='\n'||line[j]==EOF)
                       		 		{
                      		  			if(line[j]==EOF){EndOfFile=true;}
                        				lineLen=j+1;
                        				break;
                        			}
                        		}					//load line

		                        for(j=0;j<500;j++){
                		        	if(line[j]==EOF){EndOfFile=true;break;}
                		        	fputc(line[j],writeFile);
                       			 	if(line[j]=='\n'||line[j]==EOF){break;}
                        		}				//write line to file
					for(j=0;j<500;j++){
						if(line[j]==';'){multiLine=false;}
					}
				}while(multiLine);
			}

		}
        }
		if(foundParen){
			for(j=0;j<500;j++){
				if(line[j]=='('){
					for(k=j;k<500;k++){
						if(line[k]==' '){foundSpace=true;}
						if(line[k]==','&&(!foundSpace)){
							foundParen=false;
							break;							
						}
					}
					if(!foundParen){break;}
				}				
			}
			if(!foundParen){
						numParen=0;
						fprintf(parseLog,"Found a function argument in line:%i.\n",lineNum);
						for(k=0;k<500;k++){
							if(line[k]=='('){numParen++;}
							if(line[k]==')'){numParen--;}
						}
						if(numParen>0){
						for(j=0;j<500;j++){line[j]='\n';}
						do{
							lineNum++;
							fprintf(parseLog,"Counting parethesis in line number: %i.\n",lineNum);
                       					for(j=0;j<500;j++){
			                         	line[j] = fgetc (readFile);
  				                       	if(line[j]=='\n'||line[j]==EOF)
			                        	{
			                        		if(line[j]==EOF){EndOfFile=true;}
			                        		lineLen=j+1;
			                        		break;
				                        	}
				                        }					//load line

 				                       for(j=0;j<500;j++){
 				                       	if(line[j]==EOF){EndOfFile=true;break;}
				                        	fputc(line[j],writeFile);
				                        	if(line[j]=='\n'||line[j]==EOF){break;}
 				                       }				//write line to file
							for(j=0;j<500;j++){
								if(line[j]=='('){numParen++;}
								if(line[j]==')'){numParen--;}
							}					//resolve numParen
							for(j=0;j<500;j++){line[j]='\n';}
						}while(numParen>0&&!EndOfFile);
						}
						for(j=0;j<500;j++){line[j]='\n';}			//if statement | clear line and move on
						foundParen=false;
			}
		}

		if(foundParen)
		{
                    numParen=0;
                    fprintf(parseLog,"Found a ( in line number: %i.\n",lineNum);
			for(j=0;j<500;j++)	//check for semicolon after parenthesis | not a function declaration
			{
                if(line[j]=='('){numParen++;}
				if(line[j]==')'){
                    numParen--;
				    if(line[j+1]==EOF){funcCall=true;EndOfFile=true;break;}
                          if(numParen==0){
                                for(i=j+1;i<500;i++){
                                      if(line[i]=='('){break;}
                                      if(line[i]==';'){funcCall=true;break;}
                                }
                          if(funcCall){fprintf(parseLog,"Is a funcCall1.\n");break;}
                    }
                 }			
				//fprintf(parseLog,"0\n");
				if(line[j]=='{'){numbeginparen++;}
				if(line[j]=='}'){numendparen++;}
				if(numbeginparen>0&&numbeginparen==numendparen){funcCall=true;}	//empty single line function definition, skip below code
				
			}

			if(!funcCall&&numParen!=0){
				multiLine=true;
				do{
					lineNum++;
					fprintf(parseLog,"Line number: %i.\n",lineNum);
		                       for(j=0;j<500;j++){
		                         	tmpLine[j] = fgetc (readFile);
               		          		if(tmpLine[j]=='\n'||tmpLine[j]==EOF)
                       		 		{
                      		  			if(tmpLine[j]==EOF){EndOfFile=true;}
                        				lineLen=j+1;
                        				break;
                        			}
                        		}					//load line

		                        for(j=0;j<500;j++){
                		        	if(tmpLine[j]==EOF){EndOfFile=true;break;}
                		        	fputc(tmpLine[j],writeFile);
                       			 	if(tmpLine[j]=='\n'||tmpLine[j]==EOF){break;}
                        		}				//write line to file
					for(j=0;j<500;j++){
						if(tmpLine[j]=='('){numParen++;}
						if(tmpLine[j]==')'){numParen--;}
						if((tmpLine[j]==')'&&tmpLine[j+1]==';')){multiLine=false;funcCall=true;break;}
						if(tmpLine[j]==')'&&numParen==0){multiLine=false;break;}
					}
					for(j=0;j<500;j++){
						if(tmpLine[j]=='{'){numbeginparen++;}
						if(tmpLine[j]=='}'){numendparen++;}						
					}
					if(numbeginparen>0&&numbeginparen!=numendparen){multiLine=true;}
					if(numbeginparen>0&&numbeginparen==numendparen){funcCall=true;multiLine=false;}
					for(j=0;j<500;j++){
						tmpLine[j]='\n';
					}
				}while(multiLine&&!EndOfFile);
			}
	
			if(!funcCall&&!EndOfFile){

				for(j=0;j<500;j++){
					if(line[j]==')'&&line[j+1]==';'){funcPrototype=true;}
					if(line[j]==EOF){funcPrototype=true;}
				}				//check for ; | function prototype
                                
				if(!funcPrototype&&!EndOfFile){		//insert snippet
					strcat(writeArg,name);
					strcat(writeArg," ");

					for(j=noParen-1;j>=0;j--)	//get function name ends
					{
						if(line[j]!=' '&&funcEndPos==0){funcEndPos=j+1;}
						if(line[j]==' '&&funcEndPos!=0){
							funcStartPos=j+1;
							functionLength=funcEndPos-funcStartPos-1;
							break;}
						if(j==0){funcStartPos=0;functionLength=funcEndPos-funcStartPos-1;}
					}
					fprintf(parseLog,"Getting funcName.\n");
					for(j=funcStartPos;j<funcEndPos;j++){funcName[j-funcStartPos]=line[j];}
					fprintf(parseLog,"Putting funcName.\n");
					strcat(writeArg,funcName);
					for(j=0;j<strlen(funcName);j++){funcName[j]=0;}
                    for(j=0;j<lineLen;j++){
						if(line[j]=='{'){numbeginparen++;/*singleLine=true;*/}
						if(line[j]=='}'){numendparen++;}
					}
                    if(/*!singleLine*/numbeginparen==0){
                           do{
                              lineNum++;
                              fprintf(parseLog,"Line number:%i.\n",lineNum);
        						for(j=0;j<500;j++){	
                                 	tmpLine[j] = fgetc (readFile);	
                                    if(tmpLine[j]=='\n'||tmpLine[j]==EOF){       
                                          if(tmpLine[j]==EOF){EndOfFile=true;}
                                          lineLen=j+1;
                                          break;
                                    }
        						}				//load tmp line	

        						for(j=0;j<500;j++){
        							if(tmpLine[j]==EOF){EndOfFile=true;break;}
        							fputc(tmpLine[j],writeFile);
        							if(tmpLine[j]=='\n'||tmpLine[j]==EOF){break;}
        						}				//write tmp line to file
                                                
        						for(j=0;j<500;j++){
                                	if(tmpLine[j]=='('){numParen++;}
        							if(tmpLine[j]==')'){numParen--;}
                                	if((tmpLine[j]==';')&&(numParen==0)){funcPrototype=true;}
        						}				//check for ; | function prototype	
                            	if(!funcPrototype){
                                	for(j=0;j<500;j++){
                                       /*if(tmpLine[j]=='{'){multiLine=false;break;}
                                       if(tmpLine[j]!='{'&&j==499){multiLine=true;break;}*/
										if(tmpLine[j]=='{'){numbeginparen++;}
										if(tmpLine[j]=='}'){numendparen++;}	
                                	}
									if(numbeginparen==0&&numendparen==0){multiLine=true;}
									if(numbeginparen>0&&numbeginparen!=numendparen){multiLine=false;}
									if(numbeginparen>0&&numbeginparen==numendparen){
										funcPrototype=true;
										for(j=0;j<strlen(writeArg);j++){writeArg[j]=0;}
									} //empty single line definition , skip below
                            	}
                               }while(numParen>0&&!EndOfFile);
                       }
                       if(multiLine&&!funcPrototype&&!EndOfFile){
                             do{
                                  multiLine=false;
                                  lineNum++;
                                  fprintf(parseLog,"Is multi-line.\n");
                                  fprintf(parseLog,"Line number:%i.\n",lineNum);
                                  for(j=0;j<500;j++){	
                        			tmpLine[j] = fgetc (readFile);	
                                	if(tmpLine[j]=='\n'||tmpLine[j]==EOF){
										if(tmpLine[j]==EOF){EndOfFile=true;}
											lineLen=j+1;
											break;
										}
                                	}					//load tmp line	

                                    for(j=0;j<500;j++){
                                            if(tmpLine[j]==EOF){EndOfFile=true;break;}
                                			fputc(tmpLine[j],writeFile);
                                			if(tmpLine[j]=='\n'||tmpLine[j]==EOF){break;}
                                		}				//write tmp line to file
                                                
                                    for(j=0;j<500;j++){
                						if(tmpLine[j]==')'&&tmpLine[j+1]==';'){funcPrototype=true;}
                					}				//check for ; | function prototype	
                                        
                                    for(j=0;j<500;j++){
                                        /*if(tmpLine[j]=='{'){multiLine=false;break;}
                                        if(tmpLine[j]!='{'&&j==499){multiLine=true;break;}*/
										if(tmpLine[j]=='{'){numbeginparen++;}
										if(tmpLine[j]=='}'){numendparen++;}	
                                	}
									if(numbeginparen==0&&numendparen==0){multiLine=true;}
									if(numbeginparen>0&&numbeginparen!=numendparen){multiLine=false;}
									if(numbeginparen>0&&numbeginparen==numendparen){
										funcPrototype=true;
										for(j=0;j<strlen(writeArg);j++){writeArg[j]=0;}
									} //empty single line definition , skip below
                        			
                                }while(multiLine&&!EndOfFile);
                                            
                       }
					
					if(!funcPrototype&&!EndOfFile){
						int k;
						fprintf(parseLog,"Printing prefix.\n");
						for(k=0;k<strlen(prefix);k++){fputc(prefix[k],writeFile);}
						fprintf(parseLog,"Printing writeArg.\n");
						for(k=0;k<strlen(writeArg);k++){
							if(writeArg[k]==34||(writeArg[k]>47&&writeArg[k]<58)||(writeArg[k]>64&&writeArg[k]<91)||(writeArg[k]>96&&writeArg[k]<123)||writeArg[k]=='.'||writeArg[k]==' '||writeArg[k]=='/'||writeArg[k]=='_'||writeArg[k]==':'||writeArg[k]=='~'){	
							fputc(writeArg[k],writeFile);}
							}
						fputc(34,writeFile);
						fputc(',',writeFile);
						fprintf(parseLog,"Getting count to string.\n");
						//sprintf(writeArgCount,"%i",strlen(writeArg));
						//fprintf(parseLog,"Printing writeArgCount.\n");
						//for(k=0;k<strlen(writeArgCount);k++){fputc(writeArgCount[k],writeFile);}
						fprintf(parseLog,"Printing suffix.\n");
						for(k=0;k<strlen(suffix);k++){fputc(suffix[k],writeFile);}
						for(k=0;k<strlen(writeArg);k++){writeArg[k]=0;}
						/*for(k=0;k<strlen(writeArgCount);k++){writeArgCount[k]=0;}/*
						for(k=0;k<94;k++){fputc(prefix[k],writeFile);}
                                                for(k=0;k<MAX_PATH;k++){
                                                    if(nameWString[k]==0){break;}
                                                    fputc(nameWString[k],writeFile);
                                                }
                                                fputc(' ',writeFile);
						for(k=funcStartPos;k<funcEndPos;k++){fputc(line[k],writeFile);}
						for(k=0;k<24;k++){fputc(suffix[k],writeFile);}*/
						for(k=0;k<500;k++){line[k]='\n';}
					}	
				}
			}

		}

	}
	noParen=-1;
 	firstAlpha=-1;
 	firstChar=0;
 	foundASpace = false;
  	itsAFunc = false;
  	foundFirstChar=false;
  	funcCall=false;
  	funcStartPos=0;
  	funcEndPos=0;
	foundParen=false;
        multiLine=false;
        funcPrototype=false;
        noComment=0;
        foundComment=false;
        endComment=false;
        noBeginComments=0;
        singleLine=false;
        endParen=false;
        keywordStruct=false;
	foundEquals=false;
        numParen=0;
	foundComma=false;
	foundSpace=false;
	foundstructdefinition=false;
	numbeginparen=0;
	numendparen=0;
	lineNum++;
        for(j=0;j<500;j++){line[j]='\n';};
        for(j=0;j<500;j++){tmpLine[j]='\n';};
        for(j=0;j<500;j++){funcName[j]=0;};
	} while (!EndOfFile);
    fclose (readFile);
    fclose (writeFile);
    remove(nameString);
    rename(nameWString,nameString);
  }
  return;
}
