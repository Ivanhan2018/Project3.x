#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class SelectBox:public cocos2d::ui::Button
{
public:

	static SelectBox* create(std::string unSelect, std::string Select);

	SelectBox(std::string unSelect, std::string Select);

	virtual bool init()override;

	virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);
	virtual void onTouchMoved(Touch *touch, Event *unusedEvent);
	virtual void onTouchEnded(Touch *touch, Event *unusedEvent);
	virtual void onTouchCancelled(Touch *touch, Event *unusedEvent);

	void addCallBack(std::function<void(cocos2d::Node*,bool)> callback){
		m_CallBack = callback;
	};

	bool getSelectStatus(){return m_bSelect;}
	void setSelectStatus(bool isSelect);
protected:

	void changeStatus();

protected:

	bool m_bSelect;
	std::string m_sUnSelectFileName;
	std::string m_sSelectFileName;

	std::function<void(cocos2d::Node*,bool)> m_CallBack;
};