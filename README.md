# IndeX360

Uses your Valve Index Controllers as an xbox 360 gamepad. Forked from XJoy and ported over to use OpenVR's Input system.
In practice any VR controller can be mapped onto the individual 360 buttons using SteamVR.



## Note
Start and Guide buttons don't work yet. Trying to map the system buttons onto them but they don't appear to activate any input actions.
Not sure how to map the bumpers, so those are optional. Should still be able to map to them using the SteamVR input map thingy.

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
| A                  | B              |
| B                  | A              |
| X                  | Y              |
| Y                  | X              |
| Left Trigger       | Left Trigger   |
| Right Trigger      | Right Trigger  |
| Left Shoulder      | Left Shoulder  |
| Right Shoulder     | Right Shoulder |
| D-PAD              | D-PAD          |
| Left Analog        | Left Analog    |
| Right Analog       | Right Analog   |
| Left Stick         | Left Thumb     |
| Right Stick        | Right Thumb    |
| Home               | Start          |
| Capture            | Back           |
| Plus               | Start          |
| Minus              | Back           |

## Building

If you wish to build XJoy yourself, simply install the ViGEm Bus Driver as outlined in the
installation steps, open the XJoy.sln file in Visual Studio 2017, and build. Everything
should work out of the box but if it does not feel free to submit an issue. Note that at
least on my end it _seems_ to be working in Visual Studio 2019 as well, which is good.
Update: Needs OpenVR.dll
