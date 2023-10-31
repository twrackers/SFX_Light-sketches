# SFX_Light-sketches #
-
This repository holds Arduino sketches which are used to create lighting special effects.

At this time two sketches have been uploaded.  Both use Adafruit PWM (pulse width modulation) driver devices to control multiple LEDs at the same time.

## LighthouseBeacon ##
This sketch will be used to create the "rotating" beacon at the top of a lighthouse.  The lighthouse itself is an N-scale (1:160 scale) model which was designed to hold a simple grain-of-rice incandescent bulb with no driving electronics.  I plan to replace that bulb with a small 3D-printed piece which will have 12 [Pico LEDs](https://evandesigns.com/products/chip-nano-pico-leds?variant=39985935646768) from [Evan Designs](https://evandesigns.com/) (Fort Collins, Colorado, USA) mounted around its tip.

The LEDs are 1 mm long, which in N scale would make them equivalent to lights of 160 mm or over 6 inches tall.  I plan to use the ice-blue color of LEDs rather than white for visual effect; we will see how that comes out.  The twelve LEDs will be arranged around the printed part facing outwards 30 degrees apart.

The LEDs have thin wires (30 gauge maybe) which should be long enough to reach to the base of the model.  In the base will be an [Adafruit 12-channel 16-bit PWM LED Driver](https://www.adafruit.com/product/1455), connected via SPI to a compact Arduino processor, probably an Arduino Micro, an Adafruit Pro Trinket, or something similar.

To simulate the rotating lighthouse light, the PWM driver will be used to illuminate the LEDs in sequence around their mount, with each LED fading from off to full on to off again in about 0.8 second.  Each LED will be timed to begin turning on at the moment the one before reaches full-on, which means it will be fully on when the prior goes fully off.  From a distance, this should give the illusion of a light of more-or-less constant brightness rotating once roughly every 5 seconds.

## SunGlitter ##
This sketch will be used to simulate sunlight glinting off ripples of water in a canal.  The canal is being built in a [single-width T-TRAK module](https://klawndyke.com/catalog/index.php?main_page=product_info&cPath=4&products_id=20) from [Klawndyke's](https://klawndyke.com/catalog/) (Des Moines, Iowa, USA).  So far I have added KATO Unitrack, a 3D-printed deck bridge (model provided by Klawndyke's), and 3D-printed "stone" walls (models created by myself using Autodesk Fusion 360).

The bottom surface of the canal will be painted over with a water-blue translucent glaze to form the rippled water surface.  Embedded below the glaze, lying on the wooden surface, will be 24 randomly placed ice-blue Pico LEDs (the same type being used fo the lighthouse beacon described above).

![](file:///home/pi/ssd/Pictures/canal-module-smaller.jpg)

The `SunGlitter` sketch will randomly make the LEDs fade from off to on to off again, to give the illusion of a brief glint of sunlight reflecting off the surface in that spot.

The LEDs will be driven by an [Adafruit 24-channel 12-bit PWM LED Driver](https://www.adafruit.com/product/1429), again connected by SPI to a compact Arduino-compatible microcontroller.