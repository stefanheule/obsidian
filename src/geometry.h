// Copyright 2015-16 Stefan Heule
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef OBSIDIAN_GEOMETRY_H
#define OBSIDIAN_GEOMETRY_H

#include "obsidian.h"

GPoint get_radial_point(const int16_t distance_from_center, const int32_t angle);
bool intersect(const GPoint a0, const GPoint a1, const GPoint b0, const GPoint b1);
bool line_rect_intersect(GPoint line0, GPoint line1, GPoint rect0, GPoint rect1);
bool line2_rect_intersect(GPoint lineA0, GPoint lineA1, GPoint lineB0, GPoint lineB1, GPoint rect0, GPoint rect1);

#endif //OBSIDIAN_GEOMETRY_H
