/* main functions */
int fb_init(char *title, int w, int h);
void fb_free(void);
unsigned fb_mode(void);
void *fb_mem(int r);
int fb_rows(void);
int fb_cols(void);
void fb_cmap(void);
void fb_update(void);

typedef unsigned int fbval_t;	/* framebuffer depth */

/* fb_mode() interpretation */
#define FBM_BPP(m)	(((m) >> 16) & 0x0f)
#define FBM_COLORS(m)	((m) & 0x0fff)

/* helper functions */
void fb_set(int r, int c, void *mem, int len);
unsigned fb_val(int r, int g, int b);

/* event functions*/
void term_setup(void);
void term_cleanup(void);
int readkey(int waitms);
