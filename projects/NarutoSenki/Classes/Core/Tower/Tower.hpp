#pragma once
#include "CharacterBase.h"
#include "Core/Warrior/Flog.hpp"
#include "Core/Utils/UnitEx.hpp"
#include "HPBar.h"
#include "HudLayer.h"

class Tower : public CharacterBase
{
public:
	CREATE_FUNC(Tower);

	bool init()
	{
		RETURN_FALSE_IF(!Sprite::init());

		setAnchorPoint(Vec2(0.5, 0.5));
		scheduleUpdate();

		return true;
	}

	void setID(const string &name, Role role, Group group)
	{
		clearActionData();
		setName(name);
		setRole(role);
		setGroup(group);

		CCArray *animationArray = CCArray::create();
		const char *filePath;

		if (getName() == TowerEnum::KonohaCenter || getName() == TowerEnum::AkatsukiCenter)
			filePath = "Unit/Tower/CenterData.xml";
		else
			filePath = "Unit/Tower/TowerData.xml";

		KTools::readXMLToArray(filePath, animationArray);

		CCArray *tmpAction = (CCArray *)(animationArray->objectAtIndex(0));
		CCArray *tmpData = (CCArray *)(tmpAction->objectAtIndex(0));
		idleArray = (CCArray *)(tmpAction->objectAtIndex(1));

		string unitName;
		uint32_t maxHP;
		int tmpWidth;
		int tmpHeight;
		uint32_t tmpSpeed;
		int tmpCombatPoint;

		readData(tmpData, unitName, maxHP, tmpWidth, tmpHeight, tmpSpeed, tmpCombatPoint);

		setMaxHPValue(maxHP, false);
		setHPValue(maxHP, false);
		setCKR(0);
		setCKR2(0);
		setHeight(tmpHeight);
		setWalkSpeed(tmpSpeed);

		// init DeadFrame
		tmpAction = (CCArray *)(animationArray->objectAtIndex(6));
		deadArray = (CCArray *)(tmpAction->objectAtIndex(1));

		initAction();
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CharacterBase::acceptAttack), "acceptAttack", nullptr);
	}

	void initAction()
	{
		setIdleAction(createAnimation(idleArray, 5, true, false));
		setDeadAction(createAnimation(deadArray, 10, false, false));
	}

	void setHPbar()
	{
		auto layer = getGameLayer();
		auto player = layer ? layer->currentPlayer : nullptr;
		if (!player || getGroup() != player->getGroup())
			_hpBar = HPBar::create("hp_bar_r.png");
		else
			_hpBar = HPBar::create("hp_bar.png");
		_hpBar->getHPBAR()->setPosition(Vec2(1, 1));
		_hpBar->setPositionY(getHeight());
		_hpBar->setDelegate(this);
		addChild(_hpBar);
	}

	void dealloc()
	{
		unschedule(schedule_selector(CharacterBase::setAI));
		setState(State::DEAD);
		stopAllActions();

		getGameLayer()->clearAllFlogsMainTarget(this);

		std::erase(getGameLayer()->_TowerArray, this);
		getGameLayer()->setTowerState(getCharId());
		getGameLayer()->checkTower();
		removeFromParent();
	}
};
