#ifndef KEYMAP_C
#define KEYMAP_C

#include "key_definitions.h"
#include "keyboard_config.h"
#include "keymap.h"

// A bit different from QMK, default returns you to the first layer, LOWER and raise increase/lower layer by order.
#define DEFAULT 0x100
#define LOWER 0x101
#define RAISE 0x102

// Keymaps are designed to be relatively interchangeable with QMK
enum custom_keycodes {
	QWERTY, NUM,
    PLUGINS,
};

//Set these for each layer and use when layers are needed in a hold-to use layer
enum layer_holds {
	QWERTY_H = LAYER_HOLD_BASE_VAL, NUM_H,FUNCS_H
};

// array to hold names of layouts for oled
char default_layout_names[LAYERS][MAX_LAYOUT_NAME_LENGTH] = { "QWERTY", "NUM",
		  "Plugins",
		};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
   {
	{KC_ESC,  KC_ENT,/**/	KC_2,    	KC_3,    KC_4,    	KC_5,    KC_6,    	KC_7,    	KC_8,    KC_9,/**/  KC_0,		KC_MINUS,    /**/KC_MINUS },
	{KC_DEL,  KC_1,    		KC_W,    	KC_E,    KC_R,    	KC_T,    KC_Y,    	KC_U,    	KC_I,    KC_O,    	KC_0,    	KC_RBRC,    /**/KC_INT1},
	{KC_TAB,  KC_Q,    		KC_S,    	KC_D,    KC_F,    	KC_G,    KC_H, 	KC_J,    	KC_K,    KC_L,    	KC_P, 		KC_LBRC,     /**/KC_EQUAL},
	{KC_RALT, KC_A,    		KC_X,    	KC_C,    KC_V,    	KC_B,    KC_N,    	KC_M,    	KC_COMM, KC_DOT,  	KC_SCLN, 	KC_QUOT,     /**/KC_DELETE},
	{KC_LCTRL,KC_Z, 		KC_LALT, 	KC_LWIN, KC_LSHIFT, KC_SPACE,  KC_BSPACE,	KC_ENTER,   KC_NUHS, KC_DELETE,	KC_SLSH,  	KC_GRV,      /**/KC_NO }
	},
   {
	{KC_NO,  KC_NO,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_NO,           KC_NO, KC_NO},
	{KC_NO,  KC_F1,    KC_HOME,    KC_UP,    KC_END,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_F10,       KC_F11,KC_NO},
	{KC_NO,  KC_NO,    KC_LEFT,    KC_DOWN,    KC_RIGHT,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_F12, KC_NO},
	{KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,           KC_NO, KC_NO} ,
	{KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,           KC_NO, KC_NO}
	}
};

//NOTE: For this keymap due to wiring constraints the the two last rows on the left are wired unconventionally
// Each keymap is represented by an array, with an array that points to all the keymaps  by order
	 uint16_t _QWERTY[MATRIX_ROWS][KEYMAP_COLS]={

			/* Qwerty
			 * ,-----------------------------------------.    .-----------------------------------------.
			 * |  ~   |   1  |   2  |   3   |   4  |   5  |   |    6  |   7  |   8  |   9  |   0  |   -  |
			 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
			 * | Esc  |   Q  |   W  |   E   |   R  |   T  |   |    Y  |   U  |   I  |   O  |   P  |  =   |
			 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
			 * | Tab  |   A  |   S  |   D   |   F  |   G  |   |    H  |   J  |   K  |   L  |   ;  |  '   |
			 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
			 * | Shift|   Z  |   X  |   C   |   V  |   B  |   |    N  |   M  |   ,  |   .  |   /  |  up  |
			 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
			 * | Ctrl | GUI  |  Alt | Space |Space |NUM_H |   |  Del  |Bspc  |Enter | Left | Right|Down  |
			 * `------------------------------------------'    ------------------------------------------'
			 *                            |default |RAISE|    |lower|NUM |
			 *                              --------------    ---------------
			 */
			  {KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINUS },
			  {KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQUAL},
			  {KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},
			  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_UP} ,
			  {KC_LCTRL,KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  NUM_H,   KC_DEL,  NUM,     LOWER,   KC_BSPC, KC_ENT,  KC_LEFT }

	};

	 uint16_t _NUM[MATRIX_ROWS][KEYMAP_COLS]={


			/* Nums
			 * ,-----------------------------------------.    .-----------------------------------------.
			 * | F1   |   F2 |   F3 |   F4  |  F5  |  F6  |   |   F7  |  F8  |  F9  |  F10 | F11  |  F12 |
			 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
			 * | Esc  |   Q  |   W  |   E   |   R  |   T  |   |    Y  |   U  |   I  |   [ |   ]   |  \   |
			 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
			 * | Tab  |   A  |   S  |   D   |   F  |   G  |   |    H  |   J  |   K  |   L  |   ;  |  '   |
			 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
			 * | Shift|   Z  |   X  |   C   |   V  |   B  |   |    N  |   M  |   ,  |   .  |   /  |  up  |
			 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
			 * | Ctrl | GUI  |  Alt | Space |Space |NUM_H |   |  Del  |Bspc  |Enter | Left | Right  |Down  |
			 * `------------------------------------------'    ------------------------------------------'
			 *                            |default |RAISE|    |lower|NUM |
			 *                              --------------    ---------------
			 */
			  {KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINUS },
			  {KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQUAL},
			  {KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},
			  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_UP} ,
			  {KC_LCTRL,KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  NUM_H,   KC_DEL,  NUM,     LOWER,   KC_BSPC, KC_ENT,  KC_LEFT}
	};

	 uint16_t _PLUGINS[MATRIX_ROWS][KEYMAP_COLS]={


				/* Plugins
				 * ,-----------------------------------------.    .-----------------------------------------.
				 * |  ~   |   1  |   2  |   3   |   4  |   5  |   |    6  |   7  |   8  |   9  |   0  |   -  |
				 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
				 * | Esc  |   Q  |   W  |   E   |   R  |   T  |   |    Y  |   U  |   I  |   O  |   P  |  =   |
				 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
				 * | Tab  |   A  |   S  |   D   |   F  |   G  |   |    H  |   J  |   K  |   L  |   ;  |  '   |
				 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
				 * | Shift|   Z  |   X  |   C   |   V  |   B  |   |    N  |   M  |   ,  |   .  |   /  |  up  |
				 * |------+------+------+-------+------+------|   |-------+------+------+------+------+------|
				 * | Ctrl | GUI  |  Alt | Space |Space |NUM_H |   |  Del  |Bspc  |Enter | Left | Right  |Down  |
				 * `------------------------------------------'    ------------------------------------------'
				 *                            |default |RAISE|    |lower|NUM |
				 *                              --------------    ---------------
				 */
				  {KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINUS},
			  {KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQUAL},
			  {KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},
			  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_UP} ,
			  {KC_LCTRL,KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  NUM_H,   KC_DEL,  NUM,     LOWER,   KC_BSPC, KC_ENT,  KC_LEFT}

		};
 //Create an array that points to the various keymaps
uint16_t (*default_layouts[])[MATRIX_ROWS][KEYMAP_COLS] = { &_QWERTY, &_NUM,
			&_PLUGINS
		};

uint8_t current_layout = 0;

#endif
