#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nano-X.h"
#include "draw.h"

static GR_WINDOW_ID wid;
static int WIDTH;
static int HEIGHT;
static int STRIDE;
static void *fb;
static int bytespp;					/* bytes per pixel*/
static int nr, ng, nb;
static int rl, rr, gl, gr, bl, br;	/* fb_color() shifts */

#if unused
static int fb_len(void)
{
	return STRIDE * HEIGHT;
}
#endif

unsigned fb_mode(void)
{
	return (bytespp << 16) | (8 << 8) | (8 << 4) | (8);
}

/* register format ARGB, memory BGRA*/
unsigned fb_val(int r, int g, int b)
{
	return ((r >> rr) << rl) | ((g >> gr) << gl) | ((b >> br) << bl) | (255 << 24);
}

static void init_colors(void)
{
	//nr = 1 << vinfo.red.length;
	//ng = 1 << vinfo.blue.length;
	//nb = 1 << vinfo.green.length;
	//rr = 8 - vinfo.red.length;
	//rl = vinfo.red.offset;
	//gr = 8 - vinfo.green.length;
	//gl = vinfo.green.offset;
	//br = 8 - vinfo.blue.length;
	//bl = vinfo.blue.offset;

	/* register format ARGB, memory BGRA*/
	nr = 1 << 8;		/* number of colors*/
	ng = 1 << 8;
	nb = 1 << 8;
	rr = 8 - 8;			/* right shift to scale R/G/B value to color byte*/
	rl = 16;			/* left shift to move color byte to framebuffer color position*/
	gr = 8 - 8;
	gl = 8;
	br = 8 - 8;
	bl = 0;
}

int fb_init(void)
{
	GR_SCREEN_INFO si;
	if (GrOpen() < 0)
		return 1;

	bytespp = 4;
	init_colors();

	/* get width/height for fb_cols/fb_rows before fb_open*/
	GrGetScreenInfo(&si);
	WIDTH = si.cols;
	HEIGHT = si.rows;
	STRIDE = WIDTH * bytespp;

	return 0;
}

int fb_open(char *title, int w, int h, int flags)
{
	GR_WM_PROPS props;

	/* reset window size from fullscreen if given*/
	if (w && h) {
		WIDTH = w;
		HEIGHT = h;
		STRIDE = WIDTH * bytespp;
	}

	switch (flags) {
	case NOFRAME:
		props = GR_WM_PROPS_CLOSEBOX;		/* will generate no frame, closebox, or border*/
		break;
	case BORDER:
		props = GR_WM_PROPS_BORDER;
		break;
	case APPFRAME:
		props = GR_WM_PROPS_APPWINDOW;
		break;
	default:
		return 1;
	}
	props |= GR_WM_PROPS_BUFFER_MMAP | GR_WM_PROPS_BUFFER_BGRA | GR_WM_PROPS_NORESIZE;

	wid = GrNewBufferedWindow(props, title, GR_ROOT_WINDOW_ID, 0, 0, WIDTH, HEIGHT, 0);
	fb = GrOpenClientFramebuffer(wid);
	if (!fb) {
		GrDestroyWindow(wid);
		return 1;
	}

	GrSelectEvents(wid, GR_EVENT_MASK_EXPOSURE|GR_EVENT_MASK_KEY_DOWN|
		GR_EVENT_MASK_BUTTON_DOWN| GR_EVENT_MASK_BUTTON_UP|
		GR_EVENT_MASK_CLOSE_REQ);
	GrMapWindow(wid);

	if (FBM_BPP(fb_mode()) != sizeof(fbval_t))
		fprintf(stderr, "fbpdf: fbval_t doesn't match fb depth\n");
	return 0;
}

void fb_free(void)
{
	GrCloseClientFramebuffer(wid);
	GrDestroyWindow(wid);
	GrClose();
}

int fb_rows(void)
{
	return HEIGHT;
}

int fb_cols(void)
{
	return WIDTH;
}

void *fb_mem(int r)
{
	return fb + r * STRIDE;
}

void fb_set(int r, int c, void *mem, int len)
{
	memcpy(fb_mem(r) + c * bytespp, mem, len * bytespp);
}

void fb_update(void)
{
	GrFlushWindow(wid);
}

void term_setup(void)
{
}

void term_cleanup(void)
{
}

/*
 * read a key and wait #msecs, 0=indefinite
 * returns key, 0 if timeout or nothing available, -1 to quit
 */
int readkey(int waitms)
{
	GR_EVENT ev;

	GrGetNextEventTimeout(&ev, waitms);
	switch (ev.type) {
	case GR_EVENT_TYPE_TIMEOUT:
		break;
	case GR_EVENT_TYPE_EXPOSURE:
		fb_update();
		break;
	case GR_EVENT_TYPE_KEY_DOWN:
		return ev.keystroke.ch;
	case GR_EVENT_TYPE_BUTTON_DOWN:
		if (ev.button.buttons & GR_BUTTON_SCROLLUP)	return 'K';
		if (ev.button.buttons & GR_BUTTON_SCROLLDN)	return 'J';
		break;
	case GR_EVENT_TYPE_CLOSE_REQ:
		return -1;
	}
	return 0;
}
