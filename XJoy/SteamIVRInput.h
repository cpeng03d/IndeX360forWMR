#pragma once
#include <openvr.h>
#include <iostream>
#include <filesystem>
class SteamIVRInput {
public:
	void Init(const bool init);
	void Loop();
	bool nextSongSet();

private:
	//Using an action manifest from a path that isn't the executable path doesn't work for whatever reason.
	const std::string m_actionManifestPath =  "..\\Xjoy\\action_manifest.json"; //D:\\SourceTree\\IndeX360\\XJoy\\

	vr::VRActionHandle_t m_buttonAHandler = {};
	vr::VRActionSetHandle_t m_mainSetHandler = {};
	vr::VRActiveActionSet_t m_activeActionSet = {};
	vr::InputDigitalActionData_t m_nextSongData = {};
};

// These two are set in action_manifest.json. They must match or you'll get errors/things that don't work with no errors.
constexpr auto k_actionSetMain = "/actions/main";
constexpr auto k_buttonA = "/actions/main/in/PlayNextTrack";

