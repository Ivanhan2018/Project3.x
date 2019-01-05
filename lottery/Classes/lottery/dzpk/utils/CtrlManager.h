//
//  CtrlManager.h
//  TexasPoker
//
//  Created by peniel on 12/26/13.
//
//

#ifndef __TexasPoker__CtrlManager__
#define __TexasPoker__CtrlManager__

#include "Inc.h"

class CtrlManager
{
    
public:
    static ControlButton* createCtrlBtnWithFile(const char* btnBgName,const char* title,bool isColor=true);
    
    static ControlButton* createCtrlBtnWithFile(const char* btnBgName,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFile(const char* btnBgName,const char* title,CCSize preferredSize,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFile(const char* btnBgName,const char* title,ccColor3B selectColor,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFile(const char* btnBgName,const char* title,CCSize preferredSize,ccColor3B selectColor,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFreame(const char* btnBgName,const char* title,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFreame(const char* btnBgName,const char* title,CCSize preferredSize,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFreame(const char* btnBgName,const char* title,CCSize preferredSize,ccColor3B selectColor,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFreame(const char* btnBgName,const char* title,ccColor3B selectColor,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFreame(CCSpriteFrame* sf,const char* title,ccColor3B selectColor,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFreame(CCSpriteFrame* sf,const char* title,int touchPriority=-1);
    
    static ControlButton* createCtrlBtnWithFreame(CCSpriteFrame* sf,const char* title,int fontSize,int touchPriority);
    
    
};

#endif /* defined(__TexasPoker__CtrlManager__) */
