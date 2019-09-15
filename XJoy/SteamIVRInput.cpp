#include "stdafx.h"
#include "SteamIVRInput.h"
#include <Windows.h>
void SteamIVRInput::Init(const bool initializeSteamVR)
{

	// If something already initialized SteamVR we don't want to do it again.
	// Intended for being able to copy paste into existing codebase to make 
	// sure everything works with a known working example, hence why this is toggleable.
	// Should always be run with true if third party doesn't init SteamVR.
	std::cout << "Action manifest path: " << std::filesystem::absolute(m_actionManifestPath);

	if (initializeSteamVR)
	{
		auto initError = vr::VRInitError_None;
		vr::VR_Init(&initError, vr::VRApplication_Background);
		if (initError != vr::VRInitError_None)
		{
			std::cout << "Error: steamvr init: " << initError;
		}
	}

	// Set the action manifest. This should be in the executable directory.
	// Defined by m_actionManifestPath.
	auto error = vr::VRInput()->SetActionManifestPath((std::filesystem::absolute(m_actionManifestPath)).string().c_str());
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cout << "Error: setActionManifest: " << error;
	}

	// Get action handle
	error = vr::VRInput()->GetActionHandle(k_buttonA,
		&m_buttonAHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cout <<"Error: getActionHandle: "  << error;
	}

	// Get set handle
	error = vr::VRInput()->GetActionSetHandle(k_actionSetMain,
		&m_mainSetHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cout << "Error: getActionSet: " << error;
	}

	m_activeActionSet.ulActionSet = m_mainSetHandler;
	m_activeActionSet.ulRestrictedToDevice = vr::k_ulInvalidInputValueHandle;
	// When I didn't manually set priority zero it didn't work for me, for unknown reasons.
	m_activeActionSet.nPriority = 0;
}

bool SteamIVRInput::nextSongSet()
{
	//auto e = vr::VRInput()->GetDigitalActionData(
	//	m_nextSongHandler,
	//	&m_nextSongData,
	//	sizeof(m_nextSongData),
	//	vr::k_ulInvalidInputValueHandle);

	//if (e != vr::EVRInputError::VRInputError_None)
	//{
	//	// Print the rror code.
	//	std::cerr << e << '\n';
	//	std::cerr << "GetDigitalAction error.\n";
	//}

	//// Will basically just spam the console. To make sure it's visible even from a distance.
	//if (m_nextSongData.bActive)
	//{
	//	std::cout << "Action Set Active!\n";
	//}

	//return m_nextSongData.bState;
	return true;
}

void SteamIVRInput::Loop()
{
	// Getting the correct sizeof is critical.
	// Make sure to match digital/analog with the function you're calling.
	auto error = vr::VRInput()->UpdateActionState(
		&m_activeActionSet, sizeof(m_activeActionSet), 1);

	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Loop error.\n";
	}
}