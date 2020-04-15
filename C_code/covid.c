#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "county.h"
#include <string.h>
#include <math.h>

int main(){
  regex_t regex;
  regcomp(&regex, "[a-zA-ZÁ-ÿ0-9.' -]+", REG_EXTENDED);
  char *word;
  
  FILE *data = fopen("us-counties.csv", "r");
  

  int row = 0;
  int column = 0;
  
  extern county *county_table[57000];
  
  char this_row[6][15];
  
  char *date_test = int_to_date(93);
  printf("%s", date_test);
  free(date_test);
  

  fseek(data,0L,SEEK_END);               //// Loading bar 
  uint32_t data_size = ftell(data);        //  preperation
  fseek(data,0L,SEEK_SET);                 //
  uint8_t percentage_printed = 0;          //
  uint32_t bytes_processed = 0;          ////
  printf("size%d\n",data_size);

  printf("\nLoading...                                                                                    100%%-]\n");
  while((word = next_word(data, &regex)) != NULL){
    
    bytes_processed+= strlen(word) + 1;
    uint8_t percentage_current = (bytes_processed * 100)/data_size;
    for(; percentage_printed < percentage_current; percentage_printed++){
      printf("#");
    }
    
    if(column == 0){
      row++;
      
      if(row > 2){
        update_or_create(this_row);
      }
    }
 
    strcpy(this_row[column], word);
    
    //This bit handles counties without fips ids
    if(column==2){
      if(strcmp("New York City",this_row[1]) == 0){
        strcpy(this_row[3],"36061");
        column++;
      }
      
      if(strcmp("Kansas City",this_row[1]) == 0){
        strcpy(this_row[3],"27021");
        column++;
      }

      if(strcmp("Unknown",this_row[1]) == 0){
        int id = state_name_to_id(this_row[2]);
	this_row[3][0] = id/10 + '0';
	this_row[3][1] = id%10 + '0';
	this_row[3][2] = '9';
	this_row[3][3] = '9';
	this_row[3][4] = '9';
	this_row[3][5] = 0;
	column++;
      }
    }

    column++;
    column%=6; 
  }
  clear_words();
  




  //6085 is santa clara
  //36061 is new york city and surrounding areas
  county *of_study = county_table[6085];

  for(uint32_t h = 4;h<of_study->last_day; h++){
     
    uint32_t *cases = &of_study->cases[h];
    if(cases[0] == 0){
      cases[0] = cases[-1];
    }

    uint8_t tens = log(cases[0])*2;
    tens+=0;
    
    uint32_t this_change = cases[0]  - cases[-2];
    uint32_t prev_change = cases[-1] - cases[-3];
    
    float growth_rate = 0;
    if(prev_change != 0){
      growth_rate = (float)this_change/prev_change;
    }
    
    
    for(int j = 0;j<growth_rate*10;j++){
      if(j<10)
        printf("*");
      else
        printf("#");
    }
    printf("                              / cases: %d day number: %d", cases[0], h);
    printf("\n");
  }
  
  
   
  for(int h = 0;h < 57000; h++){
    output_county(h);
    delete_county(county_table[h]); 
  }

  regfree(&regex);
  fclose(data);

  return 0;
}
