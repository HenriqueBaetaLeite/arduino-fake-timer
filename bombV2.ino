#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Tone.h>

Tone tone1;

// CONFIG KEYPAD
const byte rows = 4;
const byte columns = 4;

char keys[rows][columns] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'},
};

byte rowPins[rows] = {11, 10, 9, 8};
byte columnsPins[columns] = {14, 15, 16, 17};

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, columnsPins, rows, columns);

// END CONFIG KEYPAD

// CONFIG LCD

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// END CONFIG LCD

// My Bomb Menu

const String welcomeMessage = "Welcome to the";
const String welcomeMessage2 = "bomb game defuse";

const String startMessage = "To start the game";
const String pressStart = "PRESS 'A'";

// BOMB CHAR

byte bombChar[] = {
    B00100,
    B01000,
    B00100,
    B01110,
    B11111,
    B11111,
    B11111,
    B01110};

//

char initialPassword[4];
int currentLength = 0;

int Scount = 12;     // count seconds
int Mcount = 10;     // count minutes
int Hcount = 0;      // count hours
int DefuseTimer = 0; // set timer to 0

long secMillis = 0;   // store last time for second add
long interval = 1000; // interval for seconds

char entered[4]; // password entered for the defuser

// FUNCÃO QUE INICIA COM MENSAGEM DE BOAS VINDAS E APERTE PARA COMEÇAR.

void startMenu()
{
  lcd.setCursor(1, 0);
  lcd.print(welcomeMessage);
  lcd.setCursor(0, 0);
  lcd.write((byte)0);
  lcd.setCursor(15, 0);
  lcd.write((byte)0);
  lcd.setCursor(0, 1);
  lcd.print(welcomeMessage2);
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print(pressStart);
  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  lcd.setCursor(7, 1);
  lcd.write((byte)0);
  lcd.setCursor(15, 1);
  lcd.write((byte)0);
  delay(3000);
  lcd.clear();
}

