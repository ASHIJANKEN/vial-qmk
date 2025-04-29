/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include <stdio.h>
#ifdef CONSOLE_ENABLE
  #include <print.h>
#endif
#include <string.h>
#include "keymap_japanese.h" // qmk_firmware_bmp_vial/quantum/keymap_extras/keymap_japanese.h
// #include "bmp.h"
#include "bmp_custom_keycodes.h" // qmk_firmware_bmp_vial/tmk_core/protocol/bmp/bmp_custom_keycodes.h
// #include "keycode_str_converter.h"
#include "process_combo.h"

extern uint8_t is_master;

// #define OLED_TIMEOUT 0
#define WINDOWS_MODE true
#define MAC_MODE false
#define KANA_MODIFIER 17296

enum layer_number {
  _QWERTY_WIN = 0,
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST,
};

enum custom_keycodes {
  QWERTY = BMP_SAFE_RANGE,
  QWERTY_WIN,
  UBUNTU,
  MAC,
  LOWER,
  RAISE,
  ADJUST,
  INFO,
  MY_EISU,
  MY_KANA,
};

const key_override_t rprn_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_0, JP_RPRN);
const key_override_t at_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_2, JP_AT);
const key_override_t circ_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_6, JP_CIRC);
const key_override_t ampr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_7, JP_AMPR);
const key_override_t astr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_8, JP_ASTR);
const key_override_t lprn_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_9, JP_LPRN);
const key_override_t unds_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_MINS, JP_UNDS);
const key_override_t plus_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_EQL, JP_PLUS);
const key_override_t tild_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_GRV, JP_TILD);
const key_override_t pipe_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_YEN, JP_PIPE);
const key_override_t coln_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_SCLN, JP_COLN);
const key_override_t dquo_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_QUOT, JP_DQUO);

uint8_t mod_state = 0;
bool os_mode = false;
// It means which key user pushed, xKANA or xEISU. User assumes IME is in the mode he chose.
bool is_kana_user = false;
// It indicates the actual mode, whether the PC is in EISU mode or KANA mode.
bool is_kana_internal = false;

// keymaps[] has no meaning but is necessary to build correctly.
const uint16_t PROGMEM keymaps[1][MATRIX_ROWS][MATRIX_COLS] = {{{
    KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_R, KC_S, KC_T
}}};
const uint16_t PROGMEM encoder_map[1][NUM_ENCODERS][NUM_DIRECTIONS] = {{{KC_MS_U, KC_MS_D}}};

// Defining this array is required to avoid build error due to commit: 55456cd
// combo_t key_combos[] = {};

int layer_state_to_num(layer_state_t state) {
  int n = 0;
  for (; n < 16; ++n) {
    if (((state>>n) & 0x1) == 1) break;
  }
  return n;
}

// Called when a layer changed.
layer_state_t layer_state_set_user(layer_state_t state) {
  // Call _ADJUST
  state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);

// #ifdef CONSOLE_ENABLE
//   uprintf("%d layer_state_set_user\n", state);
//   uprintf("%d eeconfig_read_default_layer\n", eeconfig_read_default_layer());
//   uprintf("%d layer_state_to_num\n", layer_state_to_num(eeconfig_read_default_layer()));
// #endif
  return state;
}

/*
 * Called after powering on.
 * I think this is like init() in Arduino.
 */
extern const key_override_t **key_overrides;
void keyboard_post_init_user(void) {
  key_overrides = (const key_override_t *[]){
    &rprn_key_override,
    &at_key_override,
    &circ_key_override,
    &ampr_key_override,
    &astr_key_override,
    &lprn_key_override,
    &unds_key_override,
    &plus_key_override,
    &tild_key_override,
    &pipe_key_override,
    &coln_key_override,
    &dquo_key_override,
    NULL
  };
  // reload_key_override();
}

void tap_without_modifier(uint8_t mods, uint16_t keycode) {
  if (mods & MOD_MASK_CSA) {
    if (mods & MOD_MASK_SHIFT) unregister_code16(KC_LSFT);
    if (mods & MOD_MASK_CTRL) unregister_code16(KC_LCTL);
    if (mods & MOD_MASK_ALT) unregister_code16(KC_LALT);
    tap_code16(keycode);
    if (mods & MOD_MASK_SHIFT) register_code16(KC_LSFT);
    if (mods & MOD_MASK_CTRL) register_code16(KC_LCTL);
    if (mods & MOD_MASK_ALT) register_code16(KC_LALT);
  } else {
    tap_code16(keycode);
  }
}

// Called when a key pressed/released.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  mod_state = get_mods();
  #ifdef CONSOLE_ENABLE
    uprintf("[lb] pru:%d\n", keycode);
  #endif // CONSOLE_ENABLE

  switch (keycode) {
    case QWERTY:
    case QWERTY_WIN:
      ;
      break;
    default:
      ;
  }
  switch (keycode) {
    case KC_1 ... KC_0:
    case JP_EXLM:
    case JP_AT:
    case JP_HASH:
    case JP_DLR:
    case JP_PERC:
    case JP_CIRC:
    case JP_AMPR:
    case JP_ASTR:
    case JP_LPRN:
    case JP_RPRN:
    case JP_LCBR:
    case JP_RCBR:
    case JP_GRV:
    case JP_YEN:
    case JP_PIPE:
    case JP_UNDS:
    case JP_EQL:
    case JP_PLUS:
    case JP_SCLN:
    case JP_COLN:
    case JP_QUOT:
      if (record->event.pressed && is_kana_internal == true) {
        tap_without_modifier(mod_state, KC_LNG2);
        is_kana_internal = false;
      }
      return true;
    // case INFO:
    //   if (record->event.pressed) {
    //     show_info_oled();
    //   }
    //   return false;
    case QWERTY:
      if (record->event.pressed) {
        os_mode = MAC_MODE;
        set_single_persistent_default_layer(_QWERTY);
        // show_info_oled();
      }
      return false;
    case QWERTY_WIN:
      if (record->event.pressed) {
        os_mode = WINDOWS_MODE;
        set_single_persistent_default_layer(_QWERTY_WIN);
        // show_info_oled();
      }
      return false;
    case MY_EISU:
      if (record->event.pressed) {
        tap_without_modifier(mod_state, KC_LNG2);
        is_kana_user = false;
        is_kana_internal = false;
      }
      return false;
    case MY_KANA:
      if (record->event.pressed) {
        tap_without_modifier(mod_state, KC_LNG1);
        is_kana_user = true;
        is_kana_internal = true;
      }
      return false;
    case LT(2,KC_LNG1):
      if (record->event.pressed && record->tap.count) {
        tap_without_modifier(mod_state, KC_LNG1);
        is_kana_user = true;
        is_kana_internal = true;
        return false;
      }
      return true;
    case LT(3,KC_LNG2):
      if (record->event.pressed && record->tap.count) {
        tap_without_modifier(mod_state, KC_LNG2);
        is_kana_user = false;
        is_kana_internal = false;
        return false;
      }
      return true;
    case KC_A ... KC_Z:
    case KC_SPACE:
      if (record->event.pressed) {
        if (is_kana_user != is_kana_internal) {
          if (is_kana_user == true) {
            tap_without_modifier(mod_state, KC_LNG1);
            is_kana_internal = true;
          } else {
            tap_without_modifier(mod_state, KC_LNG2);
            is_kana_internal = false;
          }
        }
      }
      return true;
  }
  return true;
}