#include "stdafx.h"
#include "Windows.h"
#include "ViGEmClient.h"
#include <iostream>
#include <string>
#include <csignal>
#include <tuple>
#include <unordered_map>
#include "Yaml.hpp"
#include <openvr.h>
#include "SteamIVRInput.h"
#include <chrono>

#define u8 uint8_t
#define u16 uint16_t

const int XBOX_ANALOG_MIN = -32768;
const int XBOX_ANALOG_MAX = 32767;
const int XBOX_ANALOG_DIAG_MAX = round(XBOX_ANALOG_MAX * 0.5 * sqrt(2.0));
const int XBOX_ANALOG_DIAG_MIN = round(XBOX_ANALOG_MIN * 0.5 * sqrt(2.0));
u8 global_counter[2] = { 0,0 };

SteamIVRInput inputhandler;
PVIGEM_CLIENT client = vigem_alloc();
PVIGEM_TARGET target;
XUSB_REPORT report;
int res;
HANDLE left_thread;
DWORD left_thread_id;
HANDLE right_thread;
DWORD right_thread_id;
HANDLE report_mutex;
bool kill_threads = false;
bool keep_rumbling = false;
UCHAR rumble_left_intensity = 0;
UCHAR rumble_right_intensity = 0;



XUSB_BUTTON string_to_xbox_button(std::string input) {
  if(input == "XUSB_GAMEPAD_DPAD_UP") return XUSB_GAMEPAD_DPAD_UP;
  if(input == "XUSB_GAMEPAD_DPAD_DOWN") return XUSB_GAMEPAD_DPAD_DOWN;
  if(input == "XUSB_GAMEPAD_DPAD_LEFT") return XUSB_GAMEPAD_DPAD_LEFT;
  if(input == "XUSB_GAMEPAD_DPAD_RIGHT") return XUSB_GAMEPAD_DPAD_RIGHT;
  if(input == "XUSB_GAMEPAD_START") return XUSB_GAMEPAD_START;
  if(input == "XUSB_GAMEPAD_BACK") return XUSB_GAMEPAD_BACK;
  if(input == "XUSB_GAMEPAD_LEFT_THUMB") return XUSB_GAMEPAD_LEFT_THUMB;
  if(input == "XUSB_GAMEPAD_RIGHT_THUMB") return XUSB_GAMEPAD_RIGHT_THUMB;
  if(input == "XUSB_GAMEPAD_LEFT_SHOULDER") return XUSB_GAMEPAD_LEFT_SHOULDER;
  if(input == "XUSB_GAMEPAD_RIGHT_SHOULDER") return XUSB_GAMEPAD_RIGHT_SHOULDER;
  if(input == "XUSB_GAMEPAD_GUIDE") return XUSB_GAMEPAD_GUIDE;
  if(input == "XUSB_GAMEPAD_A") return XUSB_GAMEPAD_A;
  if(input == "XUSB_GAMEPAD_B") return XUSB_GAMEPAD_B;
  if(input == "XUSB_GAMEPAD_X") return XUSB_GAMEPAD_X;
  if(input == "XUSB_GAMEPAD_Y") return XUSB_GAMEPAD_Y;
  throw "invalid xbox button: " + input;
}

std::string vigem_error_to_string(VIGEM_ERROR err) {
  switch(err) {
    case VIGEM_ERROR_NONE: return "none";
    case VIGEM_ERROR_ALREADY_CONNECTED: return "already connected";
    case VIGEM_ERROR_BUS_ACCESS_FAILED: return "bus access failed";
    case VIGEM_ERROR_BUS_ALREADY_CONNECTED: return "bus already connected";
    case VIGEM_ERROR_BUS_NOT_FOUND: return "bus not found";
    case VIGEM_ERROR_BUS_VERSION_MISMATCH: return "bus version mismatch";
    case VIGEM_ERROR_CALLBACK_ALREADY_REGISTERED: return "callback already registered";
    case VIGEM_ERROR_CALLBACK_NOT_FOUND: return "callback not found";
    case VIGEM_ERROR_INVALID_TARGET: return "invalid target";
    case VIGEM_ERROR_NO_FREE_SLOT: return "no free slot";
    case VIGEM_ERROR_REMOVAL_FAILED: return "removal failed";
    case VIGEM_ERROR_TARGET_NOT_PLUGGED_IN: return "target not plugged in";
    case VIGEM_ERROR_TARGET_UNINITIALIZED: return "target uninitialized";
    default: return "none";
  }
}

