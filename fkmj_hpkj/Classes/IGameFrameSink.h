#ifndef _IGameFrameSink_H_
#define _IGameFrameSink_H_

class IGameFrameSink
{
public:
	static IGameFrameSink* getInterface();
	
	virtual ~IGameFrameSink(){};

	//////////////////////////////////////////////////////////////////////////
	//配置接口
public:
	//房间配置
	virtual void OnGFConfigServer(const tagUserAttribute& UserAttribute, const tagGameAttribute& GameAttribute, const tagServerAttribute& ServerAttribute )=0;
	//配置完成
	virtual void OnGFConfigFinish()=0;

	//////////////////////////////////////////////////////////////////////////
	//Socket接口
public:
	virtual bool OnGFEventSocket(int main, int sub, void* data, int dataSize)=0;
	
	//////////////////////////////////////////////////////////////////////////
	//用户接口
public:
	//用户进入
	virtual void OnGFUserEnter(IClientUserItem* pIClientUserItem)=0;
	//用户积分
	virtual void OnGFUserScore(dword dwUserID, const tagUserScore& UserScore)=0;
	//用户状态
	virtual void OnGFUserStatus(dword dwUserID, const tagUserStatus& UserStatus)=0;
	//用户属性
	virtual void OnGFUserAttrib(dword dwUserID, const tagUserAttrib& UserAttrib)=0;
	//用户自定义头像
	virtual void OnGFUserCustomFace(dword dwUserID,  dword dwCustomID, const tagCustomFaceInfo& CustomFaceInfo)=0;


}; // IGameFrameSink

#endif