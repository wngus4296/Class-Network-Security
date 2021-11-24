# NetworkSecurity
2021 - 2 NetworkSecurity and Programming


### 1️⃣ daytime 실습
* 수정 내용
  1. windows.h와 winsock2.h의 충돌이 있어서 강의자료에 있는 그대로 사용하면 에러 발생 (Windows OS의 경우) <br>
    winSock2.h 헤더가 먼저 include 되어야함

  2. 헤더파일은 필수적인 것만 포함
    err_sys 등의 에러 처리 함수는 단순히 화면 출력을 위해 printf로 수정

  3. server 소스파일 내 ctime() 함수가 보안적 문제로 에러를 띄우기 때문에 상단에 에러 무시 매크로 추가

```
- client와 server는 독립적인 프로젝트여야 함
- client 프로젝트에 daytimetcpcli.cpp 와 unp.h 추가
- server 프로젝트에 daytimetcpsvr.cpp 와 unp.h 추가
- 각기 컴파일 이후, server를 먼저 실행 --> 방화벽 경고 문구 --> 예외 추가
- cmd 콘솔창을 열어 "daytimetcpcli.exe 127.0.0.1" 실행
- 시간 날짜가 제대로 출력되는지 확인
```
