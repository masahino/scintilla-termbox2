#ifndef SCINTILLATERMBOX2_H
#define SCINTILLATERMBOX2_H

#ifndef TB_OPT_ATTR_W
#define TB_OPT_ATTR_W 32
#endif

#ifndef TB_OPT_EGC
#define TB_OPT_EGC
#endif

#include <termbox2.h>

/* Convert an RGB colour to the representation expected by the active
 * termbox2 output mode. */
static inline uintattr_t scintilla_termbox2_color(uint32_t rgb) {
  const int mode = tb_set_output_mode(TB_OUTPUT_CURRENT);
  const int red = (rgb >> 16) & 0xff;
  const int green = (rgb >> 8) & 0xff;
  const int blue = rgb & 0xff;

  if (mode == TB_OUTPUT_TRUECOLOR)
    return (uintattr_t)rgb;

  if (mode == TB_OUTPUT_256) {
    static const int levels[] = {0, 95, 135, 175, 215, 255};
    const int ri = (red * 5 + 127) / 255;
    const int gi = (green * 5 + 127) / 255;
    const int bi = (blue * 5 + 127) / 255;
    const int cube = 16 + 36 * ri + 6 * gi + bi;
    const int dr = red - levels[ri];
    const int dg = green - levels[gi];
    const int db = blue - levels[bi];
    const int cube_distance = dr * dr + dg * dg + db * db;

    int gray_index = ((red + green + blue) / 3 - 8 + 5) / 10;
    if (gray_index < 0)
      gray_index = 0;
    else if (gray_index > 23)
      gray_index = 23;
    const int gray_level = 8 + gray_index * 10;
    const int gr = red - gray_level;
    const int gg = green - gray_level;
    const int gb = blue - gray_level;
    const int gray_distance = gr * gr + gg * gg + gb * gb;

    return (uintattr_t)(gray_distance < cube_distance ? 232 + gray_index : cube);
  }

  const int ansi = (red >= 128 ? 1 : 0) |
                   (green >= 128 ? 2 : 0) |
                   (blue >= 128 ? 4 : 0);
  const int brightest = red > green ? (red > blue ? red : blue)
                                    : (green > blue ? green : blue);
  return (uintattr_t)(TB_BLACK + ansi) |
         (brightest >= 192 ? TB_BRIGHT : 0);
}

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new Scintilla window.
 * Curses does not have to be initialized before calling this function.
 * @param callback A callback function for Scintilla notifications.
 * @param userdata Userdata to pass to *callback*.
 */
void *scintilla_new(
  void (*callback)(void *sci, int iMessage, SCNotification *n, void *userdata), void *userdata);
/**
 * Sends the given message with parameters to the given Scintilla window.
 * Curses does not have to be initialized before calling this function.
 * @param sci The Scintilla window returned by `scintilla_new()`.
 * @param iMessage The message ID.
 * @param wParam The first parameter.
 * @param lParam The second parameter.
 */
sptr_t scintilla_send_message(void *sci, unsigned int iMessage, uptr_t wParam, sptr_t lParam);
/**
 * Deletes the given Scintilla window.
 * Curses must have been initialized prior to calling this function.
 * @param sci The Scintilla window returned by `scintilla_new()`.
 */
/**
 * Sends the specified key to the given Scintilla window for processing.
 * If it is not consumed, an SCNotification will be emitted.
 * Curses does not have to be initialized before calling this function.
 * @param sci The Scintilla window returned by `scintilla_new()`.
 * @param key The keycode of the key.
 * @param shift Flag indicating whether or not the shift modifier key is pressed.
 * @param ctrl Flag indicating whether or not the control modifier key is pressed.
 * @param alt Flag indicating whether or not the alt modifier key is pressed.
 */
void scintilla_send_key(void *sci, int key, bool shift, bool ctrl, bool alt);
/**
 * Sends the specified mouse event to the given Scintilla window for processing.
 * Curses must have been initialized prior to calling this function.
 * @param sci The Scintilla window returned by `scintilla_new()`.
 * @param event The mouse event (`SCM_CLICK`, `SCM_DRAG`, or `SCM_RELEASE`).
 * @param button The button number pressed, or `0` if none.
 * @param y The absolute y coordinate of the mouse event.
 * @param x The absolute x coordinate of the mouse event.
 * @param shift Flag indicating whether or not the shift modifier key is pressed.
 * @param ctrl Flag indicating whether or not the control modifier key is pressed.
 * @param alt Flag indicating whether or not the alt modifier key is pressed.
 * @return whether or not Scintilla handled the mouse event
 */
bool scintilla_send_mouse(void *sci, int event, int button, int y, int x,
  bool shift, bool ctrl, bool alt);
/**
 * Returns a NUL-terminated copy of the text on Scintilla's internal clipboard, not the primary
 * and/or secondary X selections.
 * The caller is responsible for `free`ing the returned text.
 * Keep in mind clipboard text may contain NUL bytes.
 * Curses does not have to be initialized before calling this function.
 * @param sci The Scintilla window returned by `scintilla_new()`.
 * @param len An optional pointer to store the length of the returned text in.
 * @return the clipboard text.
 */
char *scintilla_get_clipboard(void *sci, int *len);
/**
 * Refreshes the Scintilla window on the physical screen.
 * This should be done along with the normal curses `refresh()`, as the physical screen is
 * updated when calling this function.
 * Curses must have been initialized prior to calling this function.
 * @param sci The Scintilla window returned by `scintilla_new()`.
 */
void scintilla_refresh(void *sci);
/**
 * Deletes the given Scintilla window.
 * Curses must have been initialized prior to calling this function.
 * @param sci The Scintilla window returned by `scintilla_new()`.
 */
void scintilla_delete(void *sci);
/**
 * Resize Scintilla window.
 */
void scintilla_resize(void *sci, int width, int height);
/**
 * Move Scintilla window.
 */
void scintilla_move(void *sci, int new_x, int new_y);

#define IMAGE_MAX 31

#define SCM_PRESS 1
#define SCM_DRAG 2
#define SCM_RELEASE 3

#ifdef __cplusplus
}
#endif

#endif
