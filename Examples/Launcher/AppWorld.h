#ifndef APPWORLD_H
#define APPWORLD_H

#include "loogl.h"

#define SCREEN_WIDTH  TFT_HEIGHT
#define SCREEN_HEIGHT TFT_WIDTH

class WorldScreen: public LooglScreen {
  public:
    WorldScreen(LooglApp* pApp):LooglScreen(pApp, SCREEN_WIDTH, SCREEN_HEIGHT) {
      m_text = new LooglWidgetText(this, 10, 10, "Hello World", 1, 2, TFT_WHITE);
      m_text1 = new LooglWidgetText(this, 60, 40, "This is a demo app", 1, 2, TFT_GOLD);
      m_text2 = new LooglWidgetText(this, 120, 70, "to show the app framework", 2, 2, TFT_GREEN);
      m_text3 = new LooglWidgetText(this, 160, 110, "with a launcher", 1, 2, TFT_RED);
      // could fail to create sprite due to memory limitation.
      setSprite(true);
      setFrameBg(false, true, 0, TFT_NAVY, 15);
    }

  protected:
    virtual bool doUpdate() {
          move(m_text, 1);
          move(m_text1, 2);
          move(m_text2, 3);
          move(m_text3, 2);
          return true;
       }

    virtual bool onKey(int key) {
       switch(key) {
         case LOOGL_KEY_UP:
         break;

         case LOOGL_KEY_DOWN:
         break;

         case LOOGL_KEY_BACK:
            // getApp<AppHello>()->exit();
            getApp()->exit();
            return true;
         break;

         default:
         break;
      }
      return false;
    }

    void move(LooglWidgetBase* w, int s){
        w->move(-s, 0);
        if(w->getX()<-w->getWidth()) w->setX(SCREEN_WIDTH);
    }

    LooglWidgetText* m_text;
    LooglWidgetText* m_text1;
    LooglWidgetText* m_text2;
    LooglWidgetText* m_text3;
};

class AppWorld: public LooglApp {
  public:
    // for app launcher
    AppWorld(TFT_eSPI& tft):LooglApp(tft, &m_screen), m_screen(this) { m_rotation=3; }
    AppWorld():LooglApp(TFT_WIDTH, TFT_HEIGHT, &m_screen), m_screen(this) { }

  private:
    WorldScreen m_screen;
};

// for launcher
class AppWorldFactory: public LooglAppFactory {
  public:
    virtual String getName() { return "World App"; }
    virtual LooglApp* createApp(TFT_eSPI& tft) { return new AppWorld(tft); }
    
};

#endif
