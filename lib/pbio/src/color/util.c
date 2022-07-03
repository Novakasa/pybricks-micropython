// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2022 The Pybricks Authors

#include <pbio/color.h>

// Cost function between two colors a and b. The lower, the closer they are.
int32_t pbio_get_hsv_cost(const pbio_color_hsv_t *x, const pbio_color_hsv_t *c) {

    // Calculate the hue error
    int32_t hue_error;

    if (c->s <= 5 || x->s <= 5) {
        // When comparing against unsaturated colors,
        // the hue error is not so relevant.
        hue_error = 0;
    } else {
        hue_error = c->h > x->h ? c->h - x->h : x->h - c->h;
        if (hue_error > 180) {
            hue_error = 360 - hue_error;
        }
    }

    // Calculate the value error:
    int32_t value_error = x->v > c->v ? x->v - c->v : c->v - x->v;

    // Calculate the saturation error, with extra penalty for low saturation
    int32_t saturation_error = x->s > c->s ? x->s - c->s : c->s - x->s;
    saturation_error += (100 - c->s) / 2;

    // Total error
    return hue_error * hue_error + 5 * saturation_error * saturation_error + 2 * value_error * value_error;
}

int32_t sin_deg_branch0(int32_t x) {
    return (201*x-x*x);
}

// simple integer sine mapping from degrees to [-100, 100]
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

// gets squared cartesian distance between hsv colors mapped into a chroma-value-cone
int32_t pbio_get_cone_cost(const pbio_color_hsv_t *hsv_a, const pbio_color_hsv_t *hsv_b, const int32_t chroma_weight) {

    int32_t a_h = hsv_a->h;
    int32_t a_s = hsv_a->s;
    int32_t a_v = hsv_a->v;

    int32_t b_h = hsv_b->h;
    int32_t b_s = hsv_b->s;
    int32_t b_v = hsv_b->v;

    // radial coordinates of a and b
    int32_t radius_a = ((10000-(100-a_v)*(100-a_v))*a_s)/10000;
    int32_t radius_b = ((10000-(100-b_v)*(100-b_v))*b_s)/10000;

    // x, y and z deltas between cartesian coordinates of a and b in HSV cone
    int32_t delx = (radius_b*cos_deg(b_h) - radius_a*cos_deg(a_h))/10000;
    int32_t dely = (radius_b*sin_deg(b_h) - radius_a*sin_deg(a_h))/10000;
    int32_t delz = b_v - a_v;

    int32_t cdist = chroma_weight*(delx*delx + dely*dely) + (100-chroma_weight)*delz*delz;

    return cdist;
}

// gets squared cartesian distance between hsv colors mapped into a chroma-lightness-bicone
int32_t pbio_get_bicone_cost(const pbio_color_hsv_t *hsv_a, const pbio_color_hsv_t *hsv_b, const int32_t chroma_weight) {

    int32_t a_h = hsv_a->h;
    int32_t a_s = hsv_a->s;
    int32_t a_v = hsv_a->v;

    int32_t b_h = hsv_b->h;
    int32_t b_s = hsv_b->s;
    int32_t b_v = hsv_b->v;

    // radial coordinates of a and b
    int32_t radius_a = (a_v*(200-a_v)*a_s*(200-a_s))/1000000;
    int32_t radius_b = (b_v*(200-b_v)*b_s*(200-b_s))/1000000;

    // x, y and z deltas between cartesian coordinates of a and b in HSV bicone
    int32_t delx = chroma_weight*(radius_b*cos_deg(b_h) - radius_a*cos_deg(a_h))/10000;
    int32_t dely = chroma_weight*(radius_b*sin_deg(b_h) - radius_a*sin_deg(a_h))/10000;
    int32_t delz = (100-chroma_weight)*(200*b_v - b_s*b_v - (200*a_v - a_s*a_v))/100;

    int32_t cdist = delx*delx + dely*dely + delz*delz;
    
    return cdist;
}
