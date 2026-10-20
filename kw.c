typedef void* HWND;
typedef void* HDC;
typedef void* HBITMAP;
typedef void* HINSTANCE;
typedef void* HGDIOBJ;
typedef void* HCURSOR;
typedef void* HMENU;
typedef void* HBRUSH;
typedef void* HANDLE;
typedef void* LPVOID;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef unsigned int UINT;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef int BOOL;
typedef int LONG;
typedef unsigned char BYTE;
typedef long long WPARAM;
typedef long long LPARAM;
typedef long long LRESULT;
typedef unsigned long long UINT_PTR;
typedef unsigned long long ULONG_PTR;
typedef unsigned long long DWORD_PTR;
typedef unsigned int MMRESULT;
typedef void* LPSECURITY_ATTRIBUTES;
typedef void* LPOVERLAPPED;
typedef void* HWAVEOUT;

typedef struct { LONG x; LONG y; } POINT;
typedef struct { LONG left, top, right, bottom; } RECT;
typedef LRESULT (__stdcall *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct {
    UINT style; WNDPROC lpfnWndProc; int cbClsExtra; int cbWndExtra;
    HINSTANCE hInstance; void* hIcon; HCURSOR hCursor; HBRUSH hbrBackground;
    LPCSTR lpszMenuName; LPCSTR lpszClassName;
} WNDCLASSA;

typedef struct { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; POINT pt; } MSG;

typedef struct {
    DWORD biSize; LONG biWidth; LONG biHeight; WORD biPlanes; WORD biBitCount;
    DWORD biCompression; DWORD biSizeImage; LONG biXPelsPerMeter;
    LONG biYPelsPerMeter; DWORD biClrUsed; DWORD biClrImportant;
} BITMAPINFOHEADER;
typedef struct { BITMAPINFOHEADER bmiHeader; DWORD bmiColors[3]; } BITMAPINFO;

typedef struct {
    HDC hdc; BOOL fErase; RECT rcPaint; BOOL fRestore; BOOL fIncUpdate;
    BYTE rgbReserved[32];
} PAINTSTRUCT;

typedef struct {
    WORD wFormatTag; WORD nChannels; DWORD nSamplesPerSec; DWORD nAvgBytesPerSec;
    WORD nBlockAlign; WORD wBitsPerSample; WORD cbSize;
} WAVEFORMATEX;

typedef struct WAVEHDR_ {
    LPSTR lpData; DWORD dwBufferLength; DWORD dwBytesRecorded; DWORD_PTR dwUser;
    DWORD dwFlags; DWORD dwLoops; struct WAVEHDR_ *lpNext; DWORD_PTR reserved;
} WAVEHDR;

extern int strequ(const char*, const char*);
extern void* perm_alloc(unsigned int);
extern void perm_reset(void);
extern char* perm_strdup(const char*);
extern HWND __stdcall CreateWindowExA(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
extern BOOL __stdcall AdjustWindowRect(RECT*, DWORD, BOOL);
extern WORD __stdcall RegisterClassA(const WNDCLASSA*);
extern LRESULT __stdcall DefWindowProcA(HWND, UINT, WPARAM, LPARAM);
extern LRESULT __stdcall DispatchMessageA(const MSG*);
extern BOOL __stdcall PeekMessageA(MSG*, HWND, UINT, UINT, UINT);
extern BOOL __stdcall TranslateMessage(const MSG*);
extern void __stdcall PostQuitMessage(int);
extern HCURSOR __stdcall LoadCursorA(HINSTANCE, LPCSTR);
extern HDC __stdcall GetDC(HWND);
extern int __stdcall ReleaseDC(HWND, HDC);
extern HDC __stdcall BeginPaint(HWND, PAINTSTRUCT*);
extern BOOL __stdcall EndPaint(HWND, const PAINTSTRUCT*);
extern BOOL __stdcall GetClientRect(HWND, RECT*);
extern UINT_PTR __stdcall SetTimer(HWND, UINT_PTR, UINT, void*);
extern BOOL __stdcall GetCursorPos(POINT*);
extern BOOL __stdcall ScreenToClient(HWND, POINT*);
extern short __stdcall GetAsyncKeyState(int);
extern HDC __stdcall CreateCompatibleDC(HDC);
extern HBITMAP __stdcall CreateDIBSection(HDC, const BITMAPINFO*, UINT, void**, HANDLE, DWORD);
extern HGDIOBJ __stdcall SelectObject(HDC, HGDIOBJ);
extern BOOL __stdcall DeleteDC(HDC);
extern BOOL __stdcall DeleteObject(HGDIOBJ);
extern BOOL __stdcall BitBlt(HDC, int, int, int, int, HDC, int, int, DWORD);
extern HANDLE __stdcall CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
extern BOOL __stdcall ReadFile(HANDLE, LPVOID, DWORD, DWORD*, LPOVERLAPPED);
extern BOOL __stdcall CloseHandle(HANDLE);
extern DWORD __stdcall GetFileSize(HANDLE, DWORD*);
extern HANDLE __stdcall CreateThread(void*, unsigned long long, unsigned long long (*)(void*), void*, DWORD, DWORD*);
extern void __stdcall Sleep(DWORD);
extern int __stdcall GetSystemMetrics(int);
extern MMRESULT __stdcall waveOutOpen(HWAVEOUT*, UINT, const WAVEFORMATEX*, DWORD_PTR, DWORD_PTR, DWORD);
extern MMRESULT __stdcall waveOutPrepareHeader(HWAVEOUT, WAVEHDR*, UINT);
extern MMRESULT __stdcall waveOutWrite(HWAVEOUT, WAVEHDR*, UINT);
extern MMRESULT __stdcall waveOutReset(HWAVEOUT);
extern MMRESULT __stdcall waveOutUnprepareHeader(HWAVEOUT, WAVEHDR*, UINT);
extern MMRESULT __stdcall waveOutClose(HWAVEOUT);

#define GENERIC_READ        0x80000000
#define OPEN_EXISTING       3
#define INVALID_HANDLE_VALUE ((HANDLE)(long long)-1)
#define WAVE_MAPPER         ((UINT)0xFFFFFFFF)
#define CALLBACK_FUNCTION   0x00030000
#define WAVE_FORMAT_PCM     1
#define WHDR_PREPARED       0x00000002
#define WOM_DONE            0x03BD
#define MM_WOM_DONE         0x03BD
#define IDC_ARROW 32512
#define WS_OVERLAPPEDWINDOW 0x00CF0000
#define WS_VISIBLE 0x10000000
#define WS_POPUP 0x80000000
#define DIB_RGB_COLORS 0
#define BI_RGB 0
#define SRCCOPY 0x00CC0020
#define PM_REMOVE 0x0001
#define WM_CREATE 0x0001
#define WM_DESTROY 0x0002
#define WM_SIZE 0x0005
#define WM_PAINT 0x000F
#define WM_ERASEBKGND 0x0014
#define WM_QUIT 0x0012
#define MM_WOM_DONE 0x03BD
#define SM_CXSCREEN 0
#define SM_CYSCREEN 1

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_a 0x41
#define VK_b 0x42
#define VK_c 0x43
#define VK_d 0x44
#define VK_e 0x45
#define VK_f 0x46
#define VK_g 0x47
#define VK_h 0x48
#define VK_i 0x49
#define VK_j 0x4A
#define VK_k 0x4B
#define VK_l 0x4C
#define VK_m 0x4D
#define VK_n 0x4E
#define VK_o 0x4F
#define VK_p 0x50
#define VK_q 0x51
#define VK_r 0x52
#define VK_s 0x53
#define VK_t 0x54
#define VK_u 0x55
#define VK_v 0x56
#define VK_w 0x57
#define VK_x 0x58
#define VK_y 0x59
#define VK_z 0x5A
#define VK_RETURN 0x0D
#define VK_ESCAPE 0x1B
#define VK_SPACE 0x20
#define VK_BACK 0x08
#define VK_TAB 0x09
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_LBUTTON 0x01
#define VK_RBUTTON 0x02
#define VK_MBUTTON 0x04

#define MOUSE_LEFT   0x01
#define MOUSE_RIGHT  0x02
#define MOUSE_MIDDLE 0x04

extern int scw, sch;
extern int msx, msy;
extern unsigned char msb;

static void* g_hwnd = 0;
static HDC g_hdc = 0;
static HBITMAP hbmp = 0;
static HDC hdc_mem = 0;
static unsigned int* g_fb = 0;

static LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
            BITMAPINFO bi;
            bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bi.bmiHeader.biWidth = scw;
            bi.bmiHeader.biHeight = -sch;
            bi.bmiHeader.biPlanes = 1;
            bi.bmiHeader.biBitCount = 32;
            bi.bmiHeader.biCompression = BI_RGB;
            bi.bmiHeader.biSizeImage = 0;
            bi.bmiHeader.biXPelsPerMeter = 0;
            bi.bmiHeader.biYPelsPerMeter = 0;
            bi.bmiHeader.biClrUsed = 0;
            bi.bmiHeader.biClrImportant = 0;
            HDC hdc = GetDC(hwnd);
            hbmp = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, (void**)&g_fb, 0, 0);
            ReleaseDC(hwnd, hdc);
            if (hbmp && g_fb) {
                hdc_mem = CreateCompatibleDC(0);
                SelectObject(hdc_mem, hbmp);
            }
            g_hwnd = hwnd;
            g_hdc = GetDC(hwnd);
            SetTimer(hwnd, 1, 16, 0);
            break;
        }
        case WM_SIZE: {
            RECT rc;
            GetClientRect(hwnd, &rc);
            scw = rc.right - rc.left;
            sch = rc.bottom - rc.top;
            if (g_fb && scw > 0 && sch > 0) {
                if (hdc_mem) DeleteDC(hdc_mem);
                if (hbmp) DeleteObject(hbmp);
                BITMAPINFO bi;
                bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bi.bmiHeader.biWidth = scw;
                bi.bmiHeader.biHeight = -sch;
                bi.bmiHeader.biPlanes = 1;
                bi.bmiHeader.biBitCount = 32;
                bi.bmiHeader.biCompression = BI_RGB;
                bi.bmiHeader.biSizeImage = 0;
                bi.bmiHeader.biXPelsPerMeter = 0;
                bi.bmiHeader.biYPelsPerMeter = 0;
                bi.bmiHeader.biClrUsed = 0;
                bi.bmiHeader.biClrImportant = 0;
                HDC hdc = GetDC(hwnd);
                hbmp = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, (void**)&g_fb, 0, 0);
                ReleaseDC(hwnd, hdc);
                if (hbmp && g_fb) {
                    hdc_mem = CreateCompatibleDC(0);
                    SelectObject(hdc_mem, hbmp);
                }
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (hdc_mem && g_fb && scw > 0 && sch > 0)
                BitBlt(hdc, 0, 0, scw, sch, hdc_mem, 0, 0, SRCCOPY);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_ERASEBKGND:
            return 1;
        case WM_DESTROY:
            if (hdc_mem) DeleteDC(hdc_mem);
            if (hbmp) DeleteObject(hbmp);
            if (g_hdc) ReleaseDC(hwnd, g_hdc);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcA(hwnd, msg, wp, lp);
    }
    return 0;
}

