#ifndef __HOTUPDATE_LAYER_H__
#define __HOTUPDATE_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

class HotUpdateLayer : public cocos2d::Layer
{
public:
	static HotUpdateLayer* create(int idx,int serverId);
	HotUpdateLayer(void);
	~HotUpdateLayer(void);
    virtual bool init(int idx,int serverId);

	// 下载对话框	
	void CreatePopWndDown();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void onLoadPercent(float percent);
    void onAllFileIsNew();
    void onLoadSuccess();
    void onLoadError(int eventCode);
    void tryDownloadFaildAssets();
	void OnStartConnectGameServer();
    
    void assetsManagerExCallBack(cocos2d::extension::EventAssetsManagerEx * ex);
 
protected:
    cocos2d::Sprite * _loading_spr;
    cocos2d::Sprite * _loading_progress_bg;
    cocos2d::ProgressTimer * _loading_progress;
    cocos2d::Label * _label_succes;
    cocos2d::Sprite * _sprite_loading_success;
    cocos2d::Sprite * _sprite_loading_error;
	cocos2d::LabelTTF* m_pProgressLabel;//显示下载进度信息
    
    cocos2d::extension::AssetsManagerEx * _assets_manager_ex;
    cocos2d::extension::EventListenerAssetsManagerEx * _assets_manager_listener;
    
    int faild_down_count;

	int m_idx;
	int m_serverId;
};

#endif // __HOTUPDATE_LAYER_H__
