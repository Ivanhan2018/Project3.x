#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

#define MISSION_FACE_NULL			0
#define MISSION_FACE_SYSTEM			1
#define MISSION_FACE_CUSTOM			2

//////////////////////////////////////////////////////////////////////////
// 修改头像提示
//////////////////////////////////////////////////////////////////////////
class IGPFaceMissionSink
{
public:
	virtual ~IGPFaceMissionSink(){}
	virtual void onGPFaceInfo(){}
	virtual void onGPFaceSuccess(const char* szDescription){}
	virtual void onGPFaceFailure(const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 修改头像
//////////////////////////////////////////////////////////////////////////
class CGPFaceMission
	: public CSocketMission
{
public:
	CGPFaceMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPFaceMissionSink* pIGPFaceMissionSink);
	// 设置系统头像
	void setToSystemFace(uint16 face);
	// 设置自定义头像
	void setToCustomFace(uint32 custom[FACE_CX*FACE_CY]);
	
	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	// 用户头像
	bool onSubUserFaceInfo(void* data, int size);
	// 个人信息
	bool onSubUserIndividual(void* data, int size);

	// 操作成功
	bool onSubOperateSuccess(void* data, int size);
	// 操作失败
	bool onSubOperateFailure(void* data, int size);
	
private:
	// 任务类型
	uint8 mMissionType;
	// 回调
	IGPFaceMissionSink* mIGPFaceMissionSink;

	//////////////////////////////////////////////////////////////////////////
	// 修改头像
	//////////////////////////////////////////////////////////////////////////
	// 系统头像
	uint16 mFace;
	// 自定义头像
	uint32 mCustom[FACE_CX*FACE_CY];
}; // CGPFaceMission
