# Lear-45-Pressure-Panel
Flight Simulator Lear 45 Pressure Panel

See the project here:
http://blog.mkme.org/index.php/lear-panel/

A while back I set about interfacing an Arduino to my existing Learjet 45 flight simulator.  Through some searching and reading I determined this would indeed be possible.

The first panel I decided to interface was the Learjet Pressurization Panel which had not yet been replicated by members on the Hangar45.net community.  I was on my own :)

I sourced the panel itself from Ron on Hangar 45 which included both the backer and the non-illuminated version of the face plate.  Thanks Ron!  From here I took an Arduino Nano and interfaced it via the Nokia 5110 LCD.  This is not a perfect match to the Lear- but was better than nothing for sure.

Here is the entire source code for all to enjoy.  

Updated Aug 2015 with Global contrast Int

Updated Dec 2015 with library zip file for those that cannot find it. 
Also added CAD files for those that need to make the bezel and knob.