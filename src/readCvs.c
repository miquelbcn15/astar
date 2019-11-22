#include <stdlib.h>
#include <string.h>
#include "auxFun.h"




void readFirst(char* name, unsigned long *nnodes,unsigned long *nways){
  (*nnodes)=0; (*nways)=0; //initialization just in case
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

void createEdge(node* nodes,unsigned long node1,unsigned long node2,int oneway, int*nsuccdim){
  unsigned long s,g;
  int i;
  s=node1; g=node2;
  for (i=0;i<(oneway+1);i++){//done once if oneway, twice interchanging nodes id go and back 
    nodes[s].nsucc++;
    if(nsuccdim[s]<nodes[s].nsucc){
      if((nodes[s].successors=(unsigned long*)realloc(nodes[s].successors,(nsuccdim[s]+2)*sizeof(unsigned long)))==NULL)
	ExitError("reserving space for children",27);
      nsuccdim[s]+=2;
    }
    nodes[s].successors[nodes[s].nsucc-1]=g;    
    s=node2; g=node1;
  }
}


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
   
   (*nodes)[i].nsucc=0; //this has to be done somewhere, and here is recorring nodes anyway
   
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
 printf("filled nodes\n");
 
 int oneway;
 unsigned long node1,node2;
 int *nsuccdim;
 if((nsuccdim=(int*)calloc(nnodes,sizeof(int)))==NULL)
   ExitError("allocating memory for nsuccdim",13);
 
 for (i=0;i<nways;i++){
   oneway=0;  //false, i.e, is go and gack
   
   if(getline(&buffer,&bufsize,fin)==-1)
      ExitError("when reading file", 2);
   line2=buffer;
   
   for(j=0;j<7;j++) strsep(&line2,delim);
   
   token=strsep(&line2,delim);
   if(strlen(token)>0) oneway=1; //something written=>oneway, so its true
   
   strsep(&line2,delim); //now line2 points to field 10, first node  
   node1=binarySearch(nodes,strtoul(line2,&ptr,10));
   token=strsep(&line2,delim);
   token=strsep(&line2,delim);//edge 2 is in token. If edge 2 does not exist, token=NULL, not while, jump next i->way discarded: DONE
   while(node1<0 && token!=NULL){//ghost first node
     node1=binarySearch(nodes,strtoul(token,&ptr,10));
     token=strsep(&line2,delim);
   } 
   while(token!=NULL){ //while the chain of edges in this way
     node2=binarySearch(nodes,strtoul(token,&ptr,10));
     if(node2+1){//>0 true. return -1=>false. No existe el nodo, se coge otro para node2, si no hay se descarta way
	createEdge(*nodes,node1,node2,oneway,nsuccdim);
	node1=node2;
     }
     token=strsep(&line2,delim);
   }
 }
 fclose(fin);
 free(buffer);
 buffer=NULL;
 printf("read\n");
}

