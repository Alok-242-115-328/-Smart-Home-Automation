#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Try 0x3F if 0x27 doesn't work

int ldrSensor = 0;   // LDR sensor connected to A0
int gasSensor = 0;   // Gas sensor connected to D8
int fireSensor = 0;  // Fire sensor connected to D4

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(A0, INPUT);   // LDR input
  pinMode(8, INPUT);    // Gas input
  pinMode(4, INPUT);    // Fire input

  pinMode(10, OUTPUT);  // LED for light (Day/Night)
  pinMode(6, OUTPUT);   // LED for gas alert
  pinMode(5, OUTPUT);   // LED for fire alert (blinks)
  pinMode(11, OUTPUT);  // Buzzer output

  Serial.begin(9600);

  // Welcome Message
  lcd.setCursor(4, 1);
  lcd.print("Hello Builders");
  lcd.setCursor(6, 2);
  lcd.print("Welcome");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read sensor values
  ldrSensor = analogRead(A0);
  gasSensor = digitalRead(8);
  fireSensor = digitalRead(4);

  // Print sensor values on Serial Monitor
  Serial.print("LDR: ");  Serial.println(ldrSensor);
  Serial.print("GAS: ");  Serial.println(gasSensor);
  Serial.print("FIRE: "); Serial.println(fireSensor);

  // Show sensor readings on LCD
  lcd.setCursor(0, 0);
  lcd.print("LDR: ");
  lcd.print(ldrSensor);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Gas: ");
  lcd.print(gasSensor);
  lcd.print("   ");

  // ---------- LDR Control ----------
  if (ldrSensor > 700) {
    digitalWrite(10, HIGH); // LED ON at night
    lcd.setCursor(0, 2);
    lcd.print("Light: Night   ");
  } else {
    digitalWrite(10, LOW);  // LED OFF in daytime
    lcd.setCursor(0, 2);
    lcd.print("Light: Day     ");
  }

  // ---------- Combined Gas + Fire Alert ----------
  if (gasSensor == LOW && fireSensor == LOW){ // Both sensors triggered
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("GAS&FIRE ALERT!");
    lcd.setCursor(3, 2);
    lcd.print("TAKE ACTION!");
    
    // Blink both LEDs + buzzer
    for (int i = 0; i < 4; i++) {
      digitalWrite(6, HIGH);
      digitalWrite(5, HIGH);
      tone(11, 1000);
      delay(300);
      digitalWrite(6, LOW);
      digitalWrite(5, LOW);
      noTone(11);
      delay(300);  
    }
    lcd.clear();
  }

  // ---------- Fire Only ----------
  else if (fireSensor == LOW) {
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("FIRE ALERT!");
    
    for (int i = 0; i < 4; i++) {
      digitalWrite(5, HIGH);
      tone(11, 1000);
      delay(300);
      digitalWrite(5, LOW);
      noTone(11);
      delay(300);
    }
    lcd.clear();
  }

  // ---------- Gas Only ----------
  else if (gasSensor == LOW) {/////
    digitalWrite(6, HIGH);
    tone(11, 600);
    lcd.setCursor(0, 3);
    lcd.print("Gas: DANGER!!! ");
  }

  // ---------- Safe Condition ----------
  else {
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    noTone(11);
    lcd.setCursor(0, 3);
    lcd.print("Gas & Fire Safe");
  }

  delay(500);
}
