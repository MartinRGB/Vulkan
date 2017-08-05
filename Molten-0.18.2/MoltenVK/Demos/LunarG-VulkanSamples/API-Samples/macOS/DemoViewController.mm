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


#pragma mark -
#pragma mark VulkanSamples extension for iOS & OSX support

#include "Samples.h"			// The LunarG VulkanSamples code


static NSView* sampleView;		// Global variable to pass NSView to LunarG sample code

/**
 * Called from sample.
 * Initialize sample from view, and resize view in accordance with the sample.
 */
void init_window(struct sample_info &info) {
	info.window = sampleView;
	sampleView.bounds = CGRectMake(0, 0, info.width, info.height);
}

/** Called from sample. Return path to resource folder. */
std::string get_base_data_dir() {
	return [NSBundle.mainBundle.resourcePath stringByAppendingString: @"/"].UTF8String;
}


#pragma mark -
#pragma mark DemoViewController

@implementation DemoViewController {}

/** Since this is a single-view app, initialize Vulkan during view loading. */
-(void) viewDidLoad {
	[super viewDidLoad];

	self.view.wantsLayer = YES;		// Back the view with a layer created by the makeBackingLayer method.

	sampleView = self.view;			// Pass the view to the sample code
	sample_main(0, NULL);			// Run the LunarG sample
}

/** Resize the window to fit the size of the content as set by the sample code. */
-(void) viewWillAppear {
	[super viewWillAppear];

	CGSize vSz = self.view.bounds.size;
	NSWindow *window = self.view.window;
	NSRect wFrm = [window contentRectForFrameRect: window.frame];
	NSRect newWFrm = [window frameRectForContentRect: NSMakeRect(wFrm.origin.x, wFrm.origin.y, vSz.width, vSz.height)];
	[window setFrame: newWFrm display: YES animate: window.isVisible];
	[window center];
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
-(CALayer*) makeBackingLayer { return [self.class.layerClass layer]; }

@end
