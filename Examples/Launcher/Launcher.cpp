
#include "Launcher.h"

LauncherScreen::LauncherScreen(AppLauncher* pAppLauncher, const std::vector<LooglAppFactory*>& apps):LooglScreen(pAppLauncher, SCREEN_WIDTH, SCREEN_HEIGHT), m_vecApps(apps) {
    int left = 10;
    int top = 10;
    int itemHeight = 20;
    for(auto it=m_vecApps.begin();it!=m_vecApps.end();it++) {
        auto text = new LooglWidgetText(this, left, top, (*it)->getName(), 1, 2, TFT_WHITE);
        m_vecTexts.push_back(text);
        top += itemHeight;
    }
    
    setSelected(0);
    // if screen is to big, could fail to create sprite due to memory limitation.
    // setSprite(true);
    setFrameBg(false, true, 0, TFT_MAGENTA, 0);
}

bool LauncherScreen::onKey(int key) {
  switch(key) {
    case LOOGL_KEY_UP:
    if(m_currentSelected>0) {
      setSelected(m_currentSelected-1);
    } else {
      // setSelected(m_vecTexts.size()-1);
    }
    break;

    case LOOGL_KEY_DOWN:
    if(m_currentSelected<m_vecTexts.size()-1) {
      setSelected(m_currentSelected+1);
    } else {
      // setSelected(0);
    }
    break;

    case LOOGL_KEY_ENTER:
      // getApp<AppLauncher>()->launch(m_vecApps[m_currentSelected]);
      log_i(String("launch [") + m_currentSelected + "] " + m_vecApps[m_currentSelected]->getName());
      (static_cast<AppLauncher*>(getApp()))->launch(m_vecApps[m_currentSelected]);
    break;

    default:
    break;
  }
  return false; 
}

void LauncherScreen::setSelected(int item) {
  if(item < 0 || item >= m_vecTexts.size()) return;

  if(m_currentSelected>=0 && m_currentSelected<m_vecTexts.size()) {
    m_vecTexts[m_currentSelected]->setFrameBg(false, false);
  }
  m_vecTexts[item]->setFrameBg(false, true, 0, TFT_RED);
  m_currentSelected = item;
  invalidate();
}

AppLauncher::AppLauncher(const std::vector<LooglAppFactory*>& apps, int32_t tftWidth, int32_t tftHeight):
                                                                LooglApp(tftWidth, tftHeight, &m_screen),
                                                                m_screen(this, apps), m_currentApp(nullptr) {
  m_rotation=3;
}

int AppLauncher::update(int key) {
    if(m_currentApp) {
      int exit = m_currentApp->update(key);

      if(exit>=0){
          //log, app exit
          delete m_currentApp;
          m_currentApp = nullptr;
          init();
      }
    } else {
      return LooglApp::update(key);
    }

    return -1;
}
