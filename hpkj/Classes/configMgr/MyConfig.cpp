#include "MyConfig.h"

const char* g_LblPosAttrName[MAX_COLUMN_NUM]={"Col0","Col1","Col2","Col3","Col4"};

/** Returns a std::stringVector that contains all the substd::strings delimited
   by the characters in the passed <code>delims</code> argument.
   @param 
   delims A list of delimiter characters to split by
   @param 
   maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
   parameters is > 0, the splitting process will stop after this many splits, left to right.
   */
std::vector<string> split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0)
{
	std::vector<string> ret;
	unsigned int numSplits = 0;

	// Use STL methods 
	size_t start, pos;
	start = 0;
	do 
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			// Do nothing
			start = pos + 1;
		}
		else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
		{
			// Copy the rest of the std::string
			ret.push_back( str.substr(start) );
			break;
		}
		else
		{
			// Copy up to delimiter
			ret.push_back( str.substr(start, pos - start) );
			start = pos + 1;
		}
		// parse up to next real data
		start = str.find_first_not_of(delims, start);
		++numSplits;

	} while (pos != std::string::npos);
	return ret;
}

MyConfig::MyConfig(const char *xmlFileName/*=0*/)
{
	//memset(this,0,sizeof(*this));
	if(xmlFileName!=NULL)
		LoadData(xmlFileName);
}

MyConfig & MyConfig::Instance()
{
	static MyConfig g_MyConfig("xml/MyConfig.xml");//配置缓冲，只读取了xml文件一次
	return g_MyConfig;
}

