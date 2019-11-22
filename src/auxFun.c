#include <stdio.h>
#include "auxFun.h"
#include <stdlib.h>

void ExitError(char* miss, int errcode) {
  fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}
