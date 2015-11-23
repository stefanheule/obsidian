# Obsidian [![Build status](https://travis-ci.org/stefanheule/obsidian.svg?branch=master)](https://travis-ci.org/stefanheule/obsidian)

![Obsidian banner](resources/images/banner.png)

Available in the Pebble App Store:

[![Available on the Pebble App Store](http://pblweb.com/badge/55ee57da2b0b31132c00008b/black/small)](https://apps.getpebble.com/applications/55ee57da2b0b31132c00008b)

[Blog post introducing the initial version of Obsidian](http://stefanheule.com/blog/obsidian-a-usable-and-elegant-analog-watchface-for-the-pebble-time/).

## Overview

This is a usable and elegant analog watchface for the Pebble, Pebble Steel, Pebble Time, Pebble Time Steel as well as the Pebble Time Round.
It's main features are:

- High contrast colors, clearly visible in most light conditions.
- Display of the current date (day and month) as well as day of the week.
- The date display is never obstructed by the watch hands.  If they would cover the date, the date moves slightly to the side.
- Display of the battery level.
- Change in background for low battery levels (yellow at 30%, orange at 20%, and red at 10%).
- Vibrates when the bluetooth connection is lost or regained, shows an information overlay for a few seconds and keeps showing an icon when no connection is available.
- Fully configurable, both in terms of appearance (colors, minute/hour ticks, etc.) as well as behavior (if and when to vibrate, show messages, etc.).

## Screenshots (Pebble Time and Pebble Time Steel)

The main goal when building this watchface was for it to be practical.  For instance, the date display should never be obstructed by the watch hands:

![Screenshot of the date display moving](screenshots/basalt/date.png)

The watchface also alerts the user when the bluetooth connection is lost or regained, by vibrating and showing an information overlay.  After a few seconds, the overlay goes away, and a small bluetooth icon stays if no connection is available:

![Screenshot of the bluetooth alerts](screenshots/basalt/bluetooth.png)

The watchface always shows a small battery indicator in the top right corner.  However, if the battery starts running low, the background changes to yellow, orange and finally red:

![Screenshot of the battery alert](screenshots/basalt/battery.png)

Finally, it is fully configurable, both in terms of what it looks like as well as its behavior (vibration, bluetooth message, etc.).  Some screenshots of different configurations:

![Alternative color theme](screenshots/basalt/overview.png)

![Alternative color theme](screenshots/basalt/theme-overview-1.png)

![Alternative color theme](screenshots/basalt/theme-overview-2.png)

The configuration dialog to achieve this flexibility looks as follows:

![Configuration](screenshots/config.png)

## Screenshots (Pebble Time Round)

The watchface also supports the Pebble Time Round:

![Screenshot of the date display moving on chalk platform](screenshots/chalk/date.png)

![Screenshot of the bluetooth alerts on chalk platform](screenshots/chalk/bluetooth.png)

![Alternative color theme](screenshots/chalk/overview.png)

![Alternative color theme](screenshots/chalk/theme-overview-1.png)

![Alternative color theme](screenshots/chalk/theme-overview-2.png)

## Screenshots (Pebble and Pebble Steel)

On the original Pebble and Pebble Steel, the colors are limited to black and white, but all other features are still available.  Some screenshots:

![Screenshot of the date display moving on aplite platform](screenshots/aplite_date.png)

![Screenshot of the bluetooth alerts on aplite platform](screenshots/aplite_bluetooth.png)

![Configuration on aplite platform](screenshots/aplite_config.png)

## Building from Source

The project can be build using the command `make build`.  The `Makefile` also includes various other targets, such as ones to install the watchface in an emulator or on the Pebble watch.  There are also targets to automatically take the screenshots that are part of this distribution.

### Versioning

The watchface itself uses relatively arbitrary version numbers of MAJOR.MINOR.  In addition to that, the configuration format (the JavaScript config object) is versioned, too, using a single integer.  Different version of the watchface may share the same configuration format.

| Watchface version | Configuration version |
|------------------:|----------------------:|
|         up to 1.2 |                     1 |
|          1.3, 1.4 |                     2 |

## Changelog

**Version 1.4**

- Adding a version for the chalk platform (Pebble Time Round).

**Version 1.3**

- Adding a version for the aplite platform (Original Pebble).

**Version 1.0, 1.1, 1.2**

- Initial version

## Contributing

Pull requests are welcome.

## License

Copyright 2015 Stefan Heule

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

## Influence

There are a few watch faces that have influenced the design of Obsidian:

- [Simple.Watch](http://apps.getpebble.com/en_US/application/5551c537c60fc8732d000006)
- [TH3](http://apps.getpebble.com/en_US/application/5551c4a91113bc93a00000de)
