#pragma once
#include "GameMode/IGameModeHandler.hpp"

class Mode1v1 : public IGameModeHandler
{
private:
	bool isAddCallback;

public:
	void init()
	{
		CCLOG("Enter 1 VS 1 mode.");

		isAddCallback = false;

		gd.isHardCore = true;
	}

	void onInitHeros()
	{
		initHeros(1, 1);
	}

	void onGameStart()
	{
	}

	void onGameOver()
	{
	}

	void onCharacterInit(CharacterBase *c)
	{
		if (!isAddCallback && !getGameLayer()->isHUDInit())
		{
			// NOTE: Following code won't get correct value
			// Because this function `onCharacterInit` is not in current stack
			// So this value always nullptr or wrong value
			//
			// getGameLayer()->onHUDInitialized(
			// 	[&c]()
			// 	{
			// 		c->setEXP(2500);
			// 		for (int i = 1; i < 6; i++)
			// 			c->changeHPbar();
			// 		c->increaseAllCkrs(50000);
			// 	});
			isAddCallback = true;
			auto layer = getGameLayer();
			layer->onHUDInitialized(
				[layer]()
				{
					if (!layer)
						return;
					for (auto hero : layer->_CharacterArray)
					{
						hero->setCoin(3000);
						hero->setEXP(2500);
						for (int i = 1; i < 6; i++)
							hero->changeHPbar();
						hero->setHPValue(hero->getMaxHP());
						hero->increaseAllCkrs(25000);
						hero->setRebornTime(10);

						if (hero->isPlayer())
						{
							layer->getHudLayer()->setEXPLose();
							layer->getHudLayer()->coinLabel->setString(to_cstr(hero->getCoin()));
							if (!hero->isEnableSkill04())
								layer->getHudLayer()->skill4Button->setLock();
							if (!hero->isEnableSkill05())
								layer->getHudLayer()->skill5Button->setLock();
						}
					}
				});
		}
	}

	void onCharacterDead(CharacterBase *c)
	{
	}

	void onCharacterReborn(CharacterBase *c)
	{
	}
};
