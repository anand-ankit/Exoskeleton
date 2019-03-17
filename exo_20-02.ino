#define TRIG_PIN  A2
#define ECHO_PIN  A1
#define dir_lefta1 53  //expansion left a
#define dir_lefta2 49  //compression left a
#define dir_leftb1 47   //expansion left b
#define dir_leftb2 43   //compression left b
#define dir_righta1 39  //expansion right b
#define dir_righta2 35   //compression right b
#define dir_rightb1 33   //expansion right a
#define dir_rightb2 29   //compression right a
#define pwm_l1 10  //left a
#define pwm_l2 8    //left b
#define pwm_r1 7  //right b
#define pwm_r2 5  //right a
#define limitswitch1 11
#define limitswitch2 12

float ultra();
int buttonPin1 =  2;     // the number of the pushbutton pin
int buttonPin2 =  3;

double X[70]={9.88534,9.88534,9.62008,9.88534,9.88534,9.35516,9.62008,9.35516,9.09062,8.82649,8.82649,8.82649,9.09062,8.56281,8.56281,8.82649,8.82649,8.29962,8.29962,8.56281,8.03695,8.03695,8.29962,8.29962,7.77484,8.29962,8.29962,7.77484,7.77484,8.03695,8.03695,7.51332,7.51332,8.03695,8.03695,7.51332,7.51332,8.03695,7.51332,7.51332,7.51332,8.29962,8.29962,7.77484,8.29962,8.29962,8.56281,8.03695,8.03695,8.82649,8.82649,8.29962,8.29962,9.35516,8.56281,9.88534,9.09062,10.1509,10.1509,9.62008,9.88534};
double x[70]={4.95585,5.04931,4.95585,5.32484,5.41506,5.14196,5.50447,5.41506,5.32484,5.32484,5.41506,5.50447,5.76778,5.59306,5.68083,5.9392,6.02367,5.85391,5.9392,6.19011,6.02367,6.10731,6.35319,6.51291,6.27207,6.66925,6.74614,6.5915,6.66925,6.89737,6.97169,6.82218,6.89737,7.26039,7.3304,7.18951,7.26039,7.60173,7.46781,7.53521,7.60173,8.04273,8.10218,7.98238,8.2752,8.33108,8.49333,8.38606,8.44014,8.79344,8.84028,8.74569,8.79344,9.18209,9.01852,9.43284,9.25835,9.58412,9.61159,9.5264,9.61159};
double c[70]={16.5, 17,17.5,18,18.5,19,19.5,20,20.5,21,21.5,22,22.5,23,23.5,24,24.5,25,25.5,26,26.5,27,27.5,28,28.5,29,29.5,30,30.5,31,31.5,32,32.5,33,33.5,34,34.5,35,35.5,36,36.6,37,37.5,38,38.5,39,39.5,40,40.5,41,41.3974,42,42.5,43,43.5,44,44.5,45,45.5,46,46.5};
int buttonState1 = 0;
int buttonState2 = 0;

float t1;
float t2;
float pulse_width ;
float cm;
float inches;
const unsigned int MAX_DIST = 23200;

float d;   //ultra sonic output
float h = 144.00       ; //height of exo
long unsigned int T1, T2, T3 , change_T1, change_T2, net_change, i  ;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  digitalWrite(buttonPin1, LOW);
  digitalWrite(buttonPin2, LOW);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(pwm_l1, OUTPUT);
  pinMode(pwm_l2, OUTPUT);
  pinMode(pwm_r1, OUTPUT);
  pinMode(pwm_r2, OUTPUT);
  pinMode(dir_lefta1, OUTPUT );
  pinMode(dir_lefta2, OUTPUT);
  pinMode(dir_leftb1, OUTPUT );
  pinMode(dir_leftb2, OUTPUT);
  pinMode(dir_righta1, OUTPUT );
  pinMode(dir_righta2, OUTPUT);
  pinMode(dir_rightb1, OUTPUT );
  pinMode(dir_rightb2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(limitswitch1),STOP,FALLING);
  attachInterrupt(digitalPinToInterrupt(limitswitch2),STOP,FALLING);
}


