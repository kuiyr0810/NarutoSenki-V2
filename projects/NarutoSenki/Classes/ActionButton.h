#pragma once
#include "Defines.h"

class HudLayer;

class ActionButton : public Sprite, public CCTouchDelegate
{
public:
	ActionButton();

	int _clickNum;
	float _clickTime;
	ProgressTimer *markSprite;
	Sprite *ougismarkSprite;
	Sprite *proressmarkSprite;
	Sprite *proressblinkSprite;
	Sprite *progressPointSprite;
	Sprite *proressblinkMask;
	const char *_cost;
	ClippingNode *clipper;
	bool _isLock;

	bool _isColdChanged;

	bool init(const string &szImage);
	bool isCanClick();
	CCRect getRect();

	void click();
	void setMarkSprite(const char *mark);
	void setOugisMark();
	void setProgressMark();
	void updateProgressMark();
	void reset();

	Sprite *gearSign;
	CCLabelBMFont *cdLabel;

	CCLabelBMFont *lockLabel1;
	CCLabelBMFont *lockLabel2;

	bool _isMarkVisable;

	PROP(ABType, _abType, ABType);
	GearType _gearType;

	PROP_UInt(_cooldown, CD);
	PROP(bool, _isDoubleSkill, DoubleSkill);
	PROP_UInt(_timeCount, TimeCount);

	void setLock();
	void unLock();
	void clearOugisMark();
	void setGearType(GearType type);

	PROP_PTR(Action, _freezeAction, FreezeAction);
	PROP(HudLayer *, _delegate, Delegate);
	void beganAnimation(bool isLock = false);
	void updateCDLabel(float dt);

	static ActionButton *create(const string &szImage);

protected:
	void onEnter();
	void onExit();
	bool ccTouchBegan(Touch *touch, Event *event);
	void ccTouchEnded(Touch *touch, Event *event);

	void createFreezeAnimation();
	void clearClick();

	bool containsTouchLocation(Touch *touch);
};
