const int currentSensor = 4;
float voltajeSensor[160] = {};
float current[160] = {};
float  v=0;
float sp;
float Im = 0;
float shootTime = 8300;
String message;



const int gate = 15;
const int zeroCross = 2;

hw_timer_t * timer = NULL;
hw_timer_t * square = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);

  digitalWrite(gate, HIGH);
  timerAlarmDisable(timer);

  portEXIT_CRITICAL_ISR(&timerMux);
}


void IRAM_ATTR detectsZero() {
  digitalWrite(gate, LOW);
  timerWrite(timer, 0);
  timerAlarmWrite(timer, shootTime, false);
  timerAlarmEnable(timer);
}

void setup() {
  Serial.begin(115200);

  pinMode(gate, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(zeroCross, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(zeroCross), detectsZero, RISING);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 8300, false);


}

void loop() {

  if (Serial.available() > 0) {
    message = Serial.readString();
  }
  sp = message.toFloat();
  sp = map(sp, 10, 0, 0, 8000);
  shootTime = sp;

  for (int i = 0; i < 160; i++) {
    for (int j = 0; j < 15; j++) {
      v = analogRead(currentSensor) * (3.3 / 4095.0);
    }
    voltajeSensor[i] = v / 15.0;
    current[i] =  voltajeSensor[i] / 0.7;
    v=0;
    delayMicroseconds(105);
  }

  for (int i = 0; i < 160; i++) {
    Im = Im + current[i];
  }

  Im = (Im / 160.0) * 100.0;

  Serial.println(Im);

  delay(100);
  Im = 0;

}
