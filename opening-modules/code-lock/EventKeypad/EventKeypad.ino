#include <GateOpenerCommunicator.h>
#include <Keypad.h>


#define MASTERADDRESS 1
#define CODELOCKADDRESS 2
#define NETWORKADDRESS 0
#define FREQUENCY     RF69_868MHZ
#define ENCRYPTKEY    "TOPSECRETPASSWRD" 
#define USEACK        true // Request ACKs or not

#define COMMUNICATION 0
#define KEYPAD 1

const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = 
{
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowsPin[rows] = {7, 8, 9, A0};
byte colsPin[cols] = {3, 4, 5, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowsPin, colsPin, rows, cols); 

GateOpenerCommunicator communicator = GateOpenerCommunicator(FREQUENCY, CODELOCKADDRESS, NETWORKADDRESS, ENCRYPTKEY);

// code
const byte maxCodeLength = 8;
char readCode[maxCodeLength];
byte pos;

// piezo pin 
byte piezo = A1;

byte state;


void setup(){
    Serial.begin(9600);
    keypad.addEventListener(keypadEvent);
    resetCodePos();
    pinMode(piezo, OUTPUT);
    state = KEYPAD;
}

void loop()
{
  if(state == KEYPAD)
  {
     keypad.getKey(); 
  }
  else if (state == COMMUNICATION)
  {
    
  }
}

void keypadEvent(KeypadEvent key)
{
  if(keypad.getState()==PRESSED)
  {
    if(key=='#')
    {
      doublePip();
      sendCodeToMaster();  
    }
    else
    {
      pip();     
      if (pos<maxCodeLength)
      {
        readCode[pos] = key;
        pos++;
      }
      else
      {
        resetCode();
      }
    }
  }
}

void sendCodeToMaster()
{
  state = COMMUNICATION;
  CodeMsg msg = CodeMsg(readCode, maxCodeLength);
  printCodeMsg(msg);
  
  boolean ok = communicator.send(MASTERADDRESS, msg);
  printDeliveryStatus(ok);
  resetCodePos();
}

void printCodeMsg(CodeMsg codeMsg)
{
  Serial.print("C ");
  for(byte i = 0; i < codeMsg.CodeLength; i++)
  {
    Serial.print((char)codeMsg.Code[i]);
  }
  Serial.print("(");
  Serial.print(codeMsg.CodeLength);
  Serial.print(") ");
  Serial.print("[");
  Serial.print(communicator.SenderId);
  Serial.println("]");
}

void printDeliveryStatus(boolean ok)
{
    if (ok)
    {
      Serial.println("Delivered");
    }
    else
    {
      Serial.println("Not delivered");
    }  
}

void pip()
{
  tone(piezo, 1000, 100); 
}

void doublePip()
{
  tone(piezo, 1200, 200);
  delay(250);
  tone(piezo, 1200, 200);
}

void resetCode()
{
  for(byte i=0;i<maxCodeLength;i++)
  {
    readCode[i]=0;  
  }
}

void resetPos()
{
  pos = 0;  
}

void resetCodePos()
{
  resetCode();
  resetPos();
}
