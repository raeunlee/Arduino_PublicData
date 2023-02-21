#include <Arduino_BuiltIn.h>

#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

//공공데이터 날씨정보를 아두이노 HTTP GET을 이용하여 LCD, rgb LED 모듈로 나타내기

#include <Arduino.h>
#include <Wire.h> //I2C 통신
#include <WiFi.h>
#include <WiFiMulti.h>
#include <LiquidCrystal_I2C.h> //LCD 라이브러리

#include <HTTPClient.h>
#define USE_SERIAL Serial
LiquidCrystal_I2C lcd(0x27,16,2); //LCD 선언
WiFiMulti wifiMulti;

int r = 4; //red
int g = 0; //green
int b = 2; //yellow


void setup() {
    USE_SERIAL.begin(115200);
    lcd.init();
    lcd.backlight();
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    wifiMulti.addAP("SK_WiFiGIGAD9EB", "1701012099"); //내 테더링 아이디, 비번
    xTaskCreate(task1,"task1",2048,NULL,1,NULL);
}
String temp;
String wfEn;

void loop() { //API에서 정보를 받아오고,원하는 정보만 Parsing
  int endtag_index = 0;
  String tag_str;
  String content_str;
    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {
        USE_SERIAL.printf("wifi 연결됨"); 
        HTTPClient http;

        //USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        http.begin("http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=1121581000"); //HTTP

        //USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) { //링크에 있는 정보를 다 불러왔을때
                String payload = http.getString(); //payload 변수에 링크에 표현되는 정보를 넣어라
                //USE_SERIAL.println(payload); //모든 정보가 열람 가능함!!
                endtag_index = payload.indexOf("</data>"); //indexOf ()안에 있는 내용의 위치(index)를 찾아라.
                //USE_SERIAL.println(payload_index);

                if(endtag_index > 0) // </data> 가 있으면,
                {
                    tag_str = "<data seq=\"0\">"; // abcd.substring(1,4) = bc
                    content_str =payload.substring(payload.indexOf(tag_str)+(tag_str.length()) , endtag_index);
                    USE_SERIAL.println(content_str);

                    endtag_index = content_str.indexOf("</wfEn>"); //날씨 영어로 나타남
                    if(endtag_index > 0)
                    {
                      tag_str = "<wfEn>";
                      wfEn =content_str.substring(content_str.indexOf(tag_str)+tag_str.length(),endtag_index);
                      USE_SERIAL.println(wfEn);
                    }

                    endtag_index = content_str.indexOf("</temp>"); //기온
                    if(endtag_index > 0)
                    {
                      tag_str = "<temp>";
                      temp =content_str.substring(content_str.indexOf(tag_str)+tag_str.length(),endtag_index);
                      USE_SERIAL.println(temp);

                      // 날씨와 온도를 LCD에 나타냄
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Weather: " + wfEn);
                      lcd.setCursor(0,1);
                      lcd.print("Temp: " + temp + "C");
                    }
                }
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    vTaskDelay(3000/portTICK_PERIOD_MS); //3초마다 초기화
}

String temp_lcd;
String wfEn_lcd;

void task1(void *parameter) //날씨를 LED로 표현
{
  while(1){
    //맑은 날은 라임색
    if(wfEn.equals("Clear") == 1)
    {
      digitalWrite(r, 191);
      digitalWrite(g, 255);
      digitalWrite(b, 0);
    }
    //비오는 날 탁한 하늘색
    if((wfEn.equals("Rain") == 1) || (wfEn.equals("Shower") == 1))
    {
      digitalWrite(r, 80);
      digitalWrite(g, 188);
      digitalWrite(b, 223);
    }
    //구름낀 날은 탁한 노란색
    if((wfEn.equals("Cloudy") == 1)||(wfEn.equals("Mostly Cloudy") == 1))
    {
      digitalWrite(r, 255);
      digitalWrite(g, 212);
      digitalWrite(b, 0);
    }
    
  }
}
