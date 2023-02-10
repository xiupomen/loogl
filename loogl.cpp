#include "loogl.h"


//////////////////////////////////////////////////

LooglWidgetBase::LooglWidgetBase(LooglWidgetGroup* pParent, int32_t x, int32_t y, int32_t w, int32_t h):
                                              m_pParent(pParent),
                                              m_x(x), m_y(y), m_w(w), m_h(h),
                                              m_sprite(false), m_bgColor(0), m_transparentColor(0), m_transparent(false),
                                              m_frame(false), m_bgfill(false), m_frameColor(0),
                                              m_invalidated(true), m_pTftSpr(nullptr)
                                              {
                                                 if(pParent) pParent->add(this);
                                              }


void LooglWidgetBase::setX(int32_t x) {
    if(m_x == x) return;

    m_x = x;
    // for a sprite, position(x, y) change does not effect content, so just invalidate parent to update position. no need to redraw content.
    if(isSprite()) {
      invalidateParent();
    } else {
      invalidate();
    }
}

void LooglWidgetBase::setY(int32_t y) {
    if(m_y == y) return;

    m_y = y;
    if(isSprite()) {
      invalidateParent();
    } else {
      invalidate();
    }
}

void LooglWidgetBase::setPosition(int32_t x, int32_t y) {
    if(m_x == x && m_y == y) return;

    m_x = x;
    m_y = y;
    if(isSprite()) {
      invalidateParent();
    } else {
      invalidate();
    }
}

void LooglWidgetBase::move(int32_t offset_x, int32_t offset_y) {
    if(offset_x == 0 && offset_y == 0) return;
    
    m_x += offset_x;
    m_y += offset_y;
    if(isSprite()) {
      invalidateParent();
    } else {
      invalidate();
    } 
}

void LooglWidgetBase::setAlign(int alignH, int alignV) {
    if(m_alignH==alignH && m_alignV==alignV) return;

    m_alignH = alignH;
    m_alignV = alignV;
    if(isSprite()) {
      invalidateParent();
    } else {
      invalidate();
    }     
}

void LooglWidgetBase::setWidth(int32_t w) {
    if(m_w == w) return;

    m_w = w;
    invalidate();
}

void LooglWidgetBase::setHeight(int32_t h) {
    if(m_h == h) return;

    m_h = h;
    invalidate();
}

void LooglWidgetBase::setSize(int32_t w, int32_t h) {
    if(m_w == w && m_h == h) return;

    m_w = w;
    m_h = h;
    invalidate();
}

void LooglWidgetBase::invalidateParent() {
   if(m_pParent) m_pParent->invalidate(); 
}

TFT_eSprite* LooglWidgetBase::getDrawSprite() { 
    if(!m_pParent) return nullptr; 
    TFT_eSprite* p = m_pParent->isInvalidated()? m_pParent->getSprite(): nullptr;
    return p ? p : m_pParent->getDrawSprite();
}

int32_t LooglWidgetBase::getLeft() {
    return getSelfLeft() + (m_pParent == nullptr ? 0 : m_pParent->getLeft());
}

int32_t LooglWidgetBase::getTop() {
    return getSelfTop() + (m_pParent == nullptr ? 0 : m_pParent->getTop());
}

// the left and top in the parent drawing buffer(TFT_eSPI/TFT_eSprite)
int32_t LooglWidgetBase::getDrawLeft() {
    return getSelfLeft() + (m_pParent == nullptr || m_pParent->isSprite() && m_pParent->isInvalidated() ? 0 : m_pParent->getDrawLeft());
}

int32_t LooglWidgetBase::getDrawTop() {
    return getSelfTop() + (m_pParent == nullptr || m_pParent->isSprite() && m_pParent->isInvalidated() ? 0 : m_pParent->getDrawTop());
}

int32_t LooglWidgetBase::getSelfLeft() {
    switch(m_alignH) {
      case LOOGL_ALIGH_H_CENTER:
        return m_x - getWidth()/2;
      case LOOGL_ALIGH_H_RIGHT:
        return m_x - getWidth();
    }
    return m_x;
}

int32_t LooglWidgetBase::getSelfTop(){
    switch(m_alignV) {
      case LOOGL_ALIGH_V_CENTER:
        return m_y - getHeight()/2;
      case LOOGL_ALIGH_V_BOTTOM:
        return m_y - getHeight();
    }
    return m_y;
}

