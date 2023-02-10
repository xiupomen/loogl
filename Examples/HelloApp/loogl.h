/*******************************************
  LOOGL: Ligh Object-Oriented GUI Library.  (by xiupomen)
  This is a tiny GUI library based on TFT_eSPI library in order to create GUI easily in an object-oriented way.

  version 0.0.1
  last edit by xiupomen: 2023/02/10
*******************************************/

#ifndef LOOGL_H
#define LOOGL_H

#include <vector>
#include <TFT_eSPI.h>       // Include the graphics library

////////////////////////////////////////
// logging
#define LOOGL_LOG_LEVEL_INFO
// #define LOOGL_LOG_LEVEL_WARN
// #define LOOGL_LOG_LEVEL_ERRO

#ifdef LOOGL_LOG_LEVEL_INFO
  #define log_i(x) Serial.println(x)
  #define log_w(x) Serial.println(x)
  #define log_e(x) Serial.println(x)
#elif defined LOOGL_LOG_LEVEL_WARN
  #define log_i(x) 
  #define log_w(x) Serial.println(x)
  #define log_e(x) Serial.println(x)
#elif defined LOOGL_LOG_LEVEL_ERRO
  #define log_i(x) 
  #define log_w(x) 
  #define log_e(x) Serial.println(x)
#else
  #define log_i(x) 
  #define log_w(x) 
  #define log_e(x)
#endif

///////////////////////////////////////////////////

#define LOOGL_KEY_NONE  0
#define LOOGL_KEY_LEFT  1
#define LOOGL_KEY_RIGHT  2
#define LOOGL_KEY_UP  3
#define LOOGL_KEY_DOWN  4
#define LOOGL_KEY_ENTER  10
#define LOOGL_KEY_BACK  11
#define LOOGL_KEY_CUSTOM  100


#define LOOGL_ALIGH_H_LEFT 0
#define LOOGL_ALIGH_H_CENTER 1
#define LOOGL_ALIGH_H_RIGHT 2
#define LOOGL_ALIGH_V_TOP 0
#define LOOGL_ALIGH_V_CENTER 1
#define LOOGL_ALIGH_V_BOTTOM 2


typedef uint16_t loogl_color;

class LooglWidgetGroup;

// LOOGL: Ligh Object-Oriented GUI Library, by xiupomen
class LooglWidgetBase {
  public:
    LooglWidgetBase(LooglWidgetGroup* pParent, int32_t x=0, int32_t y=0, int32_t w=0, int32_t h=0);
    virtual ~LooglWidgetBase() { if(m_pTftSpr) delete m_pTftSpr; }

    void setX(int32_t x);

    void setY(int32_t y);

    void setPosition(int32_t x, int32_t y);
    void move(int32_t offset_x, int32_t offset_y);
    
    void setWidth(int32_t w);

    void setHeight(int32_t h);

    void setSize(int32_t w, int32_t h);
    void setAlign(int alignH, int alighV);
    void setVisible(bool v) { m_visible = v; }
    void setFrameBg(bool frame, bool fill, loogl_color frameColor=0, loogl_color bgColor=0, int32_t radius=0) {
                                             m_frame = frame; m_frameColor = frameColor; m_bgfill = fill; m_bgColor = bgColor; m_radius = radius; invalidate(); }

    LooglWidgetGroup* getParent() { return m_pParent; }

    // regarding to sprite creating issue(memory usage, the size limitation), refer to https://github.com/Bodmer/TFT_eSPI#sprites
    // instant: delete after draw to save memory usage
    void setSprite(bool sprite, loogl_color bgColor=0, bool instant=false) { m_sprite = sprite; m_sprBgColor = bgColor; m_transparentColor = 0; m_transparent = false; m_spriteInstant = instant; 
                                                        if(!sprite && m_pTftSpr){
                                                            delete m_pTftSpr;
                                                            m_pTftSpr = nullptr;
                                                        } }
    void setSprite(loogl_color bgColor, loogl_color transparentColor, bool instant=false) { m_sprite = true; m_sprBgColor = bgColor; m_transparentColor = transparentColor; m_transparent = true; m_spriteInstant = instant;  }
    bool isSprite() { return m_sprite; }
    TFT_eSprite* getSprite() { return m_pTftSpr; }
    TFT_eSprite* getDrawSprite();

    int32_t getX() {
        return m_x;
    }

    int32_t getY() {
        return m_y;
    }

