#include <Servo.h> // 서보 모터 사용에 필요한 라이브러리를 포함

/* 센서 핀 번호 지정 */
int trigPin = 5; // 초음파 센서의 trig 핀을 5번 핀으로 지정
int echoPin = 6; // 초음파 센서의 echo 핀을 6번 핀으로 지정
int IRPin = 7; // 적외선 센서의 입력 핀을 7번 핀으로 지정

/* 모터 핀 번호 지정 */
int motorPin = 8; // DC 모터의 핀을 8번 핀으로 지정
int servoPin = 9; // 서보 모터의 핀을 9번 핀으로 지정

int m1 = 10;
int m2 = 11;

Servo servo; // 서보 모터 객체 선언

unsigned long duration; // 초음파 센서 시간 값을 저장할 변수를 선언
float distance; // 초음파 센서 시간 값을 바탕으로 계산한 거리 값을 저장할 변수를 선언
int IRstatus; // 적외선 센서 상태 값을 저장할 변수를 선언

void setup() {
  /* 센서 핀 모드 설정 */
  pinMode(trigPin, OUTPUT); // 초음파 센서의 trig 핀을 출력 모드로 설정
  pinMode(echoPin, INPUT); // 초음파 센서의 echo 핀을 입력 모드로 설정
  pinMode(IRPin, INPUT); // 적외선 센서의 입력 핀을 입력 모드로 설정

  /* 모터 설정 */
  pinMode(motorPin, OUTPUT); // DC 모터 핀을 출력 모드로 설정
  servo.attach(servoPin); // 서보 모터 핀으로 지정한 핀에서 서보 모터를 사용
  servo.write(0); // 서보 모터를 원래 자리로 회전

  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);

  Serial.begin(9600); // 시리얼 통신을 9,600 baud 속도로 시작
}

void loop() {
  /* 초음파 센서로 초음파를 발생함 */
  digitalWrite(trigPin, LOW); digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  /* 초음파 센서로 발생한 초음파가 되돌아오면 그 시간을 바탕으로 물체까지의 거리를 계산함 */
  duration = pulseIn(echoPin, HIGH); // 초음파 센서로 초음파가 되돌아오기까지의 시간을 duration 변수에 저장
  distance = ((float)(345.26 * duration) / 10000) / 2; // 초음파가 되돌아오기까지의 시간을 바탕으로 거리 계산(23℃ 기준)
  Serial.print("초음파 센서 거리: "); Serial.print(distance); Serial.println(" cm"); // 시리얼 모니터에 거리를 출력

  /* 초음파 센서로 측정된 거리가 일정 거리 미만이면 */
  if(distance < 4) {
    Serial.println("초음파 센서에 물체가 감지됨");
    delay(1000);
    Serial.print("모터 작동 중...");
    //digitalWrite(motorPin, HIGH); delay(5000); digitalWrite(motorPin, LOW); // 일정 시간동안 모터를 작동함

    digitalWrite(m1, HIGH); digitalWrite(m2, LOW); delay(3500);
    digitalWrite(m1, LOW); digitalWrite(m2, LOW);

    Serial.println(" 작동 완료.");
    servo.write(0); delay(500); 
  }

  IRstatus = digitalRead(IRPin); // 적외선 센서로부터 상태 값을 불러와 IRstatus 변수에 저장
  Serial.print("적외선 센서 상태: "); Serial.println(IRstatus ? "감지된 물체가 없음" : "물체를 감지함"); // 시리얼 모니터에 적외선 센서 상태를 출력

  /* 적외선 센서에 물체가 감지되면 */
  if(IRstatus == 0) {
    Serial.println("적외선 센서에 물체가 감지됨");
    delay(500);
    for(int i = 1; i <= 180; i ++) {
      servo.write(i); //delay(1000); // 서보 모터를 180도 회전시켰다가 다시 원래 자리로 회전
      delay(5);
    }
  }

  delay(80); Serial.println();
}
