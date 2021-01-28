#include "FastLED.h"
#define NUM_LEDS 25  //nr de leduri
#define MIC A0  // pinul microfonului
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

char data;

int br = 255; //luminozitatea
int r = 255;  // rosu
int g = 255;  // galben
int b = 255;  // albastru

int tres = 620; //limita inferioara de unde reactioneaza la sunet
int tresu = 900; //limita inferioara de unde reactioneaza la sunet

int i,j,k,l; // variabile sa nu le mai initializez in foruri

// variabile pentru perlin noise
const float circleRadius = NUM_LEDS / PI;
const float angle = 2 * PI / NUM_LEDS;
float xOffsets[NUM_LEDS];
float yOffsets[NUM_LEDS];

int speedH = 2;   //Hue value is 16-bit
int scaleH = 50;
int speedS = 4;
int scaleS = 30;
int speedV = 5;
int scaleV = 50;

float colorStart = 0;     
float colorRange = .7;  //Range of each section of color 1 = 100%
float colorSpeed = .1;    //Speed of color cycling
int colorMax = 255;

String mode = "none"; // options 1: normal, 2: complementary, 3: triad


float x, y, zH, zS, zV;

uint8_t colorLoop = 1;






void func3(int Bright, int red, int green,int blue)//animatie care urmareste coada cu valorile de rgb globale
{
  FastLED.setBrightness(Bright);
  for(int i=0;i<NUM_LEDS;i++){
  leds[i] = CRGB(red,green,blue); FastLED.show(); delay(30);

  }
   for(int i=0;i<NUM_LEDS;i++){
 
  leds[i] = CRGB::Black; FastLED.show(); delay(30);
  }
}

void func(int Bright, int red, int green,int blue)// animatie reactiva care foloseste valorile globale de rgb 
{
        FastLED.setBrightness(Bright);
        int val = analogRead(MIC);
        int numLedsToLight = map(val, tres, tresu, 0, NUM_LEDS);

        // First, clear the existing led values
        FastLED.clear();
        for(int led = 0; led < numLedsToLight; led++) { 
            leds[led] = CRGB(red,green,blue); 
        }
        FastLED.show();
        //delay(60);
        }

        
void plain(int Bright, int red, int green,int blue) //aprinde ledurile
{
  FastLED.setBrightness(Bright);
  for(int i=0;i<NUM_LEDS;i++){
       leds[i] = CRGB(red,green,blue); 
       FastLED.show();
  }
  }

void Palette()// epilepsie
{
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV( random8(), 255, random8());
    FastLED.show();
    
  }
}



void ledOff() //stinge luminile
{
for(int i=0;i<NUM_LEDS;i++){
        leds[i] = CRGB::Black; FastLED.show(); delay(30);
}
  }

void gradient() //gradient simplu
{
  for (int i = 0;i<=255;i++){
    for(int j = 0;j<=NUM_LEDS;j++){
      leds[j] = CHSV(i,255,255);
      FastLED.show();
      delay(20);
    }
  }
  }

void expe() // isi schimba culoarea in functie de microfon
{
        int val = analogRead(MIC);
        FastLED.clear();
        for(int led = 0; led < NUM_LEDS; led++) { 
            leds[led] = CHSV(255,val%255,255); 
        }
        FastLED.show();
        //delay(60);
  
}
void expe1(){
        int val = analogRead(MIC);
        FastLED.clear();
        for(int led = 0; led < NUM_LEDS; led++) { 
            leds[led] = CHSV(val%255,255,255); 
        }
        FastLED.show();
       // delay(60);
  
}
void expe2()  
{
        int val = analogRead(MIC);
        FastLED.clear();
        for(int led = 0; led < NUM_LEDS; led++) { 
            leds[led] = CHSV(255,255,val%255); 
        }
        FastLED.show();
       // delay(60);
  
}

void fillnoise() // perlin noise
{

  for (int i = 0; i < NUM_LEDS; i++)
  {
    int noiseValH = inoise8(x + xOffsets[i] * scaleH, y + yOffsets[i] * scaleH, zH);
    int noiseValS = inoise8(x + xOffsets[i] * scaleS, y + yOffsets[i] * scaleS, zS);
    int noiseValV = inoise8(x + xOffsets[i] * scaleV, y + yOffsets[i] * scaleS, zV);

    int hue = (int) (noiseValH * colorRange + colorStart);
    if (mode=="TRIAD"){
      if (noiseValH > colorMax * 5 / 8) {
        hue = hue + colorMax * 2 / 3 - colorMax * colorRange * 5 / 8;
      } else if (noiseValH > colorMax * 3 / 8) {
        hue = hue + colorMax / 3 - colorMax * colorRange * 3 / 8;
      }
    }
    if (mode=="COMPLEMENTARY") {
      if (noiseValH > colorMax / 2) {
        hue = hue + colorMax / 2 - colorMax * colorRange / 2;
      } 
    }
    hue = hue % colorMax;
    
    int saturation = constrain(noiseValS + 70, 0, 255);
    int value = constrain(noiseValV - 20, 0, 255);
    
    leds[i] = CHSV(hue, saturation, value);
  }

  zH += speedH;
  zS += speedS;
  zV += speedV;

  // apply slow drift to X and Y, just for visual variation.
  x += speedS / 8;
  y -= speedS / 16;
  
  colorStart += colorSpeed;
  if (colorStart > colorMax) { colorStart -= colorMax; }
  LEDS.show();
  delay(50);
  
}

