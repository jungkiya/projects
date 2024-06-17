#include <Stepper.h>

// 모터 200 펄스 (1펄스당 1.8도)
const int stepsPerRevolution = 200;
// 스텝모터 A, B, /A, /B
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

// 시리얼 포트 통신에 사용하는 문자
char cmd;

// 컨베이어 벨트 모터
int PWM = 6;
int IN1 = 4;
int IN2 = 5;

// 보관함 스텝모터
int st1 = 8;
int st2 = 9;
int st3 = 10;
int st4 = 11;
int st_1 = 12;
int st_2 = 13;
int stt1 = 2;
int stt2 = 3;

// delay 대체 millis() 사용 함수 time
// 컨베이어 벨트 정지 관련 체크 check
unsigned long time;
unsigned long time_to;
bool check, check2;

// 출력문자 저장 (구동 종료시 문자 출력)
int to_py;


void setup() {
  // 컨베이어 벨트 모터제어
  pinMode(PWM, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // // 보관함 스텝모터 제어
  // pinMode(st1, OUTPUT);
  // pinMode(st2, OUTPUT);
  // pinMode(st3, OUTPUT);
  // pinMode(st4, OUTPUT);
  pinMode(st_1, OUTPUT);
  pinMode(st_2, OUTPUT);
  pinMode(stt1, OUTPUT);
  pinMode(stt2, OUTPUT);

  digitalWrite(st_1, HIGH);
  digitalWrite(st_2, HIGH);
  digitalWrite(stt1, HIGH);
  digitalWrite(stt2, HIGH);

  // 스텝모터 속도
  myStepper.setSpeed(30);

  // 아두이노 통신속도
  Serial.begin(9600);
}

void loop() {
  // 딜레이 대체 타임
  unsigned long time  = millis();

  // 시리얼 입력이 들어왔을때 실행
  if(Serial.available())
  {
    // cmd에 시리얼 입력값 저장
    cmd = Serial.read();

    if(cmd == 'a')  // 투입구 -> 카메라
    {
      time_to = time - 2000;
      check = true;

      to_py = 1;
      digitalWrite(IN2, LOW);
      digitalWrite(IN1, HIGH);
      analogWrite(PWM, 255);
    }
    else if(cmd == 'b') // 카메라 -> 투입구 (반출)
    {
      time_to = time - 1800;
      check = true;

      to_py = 2;
      digitalWrite(IN2, HIGH);
      digitalWrite(IN1, LOW);
      analogWrite(PWM, 255);
    }
    else if(cmd == 'c') // 카메라 -> 보관함 (반입)
    {
      time_to = time;
      check = true;

      to_py = 3;
      digitalWrite(IN2, LOW);
      digitalWrite(IN1, HIGH);
      analogWrite(PWM, 255);
    }
    else if(cmd == 'd') // 카메라 캡쳐
    {
      time_to = time - 5400;
      check = true;
      
      to_py = 4;
      digitalWrite(IN2, LOW);
      digitalWrite(IN1, LOW);
      analogWrite(PWM, 0);
    }
    
    // 스텝모터 회전 90 (천 -> 오천)
    else if(cmd == 'e') 
    { 
      time_to = time; 
      check = true;  
      to_py = 5;  

      digitalWrite(IN2, LOW);
      digitalWrite(IN1, HIGH);
      analogWrite(PWM, 255);
      myStepper.step(50); 
    }
    // 스텝모터 회전 180  (천 -> 만)
    else if(cmd == 'f') 
    { 
      time_to = time; 
      check = true;  
      to_py = 6;  

      digitalWrite(IN2, LOW);
      digitalWrite(IN1, HIGH);
      analogWrite(PWM, 255);
      myStepper.step(100); 
    }
    // 스텝모터 회전 -90  (천 -> 오만)
    else if(cmd == 'g')
    { 
      time_to = time; 
      check = true;  
      to_py = 7;  

      digitalWrite(IN2, LOW);
      digitalWrite(IN1, HIGH);
      analogWrite(PWM, 255);
      myStepper.step(-50); 
    }
  }

  // 5.5초가 지나면 정지
  if (time - time_to >= 5500 && check)
  {
    Serial.println(to_py);
    digitalWrite(IN2, LOW);
    digitalWrite(IN1, LOW);
    analogWrite(PWM, 0);

    if (to_py == 5) { myStepper.step(-50); }
    else if (to_py == 6) { myStepper.step(-100); }
    else if (to_py == 7) { myStepper.step(50); }

    check = false;
    to_py = 0;
  }
}
