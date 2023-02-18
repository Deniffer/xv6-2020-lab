#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  if (argc != 2) {
    printf("Error Usage! using sleep should passing a int, like sleep(20)\n");
    exit(-1);
  }
  
  int number_of_ticks = atoi(argv[1]);

  if (number_of_ticks == 0) {
    printf("");
    exit(-1);
  }

  sleep(number_of_ticks);
  exit(0);
}
