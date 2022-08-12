#define pirPin 2 //Hareket sensörü pin'i
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Keypad.h>

const byte rows = 4;// keypad'in satir sayisi
const byte cols = 3;// keypad'in sutun sayisi

char key[rows][cols] = { //Keypad dizisi
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[rows] = {8, 9, 10, 11}; //Keypad satir pinlerini tanitma
byte colPins[cols] = {3, 4, 5}; //Keypad sutun pinlerini tanitma
Keypad keypad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);
const String password = "1903"; //Sifremiz

int red = 31; // Kirmizi isigin bagli oldugu pin
int green = 30; // Yesil isigin bagli oldugu pin

LiquidCrystal lcd(27, 26, 25, 24, 23, 22); // LCD ekranın bagli oldugu pinler
float temp;

boolean lockLow = true;
boolean takeLowTime;
int PIRValue = 0;

const int buzzerPin = 7;//BUZZER'IN bagli oldugu pin
const int flamePin = 6; // Yangin sensorunun bagli oldugu pin
int Flame = HIGH;

int pinTemp = A0;//Derecenin bagli oldugu pin

void setup()//PRogram calistiginde bir kere calisan alan
{
  Serial.begin(9600);//Serial.begin() establishes serial communication between your Arduino board and another device
  pinMode(pirPin, INPUT);//Sensor pinini tanitma
  pinMode(buzzerPin, OUTPUT);//buzzer pinini tanitma
  pinMode(flamePin, INPUT);//yangın sensoru pinini tanitma
  pinMode(30, OUTPUT);//yesil ısık pinini tanitma
  pinMode(31, OUTPUT);//kirmizi isik pinini tanitma
  pinMode(32, OUTPUT);//Hareket sensoru isigi pini tanıtma
  analogReference(INTERNAL1V1);
}

void loop()//program calisirken sürekli tekrar eden alan
{
//KAPI KILIT SISTEMI
  char code1 = keypad.getKey(); //keypadden girdi alan fonksiyon
  Serial.print(code1); //visual terminalde girdiyi yazdır

  if (code1 != NO_KEY)//girdi girildiyse sifrenin devamını isteyen if
  {
    char code2 = keypad.waitForKey();// waitForKey bir girdi bekle fonksiyonu

    Serial.print(code2);//girdiyi yaz

    char code3 = keypad.waitForKey();

    Serial.print(code3);

    char code4 = keypad.waitForKey();

    Serial.print(code4);

    if (code1 == password[0] && code2 == password[1] && code3 == password[2] && code4 == password[3])//girdiler sifremizle eslesiyorsa
    {
      digitalWrite(30, HIGH);//yesil isigin bagli oldugu 30'daki lambayi yak
      delay(250);//bekle
      digitalWrite(30, LOW);//yanan lambayi sondur
    }
    else//girdiler eslesmiyorsa
    {
      digitalWrite(31, HIGH);//31'deki kirmizi lambayi yak
      delay(250);//bekle
      digitalWrite(31, LOW);//lambayi sondur
    }
    Serial.println();
  }

//YANGIN SENSORU
  Flame = digitalRead(flamePin);//flame pinini oku

  if (Flame == LOW)//okunan deger LOW'sa neden HIGH degil bilmiyorum
  {
    digitalWrite(buzzerPin, HIGH);//buzzer'ı calistir
  }
  else
  {
    noTone(buzzerPin);//buzzeri sustur
  }

//SICAKLIK OLCER
  temp = analogRead(pinTemp);//pinTemp'te tutulan pini oku
  temp=temp*1100/(1024*10);//degeri Celcius'a ceviren formul

  lcd.begin(2, 16);//lcd ekranini baslat
  if(temp<20)
  {
     lcd.print("Sicaklik dustu:");//lcd'ye sicaklik yazdir
  }
  else if(temp>30)
  {
      lcd.print("Sicaklik yukseldi:");//lcd'ye sicaklik yazdir
  }
  else
  {
     lcd.print("Sicaklik:");//lcd'ye sicaklik yazdir
  }
  lcd.setCursor(0, 1);//imleci alta getir
  lcd.print(temp);//Celciusa çevrilen degeri lcd ekrana yazdır
  lcd.println(" C"); //Celciusu belirtmek icin C yazdir
 
  PIRSensor();//Hareket sensoru fonksiyonu
  
  delay(250);//bekle
  lcd.clear();//lcd'i temizle
}

void PIRSensor()//HAREKET SENSORU
{
  if (digitalRead(pirPin) == HIGH)//pinPin hareket sensorunun bagli oldugu pini oku ve HIGH ise
  {
    if(lockLow)//lockLow global olarak tanimli true bir deger buraya girdigi zaman false oluyor ve buraya bir daha girmiyiyor
    {
      PIRValue = 1;
      lockLow = false;
      digitalWrite(32, HIGH);//Lamba yak
      delay(250);
      digitalWrite(32, LOW);//Lambayi sondur
    }
    takeLowTime = true;
  }
  if (digitalRead(pirPin) == LOW)
  {
      PIRValue = 0;
      lockLow = true;//hareket bitince lockLow degerini tekrar true yal
  }
  
}
