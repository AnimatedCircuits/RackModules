/**
 * @file Folding.cpp
 * @author Animated Circuits
 * @brief  wave folding module
 * @version 1.0
 * @date July 2019
 * @copyright See License file
 */

#include "Folding.hpp"

Folding::Folding() : Module()
{
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  
    configParam(SHAPE_TYPE_PARAM, 0.f, 1.f, 0.f, "Shape");
    configParam(DEPTH_PARAM, 1.f, 10.f, 2.5f, "Folding Depth");
    configParam(DEPTH_CV_PARAM, 0.f, 40.f, 0.f, "CV Depth range", "%", 0.f, 10.f, 0.f);
    configParam(DEPTH_CV_BIPOLAR_PARAM, 0.f, 1.f, 0.f, "CV Depth bipolar");
    configParam(OFFSET_PARAM, -2.5f, 2.5f, 0.f, "Folding Offset");
    configParam(OFFSET_CV_PARAM, 0.f, 20.f, 0.f, "CV Offset range", "%", 0.f, 10.f, 0.f);
    configParam(OFFSET_CV_BIPOLAR_PARAM, 0.f, 1.f, 0.f, "CV Offset bipolar");
}

void Folding::process(const ProcessArgs &args)
{
    if (!inputs[IN_INPUT].isConnected() || !outputs[MAIN_OUTPUT].isConnected()) return;
    
    float value = inputs[IN_INPUT].getVoltage() / 5.0f; // Audio or bipolar input signal

    float range = params[DEPTH_CV_BIPOLAR_PARAM].getValue() < 0.5f ? 10.f : 5.f;
    float modulation = inputs[DEPTH_CV_INPUT].getVoltage() / range * params[DEPTH_CV_PARAM].getValue(); // * 10.f * 4.f;
    value *= params[DEPTH_PARAM].getValue() + modulation;
    
    range = params[OFFSET_CV_BIPOLAR_PARAM].getValue() < 0.5f ? 10.f : 5.f;
    modulation = inputs[OFFSET_CV_INPUT].getVoltage() / range * params[OFFSET_CV_PARAM].getValue(); // * 10.f * 2.f;
    value += params[OFFSET_PARAM].getValue() + modulation;

    value = params[SHAPE_TYPE_PARAM].getValue() >= 0.5f ? 
            sin(value) : 4.f * (abs(0.25f * value + 0.25f - round(0.25f * value + 0.25f)) - 0.25f);
  
    outputs[MAIN_OUTPUT].setVoltage(value * 5.0f);
}
