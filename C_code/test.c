#include "county.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  char* date;
  date = int_to_date(367);
  printf("%s\n",date);
  free(date);

  char *name = "Texas";
  int id = state_name_to_id(name);
  printf("id : %d",id);
  return 0;
}
