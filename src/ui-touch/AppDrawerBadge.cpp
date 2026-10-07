// SPDX-License-Identifier: GPL-3.0-or-later
// App-drawer badge in-place updates — see AppDrawerBadge.h.
#include <lvgl.h>
#include <cstdio>

extern int   uiUnreadTotal();
extern int   uiUnreadMentions();
extern lv_obj_t** appDrawerRootPtr();
extern uint32_t*  appDrawerBadgeSigPtr();
extern bool*      appDrawerUpdateAvailPtr();

static const int kActChats     = 0;
static const int kActMentions  = 6;
static const int kActSettings  = 3;
static const int kActLuaBase   = 100;
static const int kLabelCount   = kActLuaBase + 16;

static lv_obj_t* s_labels[kLabelCount] = {};

void appDrawerBadgeSetLabel(int act, lv_obj_t* lbl) {
  if (act >= 0 && act < kLabelCount) s_labels[act] = lbl;
}

void appDrawerBadgeClear() {
  for (int i = 0; i < kLabelCount; ++i) s_labels[i] = nullptr;
}

void appDrawerRefreshBadges() {
  if (!*appDrawerRootPtr()) return;

  const int unread   = uiUnreadTotal();
  const int mentions = uiUnreadMentions();
  const bool upd     = *appDrawerUpdateAvailPtr();

  struct { int act; int count; } entries[] = {
    { kActChats,    unread   },
    { kActMentions, mentions },
    { kActSettings, upd ? -1 : 0 },
  };

  for (size_t i = 0; i < sizeof(entries) / sizeof(entries[0]); ++i) {
    lv_obj_t* lbl = (entries[i].act >= 0 && entries[i].act < kLabelCount)
                  ? s_labels[entries[i].act] : nullptr;
    if (!lbl) continue;
    lv_obj_t* bdg = lv_obj_get_parent(lbl);
    if (!bdg) continue;

    char bn[8];
    if (entries[i].count < 0)      snprintf(bn, sizeof bn, "!");
    else if (entries[i].count > 0) snprintf(bn, sizeof bn, "%d", entries[i].count);
    else bn[0] = '\0';

    lv_label_set_text(lbl, bn);
    if (entries[i].count == 0) lv_obj_add_flag(bdg, LV_OBJ_FLAG_HIDDEN);
    else lv_obj_clear_flag(bdg, LV_OBJ_FLAG_HIDDEN);
  }

  *appDrawerBadgeSigPtr() = ((uint32_t)(unread & 0x7FFF) << 16) |
                             (uint32_t)(mentions & 0xFFFF) |
                             (upd ? 0x80000000u : 0u);
}
