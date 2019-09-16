#include "stdafx.h"
#include "SteamIVRInput.h"
#include <math.h>
#include <filesystem>
using namespace VRInputMemes;
void SteamIVRInput::Init(const bool initializeSteamVR)
{

	// If something already initialized SteamVR we don't want to do it again.
	// Intended for being able to copy paste into existing codebase to make 
	// sure everything works with a known working example, hence why this is toggleable.
	// Should always be run with true if third party doesn't init SteamVR.
	std::cout << "Action manifest path: " << std::filesystem::absolute(m_actionManifestPath) << std::endl;

	if (initializeSteamVR)
	{
		auto initError = vr::VRInitError_None;
		vr::VR_Init(&initError, vr::VRApplication_Background);
		if (initError != vr::VRInitError_None)
		{
			std::cout << "Error: steamvr init: " << initError << std::endl;
		}
	}

	// Set the action manifest. This should be in the executable directory.
	// Defined by m_actionManifestPath.
	auto error = vr::VRInput()->SetActionManifestPath((std::filesystem::absolute(m_actionManifestPath)).string().c_str());
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cout << "Error: setActionManifest: " << error << std::endl;
	}

	for (auto&& kv : m_actionHandleMap)
	{
		// Get action handle
		error = vr::VRInput()->GetActionHandle(kv.first, &(kv.second.first));
		if (error != vr::EVRInputError::VRInputError_None)
		{
			std::cout << "Error: getActionHandle: "<< kv.first<<" code: " << error << std::endl;
		}
	}

	// Get set handle
	error = vr::VRInput()->GetActionSetHandle(k_actionSetMain,
		&m_mainSetHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cout << "Error: getActionSet: " << error << std::endl;
	}

	m_activeActionSet.ulActionSet = m_mainSetHandler;
	m_activeActionSet.ulRestrictedToDevice = vr::k_ulInvalidInputValueHandle;
	// When I didn't manually set priority zero it didn't work for me, for unknown reasons.
	m_activeActionSet.nPriority = 0;

}

ControllerState SteamIVRInput::Loop()
{
	// Getting the correct sizeof is critical.
	// Make sure to match digital/analog with the function you're calling.
	auto error = vr::VRInput()->UpdateActionState(&m_activeActionSet, sizeof(m_activeActionSet), 1);
	ControllerState state = {};
	for (auto ahandle : m_actionHandleMap)
	{
		if (ahandle.first == k_actionstart)
		{
			int i = 0;
		}
		vr::EVRInputError error;
		switch (ahandle.second.second)
		{
			case VRInputType::VRInputType_Analog:
				vr::InputAnalogActionData_t analogstate;
				error = vr::VRInput()->GetAnalogActionData(ahandle.second.first, &analogstate, sizeof(analogstate), vr::k_ulInvalidInputValueHandle);
				if (error != vr::EVRInputError::VRInputError_None)
				{
					std::cerr << "Error getting analog action state. Error code: " << error << " culprit: " << ahandle.first << std::endl;
				}
				handleAnalogAction(analogstate, ahandle.first, state);
				break;
			case VRInputType::VRInputType_Digital:
				vr::InputDigitalActionData_t digitalstate;
				error = vr::VRInput()->GetDigitalActionData(ahandle.second.first, &digitalstate, sizeof(digitalstate), vr::k_ulInvalidInputValueHandle);
				if (error != vr::EVRInputError::VRInputError_None)
				{
					std::cerr << "Error getting digital action state. Error code: " << error << " culprit: " << ahandle.first << std::endl;
				}
				handleDigitalAction(digitalstate, ahandle.first, state);
				break;
		}
	
	}
	return state;
}

void SteamIVRInput::rumbleController(int controller, float duration, float frequency, float amplitude)
{
	auto error = vr::VRInput()->TriggerHapticVibrationAction(controller ? m_actionHandleMap[k_actionrumbleLeft].first : m_actionHandleMap[k_actionrumbleRight].first, 0, duration, frequency, amplitude, vr::k_ulInvalidInputValueHandle);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Rumble error. Code: " << error << std::endl;
	}

}

void SteamIVRInput::handleDigitalAction(vr::InputDigitalActionData_t& state, const char* key, ControllerState& report)
{
	if (state.bActive && state.bState)
	{
		if (key == k_actionbuttonA)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_A;
		}
		else if (key == k_actionbuttonB)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_B;
		}
		else if (key == k_actionbuttonX)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_X;
		}
		else if (key == k_actionbuttonY)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_Y;
		}
		else if (key == k_actionleftDPADUp)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_DPAD_UP;
		}
		else if (key == k_actionleftDPADLeft)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_DPAD_LEFT;
		}
		else if (key == k_actionleftDPADDown)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_DPAD_DOWN;
		}
		else if (key == k_actionleftDPADRight)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_DPAD_RIGHT;
		}
		else if (key == k_actionleftTriggerClick)
		{
			//report.wButtons |= _XUSB_BUTTON::XUSB_GAMEPAD_A;
		}
		else if (key == k_actionrightTriggerClick)
		{
			//report.wButtons |= _XUSB_BUTTON::XUSB_GAMEPAD_A;
		}
		else if (key == k_actionleftBumper)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_LEFT_SHOULDER;
		}
		else if (key == k_actionrightBumper)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_RIGHT_SHOULDER;
		}
		else if (key == k_actionleftStickClick)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_LEFT_THUMB;
		}
		else if (key == k_actionrightStickClick)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_RIGHT_THUMB;
		}
		else if (key == k_actionselect)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_BACK;
		}
		else if (key == k_actionstart)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_START;
		}
		else if (key == k_actionguide)
		{
			report.wButtons |= ControllerButtons::XUSB_GAMEPAD_GUIDE;
		}
	}
}

void SteamIVRInput::handleAnalogAction(vr::InputAnalogActionData_t& state, const char* key, ControllerState& report)
{
	if (state.bActive)
	{
		if (key == k_actionleftTriggerValue)
		{
			unsigned char value = std::round(state.x * 255);
			report.bLeftTrigger = value;
		}
		else if (key == k_actionrightTriggerVallue)
		{
			unsigned char value = std::round(state.x * 255);
			report.bRightTrigger = value;
		}
		else if (key == k_actionleftStickPosition)
		{
			short valuex = std::round(state.x * std::pow(2,15));
			short valuey = std::round(state.y * std::pow(2, 15));
			report.sThumbLX = valuex;
			report.sThumbLY = valuey;
		}
		else if (key == k_actionrightStickPosition)
		{
			short valuex = std::round(state.x * std::pow(2, 15));
			short valuey = std::round(state.y * std::pow(2, 15));
			report.sThumbRX = valuex;
			report.sThumbRY = valuey;
		}
	}
}
