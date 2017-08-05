/*
 * DemoViewController.mm
 *
 * Copyright (c) 2014-2017 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Use of this document is governed by the Molten License Agreement, as included
 * in the Molten distribution package. CAREFULLY READ THAT LICENSE AGREEMENT BEFORE
 * READING AND USING THIS DOCUMENT. BY READING OR OTHERWISE USING THIS DOCUMENT,
 * YOU ACCEPT AND AGREE TO BE BOUND BY THE TERMS AND CONDITIONS OF THAT LICENSE
 * AGREEMENT. IF YOU DO NOT ACCEPT THE TERMS AND CONDITIONS OF THAT LICENSE AGREEMENT,
 * DO NOT READ OR USE THIS DOCUMENT.
 */

#import "DemoViewController.h"
#import <QuartzCore/CAMetalLayer.h>

#include "ShellMVK.h"
#include "Hologram.h"


#pragma mark -
#pragma mark DemoViewController

@implementation DemoViewController {
	CVDisplayLinkRef	_displayLink;
	ShellMVK* _shell;
    Game* _game;
}

-(void) dealloc {
    delete _shell;
    delete _game;
	CVDisplayLinkRelease(_displayLink);
	[super dealloc];
}

/** Since this is a single-view app, initialize Vulkan during view loading. */
-(void) viewDidLoad {
	[super viewDidLoad];

	self.view.wantsLayer = YES;		// Back the view with a layer created by the makeBackingLayer method.

    std::vector<std::string> args;
//  args.push_back("-p");           // Uncomment to use push constants
//  args.push_back("-s");           // Uncomment to use a single thread
    _game = new Hologram(args);

    _shell = new ShellMVK(*_game);
    _shell->run(self.view);

	CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
	CVDisplayLinkSetOutputCallback(_displayLink, &DisplayLinkCallback, _shell);
	CVDisplayLinkStart(_displayLink);
}


#pragma mark Display loop callback function

/** Rendering loop callback function for use with a CVDisplayLink. */
static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink,
									const CVTimeStamp* now,
									const CVTimeStamp* outputTime,
									CVOptionFlags flagsIn,
									CVOptionFlags* flagsOut,
									void* target) {
   ((ShellMVK*)target)->update_and_draw();
	return kCVReturnSuccess;
}

-(void) viewDidAppear {
    self.view.window.initialFirstResponder = self.view;
}

// Delegated from the view as first responder.
-(void) keyDown:(NSEvent*) theEvent {
    Game::Key key;
    switch (theEvent.keyCode) {
        case 53:
            key = Game::KEY_ESC;
            break;
        case 126:
            key = Game::KEY_UP;
            break;
        case 125:
            key = Game::KEY_DOWN;
            break;
        case 49:
            key = Game::KEY_SPACE;
            break;
        case 3:
            key = Game::KEY_F;
            break;
        default:
            key = Game::KEY_UNKNOWN;
            break;
    }

    _game->on_key(key);
}

@end


#pragma mark -
#pragma mark DemoView

@implementation DemoView

/** Indicates that the view wants to draw using the backing layer instead of using drawRect:.  */
-(BOOL) wantsUpdateLayer { return YES; }

/** Returns a Metal-compatible layer. */
+(Class) layerClass { return [CAMetalLayer class]; }

/** If the wantsLayer property is set to YES, this method will be invoked to return a layer instance. */
-(CALayer*) makeBackingLayer {
	CALayer* layer = [self.class.layerClass layer];
	CGSize viewScale = [self convertSizeToBacking: CGSizeMake(1.0, 1.0)];
	layer.contentsScale = MIN(viewScale.width, viewScale.height);
	return layer;
}

-(BOOL) acceptsFirstResponder { return YES; }

@end
