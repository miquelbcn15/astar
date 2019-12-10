#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "writebin.h"
#include "auxFun.h"

void readFirst(char *name, unsigned long *nnodes, unsigned long *nways) {
    /* returns the number of nodes and number of ways for the file 'name' */
    fprintf(stderr, "readFirst(): init\n");

    /* Initializing just in case */
    (*nnodes) = 0; (*nways) = 0;     
    FILE *fin;
    int i;

    char   *buffer = NULL;
    size_t bufsize = 0;
    char *r = "r";       /* relation */
    
    /* Open the file */
    if ((fin = fopen(name, "r")) == NULL) {
        ExitError("the data file does not exist or cannot be opened", 11);
    }
    
    /* Skip the header */
    for (i = 0; i < 3; i++) {                   
        if (getline(&buffer, &bufsize, fin) == -1) {
            ExitError("when reading header", 12);
        }
    }
    
    /* Read the next line*/
    if (getline(&buffer, &bufsize, fin) == -1) {
        ExitError("reading line", 2);
    }
    
    /* Count the nnodes and nways */
    while (buffer[0] != r[0]) {
        (*nnodes) += (buffer[0] + 1) % 2;
        (*nways)  += (buffer[0] % 2);
        if (getline(&buffer, &bufsize, fin) == -1) {
            ExitError("reading line", 3);
        }
    }

    fprintf(stderr, "readFirst(): first lecture finished\n");    

    fclose(fin);
    free(buffer); buffer=NULL;
}

/*----------------------------------------------------------------*/

void createEdge(node **nodes, unsigned long node1, unsigned long node2,
                int oneway, int *nsuccdim) {
    unsigned long s, g;
    int i;
    s = node1; g = node2;
    /* Do it once if oneway, twice interchanging nodes id go and back  */
    for (i = 0; i < (oneway+1); i++) { 
        (*nodes)[s].nsucc++;
        if (nsuccdim[s] < (*nodes)[s].nsucc) { 
            if (((*nodes)[s].successors = (unsigned long*)realloc((*nodes)[s].successors, (nsuccdim[s]+2) * sizeof(unsigned long))) == NULL) {
                ExitError("reserving space for children", 27);
            }
            nsuccdim[s]+=2;
        }
        (*nodes)[s].successors[(*nodes)[s].nsucc-1] = g;    
        s = node2; g = node1;
    }
}

/*-----------------------------------------------------------------*/

void readNodes(char *name, node **nodes, unsigned long nnodes, unsigned long nways) {
    fprintf(stderr, "readNodes(): init\n");

    FILE *fin;

    char   *buffer = NULL;
    size_t bufsize = 0; 
    char    *delim = "|";

    char *ptr;
    unsigned long i;
    
    /* Open the file */
    if ((fin = fopen (name, "r")) == NULL) {
        ExitError("the data file does not exist or cannot be opened", 11);
    }
    
    /* Allocate memory for nodes */
    if ( ((*nodes) = (node*)malloc(nnodes * sizeof(node))) == NULL) { 
        ExitError("allocating  memory for node vector\n", 3);
    }
    
    /* Skip the header */
    for (i = 0; i < 3; i++) {
        if (getline(&buffer, &bufsize, fin) == -1) {
            ExitError("when reading file", 2);
        }
    }

    int j;
    char *token;
    char *line2;
    int *nsuccdim;

    /* Read the nodes */
    for (i = 0; i < nnodes; i++) {
        if (getline(&buffer, &bufsize, fin) == -1) {
            ExitError("when reading file", 2);
        }
        line2 = buffer;

        /* This has to be done somewhere, and here is recorring nodes anyway */
        (*nodes)[i].nsucc = 0;
	(*nodes)[i].successors=NULL;
        /* Trying to allocate memory before reallocating ... */
        /* if ( ((*nodes)[i].successors = malloc(2 * sizeof(unsigned long))) == NULL) { */
        /*     ExitError("allocating memory for successors", 13); */
        /* } */

        /* Skip line type */
        strsep(&line2, delim);
        /* Save the id */
        token = strsep(&line2, delim);
        (*nodes)[i].id=strtoul(token, &ptr, 10);
        /* Save the name */
        token = strsep(&line2, delim);
        if(strlen(token) == 0) {
            (*nodes)[i].name = NULL; //EN VEZ DE NULL, NO TOCAR__version [187]
        } 
        else {
            (*nodes)[i].name = (char*)malloc(strlen(token)+1);  //OJO NO RESERVO MEMORIA NOMBRES
            strcpy((*nodes)[i].name, token); //token does not have the null terminator (i think) but strcpy adds one, so reserve space for it
        }
        /* Skip useless information */
        for (j = 4; j < 10; j++) strsep(&line2, delim);
        /* Save lat and lon */
        (*nodes)[i].lat=strtod(line2, NULL);
        strsep(&line2,delim);
        (*nodes)[i].lon=strtod(line2, NULL);   
    }

    fprintf(stderr, "readNodes(): filled nodes\n");

    int oneway;
    long node1, node2;
    
    /* Allocate memory for additional sucessors */
    if ( (nsuccdim = (int*)calloc(nnodes, sizeof(int))) == NULL) {
        ExitError("allocating memory for nsuccdim", 13);
    } //we need it initialized to 0, and calloc is faster than malloc+memset

    /* Read ways */
    for (i = 0; i < nways; i++) {
        /* True means go and back */
        oneway=1;  
        /* Read first line */
        if (getline(&buffer, &bufsize, fin) == -1) {
            ExitError("when reading file", 2);
        }
        line2 = buffer;
        
        /* Skip useless information */
        for(j = 0; j < 7; j++) strsep(&line2, delim);
        
        /* Type of way */
        token = strsep(&line2, delim);
        if (strlen(token) > 0) oneway = 0; // something written=>oneway, so its false
        
        /* line2 points to field 10, first node */  
        strsep(&line2, delim);  
        node1 = binarySearch(*nodes, strtoul(line2, &ptr, 10), nnodes);

        token = strsep(&line2, delim);
        token = strsep(&line2, delim); 
        /* 
         * edge 2 is in token. 
         * If edge 2 does not exist, token=NULL, not while, 
         * jump next i->way discarded: DONE
         */
        while (node1 < 0 && token != NULL) { //ghost first node
            node1 = binarySearch(*nodes, strtoul(token, &ptr, 10), nnodes);
            token = strsep(&line2, delim);    
        } 
        while (token != NULL) { //while the chain of edges in this way
            node2 = binarySearch(*nodes, strtoul(token, &ptr, 10), nnodes);
            if ( node2 + 1 ) {
               // >0 true. return -1=>false. 
               // Node does not exist. Take another for node2. No node? Discard way
               createEdge(nodes, node1, node2, oneway, nsuccdim);
               node1 = node2;
            }
            token = strsep(&line2, delim);
        }
    }

    fclose(fin);
    free(buffer);
    buffer = NULL;
    free(nsuccdim);
    nsuccdim=NULL;
    fprintf(stderr, "readNodes(): read\n");
}
