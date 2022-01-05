#include <WiFi.h> // 와이파이 라이브러리
#include <WebServer.h>  // 웹서버 라이브러리

#define LED 5 // LED 핀번호 

const char* ssid     = "409";
const char* password = "polybot409";

WebServer server(80);  // 웹서버 오브젝트

void setup()
{
  // LED 핀 세팅
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // 시리얼 세팅
  Serial.begin(115200);
  Serial.println();

  // 와이파이 접속
  WiFi.begin(ssid, password); // 공유기 이름과 비밀번호

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) // 와이파이 접속하는 동안 "." 출력
  {
    delay(500); 
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); // 접속된 와이파이 주소 출력

  server.on("/on", handleOn); // /on 에 접속 되었을때 실행할 함수 연결
  server.on("/off", handleOff); // /off 에 접속 되었을때 실행할 함수 연결

  server.begin(); // 웹서버 시작
  Serial.println("HTTP server started");
}

void handleOn() { // /on 에 접속할때 실행할 내용
  digitalWrite(LED, HIGH);
  server.send(200, "text/plain", "LED ON");
}

void handleOff() { // /off 에 접속할때 실행할 내용
  digitalWrite(LED, LOW);
  server.send(200, "text/plain", "LED OFF");
}

void loop() {
  server.handleClient();
}
