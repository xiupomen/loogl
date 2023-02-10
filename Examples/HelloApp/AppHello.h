#ifndef APPHELLO_H
#define APPHELLO_H

#include "loogl.h"

#define SCREEN_WIDTH  TFT_HEIGHT
#define SCREEN_HEIGHT TFT_WIDTH

class AppHello;

class HelloScreen: public LooglScreen {
  public:
    HelloScreen(LooglApp* pApp):LooglScreen(pApp, SCREEN_WIDTH, SCREEN_HEIGHT) {
      m_text = new LooglWidgetText(this, 10, 10, "Hello World", 1, 2, TFT_WHITE);
      m_text1 = new LooglWidgetText(this, 60, 40, "Hi there", 1, 2, TFT_GOLD);
      m_text2 = new LooglWidgetText(this, 120, 70, "Are U OK", 2, 2, TFT_GREEN);
      m_text3 = new LooglWidgetText(this, 160, 110, "ha ha ha ~~~", 1, 2, TFT_SILVER);
      // if screen is to big, could fail to create sprite due to memory limitation.
      setSprite(true);
      setFrameBg(false, true, 0, TFT_RED, 10);
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
          // return true;
         break;

         case LOOGL_KEY_DOWN:
        //  return true;
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

class AppHello: public LooglApp {
  public:
    // for app launcher
    AppHello(TFT_eSPI& tft):LooglApp(tft, &m_screen), m_screen(this) { m_rotation=3; }
    AppHello():LooglApp(TFT_WIDTH, TFT_HEIGHT, &m_screen), m_screen(this) { m_rotation=3; }
  protected:
    // do init jobs in doInit(), which are supposed to do in setup()
    // virtual void doInit() {}

  private:
    HelloScreen m_screen;
};

// for launcher
class AppHelloFactory: public LooglAppFactory {
  public:
    virtual String getName() { return "Hello App"; }
    virtual LooglApp* createApp(TFT_eSPI& tft) { return new AppHello(tft); }
    
};

#endif
