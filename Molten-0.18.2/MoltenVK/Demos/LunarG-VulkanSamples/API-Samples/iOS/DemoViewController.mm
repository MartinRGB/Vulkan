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


#pragma mark -
#pragma mark VulkanSamples extension for iOS & macOS support

#include "Samples.h"			// The LunarG VulkanSamples code


static UIView* sampleView;		// Global variable to pass UIView to LunarG sample code

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

/** Since this is a single-view app, init Vulkan when the view is loaded. */
-(void) viewDidLoad {
	[super viewDidLoad];

	sampleView = self.view;			// Pass the view to the sample code
	sample_main(0, NULL);			// Run the LunarG sample
}

@end


#pragma mark -
#pragma mark DemoView

@implementation DemoView

/** Returns a Metal-compatible layer. */
+(Class) layerClass { return [CAMetalLayer class]; }

@end

