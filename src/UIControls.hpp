/**
 * @file	UIControls.hpp
 * @author	Animated Circuits
 * @brief	common UI controls
 * @version	2.1
 * @date January 2022
 * @copyright See License file
 */

#include "rack.hpp"

namespace UIControls {
	
/**
 * Knobs
 */
struct BaseKnob : app::SvgKnob {
	BaseKnob() {
		minAngle = -0.83*M_PI;
		maxAngle = 0.83*M_PI;
	}
};
struct BaseBigKnob : BaseKnob {
	widget::SvgWidget* fg;
	widget::SvgWidget* bg;

	BaseBigKnob() {
		bg = new widget::SvgWidget;
		fb->addChildBelow(bg, tw);

		fg = new widget::SvgWidget;
		fb->addChildAbove(fg, tw);
		
		bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/Knob_Ring_Light_42_bg.svg")));
		fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/Knob_Cap_Light_42_fg.svg")));	
	}
};
struct BigWhiteKnob : BaseBigKnob {
	BigWhiteKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Knob_White_Light_42.svg")));
	}
};
struct BigBlueKnob : BaseBigKnob {
	BigBlueKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Knob_Blue_Light_42.svg")));
	}
};
struct BigGreyKnob : BaseBigKnob {
	BigGreyKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Knob_Grey_Light_42.svg")));
	}
};
struct SmallBlackKnob : BaseKnob {
	SmallBlackKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Knob_Black_Light_21.svg")));
	}
};
/**
 * Switches
 */
struct LinExpSwitch : app::SvgSwitch {
	LinExpSwitch() {
		shadow->opacity = 0.0;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_Blue_LinExp_LinOn_Light.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_Blue_LinExp_ExpOn_Light.svg")));
	}
};
struct TriSineSwitch : app::SvgSwitch {
	TriSineSwitch() {
		shadow->opacity = 0.0;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_Blue_TriSine_TriOn_Light.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_Blue_TriSine_SineOn_Light.svg")));
	}
};
struct BipolarSwitch : app::SvgSwitch {
	BipolarSwitch() {
		shadow->opacity = 0.0;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_Grey_Bi_Off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_Grey_Bi_On.svg")));
	}
};
/**
 * Ports
 */
struct LightPort : app::SvgPort {
	LightPort() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PlugLight.svg")));
	}
};

} // namespace UIControls
