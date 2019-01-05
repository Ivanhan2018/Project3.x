#include "ClipLayer.h"

ClipLayer* ClipLayer::create(CCSize size)
{
    ClipLayer * pCLayer = new ClipLayer();
    if(pCLayer && pCLayer->init())
	{
        pCLayer->autorelease();
        pCLayer->setClipSize(size);
        
        return pCLayer;
    }
    CC_SAFE_DELETE(pCLayer);
    return NULL;
}

bool ClipLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //ignoreAnchorPointForPosition(false);
    //setAnchorPoint(ccp(0.5f, 0.5f));
    
    return true;
}

void ClipLayer::setClipSize(CCSize size)
{
    m_clipSize = size;
    setContentSize(m_clipSize);
}

CCSize ClipLayer::getClipSize()
{
	return m_clipSize;
}

//void ClipLayer::visit()
//{
//    
//	kmGLPushMatrix();
//	
//    if (m_pGrid && m_pGrid->isActive())
//    {
//        m_pGrid->beforeDraw();
//        this->transformAncestors();
//    }
//    
//	this->transform();
//    
//    //默认情况，剪裁是禁用的
//    //启用剪裁测试
//    glEnable(GL_SCISSOR_TEST);
//    //当前layer缩放的倍数
//    float s = this->getScale();
//    //获取缩放倍率
//    s *= CCDirector::sharedDirector()->getContentScaleFactor();
//    CCPoint screenPos = this->getParent()->convertToWorldSpace(this->getPosition());
//    CCPoint a =  this->getParent()->getPosition();
//    //默认不设置Scissor的大小是整个视图的大小
//    // glScissor((GLint)screenPos.x, (GLint)screenPos.y, (GLsizei)(m_sizeClip.width*s), (GLsizei)(m_sizeClip.height*s));
//    CCEGLView::sharedOpenGLView()->setScissorInPoints(screenPos.x * s, screenPos.y * s, m_clipSize.width * s, m_clipSize.height * s);
//	if (m_pChildren && m_pChildren->count() > 0)
//    {
//        this->sortAllChildren();
//		ccArray *arrayData = m_pChildren->data;
//		unsigned int i = 0;
//		
//		for(; i < arrayData->num; i++ )
//        {
//			CCNode *child =  (CCNode*)arrayData->arr[i];
//			if (child->getZOrder() < 0)
//            {
//				child->visit();
//			}
//            else
//            {
//				break;
//            }
//		}
//        
//		this->draw();
//        
//		for(; i < arrayData->num; i++)
//        {
//			CCNode* child = (CCNode*)arrayData->arr[i];
//			child->visit();
//		}
//        
//	}
//    else
//    {
//		this->draw();
//    }
//    
//    //禁用剪裁
//    glDisable(GL_SCISSOR_TEST);
//    
//	if (m_pGrid && m_pGrid->isActive())
//    {
//		m_pGrid->afterDraw(this);
//    }
//    
//	kmGLPopMatrix();
//}