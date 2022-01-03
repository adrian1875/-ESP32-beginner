#include <WiFi.h> // 와이파이 라이브러리
#include <ESPAsyncWebServer.h>
//#include <WebServer.h> // 웹서버 라이브러리

const char* ID = "409";  
const char* PW = "polybot409";

AsyncWebServer server(80);  // 웹서버 오브젝트

int led = 1; // LED 핀번호 

void setup()
{
  // LED 핀 세팅
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  // 시리얼 세팅
  Serial.begin(115200);
  Serial.println();

  // 와이파이 접속
  WiFi.begin(ID, PW); // 공유기 이름과 비밀번호

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) // 와이파이 접속하는 동안 "." 출력
  {
    delay(500); 
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); // 접속된 와이파이 주소 출력



  server.on("/", handleRoot); // 루트에 접속 되었을때 실행할 함수 연결,  
                              // "/"는 google.com/과 같은 상태로 보면 됨 /뒤에 무언가가 써져서 동작을 하게 됨

  server.begin(); // 웹서버 시작
  Serial.println("HTTP server started");
}

void handleRoot() { // 루트에 접속할때 실행할 내용
  digitalWrite(5, LOW);
  server.send(200, "text/plain", "hello from esp32!");  // 200 : error code
                                                // text/plain: 옆에있는 "hello from esp32!"를 텍스트로 보냄
  delay(500);
  digitalWrite(5, HIGH);
  delay(500);
}

void loop() {
  //server.handleClient();  // 이걸 실행해주지 않으면 웹브라우저로 접속을 해도 아무 동작을 하지 않는다.

}
