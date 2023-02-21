# Arduino_PublicData
## 공공데이터를 활용한 아두이노 프로젝트
공공데이터 날씨정보를 아두이노 HTTP GET을 이용하여 LCD, rgb LED 모듈로 나타냄
### 활용데이터 및 설계방식
http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=1121581000
DATA를 GET하여 필요한 정보인 기온(Temp)과 날씨(WfEn)에 해당하는 부분만 인덱싱하여 LCD에 나타냄 (새로고침 하는 시간은 별도 설정가능)
날씨는 크게 3가지로 분류하여 RGB LED 모듈을 이용하여 색상을 넣어줌
- 맑음 (연두)
- 비 (하늘)
- 흐림 (노랑)