    int32_t getWidth() {
        return m_w;
    }

    int32_t getHeight() {
        return m_h;
    }

    // the absolute left and top on the whole screen
    int32_t getLeft();

    int32_t getTop();

    // the left and top in the parent drawing buffer(TFT_eSPI/TFT_eSprite)
    int32_t getDrawLeft();

    int32_t getDrawTop();

    int32_t getRight() { return getLeft() + m_w; }

    int32_t getBottom() { return getTop() + m_h; }
    
    bool isVisible() { return m_visible; }
    virtual void invalidate() { setInvalidate(true); if(!isSprite()) invalidateParent(); }
    bool isInvalidated() { return m_invalidated; }
    void setInvalidate(bool b) { m_invalidated = b; }
    virtual void invalidateParent();
    //forceRedraw, whether redraw not depending on m_invalidated
    void draw(TFT_eSPI& tft, bool forceRedraw=false);

    // update inner state, such as invalidated flag, timer, animation etc. (in updateImp())
    bool update() {
      // bool m_invalidated = updateImp() || m_invalidated;  // not working!
        bool needsDraw = false;
        if(updateImp()) needsDraw = true;
        if(doUpdate()) needsDraw = true;
        return m_invalidated || needsDraw; 
    }

  protected:
    virtual void onDraw(TFT_eSPI& tft) {};

    // virtual functions, draw frame and bg, derived class should call base class
    virtual void drawImp(TFT_eSPI& tft, int32_t x, int32_t y) { drawFrameBg(tft, x, y); }
    // for inner core class updating
    virtual bool updateImp() { return false; }
    // for user customized widgets updating
    virtual bool doUpdate() { return false; }

    // mainly for text widget, which needs tft instance to calculate size
    virtual void onUpdateSize(TFT_eSPI& tft) {}

    // normal functions
    virtual void drawFrameBg(TFT_eSPI& tft, int32_t x, int32_t y);

    // translate left/top from x/y according to align flag
    int32_t getSelfLeft();

    int32_t getSelfTop();

    // properties
    LooglWidgetGroup* m_pParent;
    //x,y, relative to parent
    int32_t m_x;
    int32_t m_y;
    int32_t m_w;
    int32_t m_h;
    int m_alignH=LOOGL_ALIGH_H_LEFT;
    int m_alignV=LOOGL_ALIGH_V_TOP;
    bool m_visible = true;
    // draw as a sprite
    bool m_sprite;
    //delete sprite after drawing, to save memory usage
    bool m_spriteInstant;
    loogl_color m_bgColor;
    loogl_color m_sprBgColor;
    loogl_color m_transparentColor;
    bool m_transparent;
    bool m_frame;
    loogl_color m_frameColor;
    bool m_bgfill;
    // frame radius
    int32_t m_radius;

    // inner variables
    TFT_eSprite* m_pTftSpr;

    // data changed, needs redraw
    bool m_invalidated;
};

class LooglWidgetText : public LooglWidgetBase {
  public:
    LooglWidgetText(LooglWidgetGroup* pParent, int32_t x, int32_t y, const String& text, uint8_t font, uint8_t fontSize=1, loogl_color textColor=0):
                    LooglWidgetBase(pParent, x, y), 
                    m_text(text),m_textSizeChanged(true), m_font(font), m_fontSize(fontSize), m_textColor(textColor), m_pfont(nullptr) {}

    const String& getText() { return m_text; }
    void setText(const String& text) { if(m_text == text) return; m_text = text; m_textSizeChanged = true; invalidate(); }
    void setFont(uint8_t font) { if(m_font == font) return; m_font=font; m_textSizeChanged = true; invalidate(); }
    void setFont(const uint8_t* pfont) { if(m_pfont == pfont) return; m_pfont=pfont; m_textSizeChanged = true; invalidate(); }
    void setFontSize(uint8_t fontSize) {  if(m_fontSize == fontSize) return; m_fontSize=fontSize; m_textSizeChanged = true; invalidate();  }
    void setTextColor(loogl_color textColor) { if(m_textColor == textColor) return; m_textColor = textColor; invalidate(); }
    void updateSize(TFT_eSPI& tft) { onUpdateSize(tft); };

