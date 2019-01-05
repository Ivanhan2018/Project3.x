#ifndef __ROL__MENU_H__
#define __ROL__MENU_H__
#include "cocos2d.h"
#include "ui/UILayout.h"


class RolMenu :public cocos2d::ui::Layout{
public:
	//构造方法
	CREATE_FUNC(RolMenu);

	//添加菜单项
	void addMenuItem(cocos2d::MenuItem *item);
	//删除房间
	void RemoveAllItem();
	//更新位置
	void updatePosition(bool forward);
	//更新位置，有动画
	void updatePositionWithAnimation();
	//位置矫正  修改位置forward为移动方向  当超过1/3，进1
	//true 为正向  false 负
	void rectify(bool forward);
	//初始化
	virtual bool init();
	// 重置  显示所引号设置为0
	void reset();
	////游戏列表
	//void loadGame(const cocos2d::ccMenuCallback& callback);
	////初始化房间
	//void loadRoom(std::int16_t wKindID,const cocos2d::ccMenuCallback& callback);
	
	int32_t getItemTag();
private:
	//设置当前显示索引
	void setIndex(int index);
	//设置当前显示菜单项的索引号
	float getIndex();
	//返回被选中的item
	cocos2d::MenuItem * getCurrentItem();
	//数学计算式width*index/(abs(index)+CALC_A) ,其中CALC_A为常数
	float calcFunction(float index, float width);
	// 获取偏移位置
	float getItemPosX(float index, float width);
private:
	//菜单菜单项的索引号
	float _index;
	//上一次菜单项的索引号
	float _lastIndex;

	//菜单项集合,_children顺序会变化，新建数组保存顺序
	cocos2d::Vector<cocos2d::MenuItem *> _items;

	//监听函数
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	//动画完结调用函数，这个主要是确定哪一个菜单项在前面
	void actionEndCallBack(float dx);

	//当前被选择的item
	cocos2d::MenuItem *_selectedItem;
};
#endif // __ROL__MENU_H__