void LooglWidgetBase::draw(TFT_eSPI& tft, bool forceRedraw) {
    if(!isVisible()) return;

    onUpdateSize(tft);

    if(m_invalidated || forceRedraw) {
      if(m_sprite){
          if(!m_pTftSpr) {
              m_pTftSpr = new TFT_eSprite(&tft);
          }
          int32_t w = getWidth();
          int32_t h = getHeight();
          if(m_pTftSpr->width()!=w || m_pTftSpr->height()!=h) {
              m_pTftSpr->deleteSprite();
              m_pTftSpr->createSprite(w, h);
              log_i("LooglWidgetBase, create sprite");
              if(!m_pTftSpr->created()) {
                log_w("LooglWidgetBase, fail to create sprite!");
                // Serial.println("fail to create sprite!");
              }
          }

          if(m_invalidated || forceRedraw && m_spriteInstant) {
            m_pTftSpr->fillSprite(m_sprBgColor);
            drawImp(*m_pTftSpr, 0, 0);
          }

          TFT_eSprite* drawSprite = getDrawSprite();
          if(drawSprite) {
              if(m_transparent){
                  m_pTftSpr->pushToSprite(drawSprite, getDrawLeft(), getDrawTop(), m_transparentColor);
              } else {
                  m_pTftSpr->pushToSprite(drawSprite, getDrawLeft(), getDrawTop());
              }
          } else {
              if(m_transparent){
                  m_pTftSpr->pushSprite(getLeft(), getTop(), m_transparentColor);
              } else {
                  m_pTftSpr->pushSprite(getLeft(), getTop());
              }
          }
          // if instant sprite, delete sprite
          if(m_spriteInstant) m_pTftSpr->deleteSprite();
      } else {
          log_i("draw() -> drawImp()");
          drawImp(tft, getDrawLeft(), getDrawTop());
      }
    } else {
      // no need to redraw self, but still pass down to all child widgets, 
      onDraw(tft);
    }

    setInvalidate(false);
}


void LooglWidgetBase::drawFrameBg(TFT_eSPI& tft, int32_t x, int32_t y) {
    if(m_bgfill) {
        tft.fillRoundRect(x, y, getWidth(), getHeight(), m_radius, m_bgColor);
    }
    if(m_frame) {
        tft.drawRoundRect(x, y, getWidth(), getHeight(), m_radius, m_frameColor);
    }
}

///////////////////////////////////////////////////////////

void LooglWidgetGroup::onDraw(TFT_eSPI& tft) {
    for(auto it=m_vecWidgets.begin();it!=m_vecWidgets.end();it++) {
      (*it)->draw(tft);
  }
}

void LooglWidgetGroup::drawImp(TFT_eSPI& tft, int32_t x, int32_t y) {
    LooglWidgetBase::drawImp(tft, x, y);

    if(!isSprite() && m_clip) {
        tft.setViewport(getLeft(), getTop(), getWidth(), getHeight(), true);
        drawGroup(tft);
        tft.resetViewport();
    } else {
        drawGroup(tft);
    }
}

void LooglWidgetGroup::drawGroup(TFT_eSPI& tft) {
    log_i("drawGroup");
    for(auto it=m_vecWidgets.begin();it!=m_vecWidgets.end();it++) {
        (*it)->draw(tft, true);
    }
}

bool LooglWidgetGroup::updateImp() {
    bool needsDraw = LooglWidgetBase::updateImp();
    for(auto it=m_vecWidgets.begin();it!=m_vecWidgets.end();it++) {
        if((*it)->update()) needsDraw = true;
    }
    
    return needsDraw;
}

TFT_eSPI& LooglScreen::getTft() {
   return m_pApp->getTft(); 
}

void LooglApp::init(int rotation) { 
  if(m_initTft) {
    m_tft.init();
    m_initTft = false;
  }
  
  if(rotation!=-1) {
    m_rotation = rotation;
  }

  m_tft.setRotation(m_rotation);
  m_tft.fillScreen(0);
  doInit();
  m_exit = -1;
  // redraw UI, could resume from other App, so needs to invalidate.
  if(m_pScreen) m_pScreen->invalidate();
}

int LooglApp::update(int key) {
  unsigned long start_time = millis();
  doUpdate();
  if(m_pScreen) {
      if(m_pScreen->update(rotateKey(key))) {
        log_i("screen draw");
        m_pScreen->draw(m_tft);
        // after drawing, unload font to release memory.
        // we are not doing this in text widgets because multiple text widgets maybe using same custom font, loading and unloading one same font frequently is not a good idea.
        m_tft.unloadFont();
        
      }
  }
  if(m_exit>=0) return m_exit;

  if(m_frameRate>0) {
    unsigned long frame_time = 1000/m_frameRate;
    unsigned long spent_time = millis()-start_time;
    if(frame_time > spent_time) {
      delay(frame_time-spent_time);
    }
  }      
  return m_exit; //-1, running
}

// rotation 0, 1, 2, 3
int KEY_DIR[][4] = {
  {LOOGL_KEY_LEFT, LOOGL_KEY_RIGHT, LOOGL_KEY_UP, LOOGL_KEY_DOWN},
  {LOOGL_KEY_UP, LOOGL_KEY_DOWN, LOOGL_KEY_RIGHT, LOOGL_KEY_LEFT},
  {LOOGL_KEY_RIGHT, LOOGL_KEY_LEFT, LOOGL_KEY_DOWN, LOOGL_KEY_UP},
  {LOOGL_KEY_DOWN, LOOGL_KEY_UP, LOOGL_KEY_LEFT, LOOGL_KEY_RIGHT},
  };

int LooglApp::rotateKey(int key){
  if(m_rotation!=0) {
    for(int i=0;i<4;i++){
      if(KEY_DIR[0][i] == key){
        return KEY_DIR[m_rotation][i];
      }
    }
  }

  return key;
}