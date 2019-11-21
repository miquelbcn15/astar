#include <stdio.h>


void ExitError(char* miss, int errcode) {
  fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}
