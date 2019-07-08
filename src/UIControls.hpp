/**
 * @file	UIControls.hpp
 * @author	Animated Circuits
 * @brief	common UI controls
 * @version	1.0
 * @date July 2019
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
struct BigWhiteKnob : BaseKnob {
	BigWhiteKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Knob_White_Light_42.svg")));
	}
};
struct BigBlueKnob : BaseKnob {
	BigBlueKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Knob_Blue_Light_42.svg")));
	}
};
struct SmallBlackKnob : BaseKnob {
	SmallBlackKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Knob_Black_Light_21.svg")));
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
