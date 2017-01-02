#include <SPI.h>
#include <RFM69.h>

#define MASTERADDRESS 3
  #define CODELOCKADDRESS 2
  #define NETWORKADDRESS 0
  #define FREQUENCY     RF69_868MHZ
  #define ENCRYPTKEY    "TOPSECRETPASSWRD" 
  #define USEACK        true // Request ACKs or not
  RFM69 radio;


/******P I N S********/
const byte relay1 = 11;
const byte relay2 = 3;

/*******C O D E******/ 
const byte maxCodeLength = 8;
const char emptySymbol = 'x';
          
char readCode[maxCodeLength];
char code[maxCodeLength] = {'0','9','0','5','1','9','9','7'};
/**V A R I A B L E**/
boolean check = true;

void setup() 
{

  Serial.begin(9600);
  Serial.print("ArduinoMaster");
  radio.initialize(FREQUENCY, MASTERADDRESS, NETWORKADDRESS);
  radio.encrypt(ENCRYPTKEY);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  resetCode();
}

void loop() 
{
  if (radio.receiveDone())
  {
    Serial.print("message [");
    int premenna = 0;
    for (int i = 0; i < radio.DATALEN; i++)
    {
      readCode[i] = (char)radio.DATA[i];
      Serial.print((char)radio.DATA[i]);
    }
    Serial.print("], RSSI ");
    Serial.println(radio.RSSI);
    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.println("Prijata sprava");
      if (verifyCode())grantAccess(); 
    }
  }
}

void resetCode()
{
  for(byte i=0;i<maxCodeLength;i++)
  {
    readCode[i]=0;  
  }
}

boolean verifyCode()
{
  for(int i=0;i<maxCodeLength;i++)
  {
    if(readCode[i] != code[i])
    {
      Serial.println("ZLE");
      return false; 
    }
    else
    {
      Serial.println("OK");
    }
  }
resetCode();
return true;
}
void grantAccess()
{    
    digitalWrite(relay2, HIGH);
    delay(2000);
    digitalWrite(relay2, LOW);  
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
