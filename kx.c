typedef unsigned long Window;
typedef unsigned long Drawable;
typedef unsigned long KeySym;

extern void* XOpenDisplay(char*);
extern int XDefaultScreen(void*);
extern unsigned long XRootWindow(void*, int);
extern unsigned long XCreateSimpleWindow(void*, unsigned long, int, int, unsigned int, unsigned int, unsigned int, unsigned long, unsigned long);
extern int XMapWindow(void*, unsigned long);
extern int XStoreName(void*, unsigned long, char*);
extern int XSelectInput(void*, unsigned long, long);
extern int XPending(void*);
extern int XNextEvent(void*, void*);
extern int XFlush(void*);
extern void* XCreateGC(void*, unsigned long, unsigned long, void*);
extern int XFreeGC(void*, void*);
extern int XCloseDisplay(void*);
extern void* XDefaultVisual(void*, int);
extern int XDefaultDepth(void*, int);
extern void* XCreateImage(void*, void*, unsigned int, int, int, char*, unsigned int, unsigned int, int, int);
extern int XDestroyImage(void*);
extern int XPutImage(void*, unsigned long, void*, void*, int, int, int, int, unsigned int, unsigned int);
extern unsigned long XLookupKeysym(void*, int);

extern int open(const char*, int, ...);
extern long read(int, void*, unsigned long);
extern int close(int);
extern long lseek(int, long, int);
extern int usleep(unsigned int);
extern int pthread_create(unsigned long*, void*, void* (*)(void*), void*);
extern int pthread_detach(unsigned long);

#define KeyPressMask        (1L<<0)
#define KeyReleaseMask      (1L<<1)
#define ButtonPressMask     (1L<<2)
#define ButtonReleaseMask   (1L<<3)
#define PointerMotionMask   (1L<<6)
#define ExposureMask        (1L<<15)
#define StructureNotifyMask (1L<<17)
#define ZPixmap 2
#define O_RDONLY 0
#define SEEK_END 2
#define SEEK_SET 0

#define MOUSE_LEFT   0x01
#define MOUSE_RIGHT  0x02
#define MOUSE_MIDDLE 0x04

extern int scw, sch;

static void* g_dpy = 0;
static unsigned long g_win = 0;
static void* g_gc = 0;
static void* g_ximg = 0;
static unsigned int* g_fb = 0;
static unsigned char g_key[256];
static int g_mx = 0, g_my = 0, g_mb = 0;
static unsigned int g_fb_buf[900 * 650];

static int ks2vk(unsigned long ks) {
    if (ks >= 0x61 && ks <= 0x7a) return ks - 0x61 + 0x41;
    if (ks >= 0x41 && ks <= 0x5a) return ks;
    if (ks >= 0x30 && ks <= 0x39) return ks;
    if (ks == 0xff1b) return 0x1b;
    if (ks == 0xff0d) return 0x0d;
    if (ks == 0x20)   return 0x20;
    if (ks == 0xff08) return 0x08;
    if (ks == 0xff09) return 0x09;
    if (ks == 0xffe1 || ks == 0xffe2) return 0x10;
    if (ks == 0xffe3 || ks == 0xffe4) return 0x11;
    if (ks == 0xffe9 || ks == 0xffea) return 0x12;
    if (ks == 0xff51) return 0x25;
    if (ks == 0xff52) return 0x26;
    if (ks == 0xff53) return 0x27;
    if (ks == 0xff54) return 0x28;
    return 0;
}

int plat_open(int w, int h, const char* title) {
    g_dpy = XOpenDisplay(0);
    if (!g_dpy) return 0;
    int scr = XDefaultScreen(g_dpy);
    unsigned long root = XRootWindow(g_dpy, scr);
    g_win = XCreateSimpleWindow(g_dpy, root, 0, 0, w, h, 0, 0, 0);
    XStoreName(g_dpy, g_win, (char*)title);
    XSelectInput(g_dpy, g_win,
        KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
        PointerMotionMask | ExposureMask | StructureNotifyMask);
    XMapWindow(g_dpy, g_win);
    g_gc = XCreateGC(g_dpy, g_win, 0, 0);
    g_fb = g_fb_buf;
    void* visual = XDefaultVisual(g_dpy, scr);
    int depth = XDefaultDepth(g_dpy, scr);
    g_ximg = XCreateImage(g_dpy, visual, depth, ZPixmap, 0, (char*)g_fb, w, h, 32, 0);
    scw = w;
    sch = h;
    XFlush(g_dpy);
    return 1;
}

void plat_pump(void) {
    char ev[192];
    while (XPending(g_dpy)) {
        XNextEvent(g_dpy, ev);
        int t = *(int*)ev;
        if (t == 2 || t == 3) {
            unsigned long ks = XLookupKeysym(ev, 0);
            int vk = ks2vk(ks);
            if (vk) g_key[vk] = (t == 2) ? 1 : 0;
        } else if (t == 4 || t == 5) {
            unsigned int btn = *(unsigned int*)(ev + 84);
            int mask = 0;
            if (btn == 1) mask = MOUSE_LEFT;
            else if (btn == 3) mask = MOUSE_RIGHT;
            else if (btn == 2) mask = MOUSE_MIDDLE;
            if (mask) { if (t == 4) g_mb |= mask; else g_mb &= ~mask; }
        } else if (t == 6) {
            g_mx = *(int*)(ev + 64);
            g_my = *(int*)(ev + 68);
        }
    }
}

void plat_blit(unsigned int* f) {
    (void)f;
    XPutImage(g_dpy, g_win, g_gc, g_ximg, 0, 0, 0, 0, scw, sch);
    XFlush(g_dpy);
}

void plat_shut(void) {
    if (g_ximg) XDestroyImage(g_ximg);
    if (g_gc) XFreeGC(g_dpy, g_gc);
    if (g_dpy) XCloseDisplay(g_dpy);
}

int plat_kdown(int k) { return g_key[k]; }
void plat_mouse(int* x, int* y, int* b) { *x = g_mx; *y = g_my; *b = g_mb; }
unsigned int* plat_fb(void) { return g_fb; }

int plat_load(const char* fn, unsigned char* buf, unsigned int max) {
    int fd = open(fn, O_RDONLY);
    long sz, got;
    if (fd < 0) return 0;
    sz = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    if (sz < 0) { close(fd); return 0; }
    if ((unsigned int)sz > max) sz = max;
    got = read(fd, buf, sz);
    close(fd);
    return got > 0 ? (int)got : 0;
}

void plat_sleep(unsigned int ms) { usleep(ms * 1000); }
void plat_thread(void* (*fn)(void*)) { unsigned long t; pthread_create(&t, 0, fn, 0); pthread_detach(t); }

int psnd(const char* fn, int ip, int lp) { (void)fn; (void)ip; (void)lp; return 0; }