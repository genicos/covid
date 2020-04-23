#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "county.h"
#include <string.h>
#include <math.h>
#include <getopt.h>

int main(int argc, char** argv){
  bool show_county = false;
  uint16_t fips_to_show = 0;

  bool show_state = false;
  uint8_t id_to_show = 0;
  
  bool show_us = false;
  
  bool show_cases = false;
  bool show_deaths = false;
  bool show_growth_rate = false;
  bool show_log_cases = false;
  
  int c = 0;
  while ((c = getopt(argc,argv, "f:i:ucdgl")) != -1){
    switch(c){
    case 'f':
      show_county = true;
      fips_to_show = atoi(optarg);
      break;
    case 'i':
      show_state = true;
      id_to_show = atoi(optarg);
      break;
    case 'u':
      show_us = true;
      break;
    case 'c':
      show_cases = true;
      break;
    case 'd':
      show_deaths = true;
      break;
    case 'g':
      show_growth_rate = true;
      break;
    case 'l':
      show_log_cases = true;
      break;
    default:
      break;
    }
  }

  if(show_county){
    show_state = false;
    show_us = false;
  }
  if(show_state){
    show_us = false;
  }

  regex_t regex;
  regcomp(&regex, "[a-zA-ZÁ-ÿ0-9.' -]+", REG_EXTENDED);
  char *word;
  
  FILE *county_data = fopen("us-counties.csv", "r");
  FILE *state_data  = fopen("us-states.csv", "r");
  FILE *us_data     = fopen("us.csv", "r");


  int row = 0;
  int column = 0;
  char this_row[6][15];
  
  extern county *county_table[57000];
  extern state  *state_table[57];
  extern us     *united_states;


  fseek(county_data,0L,SEEK_END);                                              //// Loading bar 
  fseek(state_data ,0L,SEEK_END);
  fseek(us_data    ,0L,SEEK_END);
  uint32_t data_size = ftell(county_data) + ftell(state_data) + ftell(us_data);  //  preperation
  fseek(county_data,0L,SEEK_SET);
  fseek(state_data ,0L,SEEK_SET);
  fseek(us_data    ,0L,SEEK_SET);
  uint8_t percentage_printed = 0;          
  uint32_t bytes_processed = 0;                                                ////
  
  printf("Reading from us-counties.csv");
  printf("\nLoading...%%\n");
  while((word = next_word(county_data, &regex)) != NULL){
    
    bytes_processed+= strlen(word) + 1;
    uint8_t percentage_current = (bytes_processed * 10)/data_size;
    for(; percentage_printed < percentage_current; percentage_printed++){
      printf("%d\n" , percentage_current*10);
    }
    
    if(column == 0){
      row++;
      
      if(row > 2){
        update_or_create_county(this_row);
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
  printf("Finished reading from us-counties.csv\n\n");
  
  column = 0;
  row = 0;

  printf("Reading from us-states.csv\n");
  while((word = next_word(state_data, &regex)) != NULL){
    
    bytes_processed+= strlen(word) + 1;
    uint8_t percentage_current = (bytes_processed * 10)/data_size;
    for(; percentage_printed < percentage_current; percentage_printed++){
      printf("%d\n" , percentage_current*10);
    }
    
    if(column == 0){
      row++;
      
      if(row > 2){
        update_or_create_state(this_row);
      }
    }
 
    strcpy(this_row[column], word);

    column++;
    column%=5; 
  }
  clear_words();
  printf("Finished reading from us-states.csv\n\n");
  
  row = 0;
  column = 0;

  printf("Reading from us.csv\n");
  while((word = next_word(us_data, &regex)) != NULL){
    
    bytes_processed+= strlen(word) + 1;
    uint8_t percentage_current = (bytes_processed * 10)/data_size;
    for(; percentage_printed < percentage_current; percentage_printed++){
      printf("%d\n" , percentage_current*10);
    }
    
    if(column == 0){
      row++;
      
      if(row > 2){
        update_or_create_us(this_row);
      }
    }
 
    strcpy(this_row[column], word);
    
    column++;
    column%=3; 
  }
  clear_words();
  printf("Finished reading from us.csv\n\n");
  







  
  county *of_study = NULL;

  if(show_county){
    of_study = (fips_to_show < 57000) ? county_table[fips_to_show] : NULL;
    if(of_study)
      printf("%s,%s\n",of_study->name,of_study->state);
  }
  if(show_state){
    of_study = (id_to_show < 57) ? (county *)state_table[id_to_show] : NULL;
    if(of_study)
      printf("%s\n",of_study->state);
  }
  if(show_us){
    of_study = (county *)united_states;
    if(of_study)
      printf("United States of America\n");
  }
  
  if(of_study){
    
    if(show_cases){                                     //Cases display
      
      printf("\nDate      ,Cases      ,Graph\n");
      
      for(uint32_t h = 1; h < of_study->last_day; h++){
        uint32_t *cases = &of_study->cases[h];
        if(cases[0] == 0){
          cases[0] = cases[-1];
        }
        
        char *date = int_to_date(h);
        
	printf("%s ",date);
        
        printf("%11d ",cases[0]);
        
        uint8_t hashes = (cases[0]*100)/of_study->most_cases_day_count;
        for(; hashes>0; hashes--){
	  printf("#");
	}

	free(date);
	printf("\n");
      }
    }
    
    if(show_deaths){                                    //Deaths display
      
      printf("\nDate      ,Deaths     ,Graph\n");
      
      for(uint32_t h = 1; h < of_study->last_day; h++){
        uint32_t *deaths = &of_study->deaths[h];
        if(deaths[0] == 0){
          deaths[0] = deaths[-1];
        }
        
        char *date = int_to_date(h);
        
	printf("%s ",date);
        
        printf("%11d ",deaths[0]);
        
        uint8_t hashes = (deaths[0]*100)/of_study->most_deaths_day_count;
        for(; hashes>0; hashes--){
	  printf("#");
	}

	free(date);
	printf("\n");
      }
    }
    
    if(show_growth_rate){                                //Growth rate display
      
      printf("\nDate      ,Growth Rate,Graph\n");
      
      for(uint32_t h = 4; h < of_study->last_day; h++){
        uint32_t *cases = &of_study->cases[h];
        if(cases[0] == 0){
          cases[0] = cases[-1];
        }
        
        char *date = int_to_date(h);
        
	printf("%s ",date);
         
        uint32_t this_change = cases[0]  - cases[-2];
        uint32_t prev_change = cases[-1] - cases[-3];
    
        float growth_rate = 0;
        if(prev_change != 0){
          growth_rate = (float)this_change/prev_change;
        }
        

        printf("%11.3f ",growth_rate);
        
        for(int j = 0;j<growth_rate*10;j++){
          if(j<10)
            printf("*");
          else
            printf("#");
        }

	free(date);
	printf("\n");
      }
    }
    
    if(show_log_cases){                                   //Log of cases display
      
      printf("\nDate      ,Log2 Cases ,Graph\n");
      
      for(uint32_t h = 0; h < of_study->last_day; h++){
        uint32_t *cases = &of_study->cases[h];
        if(cases[0] == 0){
          cases[0] = cases[-1];
        }
        
        char *date = int_to_date(h);
        
	printf("%s ",date);
        
        float log_cases = log(cases[0])/log(2);

        printf("%11.3f ",log_cases);
        
        uint8_t hashes = (int)log_cases;
        for(; hashes>0; hashes--){
	  printf("#");
	}


	free(date);
	printf("\n");
      }
    }
  
  }else{
    if(show_county || show_state){
      printf("Invalid fips code\n");
    }else if(!show_us){
      printf("FLAG USES\n\n");
      printf("-c        show cases graph\n");
      printf("-d        show deaths graph\n");
      printf("-g        show growth rate graph\n");
      printf("-l        show logarithmic cases graph\n\n");
      printf("-f <fips> take info from county with fips code <fips>\n");
      printf("-i <fips> take info from state  with fips code <fips>\n");
      printf("-u        take info from the entire United States\n");
    }
  }
  



  printf("\n\nStowing county information\n");
  printf("Loading...%%\n");
  for(int h = 0; h < 10; h++){
    for(int i = 0; i < 5700; i++){
      output_county(h*5700 + i);
      delete_county(county_table[h*5700 + i]);
    }
    printf("%d\n",h*10);
  }
  printf("Completed\n");


  printf("\nStowing state information\n");
  for(int h = 0; h < 57; h++){
    output_state(h);
    delete_state(state_table[h]);
  }
  printf("Completed\n");
  

  printf("\nStowing US information\n");
  output_us();
  delete_us();
  printf("Completed\n");
  
  
  regfree(&regex);
  fclose(county_data);
  fclose(state_data);
  fclose(us_data);

  return 0;
}