void loop()
{
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  while (buttonState1 == HIGH && buttonState2 == LOW)
  {
    d=ultra();
    Serial.println(h-d);
    Serial.println("high");
    for (i = 0; i < 61; i++)
    {

      Serial.println(i);
      if (( c[i] < (h - d) ) && (c[i] > (h - d + 1) ) )
      { 
        
        //  Serial.println(h - d);
        Serial.println("dataset");
        change_T1 = (abs(X[i+1] - X[i]) / .8);
        change_T2 = (abs(x[i+1] - x[i]) / .8);
        net_change = change_T1 - change_T2;
        break;
      }
    }

    if (net_change <= 0)
    {
      Serial.println("net_change <= 0");
      T1 = micros();
      T2 = T1 + abs( change_T1 * 1000000) ;
      while (1)
      {
        Serial.println("actuators run");
        act1();
        act2();
        T3 = micros();
        if (T3 >= T2)
        {
          T3 = micros();
          Serial.println("actuators one stop");
          break;
        }break;
      }
    }
    else
    {
      Serial.println("net_change > 0");
      T1 = micros();
      T2 = T1 + (change_T2 * 1000000) ;
      while (1)
      {
        Serial.println("actuators run");
        act1();
        act2();
        T3 = micros();
        if (T3 >= T2)
        {
          T3 = micros();
          Serial.println("actuator two stop");
          break;
        }
      }
    }
    if (net_change <= 0)
    {
      Serial.println("net_change <= 0");
      T1 = micros();
      T2 = T1 + (net_change * 1000000) ;
      while (1)
      {
        act2();
        T3 = micros();
        if (T3 >= T2)
        {
          T3 = micros();
          Serial.println("actuator two stop");
          break;
        }
      }
    }
    else
    {
      Serial.println("net_change> 0");
      T1 = micros();
      T2 = T1 + (net_change * 1000000) ;
      while (1)
      {

        act1();
        T3 = micros();
        if (T3 >= T2)
        {
          T3 = micros();
          Serial.println("actuator one stop");
          break;
        }
      }
    }
    STOP();
    //delay(500);
    break;
  }
  while (buttonState2 == HIGH && buttonState1 == LOW)
  {
    Serial.println("LOW");
    d=ultra();
    //Serial.println(h-d);
    for (i = 0; i < 61; i++)
    {
     //d=ultra();
      Serial.println(i);
      if (( c[i] < (h - d) ) && (c[i] > (h - d - 1) ) )
      {
        //Serial.println(h - d);
        Serial.print("dataset");
        change_T1 = (X[i] - X[i - 1] / .8);
        change_T2 = (x[i] - x[i - 1] / .8);
        net_change = change_T1 - change_T2;
        break;
      }
    }

    if (net_change <= 0)
    {
      Serial.println("net_change <= 0");
      T1 = micros();
      T2 = T1 + abs( change_T1 * 1000000) ;
      while (1)
      {
        Serial.println("actuator runs");
        act3();
        act4();
        T3 = micros();
        if (T3 >= T2)
        {
          T3 = micros();
          Serial.println("actuator one stop");
          //delay(10000);
           break;
        }
      }
    }
    else
    {
      Serial.println("net_change > 0");
      T1 = micros();
      T2 = T1 + (change_T2 * 1000000) ;
      while (1)
      {
        act3();
        act4();
        Serial.println("actuator runs");
        T3 = micros();
        if (T3 >= T2)  
        {
          T3 = micros();
          Serial.println("actuator two stop");
          break;
        }
      }
    }
    if (net_change <= 0)
    {
      Serial.println("net_change <= 0");
      T1 = micros();
      T2 = T1 + (net_change * 1000000) ;
      while (1)
      {
        act4();
        T3 = micros();
        if (T3 >= T2)
        {
          T3 = micros();
          Serial.println("actuator two stop");
          break;
        }
      }
    }
    else
    {
      Serial.println("net_change > 0");
      T1 = micros();
      T2 = T1 + (net_change * 1000000) ;
      while (1)
      {

        act3();
        T3 = micros();
        if (T3 >= T2)
        {
          T3 = micros();
          Serial.println("actuator one stop");
          break;
        }
      }
    }
    STOP();
    //delay(500);
    break;
  }


  
  while (buttonState2 == HIGH && buttonState1 == HIGH)
  {
    //Serial.println('c');
    STOP();
    //delay(500);
    break;
  }
  while (buttonState2 == LOW && buttonState1 == LOW)
  {
    //Serial.println('d');
    STOP();
    //delay(500);
    break;
  }
}
void act1()
{
  if (X[i] >= X[i+1])
  {
    //compress
    digitalWrite(dir_lefta2, 1);
    digitalWrite(dir_lefta1, 0);
    analogWrite(pwm_l1, 255);
    digitalWrite(dir_rightb2, 1);
    digitalWrite(dir_rightb1, 0);
    analogWrite(pwm_r1, 255);
  }
  else
  {
    //expansion2
    digitalWrite(dir_lefta1, 1);
    digitalWrite(dir_lefta2, 0);
    analogWrite(pwm_l1, 255);
    digitalWrite(dir_rightb2, 0);
    digitalWrite(dir_rightb1, 1);
    analogWrite(pwm_r1, 255);
  }
}
 void act2()
{
  if (x[i] >= x[i+1])
  {
    //compress
    digitalWrite(dir_leftb2, 1);
    digitalWrite(dir_leftb1, 0);
    analogWrite(pwm_l2, 255);
    digitalWrite(dir_righta2, 1);
    digitalWrite(dir_righta1, 0);
    analogWrite(pwm_r2, 255);
  }
  else
  {
    digitalWrite(dir_leftb1 , 1);
    digitalWrite(dir_leftb2 , 0);
    analogWrite(pwm_l2, 255);
    digitalWrite(dir_righta1 , 1);
    digitalWrite(dir_righta2 , 0);
    analogWrite(pwm_r2, 255);
  }
}


