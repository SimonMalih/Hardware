#ifndef CustomAlarm_H_
#define CustomAlarm_H_

#include <Arduino.h>

class CustomAlarm {
   private:
    unsigned int delayTime = 30 * 1000;

   public:
    bool auth = false;
    unsigned int prevTime = 0;

    void setAuth() {
        prevTime = millis();
        auth = true;
    }

    void checkAuth() {
        if(millis() - prevTime > delayTime && auth) {
            auth = false;
            printf("Auth is finishrf\n");
        }
    }
};

#endif