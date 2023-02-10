// using Paj7620 gesture detector as input
// init to home UI, hand move up/down to move selection.
// move clockwise to enter selection, launch an app.  
// move anti-clockwise to leave an app, return to home UI.

#include "loogl.h"
#include "AppHello.h"
#include "AppWorld.h"
#include "Launcher.h"
#include "paj7620.h"


std::vector<LooglAppFactory*> apps = { new AppHelloFactory(), new AppWorldFactory(), };
AppLauncher launcher(apps, TFT_WIDTH, TFT_HEIGHT);

// -------------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  // while(!Serial.availableForWrite()) delay(100);
  delay(100);

  uint8_t error = paj7620Init();
  if (error) 
  {
    log_e(String("INIT ERROR,CODE:") + error);
  }
  
  launcher.init();
}

// -------------------------------------------------------------------------
// Main loop
// -------------------------------------------------------------------------
void loop()
{
  char c = gesture7();
  // rotate to TFT default direction
  char g = rotateGesture(c, 2);
  int key = LOOGL_KEY_NONE;
  if(g!=0) {
    log_i(String("key: ") + g);
  }

  switch(g){
    case 'L':
      key = LOOGL_KEY_LEFT;
    break;
    case 'R':
      key = LOOGL_KEY_RIGHT;
    break;
    case 'U':
      key = LOOGL_KEY_UP;
    break;
    case 'D':
      key = LOOGL_KEY_DOWN;
    break;
    case 'C':
      key = LOOGL_KEY_ENTER;
    break;
    case 'A':
      key = LOOGL_KEY_BACK;
    break;
  }

  launcher.update(key);
  // MyApp.update();
}

//#define PIN_BAT_VOLT                 4
//uint32_t volt = (analogRead(PIN_BAT_VOLT) * 2 * 3.3 * 1000) / 4096; //mv
//
///////////////////////////////////////////////////////////////////////////////////
// gesture sensor
#define GES_REACTION_TIME		800
#define GES_QUIT_TIME			1000

const char* LRUD[] = {"LRUD","UDRL","RLDU","DULR"};
// r, rotate clockwise. 0:0, 1:90, 2:180, 3:270
char rotateGesture(char c, int r){
  for(int i=0;i<4;i++){
    if(LRUD[0][i] == c){
      return LRUD[r][i];
    }
  }

  return c;
}

char gesture7() {
  uint8_t data = 0, data1 = 0, error; 

	error = paj7620ReadReg(0x43, 1, &data);				// Read Bank_0_Reg_0x43/0x44 for gesture result.
	if (!error) 
	{
		switch (data) 									// When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
		{
			case GES_RIGHT_FLAG:
        return 'R';
			case GES_LEFT_FLAG:
        return 'L';
			case GES_UP_FLAG:
				return 'U';
			case GES_DOWN_FLAG:
				return 'D';
			// case GES_FORWARD_FLAG:
			// 	return "F";
			// case GES_BACKWARD_FLAG:		  
  		// 	return "B";
			case GES_CLOCKWISE_FLAG:
				return 'C';
			case GES_COUNT_CLOCKWISE_FLAG:
				return 'A';
			default:
				paj7620ReadReg(0x44, 1, &data1);
				if (data1 == GES_WAVE_FLAG) 
				{
					return 'W';
				}
				break;
		}
	}
  return 0;
}