//返回值
//	0			成功
//	1			没有加载文件或打开文件失败
int MyConfig::LoadData(const char *xmlFileName)
{
	if(xmlFileName==NULL)
		return 1;

	tinyxml2::XMLDocument* m_xmlDoc = new tinyxml2::XMLDocument();
	CCLOG("new tinyxml2::XMLDocument()");
	ssize_t size;
	const char* pXmlBuffer = (const char*)CCFileUtils::sharedFileUtils()->getFileData(xmlFileName, "r", &size);
	//if (!m_xmlDoc->LoadFile(xmlFileName))
	if(!pXmlBuffer)
	{
		delete m_xmlDoc;
		CCLOG("pXmlBuffer==0");
		return 1;
	}
	else
	{
		CCLOG("pXmlBuffer!=0");
		m_xmlDoc->Parse(pXmlBuffer);
	    delete pXmlBuffer;
#ifdef WIN32
#define SetSTRING(Node,Attr) {const char*##Attr##=##Node##->Attribute(#Attr);if(##Attr##!=NULL)this->m_##Attr##=##Attr##;}
#define SetINT(Node,Attr) {const char*##Attr##=##Node##->Attribute(#Attr);if(##Attr##!=NULL)this->m_##Attr##=atoi(##Attr##);}
#define SetDOUBLE(Node,Attr) {const char*##Attr##=##Node##->Attribute(#Attr);if(##Attr##!=NULL)this->m_##Attr##=atof(##Attr##);}
#define SetPOINT(Node,Attr) {vector<string> vec##Attr##;const char*##Attr##=##Node##->Attribute(#Attr);if(##Attr##!=NULL)vec##Attr##=split(##Attr##,",");if(vec##Attr##.size()==2){char* pp=NULL;m_##Attr##[0]=strtod(vec##Attr##[0].c_str(),&pp);m_##Attr##[1]=strtod(vec##Attr##[1].c_str(),&pp);}}
#endif
		XMLElement *root = m_xmlDoc->FirstChildElement("config");
		if (root)
		{
			XMLElement* Node1a=root->FirstChildElement("address");
			if(Node1a)
			{
#ifdef WIN32				
				SetSTRING(Node1a,ip1)
				SetSTRING(Node1a,ip2)
				SetSTRING(Node1a,ip3)
				SetINT(Node1a,port)
#else
				CCLOG("Node1a!=0");
				SetString(Node1a,"ip1",m_ip1);
				SetString(Node1a,"ip2",m_ip2);
				SetString(Node1a,"ip3",m_ip3);
				SetInt(Node1a,"port",m_port);
#endif
			}
			XMLElement* Node1b=root->FirstChildElement("BJL");
			if(Node1b)
			{
				SetPoint(Node1b,"GameRecordPos",m_GameRecordPos);
				SetPoint(Node1b,"BankerUserPos",m_BankerUserPos);
                SetPoint(Node1b,"ClosePos",m_ClosePos);
				SetPoint(Node1b,"UpBankerPos",m_UpBankerPos);
				SetDouble(Node1b,"padding",m_padding);
				SetPoint(Node1b,"PaiZiPos",m_PaiZiPos);
				SetPoint(Node1b,"YanwuParticlePos",m_YanwuParticlePos);
				XMLElement* Node1b1=Node1b->FirstChildElement("GameRecord");
				if(Node1b1)
				{
					SetDouble(Node1b1,"fOffsetX",m_fOffsetX);
                    SetDouble(Node1b1,"fStartX",m_fStartX);
					SetDouble(Node1b1,"fOffsetY",m_fOffsetY);
                    SetDouble(Node1b1,"fStartY",m_fStartY);
				}
				XMLElement* Node1b2=Node1b->FirstChildElement("BankerUser");
				if(Node1b2)
				{				
					SetPoint(Node1b2,"BankerPicPos",m_BankerPicPos);
                    SetPoint(Node1b2,"BankerNamePos",m_BankerNamePos);
					SetPoint(Node1b2,"BankerChengjiPos",m_BankerChengjiPos);
                    SetPoint(Node1b2,"BankerGoldPos",m_BankerGoldPos);
					SetPoint(Node1b2,"BankerJiNumberPos",m_BankerJiNumberPos);
				}
				XMLElement* Node1b3=Node1b->FirstChildElement("GameStateTip");
				if(Node1b3)
				{
					SetPoint(Node1b3,"TimerPos",m_TimerPos);
                    SetPoint(Node1b3,"NikePos",m_NikePos);
					SetPoint(Node1b3,"GameFreeTipPos",m_GameFreeTipPos);
                    SetPoint(Node1b3,"GameStartTipPos",m_GameStartTipPos);
					SetPoint(Node1b3,"CallScoreTipPos",m_CallScoreTipPos);
                    SetPoint(Node1b3,"GameEndTipPos",m_GameEndTipPos);
					SetPoint(Node1b3,"ChangeBankerTipPos",m_ChangeBankerTipPos);
				}
				XMLElement* Node1b4=Node1b->FirstChildElement("MyUser");
				if(Node1b4)
				{
					SetPoint(Node1b4,"MyPicPos",m_MyPicPos);
                    SetPoint(Node1b4,"MyNamePos",m_MyNamePos);
					SetPoint(Node1b4,"MyChengjiPos",m_MyChengjiPos);
                    SetPoint(Node1b4,"MyGoldPos",m_MyGoldPos);
				}
				XMLElement* Node1b5=Node1b->FirstChildElement("CardManager");
				if(Node1b5)
				{
					SetPoint(Node1b5,"XianZhuangBgPos",m_XianZhuangBgPos);
					SetPoint(Node1b5,"UserPlayPos",m_UserPlayPos);
                    SetPoint(Node1b5,"UserBankPos",m_UserBankPos);
					SetPoint(Node1b5,"PlayerPos",m_PlayerPos);
                    SetPoint(Node1b5,"BankerPos",m_BankerPos);
					SetPoint(Node1b5,"AndBureauPos",m_AndBureauPos);
                    SetPoint(Node1b5,"GAMEoverPos1",m_GAMEoverPos1);
					SetPoint(Node1b5,"GAMEoverPos2",m_GAMEoverPos2);
                    SetPoint(Node1b5,"playerCardPos1",m_playerCardPos1);
					SetPoint(Node1b5,"PlayOneCardPos1",m_PlayOneCardPos1);
					SetPoint(Node1b5,"PlayOneCardPos2",m_PlayOneCardPos2);
					SetPoint(Node1b5,"BankOneCardPos1",m_BankOneCardPos1);
					SetPoint(Node1b5,"BankOneCardPos2",m_BankOneCardPos2);
					SetDouble(Node1b5,"CardOffsetX",m_CardOffsetX);
				}
				XMLElement* Node1b6=Node1b->FirstChildElement("GAMEover");
				if(Node1b6)
				{
					SetPoint(Node1b6,"Pos0",&m_GAMEoverLblPos[0][0]);
                    SetPoint(Node1b6,"Pos1",&m_GAMEoverLblPos[1][0]);
					SetPoint(Node1b6,"Pos2",&m_GAMEoverLblPos[2][0]);
                    SetPoint(Node1b6,"Pos3",&m_GAMEoverLblPos[3][0]);
					SetPoint(Node1b6,"Pos4",&m_GAMEoverLblPos[4][0]);
                    SetPoint(Node1b6,"Pos5",&m_GAMEoverLblPos[5][0]);
					SetPoint(Node1b6,"Pos6",&m_GAMEoverLblPos[6][0]);
                    SetPoint(Node1b6,"Pos7",&m_GAMEoverLblPos[7][0]);
					SetPoint(Node1b6,"Pos8",&m_GAMEoverLblPos[8][0]);
                    SetPoint(Node1b6,"Pos9",&m_GAMEoverLblPos[9][0]);
					SetPoint(Node1b6,"Pos10",&m_GAMEoverLblPos[10][0]);
				}
				XMLElement* Node1b7=Node1b->FirstChildElement("ChipManagerNunber");
				if(Node1b7)
				{
					SetPoint(Node1b7,"Pos0",&m_ChipManagerNunberPos[0][0]);
                    SetPoint(Node1b7,"Pos1",&m_ChipManagerNunberPos[1][0]);
					SetPoint(Node1b7,"Pos2",&m_ChipManagerNunberPos[2][0]);
                    SetPoint(Node1b7,"Pos3",&m_ChipManagerNunberPos[3][0]);
					SetPoint(Node1b7,"Pos4",&m_ChipManagerNunberPos[4][0]);
                    SetPoint(Node1b7,"Pos5",&m_ChipManagerNunberPos[5][0]);
					SetPoint(Node1b7,"Pos6",&m_ChipManagerNunberPos[6][0]);
                    SetPoint(Node1b7,"Pos7",&m_ChipManagerNunberPos[7][0]);
				}
				XMLElement* Node1b8=Node1b->FirstChildElement("ChipManagerNunber");
				if(Node1b8)
				{
					SetPoint(Node1b8,"Pos0",&m_OperaLayerMyNunberPos[0][0]);
                    SetPoint(Node1b8,"Pos1",&m_OperaLayerMyNunberPos[1][0]);
					SetPoint(Node1b8,"Pos2",&m_OperaLayerMyNunberPos[2][0]);
                    SetPoint(Node1b8,"Pos3",&m_OperaLayerMyNunberPos[3][0]);
					SetPoint(Node1b8,"Pos4",&m_OperaLayerMyNunberPos[4][0]);
                    SetPoint(Node1b8,"Pos5",&m_OperaLayerMyNunberPos[5][0]);
					SetPoint(Node1b8,"Pos6",&m_OperaLayerMyNunberPos[6][0]);
                    SetPoint(Node1b8,"Pos7",&m_OperaLayerMyNunberPos[7][0]);
				}
				XMLElement* Node1b9=Node1b->FirstChildElement("ChipPosRect");
				if(Node1b9)
				{
					SetRect(Node1b9,"Rect0",&m_ChipPosRect[0][0]);
                    SetRect(Node1b9,"Rect1",&m_ChipPosRect[1][0]);
					SetRect(Node1b9,"Rect2",&m_ChipPosRect[2][0]);
                    SetRect(Node1b9,"Rect3",&m_ChipPosRect[3][0]);
					SetRect(Node1b9,"Rect4",&m_ChipPosRect[4][0]);
                    SetRect(Node1b9,"Rect5",&m_ChipPosRect[5][0]);
					SetRect(Node1b9,"Rect6",&m_ChipPosRect[6][0]);
                    SetRect(Node1b9,"Rect7",&m_ChipPosRect[7][0]);
				}
				XMLElement* Node1b10=Node1b->FirstChildElement("ChipMenu");
				if(Node1b10)
				{
					SetDouble(Node1b10,"Scale",m_ChipMenuScale);
					SetPoint(Node1b10,"Pos0",&m_ChipMenuPos[0][0]);
                    SetPoint(Node1b10,"Pos1",&m_ChipMenuPos[1][0]);
					SetPoint(Node1b10,"Pos2",&m_ChipMenuPos[2][0]);
                    SetPoint(Node1b10,"Pos3",&m_ChipMenuPos[3][0]);
					SetPoint(Node1b10,"Pos4",&m_ChipMenuPos[4][0]);
                    SetPoint(Node1b10,"Pos5",&m_ChipMenuPos[5][0]);
					SetPoint(Node1b10,"Pos6",&m_ChipMenuPos[6][0]);
				}
				XMLElement* Node1b11=Node1b->FirstChildElement("BankerList");
				if(Node1b11)
				{
					SetInt(Node1b11,"NumberOfCells",m_NumberOfCells);
					SetPoint(Node1b11,"CellSize",m_CellSize);
                    SetPoint(Node1b11,"ViewSize",m_ViewSize);
					SetPoint(Node1b11,"ViewPos",m_ViewPos);
					SetInt(Node1b11,"ColumnNum",m_ColumnNum);
					if(m_ColumnNum>MAX_COLUMN_NUM)
						m_ColumnNum=MAX_COLUMN_NUM;
					for(int i=0;i<m_ColumnNum;i++){
					    SetPoint(Node1b11,g_LblPosAttrName[i],&m_LblPos[i][0]);
					}
				}
			}
		}
#ifdef WIN32
#undef SetSTRING
#undef SetINT
#undef SetDOUBLE
#undef SetPOINT
#endif
	}
	delete m_xmlDoc;
	this->m_HasLoadOK=true;
	return 0;	
}

