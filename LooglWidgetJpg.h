#ifndef LooglWidgetJpg_H
#define LooglWidgetJpg_H

#include <TJpg_Decoder.h>

#include "loogl.h"

class LooglWidgetJpg : public LooglWidgetBase {
  public:
    LooglWidgetJpg(LooglWidgetGroup* pParent, int32_t x, int32_t y, uint8_t scale): LooglWidgetJpg(pParent, x, y, nullptr, 0, scale) { }

    LooglWidgetJpg(LooglWidgetGroup* pParent, int32_t x, int32_t y, const uint8_t array[]=nullptr, uint32_t size=0, uint8_t scale=1):
                    LooglWidgetBase(pParent, x, y), m_pJpgArray(array), m_jpgArraySize(size), m_jpgScale(scale)
                    {
                        TJpgDec.setSwapBytes(true);
                        TJpgDec.setCallback(LooglWidgetJpg::tft_output);
                    }

    void setJpgScale(uint8_t scale) { if(m_jpgScale==scale) return; m_jpgScale = scale; invalidate(); }
    void setJpg(const uint8_t array[], uint32_t size) {
      if(m_pJpgArray==array) return;

      m_pJpgArray = array;
      m_jpgArraySize = size;
      uint16_t width;
      uint16_t height;
      TJpgDec.getJpgSize(&width, &height, array, size);
      setSize(width, height);
      invalidate();
    }
    
  static bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
      if (y >= m_pTft->height()) return false;
      m_pTft->pushImage(x, y, w, h, bitmap);
      // Return 1 to decode next block
      return true;
  }

  protected:
    // virtual functions
    void drawImp(TFT_eSPI& tft, int32_t x, int32_t y) {
        LooglWidgetBase::drawImp(tft, x, y);
        m_pTft = &tft;
        TJpgDec.setJpgScale(m_jpgScale);
        TJpgDec.drawJpg(x, y, m_pJpgArray, m_jpgArraySize);
   }

    const uint8_t* m_pJpgArray = nullptr;
    uint32_t m_jpgArraySize = 0;
    uint8_t m_jpgScale = 1;
    static TFT_eSPI* m_pTft;

};

TFT_eSPI* LooglWidgetJpg::m_pTft = nullptr;

#endif