void noisev()  //perlin noise reactiv
{
   for (int i = 0; i < NUM_LEDS; i++)
  {
    int noiseValH = inoise8(x + xOffsets[i] * scaleH, y + yOffsets[i] * scaleH, zH);
    int noiseValS = inoise8(x + xOffsets[i] * scaleS, y + yOffsets[i] * scaleS, zS);
    int noiseValV = inoise8(x + xOffsets[i] * scaleV, y + yOffsets[i] * scaleS, zV);

    int hue = (int) (noiseValH * colorRange + colorStart);
    if (mode=="TRIAD"){
      if (noiseValH > colorMax * 5 / 8) {
        hue = hue + colorMax * 2 / 3 - colorMax * colorRange * 5 / 8;
      } else if (noiseValH > colorMax * 3 / 8) {
        hue = hue + colorMax / 3 - colorMax * colorRange * 3 / 8;
      }
    }
    if (mode=="COMPLEMENTARY") {
      if (noiseValH > colorMax / 2) {
        hue = hue + colorMax / 2 - colorMax * colorRange / 2;
      } 
    }
    hue = hue % colorMax;
    
    int saturation = constrain(noiseValS + 70, 0, 255);
    int value = constrain(noiseValV - 20, 0, 255);
    
     int val = analogRead(MIC);
     int numLedsToLight = map(val, tres, tresu, 0, NUM_LEDS);

        // First, clear the existing led values
        FastLED.clear();
     for(int led = 0; led < numLedsToLight; led++) { 
            leds[led] = CHSV(hue, saturation, value); 
        }
  }

  zH += speedH;
  zS += speedS;
  zV += speedV;

  // apply slow drift to X and Y, just for visual variation.
  x += speedS / 8;
  y -= speedS / 16;
  
  colorStart += colorSpeed;
  if (colorStart > colorMax) { colorStart -= colorMax; }
  LEDS.show();
  delay(40);
  
  }






// SKETCH 





  

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS); 
  LEDS.addLeds<LED_TYPE,6,COLOR_ORDER>(leds,NUM_LEDS);
  Serial.begin(9600);
  FastLED.clear();
  ledOff();

  x = 0.0;
  y = 0.0;
  zH = 100;
  zS = 50;
  zV = 0;
  
  for (int i = 0; i < NUM_LEDS; i++)
  {
    float thisAngle = angle * i;
    float xoffset = cos(thisAngle) * circleRadius;
    float yoffset = sin(thisAngle) * circleRadius;
    xOffsets[i] = xoffset;
    yOffsets[i] = yoffset;
  } 
}


void loop() {

 //noisev();
  // aici poti sa schimbi valoriile de RGB prin bluetooth
  
  if(Serial.available() > 0)  
  {
    data = Serial.read(); //citeste un char sii are mai multe situatii
  }
  
  if(data == 'l'){
         if(Serial.available() > 0)  
        {
         br = Serial.parseInt();
        }
    }
  if(data == 'r'){
         if(Serial.available() > 0)  
        {
         r = Serial.parseInt();
        }
    }
  if(data == 'g'){
         if(Serial.available() > 0)  
        {
         g = Serial.parseInt();
        }
    }
  if(data == 'b'){
         if(Serial.available() > 0)  
        {
         b = Serial.parseInt();
        }
    }


  // animatiile disponibile
  
  if(data == 'q'){
      Palette();
    }
  if(data == 'w'){
    //  ledOff();
      func(br,r,g,b);
  }
  
  if(data == 'e'){
  //  ledOff();
    plain(br,r,g,b);
  }
  if(data == 'a'){
    ledOff();
  }
  if(data == 's'){
    func3(br,r,g,b);
  }
  if(data == 'd'){
    gradient();
  }
  if(data == 'f'){
    expe();
  }
  if(data == 'z'){
    expe1();
  }
  if(data == 'x'){
    expe2();
  }
  if(data == 'c'){
    fillnoise();
  }
  if(data == 'v'){
    noisev();
  }
    
  }