void act3()
{
  if (X[i] >= X[i-1])
  {
    //compress
    digitalWrite(dir_lefta2, 1);
    digitalWrite(dir_lefta1, 0);
    analogWrite(pwm_l1, 255);
    digitalWrite(dir_rightb2, 1);
    digitalWrite(dir_rightb1, 0);
    analogWrite(pwm_r1, 255);
  }
  else
  {
    //expansion2
    digitalWrite(dir_lefta1, 1);
    digitalWrite(dir_lefta2, 0);
    analogWrite(pwm_l1, 255);
    digitalWrite(dir_rightb2, 0);
    digitalWrite(dir_rightb1, 1);
    analogWrite(pwm_r1, 255);
  }
}
  void act4()
{
  if (x[i] >= x[i-1])
  {
    //compress
    digitalWrite(dir_leftb2, 1);
    digitalWrite(dir_leftb1, 0);
    analogWrite(pwm_l2, 255);
    digitalWrite(dir_righta1, 0);
    digitalWrite(dir_righta2, 1);
    analogWrite(pwm_r2, 255);
  }
  else
  {
    digitalWrite(dir_leftb1 , 1);
    digitalWrite(dir_leftb2 , 0);
    analogWrite(pwm_l2, 255);
    digitalWrite(dir_righta1 , 1);
    digitalWrite(dir_righta2 , 0);
    analogWrite(pwm_r2, 255);
  }
}

float ultra()
{
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  pulse_width = pulseIn(ECHO_PIN , HIGH); // t2 - t1;
  cm = pulse_width / 58.0;
  // inches = pulse_width / 148.0;
  if ( pulse_width > MAX_DIST )
  {
    Serial.println("Out of range");
  }
  else
  {
    //Serial.print(cm);
    //Serial.println(" cm \t");
    return cm;
  }
}
void STOP()
{
  analogWrite(pwm_l1, 0);
  analogWrite(pwm_l2, 0);
  analogWrite(pwm_r1, 0);
  analogWrite(pwm_r2, 0);
}

