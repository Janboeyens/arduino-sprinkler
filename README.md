# arduino-sprinkler
Bits and pieces of a multi-system automated sprinkler

1. Main Sprinkler that sets next time on - and duration per zone.  Also cycles through the zones (up to 6) switching each on for the set duration.  In addition it switches on a 12V transformer that drives the valves that allows the zones to be switched on and off.  At the same time as the transformer is switched on an optiopnal AC plug (1.1) is also switched on that can be used to drive the main pressure pump.
(This can be used as is seperate from the other optional components)

2. A Messenger that connects to main sprinkler - by checking if the transformer is currently on.  If so it sends a message (via Serial print using an XBee chip) to alert a third (smart-plug) to switch on.  This facilitates the implementation of a distant pressure pump - and replaces the need to run a lenghty cables (from 1.1) to the pressure pump.  The Messanger also implements a smple switch that can be mounted near additional tap(s) that allow the pressure pump  to be switched on and off manually.

3. Smart plug - with three buttons on/off/auto  If in auto mode it switchess on and off upon receiving remote signals (via Xbee) from the Messenger(2) app.  The smart plug is used to switch the pressure pump on and off.

If the project uses pressured water (such as from a municipal source) parts 2 and 3 are completely superflous.  Most pressure pumps have a auto-on/off that works with the pressure in the pipes.  However, I decided I prefer the pump to be switched off completely when not needed.

