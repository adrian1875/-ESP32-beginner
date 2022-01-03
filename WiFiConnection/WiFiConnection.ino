#include <WiFi.h>

const char* ID = "409";  /* const char*:  문자열 데이터를 상수로 사용 */
const char* PW = "polybot409";
void setup()
{
  Serial.begin(115200);  
  Serial.println();

  WiFi.begin(ID, PW);  /* 와이파이의 이름과 비번*/

  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED)  /* 연결되지 않는 동안 시리얼 모니터에 0.5초 간격으로 .을 찍는다*/
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  Serial.print("Connected, IP address: ");  /*연결 됬고*/
  Serial.println(WiFi.localIP());  /*연결되면 아이피 알려줌*/
}

void loop() {}
