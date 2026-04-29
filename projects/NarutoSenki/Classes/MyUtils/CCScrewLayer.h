#pragma once
#include "Defines.h"

class CCScrewLayer : public Layer
{
public:
	float prePosY;
	float minY = 154;
	int totalRow = 100;
	int line_height = 26;
	// PROP_Vector(vector<CCScrewItem *>, itemArray, ItemArray);

	CREATE_FUNC(CCScrewLayer);

protected:
	bool ccTouchBegan(Touch *touch, Event *event);
	void ccTouchMoved(Touch *touch, Event *event);
	void ccTouchEnded(Touch *touch, Event *event);

	void onEnter();
	void onExit();
};

class CCScrewItem : public Sprite, public CCTouchDelegate
{
public:
	Menu *linemenu;
	Sprite *lockItem;
	PROP(CCScrewLayer *, _delegate, Delegate);

	CREATE_FUNC(CCScrewItem);

protected:
	void onEnter();
	void onExit();
	bool ccTouchBegan(Touch *touch, Event *event);
	void ccTouchEnded(Touch *touch, Event *event);
	CCRect getRect2();

	inline bool containsTouchLocation(Touch *touch);
};
