// km.c -- native macOS, no includes, raw Cocoa via objc_msgSend

typedef void* id;
typedef void* SEL;
typedef void* Class;
typedef long NSInteger;
typedef unsigned long NSUInteger;
typedef double CGFloat;
typedef signed char BOOL;

typedef struct { CGFloat x; CGFloat y; } NSPoint;
typedef struct { CGFloat x; CGFloat y; CGFloat w; CGFloat h; } NSRect;

extern id   objc_getClass(const char*);
extern SEL  sel_registerName(const char*);
extern id   objc_msgSend(id, SEL, ...);
extern id   objc_msgSend_stret(void*, id, SEL, ...);

extern unsigned int CGMainDisplayID(void);
extern unsigned long CGDisplayPixelsHigh(unsigned int);
extern unsigned long CGDisplayPixelsWide(unsigned int);
extern int CGEventSourceKeyState(unsigned int, unsigned short);
extern int CGEventSourceButtonState(unsigned int, unsigned int);
extern void* CGDataProviderCreateWithData(void*, const void*, unsigned long, void*);
extern void* CGColorSpaceCreateDeviceRGB(void);
extern void* CGImageCreate(unsigned long, unsigned long, unsigned long, unsigned long,
                           unsigned long, void*, unsigned int, void*,
                           const double*, int, int);
extern void  CGImageRelease(void*);
extern void  CGColorSpaceRelease(void*);
extern void  CGDataProviderRelease(void*);
extern void  CGContextDrawImage(void*, NSRect, void*);

#define MOUSE_LEFT   0x01
#define MOUSE_RIGHT  0x02
#define MOUSE_MIDDLE 0x04

extern int scw, sch;
extern unsigned int* fb;
extern unsigned int frm;

static id g_app = 0;
static id g_win = 0;
static id g_view = 0;
static id g_pool = 0;
static unsigned int* g_fb = 0;
static unsigned char g_key[256];
static int g_mx = 0, g_my = 0, g_mb = 0;

static unsigned int g_fb_buf[900 * 650];

static unsigned short vk2mac(int vk) {
    switch (vk) {
        case 0x1B: return 0x35;
        case 0x0D: return 0x24;
        case 0x20: return 0x31;
        case 0x08: return 0x33;
        case 0x09: return 0x30;
        case 0x10: return 0x38;
        case 0x11: return 0x3B;
        case 0x12: return 0x3A;
        case 0x25: return 0x7B;
        case 0x26: return 0x7E;
        case 0x27: return 0x7C;
        case 0x28: return 0x7D;
        case 0x30: return 0x1D;
        case 0x31: return 0x12;
        case 0x32: return 0x13;
        case 0x33: return 0x14;
        case 0x34: return 0x15;
        case 0x35: return 0x17;
        case 0x36: return 0x16;
        case 0x37: return 0x1A;
        case 0x38: return 0x1C;
        case 0x39: return 0x19;
        case 0x41: return 0x00;
        case 0x42: return 0x0B;
        case 0x43: return 0x08;
        case 0x44: return 0x02;
        case 0x45: return 0x0E;
        case 0x46: return 0x03;
        case 0x47: return 0x05;
        case 0x48: return 0x04;
        case 0x49: return 0x22;
        case 0x4A: return 0x26;
        case 0x4B: return 0x28;
        case 0x4C: return 0x25;
        case 0x4D: return 0x2E;
        case 0x4E: return 0x2D;
        case 0x4F: return 0x1F;
        case 0x50: return 0x23;
        case 0x51: return 0x0C;
        case 0x52: return 0x0F;
        case 0x53: return 0x01;
        case 0x54: return 0x11;
        case 0x55: return 0x20;
        case 0x56: return 0x09;
        case 0x57: return 0x0D;
        case 0x58: return 0x07;
        case 0x59: return 0x10;
        case 0x5A: return 0x06;
        default:   return 0;
    }
}

int plat_open(int w, int h, const char* title) {
    Class cls_NSApp = objc_getClass("NSApplication");
    Class cls_NSWin = objc_getClass("NSWindow");
    Class cls_NSView = objc_getClass("NSView");
    Class cls_NSPool = objc_getClass("NSAutoreleasePool");
    Class cls_NSStr = objc_getClass("NSString");
    Class cls_NSColor = objc_getClass("NSColor");

    SEL sel_alloc = sel_registerName("alloc");
    SEL sel_init = sel_registerName("init");
    SEL sel_shared = sel_registerName("sharedApplication");
    SEL sel_setPolicy = sel_registerName("setActivationPolicy:");
    SEL sel_initStr_utf8 = sel_registerName("initWithUTF8String:");
    SEL sel_initWin = sel_registerName("initWithContentRect:styleMask:backing:defer:");
    SEL sel_setTitle = sel_registerName("setTitle:");
    SEL sel_makeKey = sel_registerName("makeKeyAndOrderFront:");
    SEL sel_setBg = sel_registerName("setBackgroundColor:");
    SEL sel_black = sel_registerName("blackColor");
    SEL sel_contentView = sel_registerName("contentView");
    SEL sel_setFrame = sel_registerName("setFrame:");
    SEL sel_activate = sel_registerName("activateIgnoringOtherApps:");
    SEL sel_setReleased = sel_registerName("setReleasedWhenClosed:");
    SEL sel_run = sel_registerName("run");

    g_pool = objc_msgSend(objc_msgSend(cls_NSPool, sel_alloc), sel_init);

    g_app = objc_msgSend(cls_NSApp, sel_shared);
    objc_msgSend(g_app, sel_setPolicy, 0);
    objc_msgSend(g_app, sel_activate, 1);

    id title_str = objc_msgSend(objc_msgSend(cls_NSStr, sel_alloc), sel_initStr_utf8, title);

    NSRect r = { 0, 0, (CGFloat)w, (CGFloat)h };
    unsigned long style = 0;
    g_win = objc_msgSend(objc_msgSend(cls_NSWin, sel_alloc), sel_initWin, r, style, 2, 0);
    objc_msgSend(g_win, sel_setTitle, title_str);
    objc_msgSend(g_win, sel_setReleased, 0);

    id bg = objc_msgSend(cls_NSColor, sel_black);
    objc_msgSend(g_win, sel_setBg, bg);

    g_view = objc_msgSend(g_win, sel_contentView);

    objc_msgSend(g_win, sel_makeKey, 0);

    g_fb = g_fb_buf;
    scw = w;
    sch = h;
    return 1;
}

