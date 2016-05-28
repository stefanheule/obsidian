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

#include "geometry.h"

/**
 * Returns a point on the line from the center away at an angle specified by tick/maxtick, at a specified distance
 */
GPoint get_radial_point(const int16_t distance_from_center, const int32_t angle) {
    GPoint result = {
            .x = (int16_t) (sin_lookup(angle) * (int32_t) distance_from_center / TRIG_MAX_RATIO) + center.x,
            .y = (int16_t) (-cos_lookup(angle) * (int32_t) distance_from_center / TRIG_MAX_RATIO) + center.y,
    };
    return result;
}

/**
 * Do the line segments a0->a1 and b0->b1 intersect?
 * Loosely based on http://stackoverflow.com/questions/4977491/determining-if-two-line-segments-intersect/4977569#4977569
 */
bool intersect(const GPoint a0, const GPoint a1, const GPoint b0, const GPoint b1) {
    GPoint va = GPoint(a1.x - a0.x, a1.y - a0.y);
    GPoint vb = GPoint(b1.x - b0.x, b1.y - b0.y);

    // test for parallel line segments
    int16_t det = vb.x * va.y - va.x * vb.y;
    if (det == 0) {
        if ((b0.x - a0.x) * va.y == (b0.y - a0.y) * va.x) {
            // the two lines are parallel, and might overlap (if the segments were extended infinitely, they would be the same line)
            return (0 <= b0.x - a0.x && b0.x - a0.x <= va.x) || (0 <= a0.x - b0.x && a0.x - b0.x <= vb.x);
        } else {
            // the two lines are parallel, and not overlapping
            return false;
        }
    }

    int16_t s = ((a0.x - b0.x) * va.y - (a0.y - b0.y) * va.x);
    int16_t t = -(-(a0.x - b0.x) * vb.y + (a0.y - b0.y) * vb.x);

    if (det < 0) {
        det = -det;
        s = -s;
        t = -t;
    }
    return 0 <= s && s <= det && 0 <= t && t <= det;
}

/**
 * Returns true if the line and rectangle intersect.
 */
bool line_rect_intersect(GPoint line0, GPoint line1, GPoint rect0, GPoint rect1) {
    return intersect(line0, line1, rect0, GPoint(rect1.x, rect0.y)) ||
           intersect(line0, line1, rect0, GPoint(rect0.x, rect1.y)) ||
           intersect(line0, line1, rect1, GPoint(rect1.x, rect0.y)) ||
           intersect(line0, line1, rect1, GPoint(rect0.x, rect1.y));
}

/**
 * Returns true if either of two lines intersects with the rectangle.
 */
bool line2_rect_intersect(GPoint lineA0, GPoint lineA1, GPoint lineB0, GPoint lineB1, GPoint rect0, GPoint rect1) {
    return line_rect_intersect(lineA0, lineA1, rect0, rect1) || line_rect_intersect(lineB0, lineB1, rect0, rect1);
}
