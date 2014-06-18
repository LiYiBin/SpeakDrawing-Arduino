
#define kInit 0x00
#define kAllHappy 0x01
#define kAllAngry 0x02
#define kAllHelpless 0x03
#define kAllWorried 0x04
#define kAllNervous 0x05
#define kAllExcited 0x06

#define kAddHappy 0x11
#define kAddAngry 0x12
#define kAddHelpless 0x13
#define kAddWorried 0x14
#define kAddNervous 0x15
#define kAddExcited 0x16  

int HappyColor[3] = {255, 249, 92};
int AngryColor[3] = {255, 26, 10};
int HelplessColor[3] = {160, 238, 255};
int WorriedColor[3] = {177, 29, 255};
int NervousColor[3] = {91, 255, 211};
int ExcitedColor[3] = {255, 137, 20};

int LedList[11] = {0, 1, 2, 8, 9, 10, 11, 4, 5, 6, 7};
const int maxLedNumber = 11;