void initialize_xbox() {
  std::cout << "initializing emulated Xbox 360 controller..." << std::endl;
  VIGEM_ERROR err = vigem_connect(client);
  if(err == VIGEM_ERROR_NONE) {
    std::cout << " => connected successfully" << std::endl;
  } else {
    std::cout << "connection error: " << vigem_error_to_string(err) << std::endl;
    vigem_free(client);
    std::cout << "press [ENTER] to exit" << std::endl;
    getchar();
    exit(1);
  }
  target = vigem_target_x360_alloc();
  vigem_target_add(client, target);
  XUSB_REPORT_INIT(&report);
  std::cout << " => added target Xbox 360 Controller" << std::endl;
  // TODO VIBRATION vigem_target_x360_register_notification()
  std::cout << std::endl;
}

void disconnect_exit() {
  vigem_target_remove(client, target);
  vigem_target_free(target);
  vigem_disconnect(client);
  vigem_free(client);
  exit(0);
}

void terminateapp() {
  kill_threads = true;
  inputhandler.Shutdown();
  Sleep(10);/*


  TerminateThread(left_thread, 0);
  TerminateThread(right_thread, 0);*/
  std::cout << "disconnecting and exiting..." << std::endl;
  disconnect_exit();
}

void exit_handler(int signum) {
	terminateapp();
  exit(signum);
}

void rumbleNotification(PVIGEM_CLIENT Client, PVIGEM_TARGET Target, UCHAR LargeMotor, UCHAR SmallMotor, UCHAR LedNumber)
{
	std::cout << "Rumble received! " << int(LargeMotor) << " | " << int(SmallMotor) << std::endl;
	keep_rumbling = LargeMotor > 0 || SmallMotor > 0;
	rumble_left_intensity = LargeMotor;
	rumble_right_intensity = SmallMotor;
}

int main() {
  signal(SIGINT, exit_handler);
  std::cout << "XJoy v0.2.0" << std::endl << std::endl;

  initialize_xbox();
  vigem_target_x360_register_notification(client, target, rumbleNotification);
  //hid_init();
  inputhandler.Init(true);
  std::cout << std::endl;
  std::cout << "initializing threads..." << std::endl;
  report_mutex = CreateMutex(NULL, FALSE, NULL);
  if(report_mutex == NULL) {
    printf("CreateMutex error: %d\n", GetLastError());
    return 1;
  }
  std::cout << " => created report mutex" << std::endl;
  /*
  left_thread = CreateThread(0, 0, left_joycon_thread, 0, 0, &left_thread_id);
  right_thread = CreateThread(0, 0, right_joycon_thread, 0, 0, &right_thread_id);*/
  auto lastTick = std::chrono::steady_clock::now();
  while (true)
  {
	  auto state = inputhandler.Loop();
	  report.bLeftTrigger = state.bLeftTrigger;
	  report.bRightTrigger = state.bRightTrigger;
	  report.sThumbLX = state.sThumbLX;
	  report.sThumbLY = state.sThumbLY;
	  report.sThumbRX = state.sThumbRX;
	  report.sThumbRY = state.sThumbRY;
	  report.wButtons = state.wButtons;
	  vigem_target_x360_update(client, target, report);
	  if (keep_rumbling)
	  {
		  std::chrono::duration<float, std::milli> delta = std::chrono::steady_clock::now() - lastTick;
		  if (rumble_left_intensity > 0)
		  {
			  inputhandler.rumbleController(1, delta.count()/ 1000, 60, rumble_left_intensity);
		  }
		  if (rumble_right_intensity > 0)
		  {
			  inputhandler.rumbleController(0, delta.count()/ 1000, 60, rumble_right_intensity);

		  }
	  }
	  lastTick = std::chrono::steady_clock::now();
	  Sleep(1);
  }
  std::cout << std::endl;
  getchar();
  terminateapp();
}

