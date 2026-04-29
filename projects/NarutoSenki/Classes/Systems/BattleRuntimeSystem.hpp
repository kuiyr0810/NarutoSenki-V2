#pragma once

class GameLayer;

class BattleRuntimeSystem
{
public:
	void onGameStart(GameLayer *layer, bool skipInitFlogs, float flogSpawnDuration) const;
	void updateGameTime(GameLayer *layer) const;
	void updateViewPoint(GameLayer *layer) const;
};
