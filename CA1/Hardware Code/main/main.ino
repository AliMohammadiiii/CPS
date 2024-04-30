#include <LiquidCrystal.h> //library for LCD 
#include <Servo.h>

struct Data
{
    String id;
    String date;
};

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
Servo myservo;

const int RedlED = 4;
const int GreenlED = 2;
const int ServoPin = 6;

char c;
String id;

int wait = 5000;

void setup() {
  Serial.begin(9600);
  pinMode(RedlED, OUTPUT);
  pinMode(GreenlED, OUTPUT);
  myservo.attach(ServoPin);
  Serial.println("Please scan your RFID Card");

  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("  THE BRIGHT LIGHT ");
  lcd.setCursor(0, 1);
  lcd.print(" RFID BASED LOCK SYS");
  lcd.setCursor(0, 2);
  lcd.print(" CARD:   ");
  lcd.setCursor(0, 3);
  lcd.print(" Date:   ");

}

void readFromRFID()
{
    int count = 0;
    if (Serial.available() > 0)
    {
        while (count != 12)
        {
            c = Serial.read();
            count++;
            id += c;
        }
    }
    else
    {
        id = "n";
    }
    
}

struct Data validate_person()
{
    struct Data person;
    if (id == "n")
    {
        person.id = "noAction";
        return person;
    }

    if (id == "E280689401A9" || id == "E2000019060C")
    {
        person.id = id;
        person.date = "now";
        return person;
    }
    else
    {
        person.id = "Access Denied!";
        return person;
    }
}

void action(struct Data person)
{
    if (person.id != "noAction")
    {
        if (person.id == "Access Denied!")
        {
            myservo.write(0);
            lcd.setCursor(0, 2);
            lcd.print("Access Denied!       ");
            lcd.setCursor(0, 3);
            lcd.print("                ");
            digitalWrite(RedlED, HIGH);
            delay(500);
            digitalWrite(RedlED, LOW);
        }
        else
        {
            myservo.write(90);
            lcd.setCursor(0, 2);
            lcd.print(" CARD: ");
            lcd.print(person.id);
            lcd.print("     ");
            lcd.setCursor(0, 3);
            lcd.print(" Date: ");
            lcd.print(person.date);
            lcd.print("     ");
            digitalWrite(GreenlED, HIGH);
            delay(500);
            digitalWrite(GreenlED, LOW);
            myservo.write(0);
        }
        
    }
    else
    {
        
    }
    
}
void loop() {
    id = "";
    readFromRFID();
    struct Data person = validate_person();
    action(person);
    delay(500);
}
