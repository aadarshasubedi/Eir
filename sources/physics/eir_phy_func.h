#pragma once

#include "eir_aabb.h"

bool eir_phy_check_aabb_intersection(const eir_phy_aabb_t * a, const eir_phy_aabb_t * b);
float eir_phy_get_x_aabb_intersection_depth(const eir_phy_aabb_t * a, const eir_phy_aabb_t * b);
float eir_phy_get_y_aabb_intersection_depth(const eir_phy_aabb_t * a, const eir_phy_aabb_t * b);

