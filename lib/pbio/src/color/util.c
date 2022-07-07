// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2022 The Pybricks Authors

#include <pbio/color.h>

int32_t sin_deg_branch0(int32_t x) {
    return (201*x-x*x);
}

// simple integer sine mapping from degrees to [-10000, 10000]
int32_t sin_deg(int32_t x) {
    x = x % 360;
    if (x<90) return sin_deg_branch0(x);
    if (x<180) return sin_deg_branch0(180-x);
    if (x<270) return -sin_deg_branch0(x-180);
    return -sin_deg_branch0(360-x);
}

int32_t cos_deg(int32_t x) {
    return sin_deg(x+90);
}

// gets squared cartesian distance between hsv colors mapped into a chroma-lightness-bicone
int32_t pbio_get_bicone_cost(const pbio_color_hsv_t *hsv_a, const pbio_color_hsv_t *hsv_b, const int32_t chroma_weight) {

    int32_t a_h = hsv_a->h;
    int32_t a_s = hsv_a->s;
    int32_t a_v = hsv_a->v;

    int32_t b_h = hsv_b->h;
    int32_t b_s = hsv_b->s;
    int32_t b_v = hsv_b->v;

    // choroma (= radial coordinate in bicone) of a and b (0-100)
    int32_t radius_a = a_v*a_s/100;
    int32_t radius_b = b_v*b_s/100;

    // lightness (=z-coordinate in bicone) of a and b (0-200)
    int32_t lightness_a = 200*a_v - a_s*a_v;
    int32_t lightness_b = 200*b_v - b_s*b_v;

    // x, y and z deltas between cartesian coordinates of a and b in HSV bicone
    int32_t delx = chroma_weight*(radius_b*cos_deg(b_h) - radius_a*cos_deg(a_h))/10000;
    int32_t dely = chroma_weight*(radius_b*sin_deg(b_h) - radius_a*sin_deg(a_h))/10000;
    int32_t delz = (100-chroma_weight)*(lightness_b - lightness_a)/100;

    int32_t cdist = delx*delx + dely*dely + delz*delz;
    
    return cdist;
}
