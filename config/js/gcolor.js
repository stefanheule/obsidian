/*

GColor
A small utility library for dealing with GColor in the new PebbleSDK.

Source: https://gist.github.com/matthewtole/49da38377b0b0a423407
Addition by Stefan Heule: GColor.toHex

----------------------
The MIT License (MIT)
Copyright © 2015 Matthew Tole
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

var GColor = (function () {
  
  return {
    fromHex: GColorFromHex,
    toName: GColorName,
    toHex: GColorToHex,

    ArmyGreen             : 212,
    BabyBlueEyes          : 235,
    Black                 : 192,
    Blue                  : 195,
    BlueMoon              : 199,
    Brass                 : 233,
    BrightGreen           : 220,
    BrilliantRose         : 246,
    BulgarianRose         : 208,
    CadetBlue             : 218,
    Celeste               : 239,
    ChromeYellow          : 248,
    CobaltBlue            : 198,
    Cyan                  : 207,
    DarkCandyAppleRed     : 224,
    DarkGray              : 213,
    DarkGreen             : 196,
    DukeBlue              : 194,
    ElectricBlue          : 223,
    ElectricUltramarine   : 211,
    FashionMagenta        : 242,
    Folly                 : 241,
    Green                 : 204,
    Icterine              : 253,
    ImperialPurple        : 209,
    Inchworm              : 237,
    Indigo                : 210,
    IslamicGreen          : 200,
    JaegerGreen           : 201,
    JazzberryJam          : 225,
    KellyGreen            : 216,
    LavenderIndigo        : 231,
    Liberty               : 214,
    LightGray             : 234,
    Limerick              : 232,
    Magenta               : 243,
    Malachite             : 205,
    MayGreen              : 217,
    MediumAquamarine      : 222,
    MediumSpringGreen     : 206,
    Melon                 : 250,
    MidnightGreen         : 197,
    MintGreen             : 238,
    Orange                : 244,
    OxfordBlue            : 193,
    PastelYellow          : 254,
    PictonBlue            : 219,
    Purple                : 226,
    Purpureus             : 230,
    Rajah                 : 249,
    Red                   : 240,
    RichBrilliantLavender : 251,
    RoseVale              : 229,
    ScreaminGreen         : 221,
    ShockingPink          : 247,
    SpringBud             : 236,
    SunsetOrange          : 245,
    TiffanyBlue           : 202,
    VeryLightBlue         : 215,
    VividCerulean         : 203,
    VividViolet           : 227,
    White                 : 255,
    WindsorTan            : 228,
    Yellow                : 252
  };
  
  // Convert a hex color (e.g. FFFFFF) into a GColor8 value used by the
  // Pebble SDK.
  function GColorFromHex(hex) {
    console.log(hex);
    var hexNum = parseInt(hex, 16);
    var a = 192;
    var r = (((hexNum >> 16) & 0xFF) >> 6) << 4;
    var g = (((hexNum >>  8) & 0xFF) >> 6) << 2;
    var b = (((hexNum >>  0) & 0xFF) >> 6) << 0;
    return a + r + g + b;
  }
  
  // Get the GColor codename for a color given the integer value.
  function GColorName(color) {
    var names = Object.keys(GColor);
    for (var n = 0; n < names.length; n += 1) {
      if (GColor[names[n]] == color) {
        return names[n];
      }
    }
    return null;
  }

  // Converts a GColor color to a hex number (e.g. FFFFFF)
  function GColorToHex(color) {
    var col = color - 192;
    var r = (col >> 4) & 0x3;
    var g = (col >> 2) & 0x3;
    var b = (col >> 0) & 0x3;
    r = r | (r << 2) | (r << 4) | (r << 6);
    g = g | (g << 2) | (g << 4) | (g << 6);
    b = b | (b << 2) | (b << 4) | (b << 6);
    var res = ((r << 16) + (g << 8) + b).toString(16);
    while (res.length < 6) res = '0' + res;
    return res;
  }

}());
