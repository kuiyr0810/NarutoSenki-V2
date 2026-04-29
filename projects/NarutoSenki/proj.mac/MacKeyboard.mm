#import <Cocoa/Cocoa.h>
#import "CCEventDispatcher.h"
#include "GameLayer.h"

@interface MacKeyboardDelegate : NSObject <CCKeyboardEventDelegate>
@end

@implementation MacKeyboardDelegate

- (BOOL)ccKeyDown:(NSEvent *)event
{
    int keyCode = (int)[event keyCode];
    GameLayer::keyEventHandle(keyCode, 1);
    return NO;
}

- (BOOL)ccKeyUp:(NSEvent *)event
{
    int keyCode = (int)[event keyCode];
    GameLayer::keyEventHandle(keyCode, 0);
    return NO;
}

@end

static MacKeyboardDelegate *s_keyboardDelegate = nil;

#ifdef __cplusplus
extern "C" {
#endif

void MacKeyboard_register()
{
    if (!s_keyboardDelegate)
        s_keyboardDelegate = [[MacKeyboardDelegate alloc] init];
    [[CCEventDispatcher sharedDispatcher] addKeyboardDelegate:s_keyboardDelegate priority:0];
}

void MacKeyboard_unregister()
{
    if (s_keyboardDelegate)
        [[CCEventDispatcher sharedDispatcher] removeKeyboardDelegate:s_keyboardDelegate];
}

#ifdef __cplusplus
}
#endif
