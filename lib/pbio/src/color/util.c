// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2022 The Pybricks Authors

#include <pbio/color.h>

int32_t sin_deg_branch0(int32_t x) {
    return 201 * x - x * x;
}

// integer sine approximation from degrees to (-10000, 10000)
int32_t sin_deg(int32_t x) {
    x = x % 360;
    if (x < 90) {
        return sin_deg_branch0(x);
    }
    if (x < 180) {
        return sin_deg_branch0(180 - x);
    }
    if (x < 270) {
        return -sin_deg_branch0(x - 180);
    }
    return -sin_deg_branch0(360 - x);
}

int32_t cos_deg(int32_t x) {
    return sin_deg(x + 90);
}

// gets squared cartesian distance between hsv colors mapped into a chroma-lightness-bicone
int32_t pbio_get_bicone_cost(const pbio_color_hsv_t *hsv_a, const pbio_color_hsv_t *hsv_b) {

    int32_t a_h = hsv_a->h;
    int32_t a_s = hsv_a->s;
    int32_t a_v = hsv_a->v;

    int32_t b_h = hsv_b->h;
    int32_t b_s = hsv_b->s;
    int32_t b_v = hsv_b->v;

    // chroma (= radial coordinate in bicone) of a and b (0-10000)
    int32_t radius_a = a_v * a_s;
    int32_t radius_b = b_v * b_s;

    // lightness (= z-coordinate in bicone) of a and b (0-20000)
    int32_t lightness_a = (200 * a_v - a_s * a_v);
    int32_t lightness_b = (200 * b_v - b_s * b_v);

    // x and y deltas of a and b in HSV bicone (-20000, 20000)
    int32_t delx = (radius_b * cos_deg(b_h) - radius_a * cos_deg(a_h)) / 10000;
    int32_t dely = (radius_b * sin_deg(b_h) - radius_a * sin_deg(a_h)) / 10000;
    // z delta of a and b in HSV bicone (-20000, 20000)
    int32_t delz = (lightness_b - lightness_a);

    int32_t cdist = delx * delx + dely * dely + delz * delz;

    return cdist;
}
