#pragma once
#include <array>
#include <memory>

// #include "GameMode/Impl/Regular.hpp"
#include "GameMode/Impl/1v1.hpp"
#include "GameMode/Impl/3v3.hpp"
#include "GameMode/Impl/4v4.hpp"

#include "GameMode/Impl/HardCore.hpp"

#include "GameMode/Impl/Boss.hpp"
#include "GameMode/Impl/Clone.hpp"

#include "GameMode/Impl/Deathmatch.hpp"
#include "GameMode/Impl/RandomDeathmatch.hpp"

extern GameMode s_GameMode;
extern std::array<std::unique_ptr<IGameModeHandler>, GameMode::__Internal_Max_Length> s_ModeHandlers;

inline GameMode getGameMode()
{
	return s_GameMode;
}

inline IGameModeHandler *getGameModeHandler()
{
	return s_ModeHandlers[static_cast<size_t>(s_GameMode)].get();
}
