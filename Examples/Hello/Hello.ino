
#include "loogl.h"

#define SCREEN_WIDTH  TFT_HEIGHT
#define SCREEN_HEIGHT TFT_WIDTH

LooglApp app(TFT_WIDTH, TFT_HEIGHT, nullptr, 1);
LooglScreen screen(&app, SCREEN_WIDTH, SCREEN_HEIGHT);
LooglWidgetText* text=nullptr;
int i=0;
// -------------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------------
void setup() {
  screen.setFrameBg(false, true, 0, TFT_GOLD, 5);
  text = new LooglWidgetText(&screen, 50, 50, "Hello World", 2, 2, TFT_WHITE);

  app.setScreen(&screen);
  app.init(3);
}

// -------------------------------------------------------------------------
// Main loop
// -------------------------------------------------------------------------
void loop()
{ 
  if(i++%2) { text->setText("Hello World");}
  else { text->setText("World Hello"); }
    
  app.update();
}
