#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>

#include "macros.c"
#include "consoleInput.c"
#include "generateLock.c"
#include "readLock.c"
#include "swapTranspoReading.c"
#include "rotateFirstKey.c"
#include "rotateSwapKey.c"
#include "rotateTranspoKey.c"
