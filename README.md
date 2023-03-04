# Arduino_PublicData
## 공공데이터를 활용한 아두이노 프로젝트
공공데이터 날씨정보를 HTTP GET으로 받아 아두이노의 LCD, rgb LED 모듈로 나타낸 미니 프로젝트
### 활용데이터 및 설계방식
http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=1121581000

DATA를 GET하여 필요한 정보인 기온(Temp)과 날씨(WfEn)에 해당하는 부분만 인덱싱하여 LCD에 나타내고,
날씨는 크게 3가지로 분류하여 RGB LED 모듈을 이용하여 색상을 넣어주었다.

- 맑음 (연두)
- 비 (하늘)
- 흐림 (노랑)


### 아두이노 관련 기본 공부 정리 
[[Arduino] 마이크로 컨트롤러와 아두이노, IDE 설치](https://dev-raeun.tistory.com/44)
[[Arduino] 아두이노 시리얼 통신, 디지털 데이터 입출력 함수](https://dev-raeun.tistory.com/45)
[[Arduino] 아두이노 LED 블링크, 버튼 연결 저항](https://dev-raeun.tistory.com/46)
