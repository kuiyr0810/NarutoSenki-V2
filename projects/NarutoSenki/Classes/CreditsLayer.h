#pragma once
#include "Defines.h"

class CreditsLayer : public Layer
{
public:
	bool init();

	CREATE_FUNC(CreditsLayer);

private:
	void onEnterTransitionDidFinish();

	void onReturnBtn(Ref *sender);
	void keyBackClicked();
};
