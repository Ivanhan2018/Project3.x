#ifndef _IUserManagerSink_H_
#define _IUserManagerSink_H_

#include "IClientUserItem.h"
class IUserManagerSink
{
public:
	virtual void OnUserItemAcitve(IClientUserItem* pIClientUserItem) = 0;
	virtual void OnUserItemDelete(IClientUserItem* pIClientUserItem) = 0;
	virtual void OnUserFaceUpdate(IClientUserItem* pIClientUserItem) = 0;
	virtual void OnUserItemUpdate(IClientUserItem* pIClientUserItem, const tagUserScore& LastScore) = 0;
	virtual void OnUserItemUpdate(IClientUserItem* pIClientUserItem, const tagUserStatus& LastStatus) = 0;
	virtual void OnUserItemUpdate(IClientUserItem* pIClientUserItem, const tagUserAttrib & UserAttrib) = 0;
};

#endif // _IUserManagerSink_H_