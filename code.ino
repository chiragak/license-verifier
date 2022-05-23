#include<LiquidCrystal.h>
LiquidCrystal lcd(13,12,11,10,9,8);
#include <SoftwareSerial.h>
SoftwareSerial fingerPrint(2, 3);
//#include<SoftwareSerial.h>                                                                                              

//SoftwareSerial mySerial(6,7);

#include <Adafruit_Fingerprint.h>
uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);
#define enroll 14     //A0
#define del 15        //A1
#define up 16         //A2
#define down 17       //A3

int RFID_ID=0;
int c;
int i=0;
 char a[12];
 char data1[]="0004932370"; 
 char data2[]="0004936689";
 char data3[]="0004934512";
 char data4[]="0004935266";

 
void setup() 
{
  delay(1000);
  
    pinMode(enroll, INPUT_PULLUP);
    pinMode(up, INPUT_PULLUP); 
    pinMode(down, INPUT_PULLUP);  
    pinMode(del, INPUT_PULLUP); 
    
    lcd.begin(16,2);

    finger.begin(57600);
    Serial.begin(9600);
   // mySerial.begin(9600);
    lcd.clear();
    lcd.print("Finding Module");
    lcd.setCursor(0,1);
    delay(3000);
    if (finger.verifyPassword()) 
    {
      Serial.println("Found fingerprint sensor!");
      lcd.clear();
      lcd.print("Found Module ");
      delay(3000);
    } 
    else 
    {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.print("module not Found");
    lcd.setCursor(0,1);
    lcd.print("Check Connections");
    while (1);
    }
}

void loop() 
{
 lcd.setCursor(0,0);
 lcd.print("SWIPE THE CARD");
// lcd.setCursor(0,1);
// lcd.print("  CARD");


 i=0;
 RFID_ID=0;
  while(i<10)
  {
    if(Serial.available()){
    int var=Serial.read();
    if((var>47)&&(var<60))
    {
    a[i]=var;
    i++;
    }
    }
     checkKeys();
  }
  
    Serial.print(" DATA:"); 
  Serial.println(a);
   c=strncmp(a,data1,10);
  if(c==0)
  {
    Serial.println("1");
    delay(1000);
    RFID_ID=1;
  }
 
 c=strncmp(a,data2,10);
  if(c==0)
  {
    Serial.println("2");
    delay(1000);
    RFID_ID=2;
  }
  c=strncmp(a,data3,10);
  if(c==0)
  {
    Serial.println("3");
    delay(1000);
  RFID_ID=3;
  }
   c=strncmp(a,data4,10);
  if(c==0)
  {
    Serial.println("4");
    delay(1000);
  RFID_ID=4;
  }

 
 if(RFID_ID!=0)
 {
  for(int i=0;i<5;i++)
  {
    lcd.clear();
    lcd.print("Place Finger");
    delay(3000);
    int result=getFingerprintIDez();
lcd.clear();
  lcd.print("ID:");
    lcd.print(result);
    if(RFID_ID==result)
    
  ///  if(result>=0)
  {
       
        lcd.clear();
        lcd.print("Allowed");
      delay(2000);
      return;
     }
     else
     {
         lcd.clear();
        lcd.print("invalid");
      delay(2000);
      return;
     }
   }
 }

 delay(1000);
}

void checkKeys()
{
   if(digitalRead(enroll) == 1)
   {
    lcd.clear();
    lcd.print("Please Wait");
    delay(100);
    while(digitalRead(enroll) == 1);
    Enroll();
   }

   else if(digitalRead(del) == 1)
   {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    delet();
   }  
}

void Enroll()
{
   int count=1;
   lcd.clear();
   lcd.print("Enroll Finger    ");

   while(1)
   {
     lcd.setCursor(9,1);
     lcd.print(count);
     if(digitalRead(up) == 1)
     {
       count++;
       if(count>9)
       count=0;
       delay(500);
     }

     else if(digitalRead(down) == 1)
     {
       count--;
       if(count<0)
       count=9;
       delay(500);
     }
     else if(digitalRead(del) == 1)
     {
          id=count;
          getFingerprintEnroll();
          return;
     }

       else if(digitalRead(enroll) == 1)
     {        
          return;
     }
 }
}

void delet()
{
   int count=0;
   lcd.clear();
   lcd.print("Delete Finger    ");
   
   while(1)
   {
     lcd.setCursor(9,1);
     lcd.print(count);
     if(digitalRead(up) == 1)
     {
       count++;
       if(count>9)
       count=0;
       delay(500);
     }

     else if(digitalRead(down) == 1)
     {
       count--;
       if(count<0)
       count=9;
       delay(500);
     }
     else if(digitalRead(del) == 1)
     {
          id=count;
          deleteFingerprint(id);
          return;
     }

       else if(digitalRead(enroll) == 1)
     {        
          return;
     }
 }
}

uint8_t getFingerprintEnroll() 
{
  int p = -1;
  lcd.clear();
  lcd.print("finger ID:");
  lcd.print(id);
  lcd.setCursor(0,1);
  lcd.print("Place Finger");
  delay(2000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No Finger");
      lcd.clear();
      lcd.print("No Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.print("Imaging Error");
      break;
    default:
      Serial.println("Unknown error");
       lcd.clear();
      lcd.print("Unknown Error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.clear();
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
       lcd.clear();
       lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.print("Unknown Error");
      return p;
  }
  
  Serial.println("Remove finger");
  lcd.clear();
  lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
   lcd.clear();
      lcd.print("Place Finger");
      lcd.setCursor(0,1);
      lcd.print("   Again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      return;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.print("Stored!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  }
  else {
    Serial.println("Unknown error");
    return p;
  }   
}

int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  
  if (p != FINGERPRINT_OK)  
  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
   lcd.clear();
   lcd.print("Finger Not Found");
   lcd.setCursor(0,1);
   lcd.print("Try Later");
   delay(2000);  
  return -1;
  }
  // found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  return finger.fingerID; 
}

uint8_t deleteFingerprint(uint8_t id) 
{
  uint8_t p = -1;  
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Deleted!");
    lcd.clear();
    lcd.print("Finger Deleted");
    lcd.setCursor(0,1);
    lcd.print("Successfully");
    delay(1000);
  } 
  
  else 
  {
    Serial.print("Something Wrong"); 
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0,1);
    lcd.print("Try Again Later");
    delay(2000);
    return p;
  }   
}
