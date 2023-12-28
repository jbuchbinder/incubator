# DOCUMENTATION

## KEYPAD

```
From bottom up facing you the ribbon pins are as follows:

Bottom: voltage in
2nd from bottom: Select
3rd from bottom: Dec button
4th from bottom: Inc button
5th from bottom: Before button
6th from bottom(top of ribbon): Next button
```

From my experimentation, I've found the following mapping, from bottom to top:

- 0 : Voltage In
- 1 : SELECT
- 2 : DEC
- 3 : BEFORE
- 4 : INC
- 5 : AFTER

## RELAY

```
Color: 1Pcs 4 Channel Relay ModuleVine Customer Review of Free Product( What's this? )
TL;DR - you can use this to interface with 3.3v logic 5v logic and even 12v / 13.6v systems and even higher voltage systems to switch any loads up to AC250V 10A ; DC30V 10A

TECHNICAL INFO::

VCC is the + supply for the optocoupler and can be about 2v-35V.
JD-VCC is the supply for the relay coils which should be about 6v-6.2v to get the required 5v at the coil but will work fine at 5v
VCC can be about any voltage from about 2v-35v as long as the current stays below the max spec of 35ma per optocoupler.
.... technical info: The optocoupler has a VF is 1.2v and there is a 1k resistor in series with the optocoupler max current 35ma see ltv-817c specs.
Usage...
you can put a jumper between VCC and JD-Vcc and run the VCC for the optocouplers at 5v. .. but keeping it split (not jumpered) you could easily have a VCC at 3.3v for compatibility for 3.3v logic or 12V / 13.5 for compatibility with 12v and so on.... the VCC-JD still needs to still be in that 5V-6v range though.

for those more technically minded, schematics are easily found on the Internet,
hope you found this helpful
```

