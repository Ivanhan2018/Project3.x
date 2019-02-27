#include "LotteryGlobal.h"

//默认序列
static int tempArr[] = {1,11,26,12,24,23,4,6,8,9,16,3,5,10,13,17,15,14,33};

MyBetNumber* MyBetNumber::_instance = nullptr;

MyBetNumber::~MyBetNumber()
{
	if(_instance) delete _instance;
	if(m_arr_caizhong != NULL) m_arr_caizhong->release();
}

MyBetNumber* MyBetNumber::getInstance()
{
	if(_instance == nullptr)
	{
		_instance = new (std::nothrow)MyBetNumber();
	}

	return _instance;
}

void MyBetNumber::addItem(Bet_Info item)
{
	_instance->betNumbers.push_back(item);
}

void MyBetNumber::removeById(int id)
{
	if(_instance->betNumbers.size() == 0) return;

	std::vector<Bet_Info>::iterator it = _instance->betNumbers.begin() + id;
	_instance->betNumbers.erase(it);
}

void MyBetNumber::clear()
{
	_instance->betNumbers.clear();
}

int MyBetNumber::size()
{
	return _instance->betNumbers.size();
}

Bet_Info MyBetNumber::getItem(int id)
{
	return _instance->betNumbers.at(id);
}

void MyBetNumber::betReady()
{
	this->betCount = this->betNumbers.size();
	retType = 0;
}

bool MyBetNumber::isEndBet()
{
	return this->betCount <= 0;
}

void MyBetNumber::betOneEnd()
{
	this->betCount --;
}

void MyBetNumber::setRetType(int retType)
{
	if(this->retType < retType) 
		this->retType = retType;
}

void MyBetNumber::addRandSeed()
{
	timeval psv;	
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec;
	srand(rand_seed);
}

void MyBetNumber::saveCaiZhong(Array* arr_caizhong)
{
	if(m_arr_caizhong == NULL)
	{
		m_arr_caizhong = Array::create();
		m_arr_caizhong->retain();
	}
	m_arr_caizhong->removeAllObjects();

	m_arr_caizhong->addObjectsFromArray(arr_caizhong);

	//检查处理
	for(int i = 0;i < m_arr_caizhong->count();i ++)
	{
		Dictionary *dic = (Dictionary *)m_arr_caizhong->getObjectAtIndex(i);
		Integer* typeId = (Integer *)dic->objectForKey("wTypeID");

		CCLOG("typeid------------>%d", typeId->getValue());

		bool isChecked = false;
		for(int j = 0;j < sizeof(tempArr)/sizeof(int);j ++)
		{
			if(tempArr[j] == typeId->getValue())
			{
				isChecked = true;
				break;
			}
		}
		if(isChecked == false)
		{
			//CCLOG("typeid-----not------->%d", typeId->getValue());
			m_arr_caizhong->removeObjectAtIndex(i);
			i --;
		}
	}
	//CCLOG("print array list");
	//测试打印listarr
	for(int i = 0;i < m_arr_caizhong->count();i ++)
	{
		Dictionary *dic = (Dictionary *)m_arr_caizhong->getObjectAtIndex(i);
		Integer* sortId = (Integer *)dic->objectForKey("wSortID");
		Integer* typeId = (Integer *)dic->objectForKey("wTypeID");

		CCLOG("=======>>>sortId %d typeId======>>> %d",sortId->getValue(),typeId->getValue());
	}
}

int MyBetNumber::getCaiZhong(const int index)
{
	int result = 0;

	if(m_arr_caizhong &&index > m_arr_caizhong->count() * 2) 
		return 1;	

	if(m_arr_caizhong == NULL)
	{
		m_arr_caizhong = Array::create();		

		//增加内容
		for(int i = 0 ; i < sizeof(tempArr)/sizeof(int) ; i++){	

			Dictionary *data = Dictionary::create();

			data->setObject(Integer::create(i + 1),"wSortID");
			data->setObject(Integer::create(tempArr[i]), "wTypeID");

			m_arr_caizhong->addObject(data);
		}
	}

	//查找彩种
	for(int i = 0;i < m_arr_caizhong->count();i ++)
	{
		Dictionary *dic = (Dictionary *)m_arr_caizhong->getObjectAtIndex(i);
		Integer* sortId = (Integer *)dic->objectForKey("wSortID");
		Integer* typeId = (Integer *)dic->objectForKey("wTypeID");

		if(sortId->getValue() == index)
		{
			result = typeId->getValue();
			break;
		}
	}

	return result;
}

int MyBetNumber::getCaiZhongCount()
{
	if(m_arr_caizhong == NULL)
		return 19;		//默认19个
	return m_arr_caizhong->count();
}