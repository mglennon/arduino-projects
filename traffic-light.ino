/*

*/

const int pinRadioPwr = 13;
const int pinRadio0 = 12;
const int pinRadio1 = 11;
const int pinRadio2 = 10;
const int pinRadio3 = 9;
const int pinRadioA = 8;

const int pinRelayR = 7;
const int pinRelayY = 6;
const int pinRelayG = 5;

const int pinHeart = 4;

int lastMode = 8;
int lastStep = 0;
const int maxStep = 14;
// RYG
const byte modes[16][15] =
            {
              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Never used
              {0b001, 0b001, 0b001, 0b001, 0b001, 0b010, 0b010, 0b010, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100}, // Mode 1  - Normal
              {0b001, 0b001, 0b001, 0b001, 0b001, 0b010, 0b010, 0b100, 0b100, 0b100, 0b100, 0b100, 0b010, 0b010}, // Mode 2  - Europe
              {0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100}, // Mode 3  - Steady Red
              {0b001, 0b011, 0b111, 0b110, 0b100, 0b000, 0b100, 0b110, 0b111, 0b011, 0b001, 0b000, 0b000, 0b000}, // Mode 4  - Cascade
              {0b010, 0b000, 0b010, 0b000, 0b010, 0b000, 0b010, 0b000, 0b010, 0b000, 0b010, 0b000, 0b010, 0b000}, // Mode 5  - Yellow Flash
              {0b100, 0b000, 0b100, 0b000, 0b100, 0b000, 0b100, 0b000, 0b100, 0b000, 0b100, 0b000, 0b100, 0b000}, // Mode 6  - Red Flash
              {0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000}, // Mode 7
              {0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000}, // Mode 8  - OFF
              {0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001}, // Mode 9  - Steady Green
              {0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000}, // Mode 10 - OVERRIDDEN!
              {0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000}, // Mode 11
              {0b001, 0b000, 0b001, 0b000, 0b001, 0b000, 0b001, 0b000, 0b001, 0b000, 0b001, 0b000, 0b001, 0b000}, // Mode 12 - Green Flash
              {0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000}, // Mode 13
              {0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000}, // Mode 14
              {0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000}  // Mode 15
              
            };
void setup()
{
    delay(5000);
    Serial.begin(9600);
    Serial.print("Hello world!\n");
    pinMode(pinRadioPwr,OUTPUT);
    pinMode(pinRadio0, INPUT);
    pinMode(pinRadio1, INPUT);
    pinMode(pinRadio2, INPUT);
    pinMode(pinRadio3, INPUT);
    pinMode(pinRadioA, INPUT);
    
    pinMode(pinRelayR,OUTPUT);
    pinMode(pinRelayY,OUTPUT);
    pinMode(pinRelayG,OUTPUT);
    pinMode(pinHeart,OUTPUT);
    
    changeLights(0b111);
    delay(2000);
    changeLights(0b000);
}

void loop()
{
    digitalWrite(pinRadioPwr, HIGH);
    int a = ActiveMode();
    byte v = modes[a][lastStep];
    if(a==10) v = RaveMode();
    Serial.print(a);
    Serial.print("-");
    Serial.print(lastStep);
    Serial.print(": ");
    //Serial.print("\n");
    
    changeLights(v);
    int flip = lastStep % 2;
    digitalWrite(pinHeart,flip);
    lastStep++;
    if(lastStep==maxStep) lastStep=0;
    
    delay(1000);
}

byte RaveMode()
{
  byte v = 0b000;
  bitWrite(v,0,random(1));
  bitWrite(v,1,random(1));
  bitWrite(v,2,random(1));
  return v;
}

int ActiveMode()
{
    bool p0 = digitalRead(pinRadio0);
    bool p1 = digitalRead(pinRadio1);
    bool p2 = digitalRead(pinRadio2);
    bool p3 = digitalRead(pinRadio3);
    bool pA = digitalRead(pinRadioA);
    if(pA == 1)
    {
      lastMode = (p0*8)+(p1*4)+(p2*2)+(p3*1);
      digitalWrite(pinRadioPwr, LOW);
      Serial.print("Chaging Mode: ");
      Serial.print(lastMode);
      Serial.print("\n");
      changeLights(0b000);
      lastStep = 0;
      digitalWrite(pinHeart, HIGH);
      delay(2000);
    }
    return lastMode;
}

void changeLights(byte data)
{
    int r = bitRead(data, 2);
    int y = bitRead(data, 1);
    int g = bitRead(data, 0);
    Serial.print(r);
    Serial.print(y);
    Serial.print(g);
    Serial.print("\n");
    digitalWrite(pinRelayR, !r);
    digitalWrite(pinRelayY, !y);
    digitalWrite(pinRelayG, !g);
}
