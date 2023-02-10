#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include "loogl.h"

#define SCREEN_WIDTH  TFT_HEIGHT
#define SCREEN_HEIGHT TFT_WIDTH

class AppLauncher;

class LauncherScreen: public LooglScreen {
  public:
    LauncherScreen(AppLauncher* pAppLauncher, const std::vector<LooglAppFactory*>& apps);

  protected:
    // virtual bool doUpdate() {
    //       return false;
    // }

    virtual bool onKey(int key);
  
    void setSelected(int item);

    const std::vector<LooglAppFactory*>& m_vecApps;
    std::vector<LooglWidgetText*> m_vecTexts;
    int m_currentSelected = -1;
};

class AppLauncher: public LooglApp {
  public:
    AppLauncher(const std::vector<LooglAppFactory*>& apps, int32_t tftWidth, int32_t tftHeight);

    void launch(LooglAppFactory* pAppFactory) { m_currentApp = pAppFactory->createApp(getTft()); m_currentApp->init(); }

    virtual int update(int key=0);


  protected:
    LauncherScreen m_screen;
    LooglApp* m_currentApp;

};

#endif