  protected:
    // virtual functions
    void drawImp(TFT_eSPI& tft, int32_t x, int32_t y) {
        // log_i(String("LooglWidgetText.drawImp. ") + x + " " + y);
        LooglWidgetBase::drawImp(tft, x, y);
        if(m_pfont) {
          tft.loadFont(m_pfont);
        } else {
          //unload custom font if any
          tft.unloadFont();
          tft.setTextFont(m_font);
        }
        tft.setTextSize(m_fontSize);
        tft.setTextColor(m_textColor);
        tft.drawString(m_text, x, y);
    }

    virtual void onUpdateSize(TFT_eSPI& tft) {
        if(!m_textSizeChanged) return;
        
        m_textSizeChanged = false;
        // custom font
        if(m_pfont) {
          tft.loadFont(m_pfont);
        } else {
          tft.setTextFont(m_font);
        }
        tft.setTextSize(m_fontSize);
        setSize(tft.textWidth(m_text), tft.fontHeight());
    }

    // properties
    String m_text;
    // text changed, needs update size
    bool m_textSizeChanged;
    uint8_t m_font;
    const uint8_t* m_pfont;
    uint8_t m_fontSize;
    loogl_color m_textColor;
};

class LooglWidgetGroup : public LooglWidgetBase {
  public:
    LooglWidgetGroup(LooglWidgetGroup* pParent, int32_t x=0, int32_t y=0, int32_t w=0, int32_t h=0, bool clip=true):LooglWidgetBase(pParent,x,y,w,h), m_clip(clip) {}
    virtual ~LooglWidgetGroup() { for(auto it=m_vecWidgets.begin();it!=m_vecWidgets.end();it++) delete *it; }
    void add(LooglWidgetBase* widget) { m_vecWidgets.push_back(widget); }

  protected:
    // virtual functions
    virtual void onDraw(TFT_eSPI& tft);
    virtual void drawImp(TFT_eSPI& tft, int32_t x, int32_t y);

    void drawGroup(TFT_eSPI& tft);

    virtual bool updateImp();
    
    // 
    std::vector<LooglWidgetBase*> m_vecWidgets;
    bool m_clip;

};

class LooglApp;

class LooglScreen : public LooglWidgetGroup {
  public:
    LooglScreen(LooglApp* pApp, int32_t w=0, int32_t h=0):LooglWidgetGroup(nullptr,0,0,w,h), m_pApp(pApp) { }
    
    bool update(int key) { bool k = onKey(key); bool ret = LooglWidgetGroup::update(); return k || ret; }

    TFT_eSPI& getTft();

  protected:

    void updateTextWidgetSize(LooglWidgetText* pText) { pText->updateSize(getTft()); }
    virtual bool onKey(int key) { return false; }

    // template <class T> T* getApp() { return static_cast<T*>(m_pApp); }
    LooglApp* getApp() { return m_pApp; }

    LooglApp* m_pApp;
};

class LooglApp {
  public:
    LooglApp(int32_t tftWidth, int32_t tftHeight, LooglScreen* pUI=nullptr, float frameRate=30.0):m_tft(tftWidth, tftHeight),m_pScreen(pUI),m_frameRate(frameRate),m_initTft(true) {}
    LooglApp(TFT_eSPI& tft, LooglScreen* pUI=nullptr, float frameRate=30.0):m_tft(tft),m_pScreen(pUI),m_frameRate(frameRate),m_initTft(false) { }
    // LooglApp(LooglWidgetBase* pUI, float frameRate = 30.0):m_tft(pUI->getWidth(), pUI->getHeight()),m_pScreen(pUI),m_frameRate(frameRate) {}
    virtual ~LooglApp() {}

    // called in setup()
    void init(int rotation=-1);
    // return >=0 exit. -1: running
    virtual int update(int key=0);
    void setScreen(LooglScreen* screen) { if(m_pScreen==screen) return; m_pScreen = screen; m_tft.fillScreen(0); if(m_pScreen) m_pScreen->invalidate(); }

    TFT_eSPI& getTft() {return m_tft;}

    void exit(int code=0) { m_exit = code; }
    // void invalidate() { m_invalidated=true; }

  protected:
    virtual void doUpdate() {}
    virtual void doInit() {}
    int rotateKey(int key);
    TFT_eSPI m_tft;
    LooglScreen* m_pScreen;
    float m_frameRate;
    bool m_invalidated = false;
    bool m_initTft;
    int m_rotation = 0;
    int m_exit = -1;
};

class LooglAppFactory {
  public:
    virtual String getName()=0;
    virtual LooglApp* createApp(TFT_eSPI& tft)=0;
};


#endif