void newPassword(String password, char key)
{
  if (password.length() < 6)
  {
    password += key;
    Serial.println(password);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  tone1.begin(20);
  lcd.begin(16, 2);
  lcd.createChar(0, bombChar);
  startMenu();
  lcd.setCursor(0, 0);
  lcd.print("Enter code:");

  while (currentLength < 4)
  {
    char key = myKeypad.getKey();
    lcd.setCursor(currentLength + 3, 1);
    lcd.cursor();

    if (key != NO_KEY && key != '*' && key != '#')
    {
      lcd.print(key);
      initialPassword[currentLength] = key;
      currentLength++;
    }
  }

  if (currentLength == 4)
  {
    delay(500);
    lcd.noCursor();
    lcd.clear();
    lcd.home();
    lcd.print("You've Entered: ");
    lcd.setCursor(6, 1);
    lcd.print(initialPassword[0]);
    lcd.print(initialPassword[1]);
    lcd.print(initialPassword[2]);
    lcd.print(initialPassword[3]);

    //    tone1.play(NOTE_E6, 200);
    delay(3000);
    lcd.clear();

    lcd.print("Bomb armed!");
    currentLength = 0;
  }
}

void loop()
{
  // put your main code here, to run repeatedly:

  char theKey = myKeypad.getKey();
  timer();

  if (theKey == '#')
  {
    currentLength = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Code: ");
    Serial.println(currentLength);

    while (currentLength < 4)
    {

      timer();

      char key2 = myKeypad.getKey();
      if (key2 == '#')
      {
        currentLength = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Code: ");
      }
      else if (key2 != NO_KEY)
      {

        lcd.setCursor(currentLength + 7, 0);
        lcd.cursor();

        lcd.print(key2);
        entered[currentLength] = key2;
        currentLength++;
        tone1.play(NOTE_C6, 200);
        delay(100);
        lcd.noCursor();
        lcd.setCursor(currentLength + 6, 0);
        lcd.print("*");
        lcd.setCursor(currentLength + 7, 0);
        lcd.cursor();
      }
    }

    if (currentLength == 4)
    {
      if (entered[0] == initialPassword[0] && entered[1] == initialPassword[1] && entered[2] == initialPassword[2] && entered[3] == initialPassword[3])
      {
        lcd.noCursor();
        lcd.clear();
        lcd.home();
        lcd.print("Bomb Defused");
        currentLength = 0;
        //              digitalWrite(ledPin3, HIGH);
        delay(2500);
        lcd.setCursor(0, 1);
        lcd.print("Reset the Bomb");
        delay(1000000);
      }
      else
      {
        lcd.noCursor();
        lcd.clear();
        lcd.home();
        lcd.print("Wrong Password!");

        if (Hcount > 0)
        {
          Hcount = Hcount - 1;
        }

        if (Mcount > 0)
        {
          Mcount = Mcount - 59;
        }
        if (Scount > 0)
        {
          Scount = Scount - 59;
        }
        delay(1500);
        currentLength = 0;
      }
    }
  }

  //  if (theKey != NO_KEY) {
  //    Serial.println(theKey);
  //    lcd.clear();
  //    lcd.setCursor(4, 0);
  //    lcd.print(theKey);
  ////    newPassword(initialPassword, theKey);
  ////    lcd.blink();
  ////    newPassword(initialPassword);
  //  };
}

void timer()
{
  Serial.print(Scount);
  Serial.println();

  if (Hcount <= 0)
  {
    if (Mcount < 0)
    {
      lcd.noCursor();
      lcd.clear();
      lcd.home();
      lcd.print("The Bomb Has ");
      lcd.setCursor(0, 1);
      lcd.print("Exploded!");

      while (Mcount < 0)
      {
        //        digitalWrite(ledPin, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100);
        //        digitalWrite(ledPin, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100);
        //        digitalWrite(ledPin2, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100);
        //        digitalWrite(ledPin2, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100);
        //        digitalWrite(ledPin3, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100);
        //        digitalWrite(ledPin3, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100);
      }
    }
  }

  lcd.setCursor(0, 1); // sets cursor to 2nd line
  lcd.print("Timer:");

  if (Hcount >= 10)
  {
    lcd.setCursor(7, 1);
    lcd.print(Hcount);
  }
  if (Hcount < 10)
  {
    lcd.setCursor(7, 1);
    lcd.write("0");
    lcd.setCursor(8, 1);
    lcd.print(Hcount);
  }

  lcd.print(":");

  if (Mcount >= 10)
  {
    lcd.setCursor(10, 1);
    lcd.print(Mcount);
  }
  if (Mcount < 10)
  {
    lcd.setCursor(10, 1);
    lcd.write("0");
    lcd.setCursor(11, 1);
    lcd.print(Mcount);
  }

  lcd.print(":");

  if (Scount >= 10)
  {
    lcd.setCursor(13, 1);
    lcd.print(Scount);
  }
  if (Scount < 10)
  {
    lcd.setCursor(13, 1);
    lcd.write("0");
    lcd.setCursor(14, 1);
    lcd.print(Scount);
  }

  if (Hcount < 0)
  {
    Hcount = 0;
  }

  if (Mcount < 0)
  {
    Hcount--;
    Mcount = 59;
  }

  if (Scount < 1) // if 60 do this operation
  {
    Mcount--;    // add 1 to Mcount
    Scount = 59; // reset Scount
  }

  if (Scount > 0) // do this oper. 59 times
  {
    unsigned long currentMillis = millis();

    if (currentMillis - secMillis > interval)
    {
      tone1.play(NOTE_G5, 200);
      secMillis = currentMillis;
      Scount--;  // add 1 to Scount
                 //          digitalWrite(ledPin2, HIGH); // sets the LED on
      delay(10); // waits for a second
                 //          digitalWrite(ledPin2, LOW); // sets the LED off
      delay(10); // waits for a second
      // lcd.clear();
    }
  }
}
