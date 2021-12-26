/**
 * @file Folding.cpp
 * @author Animated Circuits
 * @brief  wave folding module
 * @version 2.0
 * @date December 2021
 * @copyright See License file
 */

#include "Folding.hpp"

using simd::float_4;


Folding::Folding() : Module()
{
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  
    configSwitch(SHAPE_TYPE_PARAM, 0, 1, 0, "Shape", {"Triangular", "Sinusoidal"});
    configParam(DEPTH_PARAM, 1.f, 10.f, 2.5f, "Folding Depth");
    configParam(DEPTH_CV_PARAM, 0.f, 40.f, 0.f, "CV Depth range", "%", 0.f, 10.f, 0.f);
    configSwitch(DEPTH_CV_BIPOLAR_PARAM, 0, 1, 0, "CV Depth", {"Unipolar", "Bipolar"});
    configParam(OFFSET_PARAM, -2.5f, 2.5f, 0.f, "Folding Offset");
    configParam(OFFSET_CV_PARAM, 0.f, 20.f, 0.f, "CV Offset range", "%", 0.f, 16.f, 0.f);
    configSwitch(OFFSET_CV_BIPOLAR_PARAM, 0, 1, 0, "CV Offset", {"Unipolar", "Bipolar"});
    configInput(IN_INPUT, "Signal");
    configInput(OFFSET_CV_INPUT, "Folding Offset CV");
    configInput(DEPTH_CV_INPUT, "Folding Depth CV");
    configOutput(MAIN_OUTPUT, "Signal");

    configBypass(IN_INPUT, MAIN_OUTPUT);
}

void Folding::process(const ProcessArgs &args)
{
    if (!inputs[IN_INPUT].isConnected() || !outputs[MAIN_OUTPUT].isConnected()) return;
    
    bool foldSine = params[SHAPE_TYPE_PARAM].getValue() >= 0.5f;

    float depth = params[DEPTH_PARAM].getValue();
    float depthCV = params[DEPTH_CV_PARAM].getValue() / (params[DEPTH_CV_BIPOLAR_PARAM].getValue() < 0.5f ? 10.f : 5.f);

    float offset = params[OFFSET_PARAM].getValue();
    float offsetCV = params[OFFSET_CV_PARAM].getValue() / (params[OFFSET_CV_BIPOLAR_PARAM].getValue() < 0.5f ? 10.f : 5.f);

    int channels = std::max(1, inputs[IN_INPUT].getChannels());
    for (int c = 0; c < channels; c += 4) {
        
        float_4 value = inputs[IN_INPUT].getPolyVoltageSimd<float_4>(c) / 5.0 
                        * (depth + inputs[DEPTH_CV_INPUT].getPolyVoltageSimd<float_4>(c) * depthCV)
                        + offset + inputs[OFFSET_CV_INPUT].getPolyVoltageSimd<float_4>(c) * offsetCV;

        float_4 out = foldSine ? 
            simd::sin(value) : 4.f * (simd::fabs(0.25f * value + 0.25f - simd::round(0.25f * value + 0.25f)) - 0.25f);
  
        outputs[MAIN_OUTPUT].setVoltageSimd(out * 5.f, c);
    }
    outputs[MAIN_OUTPUT].setChannels(channels);
}
