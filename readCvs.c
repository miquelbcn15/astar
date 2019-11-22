#include <stdlib.h>
#include <string.h>
#include "auxFun.h"




void readFirst(char* name, unsigned long *nnodes,unsigned long *nways){
  (*nnodes)=0; (*nways)=0; //initialization just in case it is not put to 0
  FILE *fin;
  int i,j;
  
  char *buffer=NULL;
  size_t bufsize =0;
  char *r="r";
  char* delim="|";

  if ((fin = fopen (name, "r")) == NULL)
    ExitError("the data file does not exist or cannot be opened", 11);
  
  for(i=0;i<3;i++){ //skip the header
    if(getline(&buffer,&bufsize,fin)==-1)
      ExitError("when reading header", 12);
  }
  if(getline(&buffer,&bufsize,fin)==-1)
    ExitError("reading line",2);
  while(buffer[0]!=r[0]){
    (*nnodes)+=(buffer[0]+1)%2;
    (*nways)+=(buffer[0]%2);
    if(getline(&buffer,&bufsize,fin)==-1)
      ExitError("reading line",3);
  }
  printf("First lecture finished\n");    
  
  fclose(fin);
  free(buffer);buffer=NULL;
}

//get line, tokenizer, ignora el 1, el segundo campo al id como lu,
//el tercero a name si no esta vacío, Campo 10 lat campo 11 lon


void readNodes( char* name,node **nodes, unsigned long nnodes,unsigned long nways){
  FILE *fin;
  
  char *buffer=NULL;
  size_t bufsize =0; 
  char* delim="|";
  char *ptr;
  unsigned long i;
  if ((fin = fopen (name, "r")) == NULL)
    ExitError("the data file does not exist or cannot be opened", 11);
  
 if(((*nodes)=(node*)malloc(nnodes*sizeof(node)))==NULL)
   ExitError("allocating  memory for node vector\n",3);
  for (i=0;i<3;i++){
   if(getline(&buffer,&bufsize,fin)==-1)
      ExitError("when reading file", 2);
  }
 int j;
 char *token;
 char*line2;

 for (i=0;i<nnodes;i++){
   if(getline(&buffer,&bufsize,fin)==-1)
      ExitError("when reading file", 2);
   line2=buffer;

   strsep(&line2,delim);  
   token=strsep(&line2,delim);
   (*nodes)[i].id=strtoul(token,&ptr,10);
    token=strsep(&line2,delim);
   if(strlen(token)==0)
     (*nodes)[i].name=NULL;
   else{
     (*nodes)[i].name=(char*)malloc(strlen(token)+1);
     strcpy((*nodes)[i].name,token);
   }
   for (j=4;j<10;j++) strsep(&line2,delim);
     (*nodes)[i].lat=strtod(line2,NULL);
     strsep(&line2,delim);
     (*nodes)[i].lon=strtod(line2,NULL);   
 }
 
//  for (i=nnodes;i<nways;i++){
//    
//    
//    
//    
//  }
 fclose(fin);
 free(buffer);
 buffer=NULL;
 printf("read\n");
}

