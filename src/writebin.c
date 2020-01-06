#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auxFun.h"

void writeBin(char *file, node *nodes, unsigned long nnodes) {
    /* Write the binary file to name.bin */
    fprintf(stderr, "writeBin(): init\n");

    unsigned long ntotnsucc = 0UL;
    char name[257];
    FILE *fin;
    unsigned long i; 
    int len;
    /* Computing the total number of successors */
    for (i = 0; i < nnodes; i++) ntotnsucc += nodes[i].nsucc;

    /* Setting the name of the binary file */
    strcpy(name, file); strcpy(strrchr(name,'.'), ".bin");
    if ((fin = fopen(name, "wb")) == NULL) 
        ExitError("the output binary data file cannot be opened",31);

    /* Global data header */
    if ( fwrite(&nnodes, sizeof(unsigned long), 1, fin) +
         fwrite(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
        ExitError("when initializing the output binary data file", 32);
    fprintf(stderr, "writeBin(): to bin, nnodes : %lu\tallsucc : %lu\n", nnodes, ntotnsucc);

    /* Writing all nodes */
    if ( fwrite(nodes, sizeof(node), nnodes, fin) != nnodes) 
        ExitError("when writing nodes to the output binary data file ", 32);

    /* Writing successors in blocks */
    for (i = 0; i < nnodes; i++) if (nodes[i].nsucc) {
        if ( fwrite(nodes[i].successors, sizeof(unsigned long),
                    nodes[i].nsucc, fin) != nodes[i].nsucc ) 
            ExitError("when writing edges to the output binary data file", 32);
    }

    /* Writing the names: first the lengths of each name */
    for (i = 0; i < nnodes; i++) { 
        /* doing the loop twice-> one: store the lenth, two:store the names */
	    if ( nodes[i].name != NULL ) {
            len = strlen(nodes[i].name);
            if ( fwrite(&len, sizeof(int), 1, fin) != 1 ) 
                ExitError("writting len names",5);
        }
        else {
            len=0; 
            if ( fwrite(&len, sizeof(int), 1, fin) != 1) 
                ExitError("writting len names",5);
        }
    }
    /* Writing the names: each name */
    for (i = 0; i < nnodes; i++) {
        /* this loop could use the length found before, but we would need to store them */
        if ( nodes[i].name != NULL ) {
            len = strlen(nodes[i].name); 
            if ( fwrite(nodes[i].name, sizeof(char),len, fin) != len ) 
                ExitError("writting names",6);}
    }    
    
    fclose(fin);
    fprintf(stderr, "writeBin(): finished\n");
}

/*------------------------------------------------------------*/

void readBin(char *name, node **nodes,
        unsigned long **allsuccessors, unsigned long *nnodes,
        unsigned long *ntotnsucc) {
    unsigned long i; 
    int *nameslen, len;
    /* Reads the binary file */
    fprintf(stderr, "readBin(): init\n");
    FILE *fin;

    if ( (fin = fopen (name, "rb")) == NULL) 
        ExitError("the data file does not exist or cannot be opened", 11);

    /* Global data --- header */
    if ( fread(nnodes, sizeof(unsigned long), 1, fin) +\
        fread(ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
        ExitError("when reading the header of the binary data file", 12);

    /* Getting memory for all data */
    if (( (*nodes) = (node*)malloc((*nnodes) * sizeof(node))) == NULL ) 
        ExitError("when allocating memory for the nodes vector", 13);
    if (( (*allsuccessors) = (unsigned long*)malloc((*ntotnsucc) * sizeof(unsigned long))) == NULL)
        ExitError("when allocating memory for the edges vector", 15);
    if (( nameslen = (int*)malloc((*nnodes) * sizeof(int))) == NULL ) 
        ExitError("when allocating memory for len of names", 13);
    
    /* Reading the nodes and successors */
    if ( fread( (*nodes), sizeof(node), (*nnodes), fin) != (*nnodes))
        ExitError("when reading nodes from the binary data file", 17);
    if ( fread((*allsuccessors), sizeof(unsigned long), (*ntotnsucc), fin) !=
            (*ntotnsucc) )
        ExitError("when reading successors from the binary data file", 18);
    
    /* Reading names length */
    if ( fread(nameslen, sizeof(int), (*nnodes),fin) != (*nnodes))
        ExitError("when reading lenth of names", 19);

    for ( i = 0; i < (*nnodes); i++) {
        len = nameslen[i]; 
        /*No need to access the vector a thousand times in the loop*/
	    if ( len ) {
	        if ( ((*nodes)[i].name = (char*)malloc(len+1)) == NULL )
                ExitError("when reserving memory for a name",22);
	        ((*nodes)[i].name)[len]='\0';
	        if ( fread((*nodes)[i].name, sizeof(char), len, fin) != len)
                ExitError("when reading a name",23);
	    } 
	    else (*nodes)[i].name = NULL; 
     }

    fclose(fin);

    /* Setting pointers to successors */
    for (i = 0; i < (*nnodes); i++) { 
        /* i'm using this loop, so the one extra in write is compensated */
        if ( (*nodes)[i].nsucc ) {
            (*nodes)[i].successors = *allsuccessors; 
	        *allsuccessors += (*nodes)[i].nsucc;
      }
    }

    /* clean(&nameslen); */
    free(nameslen); nameslen = NULL;
    fprintf(stderr, "readBin(): end\n");
}
