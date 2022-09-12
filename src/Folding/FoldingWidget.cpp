/**
 * @file FoldingWidget.cpp
 * @author Animated Circuits
 * @brief  wave folding widget
 * @version 2.0
 * @date December 2021
 * 
 * @copyright See License file
 * 
 */

#include "Folding.hpp"
#include "../DarkModeUtils.hpp"
#include "../UIControls.hpp"

using namespace UIControls;

/// No Specific UI controls

struct FoldingWidget: ModuleWidget { 
	std::shared_ptr<Svg> darkSvg;
	std::shared_ptr<Svg> normalSvg;
	ScrewBlack* blackScrews[2];
	ScrewSilver* silverScrews[2];
#ifdef USING_CARDINAL_NOT_RACK
	bool darkMode = settings::preferDarkPanels;
#else
	bool darkMode = false;
#endif

	FoldingWidget(Folding *module) : ModuleWidget(module)
	{
		normalSvg = SVG::load(asset::plugin(pluginInstance, "res/FoldingLight.svg"));
		darkSvg = SVG::load(asset::plugin(pluginInstance, "res/./FoldingLight.svg")); // forcing different svg handle

		// invert for dark colors once
		static bool inverted = false;
		if (!inverted) {
			inverted = true;
			invertColorsOfSVG(darkSvg->handle);
		}

		if (darkMode) {
			// use black panel
			setPanel(darkSvg);
		} else {
			// use light panel
			setPanel(normalSvg);
		}

		addChild(blackScrews[0] = createWidget<ScrewBlack>(Vec(box.size.x-30, 0)));
		addChild(blackScrews[1] = createWidget<ScrewBlack>(Vec(15, 365)));

		addChild(silverScrews[0] = createWidget<ScrewSilver>(Vec(box.size.x-30, 0)));
		addChild(silverScrews[1] = createWidget<ScrewSilver>(Vec(15, 365)));

		if (darkMode) {
			// use black screws
			silverScrews[0]->hide();
			silverScrews[1]->hide();
		} else {
			// use silver screws
			blackScrews[0]->hide();
			blackScrews[1]->hide();
		}

		addParam(createParamCentered<TriSineSwitch>(Vec(box.size.x/2.f, 41.5), module, Folding::SHAPE_TYPE_PARAM));

		addParam(createParamCentered<BigBlueKnob>(Vec(box.size.x/2.f, 93.9f), module, Folding::DEPTH_PARAM));
		addParam(createParamCentered<BigWhiteKnob>(Vec(box.size.x/2.f, 204.4f), module, Folding::OFFSET_PARAM));

		addParam(createParamCentered<Trimpot>(Vec(box.size.x/2.f, 138.5f), module, Folding::DEPTH_CV_PARAM));
		addParam(createParamCentered<Trimpot>(Vec(box.size.x/2.f, 249.5f), module, Folding::OFFSET_CV_PARAM));

		addParam(createParam<BipolarSwitch>(Vec(13.5, 267), module, Folding::DEPTH_CV_BIPOLAR_PARAM));
		addParam(createParam<BipolarSwitch>(Vec(48.5, 267), module, Folding::OFFSET_CV_BIPOLAR_PARAM));

		addInput(createInput<LightPort>(Vec(11, 298), module, Folding::DEPTH_CV_INPUT));
		addInput(createInput<LightPort>(Vec(47, 298), module, Folding::OFFSET_CV_INPUT));
	
		addInput(createInput<LightPort>(Vec(11, 330), module, Folding::IN_INPUT));
		addOutput(createOutput<LightPort>(Vec(47, 330), module, Folding::MAIN_OUTPUT));
	}

#ifdef USING_CARDINAL_NOT_RACK
	void step() override {
		if (darkMode != settings::preferDarkPanels) {
			darkMode = settings::preferDarkPanels;
			if (darkMode) {
				// use black panel
				setPanel(darkSvg);
				// use black screws
				silverScrews[0]->hide();
				silverScrews[1]->hide();
				blackScrews[0]->show();
				blackScrews[1]->show();
			} else {
				// use light panel
				setPanel(normalSvg);
				// use silver screws
				blackScrews[0]->hide();
				blackScrews[1]->hide();
				silverScrews[0]->show();
				silverScrews[1]->show();
			}
		}
		ModuleWidget::step();
	}
#endif
};

Model *model_AC_Folding = createModel<Folding, FoldingWidget>(SlugFolding);


