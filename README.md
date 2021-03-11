# egb220_line_robot
Group 34's line following robot.


# Setup JDLuck Spark Mark II For Platformio
This is useful as an easy alternative to setting up `make` scripts, for managing the builds.
Install VSCode. Install Plaformio extension for VSCode.

1) [Find `core_dir` of Platformio](https://docs.platformio.org/en/latest/projectconf/section_platformio.html#projectconf-pio-core-dir).

1) Create a new folder called `boards` under `core_dir` folder.

1) In the `sparkmarkII` folder you will find `jdluck_sparkmark2.json`. Copy this to the `boards` folder you just made.

1) Go back to the `core_dir` of Platformio and create folders in this structure under it: `packages/framework-arduino-avr/variants/jdluck_spark/`.

1) Finally, copy `pins_arduino.h` from the `sparkmarkII` folder into the folder created in the previous step.