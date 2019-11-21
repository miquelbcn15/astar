#include <stdlib.h>
#include <string.h>
#include "auxFun.h"




void readFirst(char* name, unsigned long *nnodes,unsigned long *nways){
  (*nnodes)=0; (*nways)=0; //initialization just in case it is not put to 0
  FILE *fin;
  int i,j;
  
  char *buffer=NULL;
  size_t bufsize =0;
  char *n="n";
  char *w="w";
  int catch=0;  
  char* delim="|";

  if ((fin = fopen (name, "r")) == NULL)
    ExitError("the data file does not exist or cannot be opened", 11);
  
  for(i=0;i<3;i++){ //skip the header
    if(getline(&buffer,&bufsize,fin)==-1)
      ExitError("when reading header", 12);
  }
  while(getline(&buffer,&bufsize,fin)){
    if(buffer[0]==n[0]) (*nnodes)++;
    else if (buffer[0]==w[0]) (*nways)++;
    else {
      catch=1;
      printf("Read succesful, relations reached\n");
      break;
    }
  }
  if(!catch){
   ExitError("when reading csv\n",2);
  }
  printf("First lecture finished\n");    
  
  fclose(fin);
}

//get line, tokenizer, ignora el 1, el segundo campo al id como lu,
//el tercero a name si no esta vacío, Campo 10 lat campo 11 lon


void readNodes( char* name,node **nodes, unsigned long nnodes){
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
 for (i=0;i<nnodes;i++){
   if(getline(&buffer,&bufsize,fin)==-1)
      ExitError("when reading file", 2);
   strsep(&buffer,delim);
   (*nodes)[i].id=strtoul(buffer,&ptr,10);
   strsep(&buffer,delim);
   if(buffer[0]==delim[0])
     (*nodes)[i].name=NULL;
   else{
     printf("done node %lu\n",i);
     printf("%s\n",buffer);
     char *ab;
     strcpy(ab,buffer);
     printf("si copio\n");
     printf("%s\n",ab);
   }
   for (j=4;j<=10;j++) strsep(&buffer,delim);
   (*nodes)[i].lat=strtoul(buffer,&ptr,10);
   (*nodes)[i].lon=strtoul(strsep(&buffer,delim),&ptr,10);
   
 }  
 fclose(fin);
}

