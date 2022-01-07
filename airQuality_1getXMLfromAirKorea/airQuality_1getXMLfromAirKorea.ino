/*
 * ESP8266 강좌 시리즈
 * 미제먼지 데이터 받아오기 1편
 * 
 * 본 저작물은 '한국환경공단'에서 실시간 제공하는 '한국환경공단_대기오염정보'를 이용하였습니다.
 * https://www.data.go.kr/tcs/dss/selectApiDataDetailView.do?publicDataPk=15073861
 */

#include <WiFi.h> // 와이파이 라이브러리
#include <HTTPClient.h> // HTTP 클라이언트

const char* ssid     = "409";
const char* password = "polybot409";

String city = "경북"; // 서울, 부산, 대구, 인천, 광주, 대전, 울산, 경기, 강원, 충북, 충남, 전북, 전남, 경북, 경남, 제주, 세종 중 입력
String key = "GoigxPSUzAF0LNPYi8mrLx%2FFPpydJY76rVh%2F0hfQUJc%2BKEzfMqbMHZxoiLtAci6dx3LtGPVwMjY4SZxKv0QAnQ%3D%3D";
String url = "http://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getCtprvnRltmMesureDnsty?sidoName=" + city + "&pageNo=1&numOfRows=100&returnType=xml&serviceKey=" + key;

void setup()
{
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
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) // 와이파이가 접속되어 있는 경우
  {
    WiFiClient client; // 와이파이 클라이언트 객체
    HTTPClient http; // HTTP 클라이언트 객체

    if (http.begin(client, url)) {  // HTTP
      // 서버에 연결하고 HTTP 헤더 전송
      int httpCode = http.GET();  // 에러가 없다면 0보다 큰 수가 들어오게 되어있음

      // httpCode 가 음수라면 에러
      if (httpCode > 0) { // 에러가 없는 경우
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString(); // 받은 XML 데이터를 String에 저장
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... 실패, 에러코드: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP] 접속 불가\n");
    }
    delay(600000);
  }
}
