#include "us.h"

bool is_leap_year(uint32_t year){
  return (year%4 == 0 &&(!year%100 == 0 || year%400 == 0));
}

static const uint16_t zeroeth_day_of[12] = {
  0,
  31,
  59,
  90,
  120,
  151,
  181,
  212,
  243,
  273,
  304,
  334,
};

uint32_t date_to_int(char *date){
  
  uint16_t year  = atoi(date);
  uint8_t month = atoi(&date[5]);
  uint8_t day   = atoi(&date[8]);
  

  uint32_t ans = 0;
  
  if(is_leap_year(year) && month>2){
    ans++;
  } 

  while(year > YEAR_0){
    ans+=365;
    year--;
    if(is_leap_year(year)){ //leap years
      ans++;
    }
  }
  
  ans += zeroeth_day_of[month-1];
   
  ans += day;
  
  return ans;
}



char *int_to_date(uint32_t daynum){
  char *ans = (char *)calloc(11, sizeof(char));
  if(!ans){
    return NULL;
  }
  
  uint16_t year = YEAR_0 + daynum/146097 * 400; //as if my code will last 400 years
  daynum%=146097;
  
  for(uint32_t h = YEAR_0; daynum > 365; h++){
    if(is_leap_year(h)){
      daynum--;
    }
    daynum-=365;
    year++;
  }
  
  if(daynum == 0){
    daynum +=366;
    year--;
  }
  
  uint8_t month = 0;

  for(uint8_t mon = 0; mon < 12; mon++){
    uint32_t zeroeth_day_of_month = zeroeth_day_of[mon];
    
    if(is_leap_year(year) && mon>1){
      zeroeth_day_of_month++;
    }
    
    if(zeroeth_day_of_month >= daynum){
      month = mon;
      break;
    }
    
    if(mon == 11){
      month = 12;
    }
  }
  
  if(is_leap_year(year) && month > 2){
    daynum--;
  }
  
  daynum-=zeroeth_day_of[month-1];
  
  uint8_t day = daynum;

  sprintf(ans, "%04d-%02d-%02d", year%10000,month%100,day%100);
  
  return ans;
}





us *update_or_create_us(char row[3][15]){
  extern us *united_states;
  
  if(!united_states){
    united_states = (us *)calloc(1,sizeof(us));
    if(!united_states){
      return NULL;
    }
  }

  uint32_t daynumber = date_to_int(row[0]);

  united_states->cases[daynumber] = atoi(row[1]);
  if(united_states->cases[daynumber] > united_states->most_cases_day_count){
    united_states->most_cases_day_count = united_states->cases[daynumber];
  }

  united_states->deaths[daynumber] = atoi(row[2]);
  if(united_states->deaths[daynumber] > united_states->most_deaths_day_count){
    united_states->most_deaths_day_count = united_states->deaths[daynumber];
  }
  
  united_states->last_day = daynumber + 1;

  return united_states;
}


void delete_us(void){
  free(united_states);
}

void buffer_4bytes(char *buffer, uint32_t index, uint32_t data){
  buffer[index + 0] = data       & 0xFF;
  buffer[index + 1] = data >>  8 & 0xFF;
  buffer[index + 2] = data >> 16 & 0xFF;
  buffer[index + 3] = data >> 24 & 0xFF;
}

bool output_us(void){
  if(!united_states){
    return false;
  }
  

  char *file_name = (char *)calloc(14, sizeof(char));
  sprintf(file_name,"united_states");
  
  FILE *out = fopen(file_name, "w");
  free(file_name);

  if(!out){  
    return false;
  }
  
  static char buffer[YEARS*366*3*4 + 34];
  uint32_t buffer_index = 0;

  us *to_write = united_states;

  buffer_4bytes(buffer,0, to_write->last_day);

  buffer_index = 4;

  //buffering cases array
  for(uint32_t h = 0; h < to_write->last_day; h++, buffer_index+=4){
    if(h > 0 && to_write->cases[h] == 0){
      to_write->cases[h] = to_write->cases[h-1];
    }
    buffer_4bytes(buffer, buffer_index, to_write->cases[h]);
  }
  
  //buffering deaths array
  for(uint32_t h = 0; h < to_write->last_day; h++, buffer_index+=4){
    if(h > 0 && to_write->deaths[h] == 0){
      to_write->deaths[h] = to_write->deaths[h-1];
    }
    buffer_4bytes(buffer, buffer_index, to_write->deaths[h]);
  }
  

  //buffering growth rate * 1000
  for(uint32_t h = 0; h < to_write->last_day; h++, buffer_index+=4){
    if(h<4){
      continue;
    }
    
    uint32_t *cases = &to_write->cases[h];
    
    uint32_t this_change = cases[0]  - cases[-2];
    uint32_t prev_change = cases[-1] - cases[-3];
    
    uint32_t growth_rate = 0;
    if(prev_change != 0){
      growth_rate = (1000)*this_change/prev_change;
    }
    
    buffer_4bytes(buffer, buffer_index, growth_rate);
  }
  
  write(fileno(out), buffer, buffer_index - 4);
  fclose(out);
  
  return true;
}
