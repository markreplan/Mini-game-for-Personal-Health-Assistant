#include <SimpleTimer.h>

#include <Adafruit_GFX.h> //Core Graphics Library
#include <SPI.h>          // Display library
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h> //Wire and FT6206 are partners

Adafruit_FT6206 ctp = Adafruit_FT6206();

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

//results variable holders
int wins = 0;
int result_x = 0; int result_y = 0; //Holds the values of the spot of last dot is


void drawGrid(int top, int bot)
{
  while (top % 80 == 0 and bot % 80 == 0)
  {
    tft.drawLine(top, 0, top, tft.height(), CYAN);
    top = top - 80;
    tft.drawLine(0, bot, tft.width(), bot, CYAN);
    bot = bot - 80;
  }
}

void drawMenu()
{
  tft.fillScreen(BLACK);
  tft.setCursor(30,60); tft.setTextSize(1); tft.println("Touch the box the magenta circle was last on!");
  delay(750);
  tft.setCursor(30,140); tft.setTextSize(2); tft.println("By the way... did I meantion");
  delay(750);
  tft.setCursor(30,200); tft.setTextSize(2); tft.println("this is timed? Watch the light!");
  
  
}

void drawDots(int counts, int delaytime)
{
  int t_nums[] = {40,120,200,280};
  int b_nums[] = {40,120,200};
  randomSeed(analogRead(0));
  for(int i = 0; i < counts; i++)
  {
    int randNumX = int(random(0,4)); int randNumY = int(random(0,3));
    tft.fillCircle(t_nums[randNumX], b_nums[randNumY], 35, MAGENTA);
    delay(0);
    tft.fillCircle(t_nums[randNumX], b_nums[randNumY], 35, BLACK);
    result_x = t_nums[randNumX]; result_y = t_nums[randNumY];
  }
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.begin();
  SimpleTimer timer;
  pinMode(13, OUTPUT);
  tft.setRotation(3);
  drawMenu();
  delay(3500);
  tft.fillScreen(BLACK);
  drawGrid(320,240);
  delay(1000);
  drawDots(10,350); 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int endtime = 0;
  ts.begin();
  for(int sec = 1; sec < 32; sec++)
  {
    digitalWrite(13,HIGH);
    delay(600 - (110*sqrt(sec)));
    digitalWrite(13,LOW);
    delay(600 - (110*sqrt(sec)));
    if(ts.touched())    {
      TS_Point p = ts.getPoint();
      if(p.x > result_x-40 && p.x < result_x +40 && p.y > result_y-40 && p.y < result_y+40)
      {
        tft.fillCircle(result_x,result_y,35,GREEN);
        endtime = sec;
        wins += 1;
        delay(3000);
        tft.fillScreen(BLACK);
        break;
      }
      else
      {
        tft.fillCircle(result_x,result_y,35,RED);
        delay(5000);
        tft.fillScreen(BLACK);
        break;
      }
      endtime = sec;
    } 
  }
  if(wins == 1)
  {
    Serial.write(endtime+"");
  }
  else
  {
    Serial.write('FAILED');
  }
  
}
