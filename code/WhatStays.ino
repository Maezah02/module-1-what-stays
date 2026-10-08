
/*
 WHAT STAYS, a generative art representing relationships. 
*/

#include "TFT_eSPI.h"

TFT_eSPI tft= TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

//size of the screen
int screenH;
int screenW;

// the white dot position
float meX;
float meY;
float destinationMX;
float destinationMY;


//A color dot struct 
struct Dot {
  float x;
  float y;
  float destinationX;
  float destinationY;

  bool active;

  float frozenX;
  float frozenY;

  uint16_t color;
};

//choosing how many color dots you want to have
const int DOT_NUM = 15;
Dot dots[DOT_NUM];



void setup() {

tft.init();
tft.setRotation(1);

screenH = tft.height();
screenW = tft.width();

//create the invisible canvas
sprite.createSprite(screenW, screenH);

//The white dot starting positions
  meX =screenW /2;
  meY =screenH /2;
  destinationMX = random(5, screenW - 5);
  destinationMY = random(5, screenH - 5);

    //Generating the color dots in random positions, colors, and movements
   for (int i = 0 ; i < DOT_NUM ; i++){

    //random position generation
    dots[i].x = random(5, screenW - 5);
    dots[i].y = random(5, screenH - 5);

    //random destination position generation
    dots[i].destinationX = random(5, screenW - 5);
    dots[i].destinationY = random(5, screenH - 5);


    /* all color dots initially start as active dots, meaning 
    that they all appear on the screen */ 
    dots[i].active = true;

     //random color generation
    dots[i].color = tft.color565(random(80,180), random(80,180),  random(80,180));


   } 

}

void loop() {
  // coloring the canvas black
  sprite.fillSprite(TFT_BLACK);


   //speed of the dots movement
   float speed = 0.01;

   //moving the white dot toward its destination
   meX += (destinationMX -   meX)*speed;
   meY += (destinationMY -   meY)*speed;

   //calculating the distance between the white dot and its destination
   float distanceMX= fabs(destinationMX -   meX);
   float distanceMY= fabs(destinationMY -   meY);

   //choosing a new random destination when the white dot reaches its destination
   if (distanceMX < 2 && distanceMY < 2 ){
    destinationMX = random(5, screenW - 5);
    destinationMY = random(5, screenH - 5);
   }
   
   

   // Draw all the colored dots
   
  for (int i = 0; i < DOT_NUM; i++) {

    //moving each color dot toward its destination
    dots[i].x += (dots[i].destinationX -  dots[i].x)*speed;
    dots[i].y += (dots[i].destinationY -  dots[i].y)*speed;
    


    //calculating the distance between the color dot and its destination
    float distanceX= fabs(dots[i].destinationX -  dots[i].x);
    float distanceY= fabs(dots[i].destinationY -  dots[i].y);
    
    
    //choosing a new random destination when the color dot reaches its destination
    if (distanceX < 2 && distanceY < 2 ){
      dots[i].destinationX = random(5, screenW - 5);
      dots[i].destinationY = random(5, screenH - 5);    

    }

    //drawing a line between the white dot and the active color dots
    if (dots[i].active ){
      sprite.drawLine(meX, meY, dots[i].x, dots[i].y, TFT_WHITE);

    }

    //keeping the line connected to the last position of a disappeared dot
    if(!dots[i].active ){
    sprite.drawLine(meX, meY, dots[i].frozenX, dots[i].frozenY, TFT_WHITE);
    }



    //drawing the color dots that are still active
    if(dots[i].active){
    sprite.fillCircle( dots[i].x,  dots[i].y,  random(4,6), dots[i].color );
    }

      

  }

   //disappearing 
    //randomly choosing a color dot to disappear
    if (random(250) == 0) {
    int randomDot = random(DOT_NUM);
    if (dots[randomDot].active){
    //saving the last position of the dot before it disappears
    dots[randomDot].frozenX = dots[randomDot].x;
    dots[randomDot].frozenY = dots[randomDot].y;
    dots[randomDot].active = false;
    }
    }

    //randomly choosing a disappeared dot to appear again
    if (random(250) == 0) {
    int randomDot = random(DOT_NUM);
    if (!dots[randomDot].active){
      //bringing the dot back to its last position
      dots[randomDot].x = dots[randomDot].frozenX;
      dots[randomDot].y = dots[randomDot].frozenY;
      dots[randomDot].active = true;
    }
    
    }
   
    //drawing the white dot
    sprite.fillCircle( meX, meY, random(4,6), TFT_WHITE);

    //displaying the canvas on the screen
    sprite.pushSprite(0,0);

    //small delay before drawing the next frame
    delay(20);

   
   
}
