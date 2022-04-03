// jury pin
const int standby_pin = 4;
const int correct_pin = 5;
const int wrong_pin = 6;
// team pin
const int team_A_pin = 7;
const int team_B_pin = 8;
const int team_C_pin = 9;
// interrupt pin
const int interrupt_pin_1 = 2;
const int interrupt_pin_2 = 3;
// display pin
const int led_pin = 12;

// variables will change:
// team index: A=1, B=2, C=3
int team_index = 0;
int team_A_point = 0;
int team_B_point = 0;
int team_C_point = 0;
int sum_of_false_answer = 0;
boolean standby = false;
boolean change_standby_state = false;

void reset_value(){
  // RESET VALUE
  standby = false;
  team_index = 0;
  sum_of_false_answer = 0;
}


//-----------------------------------------
void setup()
{
  // init usart with 115200
  Serial.begin(115200);

  pinMode(standby_pin, INPUT);
  pinMode(correct_pin, INPUT);
  pinMode(wrong_pin, INPUT);
  
  pinMode(team_A_pin, INPUT);
  pinMode(team_B_pin, INPUT);
  pinMode(team_C_pin, INPUT);
  
  pinMode(interrupt_pin_1, INPUT);
  pinMode(interrupt_pin_2, INPUT);


  pinMode(led_pin, OUTPUT);
  // Changed Rising to Falling, so this is executed when changing
  // from high to low.
  attachInterrupt(digitalPinToInterrupt(interrupt_pin_1), push, RISING);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin_2), unpush, FALLING);
}

//-----------------------------------------
void loop()
{  
  // Logging
  Serial.print("standby: ");
  Serial.println(standby);
  Serial.print("team index: ");
  Serial.println(team_index);
  Serial.print("A: ");
  Serial.println(team_A_point);
  Serial.print("B: ");
  Serial.println(team_B_point);
  Serial.print("C: ");
  Serial.println(team_C_point);
  Serial.print("sum_of_false_answer: ");
  Serial.println(sum_of_false_answer);
  Serial.print("\n");
  
  boolean standby_read = digitalRead(standby_pin);
  boolean correct_read = digitalRead(correct_pin);
  boolean wrong_read = digitalRead(wrong_pin);
  
  if (standby){
    // TODO: Display teams who can answer (ABC)
    
    if (standby_read == HIGH){
      Serial.print("standby ");
      // TODO: Display point all teams with 2 second interval 
    }

    if (correct_read == HIGH) {
      Serial.print("correct pin ");
      if (team_index == 1) {
        team_A_point = team_A_point + 2;
      } else if (team_index == 2) {
        team_B_point = team_B_point + 2;
      } else if (team_index == 3) {
        team_C_point = team_C_point + 2;
      }
      reset_value();
    } else if (wrong_read == HIGH){
      Serial.print("wrong pin ");
      
      if (team_index == 1) {
        team_A_point = team_A_point - 1;
      } else if (team_index == 2) {
        team_B_point = team_B_point - 1;
      } else if (team_index == 3) {
        team_C_point = team_C_point - 1;
      }
      change_standby_state = true;
      sum_of_false_answer += 1;

      // Max answer per round 2, if already 2
      // back to not ready state
      if (sum_of_false_answer == 2){
        reset_value(); 
      }
    }
  
  } else {
    // To Make State Ready
    if (standby_read == HIGH){
      Serial.print("standby ");
      //Serial.println(change_standby_state);
      standby = true;
      change_standby_state=true;
    }
  }

  delay(500);
}

//-----------------------------------------
// interrupt service routine (ISR)
void push()
{
  Serial.print("interrupt in");
  
  if (standby && change_standby_state){
    // Every round only csn answered max 2 times
    boolean A = digitalRead(team_A_pin);
    boolean B = digitalRead(team_B_pin);
    boolean C = digitalRead(team_C_pin);
    
    if (sum_of_false_answer <= 2) {
      if (A == HIGH && team_index != 1){
        Serial.println("----A-----");
        team_index = 1;
        digitalWrite(led_pin, HIGH);
        change_standby_state = false;
      } else if (B == HIGH && team_index != 2){
        Serial.println("----B-----");
        team_index = 2;
        digitalWrite(led_pin, HIGH);
        change_standby_state = false;
      } else if (C == HIGH && team_index != 3){
        Serial.println("----C-----");
        team_index = 3;
        digitalWrite(led_pin, HIGH);
        change_standby_state = false;
      }
    }
  }
}

void unpush()
{
  if (standby){
    digitalWrite(led_pin, LOW);
  }
}
