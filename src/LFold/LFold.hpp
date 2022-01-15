/**
 * @file LFold.hpp
 * @author Animated Circuits
 * @brief  LFO with wave folding module
 * @version 2.0
 * @date January 2022
 * @copyright See License file
 */

#ifndef LFold_hpp
#define LFold_hpp

#include "../plugin.hpp"

using simd::float_4;

struct LFold: Module {

	enum LFoldParamIds {
		FREQ_PARAM,
		FREQ_RANGE_PARAM,
		RESERVED1,

		FOLD_DEPTH_PARAM,
		RESERVED2,

		FOLD_OFFSET_PARAM,
		RESERVED3,

		FOLD_SHAPE_TYPE_PARAM,
		INPUT_BIPOLAR_PARAM, 
		OUTPUT_BIPOLAR_PARAM,
		RESERVED4,

		NUM_PARAMS
	};

	enum LFoldInputIds {
		IN_INPUT, 
		SYNC_INPUT,

		NUM_INPUTS
	};

	enum LFoldOutputIds {
		LFOLD_OUTPUT,
		MAIN_OUTPUT,

		NUM_OUTPUTS
	};

	enum LFoldLightIds {
		NUM_LIGHTS
	};

	LFold();
  	void onReset() override;
  	void process(const ProcessArgs &args) override;

	float_4 phases[4];
	float syncFreq, prevFreq;
	dsp::SchmittTrigger syncTrigger;
	dsp::Timer syncTimer;
};

#endif /* LFold_hpp */
