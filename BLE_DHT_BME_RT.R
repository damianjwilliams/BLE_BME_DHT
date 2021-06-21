
library(tidyverse)

f_measurement_type <- sensor <- function(x) {
  case_when(
    grepl("41|47|53", x) ~ "Temperature_C",
    grepl("44|50|56", x) ~ "Humidity_percent",
    TRUE ~ 'Unknown')
}

f_sensor <- function(x) {
  case_when(
    grepl("41|44", x) ~ "BME280",
    grepl("47|50", x) ~ "DHT11",
    grepl("53|56", x) ~ "DHT22",
    TRUE ~ 'Unknown')
}


data_df <- read_csv("/Users/damianwilliams/Desktop/test_data.csv",col_names = F)

data_df <- data_df %>%
mutate(Time = as.POSIXct(X1, origin="1970-01-01"))%>%
rename(Handle = X2)%>%
rename(Value = X3)%>%
mutate(sensor = as_factor(f_sensor(Handle)))%>%
mutate(measurement = as_factor(f_measurement_type(Handle)))%>%
filter(Value<100)
  
ggplot(data_df,aes(Time,Value,colour = sensor))+
  geom_line()+
  facet_wrap(~measurement,scales = "free")
                    