int plat_open(int w, int h, const char* title) {
    WNDCLASSA wc;
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = 0;
    wc.hIcon = 0;
    wc.hCursor = LoadCursorA(0, (LPCSTR)(unsigned long long)IDC_ARROW);
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = title;
    if (!RegisterClassA(&wc)) return 0;
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    RECT rc;
    rc.left = 0; rc.top = 0; rc.right = w; rc.bottom = h;
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, 0);
    HWND hwnd = CreateWindowExA(0, title, title,
        WS_POPUP | WS_VISIBLE, 0, 0, sw, sh, 0, 0, 0, 0);
    scw = sw; sch = sh;
    if (!hwnd) return 0;
    return 1;
}

void plat_pump(void) {
    MSG msg;
    while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return;
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void plat_blit(unsigned int* f) { (void)f; BitBlt(g_hdc, 0, 0, scw, sch, hdc_mem, 0, 0, SRCCOPY); }
void plat_shut(void) { if (hdc_mem) DeleteDC(hdc_mem); if (hbmp) DeleteObject(hbmp); if (g_hdc) ReleaseDC((HWND)g_hwnd, g_hdc); }
int  plat_kdown(int k) { return (GetAsyncKeyState(k) >> 15) & 1; }
unsigned int* plat_fb(void) { return g_fb; }

void plat_mouse(int* x, int* y, int* b) {
    POINT p; GetCursorPos(&p); ScreenToClient((HWND)g_hwnd, &p);
    *x = p.x; *y = p.y; *b = 0;
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) *b |= MOUSE_LEFT;
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) *b |= MOUSE_RIGHT;
    if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) *b |= MOUSE_MIDDLE;
}

