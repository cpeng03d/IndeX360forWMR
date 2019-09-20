# IndeX360

Uses your Valve Index Controllers as an xbox 360 gamepad. Forked from XJoy and ported over to use OpenVR's Input system.
In practice any VR controller can be mapped onto the individual 360 buttons using SteamVR.



## Note
Bumpers are hardcoded to the to middle fingers, since those kind of inputs aren't directly available through SteamVR's input binding interface.

## Installation

1. [Install the ViGEm Bus Driver](https://github.com/ViGEm/ViGEmBus/releases/tag/v1.16.112) (install all requirements as well)

## Usage

```

## Customization

Right now all buttons are hard-coded to their "default" xbox equivalents. If you wish to
customize these mappings, feel free to modify the `process_button` method in XJoy.cpp and
recompile yourself. I plan to add support for a configuration file and maybe a GUI in later
versions. The default mappings are shown below:


| Joy-Con Button     | Xbox Button    |
|--------------------|----------------|
| Right B            | B              |
| Right A            | A              |
| Left B             | Y              |
| Left A             | X              |
| Left Trigger       | Left Trigger   |
| Right Trigger      | Right Trigger  |
| Left Middle Finger | Left Shoulder  |
| Right Middle Finger| Right Shoulder |
| Left Touchpad      | D-PAD          |
| Left Analog        | Left Analog    |
| Right Analog       | Right Analog   |
| Left Stick         | Left Thumb     |
| Right Stick        | Right Thumb    |
| Right Touchpad D   | Start          |
| Right Touchpad U   | Back           |
| Right Touchpad R   | Guide          |


## Building

If you wish to build XJoy yourself, simply install the ViGEm Bus Driver as outlined in the
installation steps, open the XJoy.sln file in Visual Studio 2017, and build. Everything
should work out of the box but if it does not feel free to submit an issue. Note that at
least on my end it _seems_ to be working in Visual Studio 2019 as well, which is good.
Update: Needs OpenVR.dll
