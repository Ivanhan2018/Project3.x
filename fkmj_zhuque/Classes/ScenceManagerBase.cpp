#include "ScenceManagerBase.h"
#include "IServerItem.h"

#include "GameLib.h"
#include "GameScriptNet.h"

FV_SINGLETON_STORAGEBASE(ScenceManagerBase);

ScenceManagerBase::ScenceManagerBase()
{
	new GameScriptNet();
}
ScenceManagerBase::~ScenceManagerBase()
{

}