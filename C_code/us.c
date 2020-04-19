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





us *update_or_create_us(char row[6][15]){
  row++;
  return NULL;
}

bool output_us(void){
  return false;
}

void delete_us(void){

}

