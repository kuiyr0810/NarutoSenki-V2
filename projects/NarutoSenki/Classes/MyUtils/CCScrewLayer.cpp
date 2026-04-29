#include "CCScrewLayer.h"

void CCScrewLayer::onEnter()
{
	Layer::onEnter();
	Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

void CCScrewLayer::onExit()
{
	Layer::onExit();
	Director::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CCScrewLayer::ccTouchBegan(Touch *touch, Event *event)
{
	prePosY = 0;
	return true;
}

void CCScrewLayer::ccTouchMoved(Touch *touch, Event *event)
{
	// touch area
	Vec2 curPoint = touch->getLocation();
	if (prePosY == 0)
	{
		prePosY = curPoint.y;
	}
	else
	{
		float distanceY = curPoint.y - prePosY;
		if (getPositionY() < totalRow * line_height || distanceY < 0)
		{
			setPositionY(getPositionY() + distanceY);
		}

		prePosY = curPoint.y;
	}
}

void CCScrewLayer::ccTouchEnded(Touch *touch, Event *event)
{
	prePosY = 0;

	// CCLOG("y:%f",getPositionY());
	if (getPositionY() > totalRow * line_height - line_height)
	{
		setPositionY(totalRow * line_height);
	}
	if (getPositionY() < minY)
	{
		setPositionY(minY);
	}
}

/*----------------------
init CCScrewItem ;
----------------------*/

void CCScrewItem::onEnter()
{
	Sprite::onEnter();
	Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void CCScrewItem::onExit()
{
	Sprite::onExit();
	Director::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

CCRect CCScrewItem::getRect2()
{
	CCSize size = getContentSize();
	return CCRect(0, 0, size.width, size.height);
}

bool CCScrewItem::containsTouchLocation(Touch *touch)
{
	return getRect2().containsPoint(convertTouchToNodeSpace(touch));
}

bool CCScrewItem::ccTouchBegan(Touch *touch, Event *event)
{
	// touch area
	if (!containsTouchLocation(touch))
		return false;

	return true;
}

void CCScrewItem::ccTouchEnded(Touch *touch, Event *event)
{
	linemenu->setVisible(true);
}
