/**
 * @file LFold.cpp
 * @author Animated Circuits
 * @brief  LFO with wave folding module
 * @version 2.0
 * @date January 2022
 * @copyright See License file
 */

#include "LFold.hpp"

using simd::float_4;

struct FrequencyQuantity : ParamQuantity {
    float getDisplayValue() override {
        LFold* module = reinterpret_cast<LFold*>(this->module);
        unit = (module->syncFreq == -1.f) ? " Hz" : " x";

        return ParamQuantity::getDisplayValue();
    }
    int getDisplayPrecision() override {
        return 3;
    }
};


LFold::LFold() : Module() 
{    
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    configParam<FrequencyQuantity>(FREQ_PARAM, 0.01f, 5.f, 1.f, "Sine LFO Frequency", " Hz", 0.f, 1.f);
    configSwitch(FOLD_SHAPE_TYPE_PARAM, 0, 1, 0, "Folding Shape", {"Triangular", "Sinusoidal"});
    configParam(FOLD_DEPTH_PARAM, 1.f, 10.f, 2.5f, "Folding Depth");
    configParam(FOLD_OFFSET_PARAM, -2.5f, 2.5f, 0.f, "Folding Offset");    
    configSwitch(INPUT_BIPOLAR_PARAM, 0, 1, 0, "Input", {"Unipolar", "Bipolar"});
    configSwitch(OUTPUT_BIPOLAR_PARAM, 0, 1, 0, "Output", {"Unipolar", "Bipolar"});
    configInput(IN_INPUT, "Input to Wavefolder (normalled to built-in LFO)");
    configInput(SYNC_INPUT, "Ext Clock");
    configOutput(LFOLD_OUTPUT, "Folded built-in LFO signal");
    configOutput(MAIN_OUTPUT, "Folded Input Signal (or built-in LFO)");
   
    configBypass(IN_INPUT, MAIN_OUTPUT);

    onReset();
}

void LFold::onReset() {
    for (int i = 0; i < 4; i++) {
        phases[i] = 0.f;
    }

    syncFreq = -1.f;
    prevFreq = -1.f;
    syncTimer.reset();
}

void LFold::process(const ProcessArgs &args)
{
    bool lfoOutConnected = outputs[LFOLD_OUTPUT].isConnected();
    bool outConnected = outputs[MAIN_OUTPUT].isConnected();

    if (!lfoOutConnected && !outConnected) return;

    bool inConnected = inputs[IN_INPUT].isConnected();
    float inOffset = params[INPUT_BIPOLAR_PARAM].getValue() >= 0.5f ? 0.f : 5.f;
    float outOffset = params[OUTPUT_BIPOLAR_PARAM].getValue() < 0.5f ? 1.f : 0.f;

    // Wavefolder params
    bool foldSine = params[FOLD_SHAPE_TYPE_PARAM].getValue() >= 0.5f;
    float depth = params[FOLD_DEPTH_PARAM].getValue();
    float offset = params[FOLD_OFFSET_PARAM].getValue();

    // built-in LFO params
    float freq = params[FREQ_PARAM].getValue();

    // Sync
    if (inputs[SYNC_INPUT].isConnected()) {
        syncTimer.process(args.sampleTime);

        if (syncTrigger.process(inputs[SYNC_INPUT].getVoltage(), 0.1f, 2.f)) {
            float extFreq = 1.f / syncTimer.getTime();
            if (!isNear(syncFreq, extFreq, 0.01f) || !isNear(freq, prevFreq, 0.01f)) {
                syncFreq = extFreq;
                for (int i = 0; i < 4; i++) {
                    phases[i] = 0.f;
                }
                prevFreq = freq;
            }
            syncTimer.reset();
        }
        freq *= syncFreq;
    } else {
        syncFreq = -1.f;
    }

    float incPhase = fmin(freq * args.sampleTime, 0.5f);

    int channels = std::max(1, inputs[IN_INPUT].getChannels());
    for (int c = 0, i = 0; c < channels; c += 4, i++) {

        // built-in LFO value
        phases[i] += incPhase;
        phases[i] -= simd::trunc(phases[i]);
        float_4 lfo = simd::sin(2 * M_PI * phases[i]);
  
        if (outConnected) {
            float_4 out = lfo;

            if (inConnected) {
                float_4 in = (inputs[IN_INPUT].getPolyVoltageSimd<float_4>(c) - inOffset) / 5.f * depth + offset;

                out = foldSine ? 
                    simd::sin(in) : 4.f * (simd::fabs(0.25f * in + 0.25f - simd::round(0.25f * in + 0.25f)) - 0.25f);
            }      
            out = out + outOffset;
            outputs[MAIN_OUTPUT].setVoltageSimd(out * 5.f, c);
        }

        if (lfoOutConnected) {
            float_4 in = lfo * depth + offset;
            
            float_4 lfold = foldSine ? 
                simd::sin(in) : 4.f * (simd::fabs(0.25f * in + 0.25f - simd::round(0.25f * in + 0.25f)) - 0.25f);
      
            lfold = lfold + outOffset;
            outputs[LFOLD_OUTPUT].setVoltageSimd(lfold * 5.f, c); 
        }
    }
    outputs[MAIN_OUTPUT].setChannels(channels);
    outputs[LFOLD_OUTPUT].setChannels(channels); 
}



