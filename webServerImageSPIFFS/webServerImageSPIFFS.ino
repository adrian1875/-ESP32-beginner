#include <SPIFFS.h>

#include <WiFi.h> // 와이파이 라이브러리
#include <WebServer.h>  //  웹서버 라이브러리
#include <FS.h> //파일 시스템 라이브러리

#define bulb  5 // BULB 핀번호

WebServer server(80); // 웹서버 오브젝트

const char* ID = "409";  
const char* PW = "polybot409";


boolean st = false;

void setup()
{
  // BULB 핀 세팅
  pinMode(bulb, OUTPUT);
  digitalWrite(bulb, LOW);

  // 시리얼 세팅
  Serial.begin(115200);
  Serial.println();

  // 와이파이 접속
  WiFi.begin(ID, PW); // 공유기 이름과 비밀번호

  SPIFFS.begin();

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) // 와이파이 접속하는 동안 "." 출력
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); // 접속된 와이파이 주소 출력

  server.on("/", handleRoot); // 루트에 접속 되었을때 실행할 함수 연결
  server.onNotFound(handleWebRequests); // 나머지 주소에 접속 되었을때 실행할 함수 연결

  server.begin(); // 웹서버 시작
  Serial.println("HTTP server started");
}

void handleRoot() { // 루트에 접속할때 실행할 내용
  if (st) server.sendHeader("Location", "/on.html", true);  // 접속하면 on.html여기로 리디랙션이 됨
  else server.sendHeader("Location", "/off.html", true);
  server.send(302, "text/html", "");
}

void loop() {
  server.handleClient();
}

// 404에러(내가 서버에 요청을 했는데 서버에 내가 여청한 url에 파일이 없다는 에러)를 보내는 함수인데 
// 이를 사용해서 지정하지 않은 주소를 입력했을 때 만약에 파일 시스템 안에 파일이 있을 때 파일을 보내주는 방식으로 
// 활용함 
void handleWebRequests() {
  if (loadFromSpiffs(server.uri())) return; // SPIFFS에 요청한 파일 확인 있다면 함수 종료
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

bool loadFromSpiffs(String path) { // SPIFFS 에서 파일 확인
  String dataType = "text/plain";

  // 요청한 파일의 확장자에 따라 데이터 타입 정의
  if (SPIFFS.exists(path)) {  // 파일 시스템에 파일이 존제하느냐?를 묻는 것
    // 요청한 주소에 따른 명령 처리
    if(path == "/on.html"){
      st = true;
      digitalWrite(bulb, st);
    }
    if(path == "/off.html"){
      st = false;
      digitalWrite(bulb, st);
    }

    // 주소의 확장자에따라 데이터 타입 지정
    if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
    else if (path.endsWith(".html")) dataType = "text/html";
    else if (path.endsWith(".htm")) dataType = "text/html";
    else if (path.endsWith(".css")) dataType = "text/css";
    else if (path.endsWith(".js")) dataType = "application/javascript";
    else if (path.endsWith(".png")) dataType = "image/png";
    else if (path.endsWith(".gif")) dataType = "image/gif";
    else if (path.endsWith(".jpg")) dataType = "image/jpeg";
    else if (path.endsWith(".ico")) dataType = "image/x-icon";
    else if (path.endsWith(".xml")) dataType = "text/xml";
    else if (path.endsWith(".pdf")) dataType = "application/pdf";
    else if (path.endsWith(".zip")) dataType = "application/zip";
    File dataFile = SPIFFS.open(path.c_str(), "r"); // SPIFFS 에서 파일 읽기
    
    // 확인된 데이터를 서버를 통해서 브라우저로 보내줌
    if (server.hasArg("download")) dataType = "application/octet-stream";
    if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    }
    dataFile.close();
    
    return true;
  }
  else return false;
}
