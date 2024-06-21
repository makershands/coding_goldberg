#include <Servo.h>

/* 센서 핀 번호 지정 */
const int trigPin = 5; // 초음파 센서의 trig 핀을 5번 핀으로 지정
const int echoPin = 6; // 초음파 센서의 echo 핀을 6번 핀으로 지정

/* 모터 핀 번호 지정 */
const int servoPin = 9;// 서보모터의 핀을 9번 핀으로 지정
const int m1 = 10;     // 모터 드라이버의 핀을 10번 핀으로 지정
const int m2 = 11;     // 모터 드라이버의 핀을 11번 핀으로 지정

const int speed = 5;
Servo servo;

const float threshold = 7;

void setup() {
  /* 센서 핀 모드 설정 */
  pinMode(trigPin, OUTPUT); // 초음파 센서의 trig 핀을 출력 모드로 설정
  pinMode(echoPin, INPUT);  // 초음파 센서의 echo 핀을 입력 모드로 설정

  /* 모터 설정 */
  pinMode(m1, OUTPUT); // 모터 드라이버 핀을 출력 모드로 설정
  pinMode(m2, OUTPUT); // 모터 드라이버 핀을 출력 모드로 설정
  
  servo.attach(servoPin); // 서보모터 핀으로 지정한 핀에서 서보모터를 사용
  servo.write(0); // 서보모터를 원래 자리(0도)로 회전

  Serial.begin(9600); // 시리얼 통신을 9,600 baud 속도로 시작
}

float measure_distance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  /* 발생한 초음파가 초음파 센서로 돌아오면 걸린 시간을 바탕으로 물체까지의 거리를 계산함 */
  unsigned long duration = pulseIn(echoPin, HIGH); // 초음파가 초음파 센서로 돌아오기까지의 시간을 duration 변수에 저장
  float result = (344.97 * (float)duration / 10000) / 2; // 초음파가 돌아오기까지 걸린 시간을 바탕으로 거리를 계산(23℃ 기준)하여 cm 단위로 distance 변수에 저장
  Serial.print("초음파 센서 거리: "); Serial.print(result); Serial.println(" cm"); // 시리얼 모니터에 거리를 출력
  return result;
}

void move(int angle, int speed){
  while(true){
    int temp = servo.read();
    if     (temp < angle) servo.write(temp + 1);
    else if(temp > angle) servo.write(temp - 1);
    else return;
    delay(speed);
  }
}

void servo_carry(){
  move(180, speed);
  Serial.println("서보모터 공 옮김.");
  delay(800);
  move(0, speed);
  Serial.println("서보모터 원위치.");
  delay(800);
}

void motor_carry(){
  digitalWrite(m1, HIGH); digitalWrite(m2, LOW); delay(3315); // 3.315초 시간 동안 DC 모터를 작동함
  digitalWrite(m1, LOW);  digitalWrite(m2, LOW); // DC 모터 작동을 종료함
}

void loop() {
  /* 초음파 센서로 초음파를 발생함 */
  float distance = measure_distance();
  /* 초음파 센서로 측정된 거리가 일정 거리 미만이면 */
  if(distance < threshold) {
    Serial.println("초음파 센서에 물체가 감지됨");
    delay(1000); // 1초 대기함

    Serial.print("서보모터 작동 중...");
    servo_carry();

    Serial.print("모터 작동 중...");
    motor_carry();
    Serial.println("모터 작동 완료.");
  }
}
