#pragma once
#include "cocos2d.h"
#include "../../../scripting/lua/cocos2dx_support/CCLuaEngine.h"

using namespace cocos2d;

// CC prefix compatibility
typedef CCObject Ref;
typedef CCEvent Event;
typedef CCUserDefault UserDefault;
typedef CCAction Action;
typedef CCFiniteTimeAction FiniteTimeAction;
typedef CCTouch Touch;
typedef CCTexture2D Texture2D;
typedef CCNode Node;
typedef CCSpriteFrame SpriteFrame;
typedef CCAnimationFrame AnimationFrame;
typedef CCAnimation Animation;
typedef CCActionInterval ActionInterval;
typedef CCSequence Sequence;
typedef CCRepeat Repeat;
typedef CCRepeatForever RepeatForever;
typedef CCSpawn Spawn;
typedef CCRotateTo RotateTo;
typedef CCRotateBy RotateBy;
typedef CCMoveBy MoveBy;
typedef CCMoveTo MoveTo;
typedef CCJumpBy JumpBy;
typedef CCJumpTo JumpTo;
typedef CCScaleTo ScaleTo;
typedef CCScaleBy ScaleBy;
typedef CCFadeIn FadeIn;
typedef CCFadeOut FadeOut;
typedef CCDelayTime DelayTime;
typedef CCAnimate Animate;
typedef CCEaseIn EaseIn;
typedef CCEaseOut EaseOut;
typedef CCEaseInOut EaseInOut;
typedef CCProgressTo ProgressTo;
typedef CCDirector Director;
typedef CCSprite Sprite;
typedef CCSpriteBatchNode SpriteBatchNode;
typedef CCLayer Layer;
typedef CCLayerColor LayerColor;
typedef CCScene Scene;
typedef CCTransitionFade TransitionFade;
typedef CCMenuItem MenuItem;
typedef CCMenuItemSprite MenuItemSprite;
typedef CCMenu Menu;
typedef CCClippingNode ClippingNode;
typedef CCProgressTimer ProgressTimer;
typedef CCRenderTexture RenderTexture;
typedef CCFileUtils FileUtils;
typedef CCApplication Application;
typedef CCAnimationCache AnimationCache;
typedef CCSpriteFrameCache SpriteFrameCache;
typedef CCTextureCache TextureCache;
typedef CCTMXObjectGroup TMXObjectGroup;
typedef CCTMXTiledMap TMXTiledMap;
typedef CCEGLView GLView;
typedef CCPoint Vec2;
// macOS system headers already define global `Size`/`Rect` typedefs.
// Avoid redefining them on macOS to prevent conflicts with MacTypes.
#if CC_TARGET_PLATFORM != CC_PLATFORM_MAC
typedef CCSize Size;
typedef CCRect Rect;
#endif
typedef ccColor3B Color3B;
typedef CCLuaEngine LuaEngine;
