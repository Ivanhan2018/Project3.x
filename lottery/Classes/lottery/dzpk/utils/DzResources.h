//
//  DzResources.h
//  
//
//  Created by luochuanting on 13-4-13.
//
//

#ifndef Dz_Resources_h
#define Dz_Resources_h

//using namespace std;
#define CLASS_NAME "com/rwhz/dzpk/"
#define PACKAGE_NAME com_rwhz_dzpk
#define define_native_function_exec(a, b) Java_##a##_##b
#define define_native_function(a, b) define_native_function_exec(a, b)


// images

// common


// marquess
#define Common_Marquee_BG "Common/Marquee/gonggao.png"

#define Common_PersonInfo_Avatar "Common/PersonInfo/tx.png"
#define Common_PersonInfo_Plist "Common/PersonInfo/setting.plist"
#define Common_PersonInfo_Png "Common/PersonInfo/setting.png"

#define Common_Setting_Plist "Common/Setting/checkbox.plist"
#define Common_Setting_Png "Common/Setting/checkbox.png"
#define Common_Setting_BG "Common/Setting/bg.png"
#define Common_Setting_Progress "Common/Setting/sliderProgress.png"
#define Common_Setting_Track "Common/Setting/sliderTrack.png"
#define Common_Setting_Thumb "Common/Setting/sliderThumb.png"

// menu
#define Common_Menu_Plist "Common/Menu/menu-icon.plist"
#define Common_Menu_Png "Common/Menu/menu-icon.png"

// hall
#define Hall_BG "Hall/hall-bg.jpg"
#define Hall_Menu_Plist "Hall/VenueMenu.plist"
#define Hall_Menu_Png "Hall/VenueMenu.png"
#define Hall_Plist "Hall/hall.plist"
#define Hall_Png "Hall/hall.png"

// load table
#define Load_Table_BG "Hall/hall-bg.jpg"

// rank
#define Rank_Plist "Rank/rank.plist"
#define Rank_Png "Rank/rank.png"

#define Rank_N1 "rank-no01.png"
#define Rank_N2 "rank-no02.png"
#define Rank_N3 "rank-no03.png"
#define Rank_N4 "rank-no04.png"
#define Rank_N5 "rank-no05.png"
#define Rank_N6 "rank-no06.png"


// help
#define Help_Plist "Help/help.plist"
#define Help_Png "Help/help.png"

// message
#define Message_Plist "Message/message.plist"
#define Message_Png "Message/message.png"


// sound


#endif
