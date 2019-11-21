#pragma once

#include <TimeLib.h>
#include <Time.h>        //http://www.arduino.cc/playground/Code/Time
#include <Timezone.h>    //https://github.com/JChristensen/Timezone

void syncTime();
time_t getLocalTime();
