/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 ****************************************************************************/

#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"

@implementation AppController

@synthesize window;

static AppDelegate s_sharedApplication;

-(void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
	NSRect rect = NSMakeRect(200, 200, (CGFloat)1280, (CGFloat)720);
	NSUInteger windowStyle = NSClosableWindowMask | NSMiniaturizableWindowMask |
		NSResizableWindowMask | NSTitledWindowMask;

	window = [[NSWindow alloc]
		initWithContentRect:rect
		styleMask:windowStyle
		backing:NSBackingStoreBuffered
		defer:YES];

	EAGLView *glView = [[EAGLView alloc]
		initWithFrame:NSMakeRect(0.0f, 0.0f, rect.size.width, rect.size.height)];

	[window setContentView:glView];
	[window becomeFirstResponder];
	[window setTitle:@"Naruto Senki"];
	[window makeKeyAndOrderFront:self];
	[window center];
	[window setAcceptsMouseMovedEvents:YES];

	// Must run GL view init before Lua / director access in AppDelegate.
	cocos2d::CCApplication::sharedApplication()->run();
}

-(BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

-(void) dealloc
{
	Director::sharedDirector()->end();
	[super dealloc];
}

#pragma mark -

-(IBAction) toggleFullScreen:(id)sender
{
	EAGLView *pView = [EAGLView sharedEGLView];
	[pView setFullScreen:!pView.isFullScreen];
}

-(IBAction) exitFullScreen:(id)sender
{
	[[EAGLView sharedEGLView] setFullScreen:NO];
}

@end
