#pragma once
#include <openvr.h>
#include <iostream>
#include <map>
#include "ViGEmClient.h"


// These two are set in action_manifest.json. They must match or you'll get errors/things that don't work with no errors.
constexpr auto k_actionSetMain = "/actions/main";
constexpr auto k_actionbuttonA = "/actions/main/in/buttona";
constexpr auto k_actionbuttonB = "/actions/main/in/buttonb";
constexpr auto k_actionbuttonX = "/actions/main/in/ButtonX";
constexpr auto k_actionbuttonY = "/actions/main/in/ButtonY";
constexpr auto k_actionleftDPADUp = "/actions/main/in/LeftDPADUp";
constexpr auto k_actionleftDPADLeft = "/actions/main/in/LeftDPADLeft";
constexpr auto k_actionleftDPADDown = "/actions/main/in/LeftDPADDown";
constexpr auto k_actionleftDPADRight = "/actions/main/in/LeftDPADRight";
constexpr auto k_actionrumbleLeft = "/actions/main/out/RumbleLeft";
constexpr auto k_actionrumbleRight = "/actions/main/out/RumbleRight";
constexpr auto k_actionleftTriggerValue = "/actions/main/in/LeftTriggerValue";
constexpr auto k_actionrightTriggerVallue = "/actions/main/in/RightTriggerValue";
constexpr auto k_actionleftTriggerClick = "/actions/main/in/LeftTriggerClick";
constexpr auto k_actionrightTriggerClick = "/actions/main/in/RightTriggerClick";
constexpr auto k_actionleftBumper = "/actions/main/in/LeftBumper";
constexpr auto k_actionrightBumper = "/actions/main/in/RightBumper";
constexpr auto k_actionleftStickClick = "/actions/main/in/LeftStickClick";
constexpr auto k_actionrightStickClick = "/actions/main/in/RightStickClick";
constexpr auto k_actionselect = "/actions/main/in/Select";
constexpr auto k_actionstart = "/actions/main/in/Start";
constexpr auto k_actionleftStickPosition = "/actions/main/in/LeftStickPosition";
constexpr auto k_actionrightStickPosition = "/actions/main/in/RightStickPosition";

enum VRInputType
{
	VRInputType_Digital = 0,
	VRInputType_Analog = 1,
	VRInputType_Output = 2
};

typedef std::pair<vr::VRActionHandle_t, VRInputType> ActionHandle;

class SteamIVRInput {
public:
	void Init(const bool init);
	void Loop(XUSB_REPORT &report);
	void rumbleController(int controller, float duration, float frequency, float amplitude);

private:
	//Using an action manifest from a path that isn't the executable path doesn't work for whatever reason.
	const std::string m_actionManifestPath =  "..\\Xjoy\\action_manifest.json"; //D:\\SourceTree\\IndeX360\\XJoy\\
	
	std::map<const char*, ActionHandle> m_actionHandleMap =
	{ 
		{k_actionbuttonA, {{},VRInputType::VRInputType_Digital}},
		{k_actionbuttonB, {{},VRInputType::VRInputType_Digital}},
		{k_actionbuttonX, {{},VRInputType::VRInputType_Digital}},
		{k_actionbuttonY, {{},VRInputType::VRInputType_Digital}},
		{k_actionleftDPADUp, {{},VRInputType::VRInputType_Digital}},
		{k_actionleftDPADLeft, {{},VRInputType::VRInputType_Digital}},
		{k_actionleftDPADDown, {{},VRInputType::VRInputType_Digital}},
		{k_actionleftDPADRight, {{},VRInputType::VRInputType_Digital}},
		{k_actionrumbleLeft, {{},VRInputType::VRInputType_Output}},
		{k_actionrumbleRight, {{},VRInputType::VRInputType_Output}},
		{k_actionleftTriggerValue, {{},VRInputType::VRInputType_Analog}},
		{k_actionrightTriggerVallue, {{},VRInputType::VRInputType_Analog}},
		{k_actionleftTriggerClick, {{},VRInputType::VRInputType_Digital}},
		{k_actionrightTriggerClick, {{},VRInputType::VRInputType_Digital}},
		{k_actionleftBumper, {{},VRInputType::VRInputType_Digital}},
		{k_actionrightBumper, {{},VRInputType::VRInputType_Digital}},
		{k_actionleftStickClick, {{},VRInputType::VRInputType_Digital}},
		{k_actionrightStickClick, {{},VRInputType::VRInputType_Digital}},
		{k_actionselect, {{},VRInputType::VRInputType_Digital}},
		{k_actionstart, {{},VRInputType::VRInputType_Digital}},
		{k_actionleftStickPosition, {{},VRInputType::VRInputType_Analog}},
		{k_actionrightStickPosition, {{},VRInputType::VRInputType_Analog}},
	};

	vr::VRActionSetHandle_t m_mainSetHandler = {};
	vr::VRActiveActionSet_t m_activeActionSet = {};
	vr::InputDigitalActionData_t m_nextSongData = {};

	void handleDigitalAction(vr::InputDigitalActionData_t& state, const char* key, XUSB_REPORT &report);
	void handleAnalogAction(vr::InputAnalogActionData_t& state, const char* key, XUSB_REPORT& report);
};
