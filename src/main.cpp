//
// Created by Maulik on 9/15/2018.
//
#include <Arduino.h>
#include <WiFi.h>
#include <RemoteDebug.h>

RemoteDebug Debug;
hw_timer_t * hw_timer= NULL;

const char* ssid       = "Shreeji";
const char* password   = "8866551417Cc";

boolean flag_1sec=false;

// Timer Interrupt Function
void IRAM_ATTR Timer_intr(){
    static unsigned int counter=0;
    counter++;
    if(counter%1000==0)
        flag_1sec=true;
}

void Timer_init() {
    hw_timer=timerBegin(0,80,true);// Timer0 , Prescaler , countUp  (80MHz)
    timerAttachInterrupt(hw_timer,&Timer_intr,true);
    timerAlarmWrite(hw_timer,1000,true); // false for only one time call

}

void setup(){
    Serial.begin(9600);
    WiFi.begin(ssid,password);
    Serial.printf("Connecting to %s",ssid);
    while(WiFi.status()!=WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Timer_init();
    Debug.begin("ESP32");
    timerAlarmEnable(hw_timer); // start timer
    
}
void loop(){
    if(flag_1sec){
        flag_1sec=false;
        Serial.println("1 SEC");
        Debug.println("1 SEC");
    }
    Debug.handle();
}