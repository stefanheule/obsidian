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

#ifndef OBSIDIAN_DRAWING_H
#define OBSIDIAN_DRAWING_H

#include "obsidian.h"

void graphics_draw_line_with_width(GContext *ctx, GPoint p0, GPoint p1, uint8_t width);
void draw_bluetooth_logo(GContext *ctx, GPoint origin);
void bluetooth_popup(GContext *ctx, bool connected);
void background_update_proc(Layer *layer, GContext *ctx);

#endif //OBSIDIAN_DRAWING_H