int plat_load(const char* fn, unsigned char* buf, unsigned int max) {
    HANDLE h = CreateFileA(fn, GENERIC_READ, 1, 0, OPEN_EXISTING, 0, 0);
    DWORD got = 0;
    if (h == INVALID_HANDLE_VALUE) return 0;
    DWORD sz = GetFileSize(h, 0);
    if (sz > max) sz = max;
    ReadFile(h, buf, sz, &got, 0);
    CloseHandle(h);
    return (int)got;
}

void plat_sleep(unsigned int ms) { Sleep(ms); }
void plat_thread(void* (*fn)(void*)) { CreateThread(0, 0, (unsigned long long (*)(void*))fn, 0, 0, 0); }

#define SND_VOICES  16
#define SND_CACHE    8

struct sndcache { char* path; char* data; unsigned int size; unsigned int channels; unsigned int rate; };
struct sndv { HWAVEOUT hwo; WAVEHDR wh; char* cur_data; unsigned int cur_size; int open, busy, loop, rearm; };
struct sndp { struct sndv v[SND_VOICES]; struct sndcache c[SND_CACHE]; int nc; HWND hwnd; };
static struct sndp g_snd;

static void __stdcall wo_cb(HWAVEOUT hwo, UINT msg, DWORD_PTR inst, DWORD_PTR p1, DWORD_PTR p2) {
    struct sndv *s;
    (void)p1; (void)p2;
    if (msg != WOM_DONE) return;
    s = (struct sndv*)inst;
    if (!s) return;
    if (s->loop) {
        if (s->wh.dwFlags & WHDR_PREPARED)
            waveOutUnprepareHeader(hwo, &s->wh, sizeof(WAVEHDR));
        s->wh.lpData = s->cur_data;
        s->wh.dwBufferLength = s->cur_size;
        s->wh.dwBytesRecorded = 0;
        s->wh.dwUser = 0;
        s->wh.dwFlags = 0;
        s->wh.dwLoops = 0;
        s->wh.lpNext = 0;
        s->wh.reserved = 0;
        if (waveOutPrepareHeader(hwo, &s->wh, sizeof(WAVEHDR)) == 0)
            waveOutWrite(hwo, &s->wh, sizeof(WAVEHDR));
    } else { s->busy = 0; s->rearm = 1; }
}

