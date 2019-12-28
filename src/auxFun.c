#include <stdio.h>
#include "auxFun.h"
#include <stdlib.h>
#include <math.h>
#include "auxFun.h"

void ExitError(char* miss, int errcode) {
  fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}

/*---------------------------------------------------------*/

long binarySearch(node *nodes, unsigned long key, unsigned long nnodes) {
    register unsigned long start = 0UL, middle, afterend = nnodes;
    register unsigned long try;
    while (afterend > start) {
        middle = start + ((afterend - start - 1) >> 1);
        try = nodes[middle].id;
        if (key == try) return middle;
        else if (key > try) start = middle + 1;
        else   afterend = middle;
    }
    return -1;
}

/*---------------------------------------------------------*/

/* heuristic functions */
double h1(node source, node dest) {
    /* haversine */
    double lat1 = source.lat * PI / 180., lon1 = source.lon * PI / 180.;
    double lat2 = dest.lat * PI / 180.,   lon2 = dest.lon * PI / 180.;
    double d_lat = fabs(lat1 - lat2), d_lon = fabs(lon1 - lon2);
    double a = sin(d_lat/2.) * sin(d_lat/2.) 
               + cos(lat1) * cos(lat2) * sin(d_lon/2.) * sin(d_lon/2.);
    double c = 2. * atan2(sqrt(a), sqrt(1.-a));
    return R * c;
}

double h2(node source, node dest) {
    /* spherical law of cosines */
    double lat1 = source.lat * PI / 180., lon1 = source.lon * PI / 180.;
    double lat2 = dest.lat * PI / 180.,   lon2 = dest.lon * PI / 180.;
    double d_lon = fabs(lon1 - lon2);
    double a = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(d_lon);
    return a * R;
}

double h3(node source, node dest) {
    /* equirectangular approximation */
    double lat1 = source.lat * PI / 180., lon1 = source.lon * PI / 180.;
    double lat2 = dest.lat * PI / 180.,   lon2 = dest.lon * PI / 180.;
    double d_lon = fabs(lon1 - lon2);
    double d_lat = fabs(lat1 - lat2);
    double x = d_lon * cos((lat1 + lat2)/2.);
    return R * sqrt(x*x + d_lat*d_lat);
}


/* void clean(int ** v) { */
/*     free(*v); v=NULL; */ 
/* } */
//#undef PI
// #undef R
