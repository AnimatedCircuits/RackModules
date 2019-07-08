/**
 * @file Folding.hpp
 * @author Animated Circuits
 * @brief  wave folding module
 * @version 1.0
 * @date July 2019
 * @copyright See License file
 */


#ifndef Folding_hpp
#define Folding_hpp

#include "../plugin.hpp"

struct Folding: Module {
	enum ParamIds {
		SHAPE_TYPE_PARAM,
		
		DEPTH_PARAM,
		DEPTH_CV_PARAM,
		DEPTH_CV_BIPOLAR_PARAM, 

		OFFSET_PARAM,
		OFFSET_CV_PARAM,
		OFFSET_CV_BIPOLAR_PARAM,

		NUM_PARAMS
	};
	enum InputIds {
		IN_INPUT,
		DEPTH_CV_INPUT, 
		OFFSET_CV_INPUT,

		NUM_INPUTS
	};
	enum OutputIds {
		MAIN_OUTPUT,

		NUM_OUTPUTS
	};

	enum LightIds {
		NUM_LIGHTS
	};

	Folding();
	void process(const ProcessArgs &args) override;
};

#endif /* Folding_hpp */
