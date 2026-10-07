// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// In-place app-drawer badge updates — replaces the old full-grid rebuild on
// every badge change (#612).
void appDrawerBadgeSetLabel(int act, struct _lv_obj_t* lbl);
void appDrawerBadgeClear();
void appDrawerRefreshBadges();
