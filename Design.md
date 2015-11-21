# Photon Software Design

## The big picture

There are three projects that comprise the thermostat software:

1. The embedded software that runs on the Photon micro-controller.
2. A mobile app that runs on iOS or Android devices and which allows the
   thermostat to be controlled remotely.
3. (Optional) A web-accessible app that also allows the thermostat to be
   controlled remotely.

We may combine projects 2 and 3, i.e., create a web app that can be accessed
from a mobile phone.  The current plan is to give the thermostat a simple
touch-screen interface, but put more complicated features, such as the setting
the schedule, on the web/mobile app.

## Photon micro-controller

Software on the Photon (and its predecessor, the Spark Core) follows the
Arduino model of `setup` function and a main `loop` function.  The `setup`
function is called once at the start of the program whereas the `loop`
function is called repeatedly from the operating system approximately once
every millisecond (I think). The amount of work done in each call to `loop`
must not be excessive, or else the OS will be starved of processing cycles and
will fail to maintain the WiFi connection.

## Structure of the embedded software

The embedded software running on the thermostat itself is decomposed into a
number of separate components, each of which should have its own source file
and header file.  Some of the components, such as the temperature sensor, are
passive, which means that they do nothing until asked.  Other components, such
as the thermostat core module, are active, which means that they make things
happen in response to either the passage of time or the occurrence of some
physical event. 

Active components must be given processor resources on a regular basis. The
way we achieve this is to call a `run` function for each active module each
time through the `loop` function, as follows:

    void loop()
    {
        theDisplay.run();
        theUserInput.run();
        theFurnaceControl.run();
        // etc...
    }

This round-robin scheduler is effectively a miniature multi-threaded operating
Note that, although every active component gets resources, no two components
are active at exactly the same time. This fact allows us to mostly ignore the
difficulties (such as race condition) typical of concurrent tasks.  One
component can communicate with another simply by setting the value of an
ordinary variable, without fear that both components will access the variable
simultaneously.

One special task it does is to main two copies of data in the settings module:
a "current" copy and a "last" copy. By comparing elements of the two copies,
any component can tell whether something it cares about has changed since the
last time `run` was called. The display module, for example, can see whether
or not the temperature setting was changed (by the user input module), and
update the display accordingly.

### List of software components

Below is an initial listing of the components in the thermostat software. Each
component is either *ACTIVE* or *PASSIVE* and also has a main class name that
is used in the actual C++ code.

* Main loop, `MainLoop`
  * Calls each active component's `run` function in turn.
  * Keeps track of the previous value of settings.
* Temperature sensing module, `TempSensor` (*PASSIVE*)
  * Reads temperature (and humidity) sensor and reports current values
* HVAC Control module, `HvacControl`  (*PASSIVE*)
  * Turn heat on/off
  * Turn A/C on/off
  * Turn fan on/off
* Settings module, `Settings` (*PASSIVE*)
  * Hold current settings
  * For each setting, also hold setting from previous run of main loop
  * Settings (partial list)
    * Fahrenheit or Celsius
    * Current target temperatures (high and low)
    * Boost temperatures (high and low)
    * Current state of the HVAC (heat, a/c, fan, off)
    * Mode (normal, override, boost, away)
* Digole library `DigoleSerialDisp` (*PASSIVE*)
  * Modified library for talking to the Digole display hardware
* Thermostat core module `ThermostatCore` (*ACTIVE*)
  * Reads the temperature from the sensing module
  * Compares temperature to current settings (including boost)
  * Turns furnace functions on and off
* Display module, `Display` (*ACTIVE*)
  * Get temperature from the temperature module
  * Get time from the system
  * Get current settings from the global settings or setting module (?)
  * Determine what's changed
  * Format and display everything
* User input module `UserInput` (*ACTIVE*)
  * Detect if user has pressed a button
  * Change a setting or dispatch the event to the appropriate module
* Schedule manager, `Schedule` (*ACTIVE*)
  * See if a scheduled event has occurred since last checked
  * Update the temperature setting
* Boost Manager, `BoostManager` (*ACTIVE*)
  * Responds to attempts to set the temperature beyond normal operating range
  * Returns to normal when boost interval has expired
* Internet command processor, `InternetCommands` (*ACTIVE*)
  * Listen for changes to mode, settings, or schedule
  * Dispatch to appropriate module
* Logger, `Logger` (*ACTIVE*)
  * Send data to internet

## Temperature representation

### Temperature units and scale

Temperature is stored in variables of type `float`, which have enough range
to store any reasonable household temperature to a precision of 0.5 degrees F.
Logically, there are at least three units that can be used to store the
temperature: Celsius, Fahrenheit, or some sort of normalized unit for internal
use. Because of the way our temperature sensor works, the normalized unit is
identical to Celsius. (We considered using Kelvin as the normalized scale, but
we were concerned that the larger numbers might have reduced precision.)

In practice, we need variables to store temperature in one of two forms:
normalized or human-readable.  The normalized form, as we said, is the same as
Celcius. The human-readable form is either Celsius or Fahrenheit, depending on
the current setting for the Thermostat.  In order to avoid confusion within
the code, we use a Hungarian-notation suffix of `FC` (for Fahrenheit/Celsius)
for the human-readable temperatures. Any temperature function or variable
without the `FC` suffix represents a normalized temperature.  The `Settings`
module contains functions for converting between normalized units and the
current temperature (human-readable) units.

### Temperature rounding

When displaying temperatures to the user, we want to round the numbers in a
reasonable way both to avoid showing unreasonable precision and to avoid the
effects of round-off error in the floating-point-to-string conversion.  The
`Settings` module contains a precision field that tells the display module how
to display temperatures. Reasonable values are 0.5 or 1.0 for Fahrenheit
and 0.2 or 0.5 for Celsius.  The `Settings` function for converting a
normalized temperature to current units lets you choose whether to round the
result to the currently-set precision or not (defaults to yes).
