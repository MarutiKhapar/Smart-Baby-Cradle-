
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

#include <LiquidCrystal.h>
LiquidCrystal lcd(4,5,6,7,8,9);
//LiquidCrystal lcd(rs, en, D4, D5, D6, D7);
//temp
int lm35 = A0;
int temp;
//moist sens
int moistpin = A1; 
int moist_Value;  
int limit = 300;
//sound sensor
int s_pin = A2;
int s_val;
//motor relay
int relay_fan = 11;
int relay_motor = 12;
//buzzer
int buzzer = 10;

int valSensor2;
int valSensor4;
int valSensor6;
void setup()
{
  Serial.begin(9600); 
  mySerial.begin(9600);
  pinMode(lm35, INPUT);
  pinMode(moistpin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay_fan, OUTPUT);
  pinMode(relay_motor, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Smart");
  lcd.setCursor(0,1);
  lcd.print("Cradle");
  delay(2000);
  lcd.clear();
}

void loop() 
{
  valSensor2 = temp1();
  valSensor4 = moist();
  valSensor6 = sound();
  if((valSensor4 < 600))
    {
      Serial.println("BUZZER");
      lcd.setCursor(0,1);
      lcd.print("BUZZER: ONN");
      delay(1000);
      lcd.clear();
      digitalWrite(buzzer, HIGH);
      delay(2000);
      digitalWrite(buzzer, LOW);
      lcd.setCursor(0,1);
      lcd.print("BUZZER: ONN");
      delay(1000);
      lcd.clear();
    }
    delay(10);
    
    if((valSensor2 > 30))
    {
      Serial.println("FAN START");
      lcd.setCursor(0,1);
      lcd.print("FAN: ONN");
      lcd.clear();
      digitalWrite(relay_fan, HIGH);
      delay(1500);
    }
    else
    {
      Serial.println("FAN STOP");
      lcd.setCursor(0,1);
      lcd.print("FAN: OFF");
      lcd.clear();
      digitalWrite(relay_fan, LOW);
      delay(1500);
    }
 if(valSensor6 > 10)
    {
      Serial.println("Cradle START");
      Serial.println("Doll START");
      lcd.setCursor(0,0);
      lcd.print("Cradle: ON");
      lcd.setCursor(0,1);
      lcd.print("Doll: ON");
      lcd.clear();
      digitalWrite(relay_motor, HIGH);
      delay(1500);
      SendMessage();
    }
    else
    {
      Serial.println("Cradle STOP");
      Serial.println("Doll STOP");
      lcd.setCursor(0,0);
      lcd.print("Cradle: OFF");
      lcd.setCursor(0,1);
      lcd.print("Doll: OFF");
      lcd.clear();
      digitalWrite(relay_motor, LOW);
      delay(1500);
    }
    
    if (mySerial.available()>0)
     Serial.write(mySerial.read());
  
}

int temp1()
{
  temp=analogRead(lm35);
  int temp_val = (temp * 4.88); /* Convert adc value to equivalent voltage */
  temp_val = (temp_val/10);                                                                      
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  Serial.print("Temperature:");
  lcd.setCursor(13, 0);
  lcd.print(temp_val);
  Serial.println(temp_val);
  delay(1000);
  lcd.clear();
  return temp_val;
}

int moist()
{
  moist_Value = analogRead(moistpin);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MOISTURE: ");
  delay(1000);
  lcd.setCursor(10, 0);
  lcd.print(moist_Value);
  delay(1000);
  lcd.clear(); 
  Serial.print("Mosit Value : ");
  Serial.println(moist_Value);
  return moist_Value;
}

int sound()
{
  s_val = analogRead(s_pin);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SOUND: ");
  delay(1000);
  lcd.setCursor(7, 0);
  lcd.print(s_val);
  delay(1000);
  lcd.clear(); 
  Serial.print("Sound Value : ");
  Serial.println(s_val);
  return s_val;
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1\r");    //Sets the GSM Module in Text Mode
  delay(5000);  // Delay of 1000 milli seconds or 1 second
  Serial.println("Gsm Into SMS mode");
  mySerial.println("AT+CMGS=\"+918296346230\""); // Replace x with mobile number
  delay(5000);
  mySerial.println("Baby Is Crying, Rush Home!!!");// The SMS text you want to send
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay (5000);
  Serial.println("Message sent");
  delay (5000);
}
 void RecieveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay (1000);
 }
