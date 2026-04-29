#include "ActionButton.h"
#include "CharacterBase.h"
#include "HudLayer.h"

ActionButton::ActionButton()
{
	_isDoubleSkill = false;
	_freezeAction = nullptr;
	markSprite = nullptr;
	ougismarkSprite = nullptr;
	_clickTime = 0;
	_clickNum = 0;
	_isMarkVisable = true;
	_isLock = false;
	_isColdChanged = false;
	clipper = nullptr;
	_cost = nullptr;
	_gearType = GearType::None;
	cdLabel = nullptr;
	lockLabel1 = nullptr;
	lockLabel2 = nullptr;

	proressblinkSprite = nullptr;
	proressmarkSprite = nullptr;
	proressblinkSprite = nullptr;
	progressPointSprite = nullptr;
	proressblinkMask = nullptr;
	gearSign = nullptr;
}

bool ActionButton::init(const string &szImage)
{
	RETURN_FALSE_IF(!Sprite::init());

	initWithSpriteFrameName(szImage.c_str());
	setAnchorPoint(Vec2(0, 0));

	return true;
}

void ActionButton::onEnter()
{
	Sprite::onEnter();
	Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -50, true);
}

void ActionButton::onExit()
{
	Sprite::onExit();
	Director::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

CCRect ActionButton::getRect()
{
	CCSize size = getContentSize();
	return CCRect(0, 0, size.width, size.height);
}

bool ActionButton::containsTouchLocation(Touch *touch)
{
	return getRect().containsPoint(convertTouchToNodeSpace(touch));
}

bool ActionButton::ccTouchBegan(Touch *touch, Event *event)
{
	if (!containsTouchLocation(touch) || _delegate->_isAllButtonLocked)
	{
		return false;
	}
	click();

	return true;
}

void ActionButton::ccTouchEnded(Touch *touch, Event *event)
{
	if (_abType == NAttack)
		_delegate->attackButtonRelease();
}

void ActionButton::click()
{
	if (_delegate && isCanClick())
	{
		if (!_isDoubleSkill)
		{
			beganAnimation();
		}

		if (!_delegate->_isAllButtonLocked)
		{
			if (_abType == GearItem)
			{
				_delegate->gearButtonClick(_gearType);
			}
			else
			{
				_delegate->attackButtonClick(_abType);
			}
		}
	}
}

bool ActionButton::isCanClick()
{
	// recorde current time sec format;

	// cc_timeval timeVal;
	// CCTime::gettimeofdayCocos2d(&timeVal, 0);
	// float currTime = timeVal.tv_sec + timeVal.tv_usec / 1000;

	if (_abType != NAttack)
	{
		if (_isDoubleSkill)
		{
			// double click solution
			if (_clickNum == 0 && _delegate->getSkillFinish() && getTimeCount() == 0 && !_delegate->ougisLayer)
			{
				return true;
			}
			else if (_clickNum == 1 && _delegate->getSkillFinish() && !_delegate->ougisLayer)
			{
				return true;
			}
		}
		else
		{
			// isSkillFinish consider the AttackAction is done or not to prevent the skill invalid release
			if (_abType == Item1)
			{
				if (!_delegate->ougisLayer && getTimeCount() == 0 && !_isLock && getGameLayer()->currentPlayer->getState() != State::DEAD)
				{
					if (_delegate->offCoin(_cost))
					{
						return true;
					}
				}
			}
			else if (_abType == GearBtn)
			{
				if (!_isLock)
				{
					getGameLayer()->onGear();
				}
				return false;
			}
			else if (_abType == GearItem)
			{
				if (!_delegate->ougisLayer && getTimeCount() == 0 && !_isLock)
				{
					if (_gearType == GearType::Gear06 && getGameLayer()->currentPlayer->getState() != State::DEAD)
					{
						return true;
					}
					else if (_gearType == GearType::Gear00 && _delegate->getSkillFinish())
					{
						return true;
					}
					else if (_gearType == GearType::Gear03)
					{
						return true;
					}
				}
			}
			// ougis click solution
			else if (_abType == OUGIS1)
			{
				if (_delegate->getSkillFinish() && _delegate->getOugisEnable(false) && !_isLock && !_delegate->ougisLayer)
				{
					_delegate->costCKR(15000, false);
					return true;
				}
			}
			else if (_abType == OUGIS2)
			{
				if (_delegate->getSkillFinish() && _delegate->getOugisEnable(true) && !_isLock && !_delegate->ougisLayer)
				{
					_delegate->costCKR(25000, true);
					return true;
				}
			}
			else
			{
				if (getTimeCount() == 0 && _delegate->getSkillFinish() && !_isLock && !_delegate->ougisLayer)
				{
					return true;
				}
			}
		}

		return false;
	}
	else
	{
		return true;
	}
}

void ActionButton::beganAnimation(bool isLock)
{
	// record the click time
	cc_timeval timeVal;
	CCTime::gettimeofdayCocos2d(&timeVal, 0);
	_clickTime = timeVal.tv_sec + timeVal.tv_usec / 1000;

	if (markSprite && _abType != OUGIS1 && _abType != OUGIS2)
	{
		if (!_freezeAction || _isColdChanged)
		{
			if (_isColdChanged)
			{
				_isColdChanged = false;
			}
			createFreezeAnimation();
		}

		setTimeCount(getCD());
		if (cdLabel)
		{
			cdLabel->removeFromParent();
			unschedule(schedule_selector(ActionButton::updateCDLabel));
		}
		if (_abType != Item1)
		{
			cdLabel = CCLabelBMFont::create(to_cstr(getCD() / 1000), Fonts::Default);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
			cdLabel->setScale(0.3f);
			cdLabel->setPosition(Vec2(getPositionX() + getContentSize().width * getScale() / 2,
									  getPositionY() + getContentSize().height * getScale() / 2));
#else
			cdLabel->setScale(0.4f);
			cdLabel->setPosition(Vec2(getPositionX() + getContentSize().width / 2,
									  getPositionY() + getContentSize().height / 2));
#endif

			_delegate->addChild(cdLabel, 200);
		}
		schedule(schedule_selector(ActionButton::updateCDLabel), 1.0f);

		markSprite->stopAllActions();
		markSprite->runAction(_freezeAction);
	}
}

void ActionButton::setGearType(GearType type)
{
	auto gearIcon = Sprite::createWithSpriteFrameName(format("gear_{:02d}.png", (int)type).c_str());
	gearIcon->setScale(0.85f);
	gearIcon->setPosition(Vec2(18, 18));
	addChild(gearIcon);
	_gearType = type;

	if (gearSign)
	{
		gearSign->removeFromParent();
		gearSign = nullptr;
	}
}

void ActionButton::updateCDLabel(float dt)
{
	if (!_delegate->ougisLayer)
	{
		if (getTimeCount() > 1000)
		{
			int tempCount = getTimeCount() - 1000;
			setTimeCount(tempCount);
			if (cdLabel)
			{
				cdLabel->setString(to_cstr(tempCount / 1000));
			}
		}
		else
		{
			unschedule(schedule_selector(ActionButton::updateCDLabel));
			setTimeCount(0);
			if (cdLabel)
			{
				cdLabel->removeFromParent();
				cdLabel = nullptr;
			}
		}
	}
}

void ActionButton::setMarkSprite(const char *mark)
{
	auto tmpSprite = Sprite::createWithSpriteFrameName(mark);
	markSprite = ProgressTimer::create(tmpSprite);
	markSprite->setType(kCCProgressTimerTypeRadial);

	markSprite->setReverseDirection(true);
	markSprite->setPosition(getPosition());
	markSprite->setAnchorPoint(Vec2(0, 0));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#else
	markSprite->setScale(DESKTOP_UI_MASK_SCALE);
#endif
	_delegate->addChild(markSprite, 500);

	if (_abType == GearBtn)
	{
		if (getGameLayer()->_enableGear)
		{
			gearSign = Sprite::createWithSpriteFrameName("gearsign.png");
			gearSign->setPosition(Vec2(getPositionX() + 17, getPositionY() + 17));
			_delegate->addChild(gearSign, 500);
		}
	}
}

void ActionButton::setOugisMark()
{
	ougismarkSprite = Sprite::createWithSpriteFrameName("skill_freeze.png");
	ougismarkSprite->setPosition(getPosition());
	ougismarkSprite->setAnchorPoint(Vec2(0, 0));
	_delegate->addChild(ougismarkSprite, 500);
	if (_abType == OUGIS1)
	{
		lockLabel1 = CCLabelBMFont::create("LV2", Fonts::Default);
	}
	else
	{
		lockLabel1 = CCLabelBMFont::create("LV4", Fonts::Default);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	ougismarkSprite->setScale(DESKTOP_UI_SCALE);

	lockLabel1->setScale(0.3f);
	lockLabel1->setPosition(Vec2(getPositionX() + getContentSize().width * getScale() / 2,
								 getPositionY() + getContentSize().height * getScale() / 2));
#else
	lockLabel1->setScale(0.4f);
	lockLabel1->setPosition(Vec2(getPositionX() + getContentSize().width / 2,
								 getPositionY() + getContentSize().height / 2));
#endif
	_delegate->addChild(lockLabel1, 200);
}

void ActionButton::setProgressMark()
{
	clipper = ClippingNode::create();
	auto stencil = Sprite::createWithSpriteFrameName("icon_bg1.png");
	stencil->setAnchorPoint(Vec2(0, 0));
	clipper->setStencil(stencil);

	proressmarkSprite = Sprite::createWithSpriteFrameName("icon_bg2.png");

	clipper->setPosition(getPosition());
	clipper->addChild(proressmarkSprite);

	proressmarkSprite->setPosition(Vec2(proressmarkSprite->getContentSize().width / 2, proressmarkSprite->getContentSize().height / 2));
	// 50,120,180

	_delegate->addChild(clipper, -50);

	proressblinkSprite = Sprite::createWithSpriteFrameName("icon_bg3.png");
	proressblinkSprite->setPosition(getPosition());
	proressblinkSprite->setPosition(Vec2(proressmarkSprite->getContentSize().width / 2, proressmarkSprite->getContentSize().height / 2));

	auto fd = FadeOut::create(0.2f);
	proressblinkSprite->runAction(RepeatForever::create(fd));
	clipper->addChild(proressblinkSprite, 50);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	clipper->setScale(0.8f);
#endif

	if (_abType == OUGIS1)
	{
		progressPointSprite = Sprite::createWithSpriteFrameName("icon_bg4.png");
		proressmarkSprite->setRotation(-50);
		proressblinkSprite->setRotation(-50);
		progressPointSprite->setPosition(getPosition());
	}
	else
	{
		progressPointSprite = Sprite::createWithSpriteFrameName("icon_bg5.png");
		proressmarkSprite->setRotation(-85);
		proressblinkSprite->setRotation(-85);
		progressPointSprite->setPosition(Vec2(getPositionX() + 1, getPositionY()));
	}

	progressPointSprite->setAnchorPoint(Vec2(0, 0));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	progressPointSprite->setScale(0.8f);
#endif
	_delegate->addChild(progressPointSprite, -25);

	auto fd2 = FadeOut::create(0.5f);
	proressblinkMask = Sprite::createWithSpriteFrameName("icon_bg6.png");
	proressblinkMask->setPosition(getPosition());
	proressblinkMask->setAnchorPoint(Vec2(0, 0));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	proressblinkMask->setScale(0.8f);
#endif
	proressblinkMask->runAction(RepeatForever::create(fd2));

	_delegate->addChild(proressblinkMask, 200);

	if (_abType == OUGIS2)
	{
		if (_delegate->skill4Button)
		{
			if (_delegate->skill4Button->proressblinkMask && !_delegate->skill4Button->_isLock)
			{
				_delegate->skill4Button->proressblinkMask->stopAllActions();
				auto fd3 = FadeOut::create(0.5f);
				_delegate->skill4Button->proressblinkMask->runAction(RepeatForever::create(fd3));
			}
		}
	}
}

void ActionButton::updateProgressMark()
{
	// NOTE: Using uint32_t will get wrong results.
	// uint32_t ckr = 100;
	// float result = -1    * ckr; //=> 4294967196
	// float result = -1.0f * ckr; //=> -100
	int ckr = getGameLayer()->currentPlayer->getCKR();
	int ckr2 = getGameLayer()->currentPlayer->getCKR2();

	if (_abType == OUGIS1)
	{
		if (ckr < 15000)
		{
			if (proressblinkSprite)
				proressblinkSprite->setRotation(0);

			if (proressmarkSprite)
				proressmarkSprite->setRotation(-1.0f * ((ckr / 15000.0f) * 50));

			if (proressblinkMask)
				proressblinkMask->setVisible(false);
		}
		else if (ckr < 30000)
		{
			if (proressblinkSprite)
				proressblinkSprite->setRotation(-50);

			if (proressmarkSprite)
				proressmarkSprite->setRotation(-1.0f * (((ckr - 15000) / 15000.0f) * 70 + 50));

			if (proressblinkMask && !_isLock)
			{
				proressblinkMask->setVisible(true);
				proressblinkMask->stopAllActions();
				auto fd = FadeOut::create(0.5f);
				proressblinkMask->runAction(RepeatForever::create(fd));

				if (_delegate->skill5Button)
				{
					if (_delegate->skill5Button->proressblinkMask && !_delegate->skill5Button->_isLock)
					{
						_delegate->skill5Button->proressblinkMask->stopAllActions();
						auto fd2 = FadeOut::create(0.5f);
						_delegate->skill5Button->proressblinkMask->runAction(RepeatForever::create(fd2));
					}
				}
			}
		}
		else if (ckr < 45000)
		{
			if (proressblinkSprite)
				proressblinkSprite->setRotation(-120);

			if (proressmarkSprite)
				proressmarkSprite->setRotation(-1.0f * (((ckr - 30000) / 15000.0f) * 60 + 120));
		}
		else
		{
			if (proressblinkSprite)
				proressmarkSprite->setRotation(-180);

			if (proressblinkSprite)
				proressblinkSprite->setRotation(-180);
		}
	}
	else
	{
		if (ckr2 < 25000)
		{
			if (proressblinkSprite)
				proressblinkSprite->setRotation(0);

			if (proressmarkSprite)
				proressmarkSprite->setRotation(-1.0f * ((ckr2 / 25000.0f) * 85));

			if (proressblinkMask)
				proressblinkMask->setVisible(false);
		}
		else if (ckr2 < 50000)
		{
			if (proressblinkSprite)
				proressblinkSprite->setRotation(-85);

			if (proressmarkSprite)
				proressmarkSprite->setRotation(-1.0f * (((ckr2 - 25000) / 25000.0f) * 90 + 90));

			if (proressblinkMask && !_isLock)
			{
				proressblinkMask->setVisible(true);
				proressblinkMask->stopAllActions();
				auto fd = FadeOut::create(0.5f);
				proressblinkMask->runAction(RepeatForever::create(fd));

				if (_delegate->skill4Button)
				{
					if (_delegate->skill4Button->proressblinkMask && !_delegate->skill4Button->_isLock)
					{
						_delegate->skill4Button->proressblinkMask->stopAllActions();
						auto fd2 = FadeOut::create(0.5f);
						_delegate->skill4Button->proressblinkMask->runAction(RepeatForever::create(fd2));
					}
				}
			}
		}
		else
		{
			if (proressmarkSprite)
				proressmarkSprite->setRotation(-180);

			if (proressblinkSprite)
				proressblinkSprite->setRotation(-180);
		}
	}
}

void ActionButton::reset()
{
	clearClick();

	if (!_isLock && markSprite)
	{
		markSprite->stopAllActions();
		markSprite->setPercentage(0);
	}

	if (!_delegate->ougisLayer)
	{
		unschedule(schedule_selector(ActionButton::updateCDLabel));
		setTimeCount(0);
		if (cdLabel)
		{
			cdLabel->removeFromParent();
			cdLabel = nullptr;
		}
	}
}

void ActionButton::setLock()
{
	if (markSprite)
	{
		markSprite->stopAllActions();
		markSprite->setPercentage(100);
	}
	else
	{
		setMarkSprite("skill_freeze.png");
	}

	if (proressblinkMask)
	{
		proressblinkMask->setVisible(false);
	}
	_isLock = true;
}

void ActionButton::unLock()
{
	if (markSprite)
	{
		markSprite->stopAllActions();
		markSprite->setPercentage(0);
	}

	uint32_t ckr = getGameLayer()->currentPlayer->getCKR();
	uint32_t ckr2 = getGameLayer()->currentPlayer->getCKR2();

	if (_abType == OUGIS1)
	{
		if (ckr >= 15000)
		{
			if (proressblinkMask)
			{
				proressblinkMask->setVisible(true);
			}
		}
	}
	else if (_abType == OUGIS2)
	{
		if (ckr2 >= 25000)
		{
			if (proressblinkMask)
			{
				proressblinkMask->setVisible(false);
			}
		}
	}

	_isLock = false;
}

void ActionButton::createFreezeAnimation()
{
	auto to = ProgressTo::create(0, 99.999f);

	int delay = getCD() / 1000;
	auto to1 = ProgressTo::create(delay, 0);

	Action *freezeAction;
	if (_isDoubleSkill)
	{
		auto callback = CallFunc::create(std::bind(&ActionButton::clearClick, this));
		freezeAction = newSequence(to, to1, callback);
	}
	else
	{
		freezeAction = newSequence(to, to1);
	}

	setFreezeAction(freezeAction);
}

void ActionButton::clearClick()
{
	_clickNum = 0;
}

void ActionButton::clearOugisMark()
{
	if (ougismarkSprite)
	{
		ougismarkSprite->removeAllChildrenWithCleanup(true);
	}

	if (clipper)
	{
		clipper->removeFromParent();
	}
	if (progressPointSprite)
	{
		progressPointSprite->removeFromParent();
	}
	if (proressblinkMask)
	{
		proressblinkMask->removeFromParent();
	}
}

ActionButton *ActionButton::create(const string &szImage)
{
	ActionButton *ab = new ActionButton();
	if (ab && ab->init(szImage))
	{
		ab->autorelease();
		return ab;
	}
	else
	{
		delete ab;
		return nullptr;
	}
}