static int snd_init_voice(struct sndv *s) {
    WAVEFORMATEX wfx;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec = 44100;
    wfx.nAvgBytesPerSec = 44100 * 2 * 2;
    wfx.nBlockAlign = 4;
    wfx.wBitsPerSample = 16;
    wfx.cbSize = 0;
    s->hwo = 0;
    if (waveOutOpen(&s->hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)&wo_cb, (DWORD_PTR)s, CALLBACK_FUNCTION) != 0) { s->hwo = 0; return 0; }
    s->open = 1; s->busy = 0; s->loop = 0; s->rearm = 0;
    s->cur_data = 0; s->cur_size = 0;
    s->wh.lpData = 0; s->wh.dwBufferLength = 0; s->wh.dwBytesRecorded = 0;
    s->wh.dwUser = 0; s->wh.dwFlags = 0; s->wh.dwLoops = 0;
    s->wh.lpNext = 0; s->wh.reserved = 0;
    return 1;
}

int psnd(const char *fn, int ip, int lp) {
    unsigned char hdr[12];
    unsigned char chunk[8];
    unsigned int got = 0;
    char *pcm = 0;
    unsigned int pcm_size = 0, pcm_ch = 0, pcm_rate = 0;
    struct sndcache *cache = 0;
    struct sndv *slot = 0;
    int i, first_init = 0;

    if (!ip) {
        for (i = 0; i < SND_VOICES; i++) {
            struct sndv *s = &g_snd.v[i];
            if (s->open) {
                s->loop = 0; s->busy = 0;
                waveOutReset(s->hwo);
                if (s->wh.dwFlags & WHDR_PREPARED) waveOutUnprepareHeader(s->hwo, &s->wh, sizeof(WAVEHDR));
                waveOutClose(s->hwo);
                s->hwo = 0; s->open = 0;
            }
            s->cur_data = 0; s->cur_size = 0; s->busy = 0; s->loop = 0; s->rearm = 0;
        }
        for (i = 0; i < g_snd.nc; i++) { g_snd.c[i].path = 0; g_snd.c[i].data = 0; g_snd.c[i].size = 0; g_snd.c[i].channels = 0; g_snd.c[i].rate = 0; }
        g_snd.nc = 0;
        return 0;
    }

    for (i = 0; i < SND_VOICES; i++) if (!g_snd.v[i].open) { first_init = 1; break; }
    if (first_init) for (i = 0; i < SND_VOICES; i++) if (!g_snd.v[i].open) snd_init_voice(&g_snd.v[i]);

    for (i = 0; i < g_snd.nc; i++) if (strequ(g_snd.c[i].path, fn)) { cache = &g_snd.c[i]; break; }
    if (!cache) return -1;

    pcm = cache->data; pcm_size = cache->size; pcm_ch = cache->channels; pcm_rate = cache->rate;

    for (i = 0; i < SND_VOICES; i++) { struct sndv *s = &g_snd.v[i]; if (s->open && !s->loop && !s->busy) { slot = s; break; } }
    if (!slot) for (i = 0; i < SND_VOICES; i++) { struct sndv *s = &g_snd.v[i]; if (s->open && !s->loop) { slot = s; break; } }
    if (!slot) return -10;

    if (slot->busy) {
        waveOutReset(slot->hwo);
        if (slot->wh.dwFlags & WHDR_PREPARED) waveOutUnprepareHeader(slot->hwo, &slot->wh, sizeof(WAVEHDR));
        slot->busy = 0;
    } else if (slot->wh.dwFlags & WHDR_PREPARED) waveOutUnprepareHeader(slot->hwo, &slot->wh, sizeof(WAVEHDR));

    slot->cur_data = pcm; slot->cur_size = pcm_size; slot->loop = lp; slot->busy = 1; slot->rearm = 0;
    slot->wh.lpData = pcm; slot->wh.dwBufferLength = pcm_size; slot->wh.dwBytesRecorded = 0;
    slot->wh.dwUser = 0; slot->wh.dwFlags = 0; slot->wh.dwLoops = 0; slot->wh.lpNext = 0; slot->wh.reserved = 0;

    if (waveOutPrepareHeader(slot->hwo, &slot->wh, sizeof(WAVEHDR)) != 0) { slot->busy = 0; return -7; }
    if (waveOutWrite(slot->hwo, &slot->wh, sizeof(WAVEHDR)) != 0) { waveOutUnprepareHeader(slot->hwo, &slot->wh, sizeof(WAVEHDR)); slot->busy = 0; return -8; }
    return 1;
}