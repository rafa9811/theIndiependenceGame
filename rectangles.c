#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memory.h>
#include "rectangles.h"


/*
  Prints on the screen the style for writing on this window,
  foreground colour, background colour, and font style.
 */
static void _prepare_font(sc_rectangle *sc) {
    printf("%c[%d;%dm", 27, sc->bg_color, sc->fg_color);
    return;
}

/*
  Moves the cursor into a window position, translating it into
  absolute screen positions. Note the "+1" used to translate from the
  0-based library coordinates to the 1-based escape codes.
 */
void _move_to(sc_rectangle *sc, int r, int c) {

    if (r < 0) r = 0;
    if (c < 0) c = 0;
    if (r >= sc->nr) r = sc->nr - 1;
    if (c >= sc->nc) c = sc->nc - 1;
    r += (sc->r1 + 1);
    c += (sc->c1 + 1);
    printf("%c[%d;%dH", 27, r, c);
    return;
}

/*
  Returns true if a window is visible, that is, if the structure is
  non-null, has valid values, and has at least one row and one column.
 */
static int _is_visible(sc_rectangle *sc) {
    if (!sc) return 0;
    if (sc->r1 < 0 || sc->c1 < 0 || sc->nr <= 0 || sc->nc <= 0) return 0;
    return 1;
}

/*
  Creates a window on the screen.
  Parameters:
     r1, c2: row and column where the window begins;
     nr, nc: number of rows and columns of the window.

  Returns:
    a pointer to a newly allocated sc_rectangle structure with the
    information for the window.

  Note that this function will do absolutely nothing on the screen
  itself. In particular, it will NOT clear the window. This function
  will simply create the structure, initialize its fields, and return
  it.
 */
sc_rectangle *win_new(int r1, int c1, int nr, int nc, int bg, int fg) {
    sc_rectangle *src = (sc_rectangle *) malloc(sizeof (sc_rectangle));
    src->r1 = r1;
    src->c1 = c1;
    src->nr = nr;
    src->nc = nc;

    src->bg_color = bg;
    src->fg_color = fg;

    return src;
}

/*
  Clears a window, leaving it all in the current background color

  Parameters:
      sc: the window structure on which the function operates.

  Returns:
      nothing.
 */
void win_cls(sc_rectangle *sc) {
    char *buf;
    int i;

    sc->last_line = 0;
    //    if (!_is_visible(sc)) return;
    buf = (char *) malloc((sc->nc + 1) * sizeof (char));
    memset(buf, ' ', sc->nc);
    buf[sc->nc] = 0;
    _prepare_font(sc);
    for (i = sc->r1; i < sc->r1 + sc->nr; i++) {
        _move_to(sc, i - sc->r1, 0); /*Este -1 esta puesto de forma aleatoria, pero funciona*/
        printf("%s", buf);
        /*    printf("%d", i);
            fflush(stdout); */
    }
    fflush(stdout);
    free(buf);
    return;
}

/*
  Releases the structure allocated for a window. After calling this
  function it is no longer possible to use the window.

  This function doesn't erase the window contents from the screen.

  Parameters:
      sc: the window structure on which the function operates.

  Returns:
      nothing.
 */
void win_delete(sc_rectangle *sc) {
    if (sc) free(sc);
    return;
}

/*
  Sets the background color for the window

  Parameters:
      sc:  the window structure on which the function operates.
      col: the new background color;

  Returns:
      1: regular execution
      0: incorrect color parameter
 */
int win_bgcol(sc_rectangle *sc, int col) {
    if (!_is_visible(sc)) return ERR;
    if (col >= 40 && col <= 49) sc->bg_color = col;
    return OK;
}

/*
  Sets the foreground colour for the window

  Parameters:
      sc:  the window structure on which the function operates.
      col: the new foreground colour;

  Returns:
      1: regular execution
      0: incorrect colour parameter
 */
int win_fgcol(sc_rectangle *sc, int col) {
    if (!_is_visible(sc)) return ERR;
    if (col >= 30 && col <= 39) sc->bg_color = col;
    return OK;
}

/*
  This function writes a string in a single line starting at a given
  position of the window, and without changing line. Each new line
  character contained in the string will be considered as a string
  termination, and if the string is too long to fit in the screen, it
  will be truncated.

  So, if on the screen of the example we execute the functions:

  win_write_line_at(sc, 3, 26, "en un lugar de la mancha");
  win_write_line_at(sc, 5, 24, "oh!\nI didn't know");


  we get:

                       19             33
     0                  v             v    39
    +----------------------------------------+
  0 |                                        |
    |                                        |
    |                          en un lu      | < 2
    |                                        |
    |                        oh!             |
    |                                        |
    |                                        | < 6
    |                                        |
    |                                        |
   9|                                        |
    +----------------------------------------+

    Parameters:
      sc:  the window structure on which the function operates.
      r:   the row of the window at which the writing begins
      c:   the column of the window at which the writing begins
      str: the screen that we must write

    Returns:
      the number of characters actually written on the screen.

 */
int win_write_line_at(sc_rectangle *sc, int r, int c, char *str) {
    char *nl_p;
    char save, av_space, ret;

    if (!_is_visible(sc)) return 0;
    if (r >= sc->nr || c >= sc->nc) return 0;
    nl_p = strchr(str, '\n');
    if (nl_p) *nl_p = 0;

    av_space = sc->nc - c;
    save = -1;
    if (strlen(str) > av_space) {
        save = str[av_space - 1];
        str[av_space - 1] = 0;
    }

    _prepare_font(sc);
    _move_to(sc, r, c);
    printf("%s", str);
    fflush(stdout);
    if (save > 0) {
        str[av_space - 1] = save;
        ret = av_space;
    } else
        ret = strlen(str);
    if (nl_p) *nl_p = '\n';
    sc->last_line = r;
    return ret;
}


/*
  Writes a char at a given position of the window.

    Parameters:
      sc:  the window structure on which the function operates.
      r:   the row of the window at which the writing begins
      c:   the column of the window at which the writing begins
      ch: the character that we must write

    Returns:
      The constant 1

*/
int win_write_char_at(sc_rectangle *sc, int r, int c, char ch) {

    if (!_is_visible(sc)) return 0;
    if (r >= sc->nr || c >= sc->nc) return 0;

    _move_to(sc, r, c);
    printf("%c", ch);
    fflush(stdout);
    return 1;
}