void MyConfig::SetString(XMLElement* Node,const char * Attr,string& m_Attr)
{
	const char*pAttr=Node->Attribute(Attr);if(pAttr!=NULL)m_Attr=pAttr;
}
	
void MyConfig::SetInt(XMLElement* Node,const char * Attr,int& m_Attr)
{
	const char*pAttr=Node->Attribute(Attr);if(pAttr!=NULL)m_Attr=atoi(pAttr);
}

void MyConfig::SetPoint(XMLElement* Node,const char * Attr,float* m_pAttr)
{
	vector<string> vecPos;
	const char*Pos=Node->Attribute(Attr);
	if(Pos!=NULL)
	{
		vecPos=split(Pos,",");
	}
	if(vecPos.size()==2)
	{
		char* pp=NULL;	
		m_pAttr[0]=strtod(vecPos[0].c_str(),&pp);
		m_pAttr[1]=strtod(vecPos[1].c_str(),&pp);
	}
}

void MyConfig::SetRect(XMLElement* Node,const char * Attr,float* m_pAttr,bool isXYWH/*=false*/)
{
	vector<string> vecPos;
	const char*Pos=Node->Attribute(Attr);
	if(Pos!=NULL)
	{
		vecPos=split(Pos,",");
	}
	if(vecPos.size()==4)
	{
		char* pp=NULL;	
		m_pAttr[0]=strtod(vecPos[0].c_str(),&pp);
		m_pAttr[1]=strtod(vecPos[1].c_str(),&pp);
		m_pAttr[2]=strtod(vecPos[2].c_str(),&pp);
		m_pAttr[3]=strtod(vecPos[3].c_str(),&pp);
		if(!isXYWH)
		{
			//x,y,x+w,y+h转换为x,y,w,h
			m_pAttr[2]=m_pAttr[2]-m_pAttr[0];
			m_pAttr[3]=m_pAttr[3]-m_pAttr[1];
		}
	}
}

void MyConfig::SetDouble(XMLElement* Node,const char * Attr,float& m_Attr)
{
	const char*pAttr=Node->Attribute(Attr);if(pAttr!=NULL)m_Attr=atof(pAttr);	
}