void plat_pump(void) {
    Class cls_NSDate = objc_getClass("NSDate");
    SEL sel_distantPast = sel_registerName("distantPast");
    SEL sel_nextEvent = sel_registerName("nextEventMatchingMask:untilDate:inMode:dequeue:");
    SEL sel_sendEvent = sel_registerName("sendEvent:");
    SEL sel_defaultMode = sel_registerName("defaultMode");
    Class cls_NSRunLoop = objc_getClass("NSRunLoop");
    SEL sel_currentRunLoop = sel_registerName("currentRunLoop");
    SEL sel_currentMode = sel_registerName("currentMode");

    id runLoop = objc_msgSend(cls_NSRunLoop, sel_currentRunLoop);
    id mode = objc_msgSend(runLoop, sel_currentMode);
    id date = objc_msgSend(cls_NSDate, sel_distantPast);

    for (;;) {
        id ev = objc_msgSend(g_app, sel_nextEvent, (unsigned long)~0UL, date, mode, 1);
        if (!ev) break;
        objc_msgSend(g_app, sel_sendEvent, ev);
    }
}

void plat_blit(unsigned int* f) {
    (void)f;
    Class cls_NSCtx = objc_getClass("NSGraphicsContext");
    SEL sel_current = sel_registerName("currentContext");
    SEL sel_cg = sel_registerName("CGContext");
    SEL sel_lock = sel_registerName("lockFocus");
    SEL sel_unlock = sel_registerName("unlockFocus");

    objc_msgSend(g_view, sel_lock);
    id nsCtx = objc_msgSend(cls_NSCtx, sel_current);
    void* cgCtx = objc_msgSend(nsCtx, sel_cg);

    void* provider = CGDataProviderCreateWithData(0, g_fb, (unsigned long)scw * sch * 4, 0);
    void* cs = CGColorSpaceCreateDeviceRGB();
    void* img = CGImageCreate((unsigned long)scw, (unsigned long)sch,
                              8, 32, (unsigned long)scw * 4,
                              cs, 0x2006, provider, 0, 0, 0);
    NSRect r = { 0, 0, (CGFloat)scw, (CGFloat)sch };
    CGContextDrawImage(cgCtx, r, img);
    CGImageRelease(img);
    CGColorSpaceRelease(cs);
    CGDataProviderRelease(provider);

    objc_msgSend(g_view, sel_unlock);
}

void plat_shut(void) {
    objc_msgSend(g_app, sel_registerName("terminate:"), 0);
}

int plat_kdown(int k) {
    unsigned short m = vk2mac(k);
    if (m == 0 && k != 0) return 0;
    return CGEventSourceKeyState(0, m);
}

void plat_mouse(int* x, int* y, int* b) {
    Class cls_NSEv = objc_getClass("NSEvent");
    SEL sel_loc = sel_registerName("mouseLocation");
    NSPoint p = {0, 0};
    objc_msgSend_stret(&p, cls_NSEv, sel_loc);
    unsigned long sh = CGDisplayPixelsHigh(CGMainDisplayID());
    g_mx = (int)p.x;
    g_my = (int)((CGFloat)sh - p.y);
    g_mb = 0;
    if (CGEventSourceButtonState(0, 0)) g_mb |= MOUSE_LEFT;
    if (CGEventSourceButtonState(0, 1)) g_mb |= MOUSE_RIGHT;
    if (CGEventSourceButtonState(0, 2)) g_mb |= MOUSE_MIDDLE;
    *x = g_mx; *y = g_my; *b = g_mb;
}

extern int open(const char*, int, ...);
extern long read(int, void*, unsigned long);
extern int close(int);
extern long lseek(int, long, int);
extern int usleep(unsigned int);
extern int pthread_create(unsigned long*, void*, void* (*)(void*), void*);
extern int pthread_detach(unsigned long);

int plat_load(const char* fn, unsigned char* buf, unsigned int max) {
    int fd = open(fn, 0);
    long sz, got;
    if (fd < 0) return 0;
    sz = lseek(fd, 0, 2);
    lseek(fd, 0, 0);
    if (sz < 0) { close(fd); return 0; }
    if ((unsigned int)sz > max) sz = max;
    got = read(fd, buf, sz);
    close(fd);
    return got > 0 ? (int)got : 0;
}

void plat_sleep(unsigned int ms) { usleep(ms * 1000); }
void plat_thread(void* (*fn)(void*)) { unsigned long t; pthread_create(&t, 0, fn, 0); pthread_detach(t); }
unsigned int* plat_fb(void) { return g_fb; }

int psnd(const char* fn, int ip, int lp) { (void)fn; (void)ip; (void)lp; return 0; }