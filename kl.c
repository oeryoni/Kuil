typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef long long i64;

extern int plat_open(int w, int h, const char* title);
extern void plat_pump(void);
extern void plat_blit(unsigned int* fb);
extern void plat_shut(void);
extern int plat_kdown(int k);
extern void plat_mouse(int* x, int* y, int* b);
extern int plat_load(const char* fn, unsigned char* buf, unsigned int max);
extern void plat_sleep(unsigned int ms);
extern void plat_thread(void* (*fn)(void*));
extern unsigned int* plat_fb(void);
extern int psnd(const char* fn, int ip, int lp);

#define W 900
#define H 650
#define MXKY 256

int scw = W;
int sch = H;
int msx = 400;
int msy = 300;
unsigned char msb = 0;
unsigned int frm = 0;
unsigned int* fb = 0;

struct inpst {
    unsigned char down    [MXKY];
    unsigned char pressed [MXKY];
    unsigned char released[MXKY];
    unsigned char prev    [MXKY];
};

static struct inpst g_in;

static unsigned char* gt[256];

static unsigned char F_A[7]={0x0E,0x11,0x11,0x1F,0x11,0x11,0x11};
static unsigned char F_B[7]={0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E};
static unsigned char F_C[7]={0x0E,0x11,0x10,0x10,0x10,0x11,0x0E};
static unsigned char F_D[7]={0x1C,0x12,0x11,0x11,0x11,0x12,0x1C};
static unsigned char F_E[7]={0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F};
static unsigned char F_F[7]={0x1F,0x10,0x10,0x1E,0x10,0x10,0x10};
static unsigned char F_G[7]={0x0E,0x11,0x10,0x13,0x11,0x11,0x0E};
static unsigned char F_H[7]={0x11,0x11,0x11,0x1F,0x11,0x11,0x11};
static unsigned char F_I[7]={0x1F,0x04,0x04,0x04,0x04,0x04,0x1F};
static unsigned char F_J[7]={0x1F,0x02,0x02,0x02,0x12,0x12,0x0C};
static unsigned char F_K[7]={0x11,0x12,0x14,0x18,0x14,0x12,0x11};
static unsigned char F_L[7]={0x10,0x10,0x10,0x10,0x10,0x10,0x1F};
static unsigned char F_M[7]={0x11,0x1B,0x15,0x15,0x11,0x11,0x11};
static unsigned char F_N[7]={0x11,0x19,0x15,0x13,0x11,0x11,0x11};
static unsigned char F_O[7]={0x0E,0x11,0x11,0x11,0x11,0x11,0x0E};
static unsigned char F_P[7]={0x1E,0x11,0x11,0x1E,0x10,0x10,0x10};
static unsigned char F_Q[7]={0x0E,0x11,0x11,0x11,0x15,0x12,0x0D};
static unsigned char F_R[7]={0x1E,0x11,0x11,0x1E,0x14,0x12,0x11};
static unsigned char F_S[7]={0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E};
static unsigned char F_T[7]={0x1F,0x04,0x04,0x04,0x04,0x04,0x04};
static unsigned char F_U[7]={0x11,0x11,0x11,0x11,0x11,0x11,0x0E};
static unsigned char F_V[7]={0x11,0x11,0x11,0x11,0x0A,0x0A,0x04};
static unsigned char F_W[7]={0x11,0x11,0x11,0x15,0x15,0x1B,0x11};
static unsigned char F_X[7]={0x11,0x11,0x0A,0x04,0x0A,0x11,0x11};
static unsigned char F_Y[7]={0x11,0x11,0x0A,0x04,0x04,0x04,0x04};
static unsigned char F_Z[7]={0x1F,0x01,0x02,0x04,0x08,0x10,0x1F};
static unsigned char F_a[7]={0x00,0x00,0x0E,0x01,0x0F,0x11,0x0F};
static unsigned char F_b[7]={0x10,0x10,0x16,0x19,0x11,0x11,0x1E};
static unsigned char F_c[7]={0x00,0x00,0x0E,0x10,0x10,0x10,0x0E};
static unsigned char F_d[7]={0x01,0x01,0x0D,0x13,0x11,0x11,0x0F};
static unsigned char F_e[7]={0x00,0x00,0x0E,0x11,0x1F,0x10,0x0E};
static unsigned char F_f[7]={0x06,0x09,0x08,0x1C,0x08,0x08,0x08};
static unsigned char F_g[7]={0x00,0x0F,0x11,0x11,0x0F,0x01,0x0E};
static unsigned char F_h[7]={0x10,0x10,0x16,0x19,0x11,0x11,0x11};
static unsigned char F_i[7]={0x04,0x00,0x0C,0x04,0x04,0x04,0x0E};
static unsigned char F_j[7]={0x02,0x00,0x06,0x02,0x02,0x12,0x0C};
static unsigned char F_k[7]={0x10,0x10,0x12,0x14,0x18,0x14,0x12};
static unsigned char F_l[7]={0x0C,0x04,0x04,0x04,0x04,0x04,0x0E};
static unsigned char F_m[7]={0x00,0x00,0x1A,0x15,0x15,0x11,0x11};
static unsigned char F_n[7]={0x00,0x00,0x16,0x19,0x11,0x11,0x11};
static unsigned char F_o[7]={0x00,0x00,0x0E,0x11,0x11,0x11,0x0E};
static unsigned char F_p[7]={0x00,0x1E,0x11,0x11,0x1E,0x10,0x10};
static unsigned char F_q[7]={0x00,0x0F,0x11,0x11,0x0F,0x01,0x01};
static unsigned char F_r[7]={0x00,0x00,0x16,0x19,0x10,0x10,0x10};
static unsigned char F_s[7]={0x00,0x00,0x0E,0x10,0x0E,0x01,0x1E};
static unsigned char F_t[7]={0x04,0x04,0x0E,0x04,0x04,0x05,0x06};
static unsigned char F_u[7]={0x00,0x00,0x11,0x11,0x11,0x13,0x0D};
static unsigned char F_v[7]={0x00,0x00,0x11,0x11,0x11,0x0A,0x04};
static unsigned char F_w[7]={0x00,0x00,0x11,0x11,0x15,0x15,0x0A};
static unsigned char F_x[7]={0x00,0x00,0x11,0x0A,0x04,0x0A,0x11};
static unsigned char F_y[7]={0x00,0x11,0x11,0x11,0x0F,0x01,0x0E};
static unsigned char F_z[7]={0x00,0x00,0x1F,0x02,0x04,0x08,0x1F};
static unsigned char F_0[7]={0x0E,0x11,0x13,0x15,0x19,0x11,0x0E};
static unsigned char F_1[7]={0x04,0x0C,0x04,0x04,0x04,0x04,0x0E};
static unsigned char F_2[7]={0x0E,0x11,0x01,0x02,0x04,0x08,0x1F};
static unsigned char F_3[7]={0x1E,0x01,0x01,0x0E,0x01,0x01,0x1E};
static unsigned char F_4[7]={0x02,0x06,0x0A,0x12,0x1F,0x02,0x02};
static unsigned char F_5[7]={0x1F,0x10,0x10,0x1E,0x01,0x01,0x1E};
static unsigned char F_6[7]={0x0E,0x10,0x10,0x1E,0x11,0x11,0x0E};
static unsigned char F_7[7]={0x1F,0x01,0x02,0x04,0x08,0x08,0x08};
static unsigned char F_8[7]={0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E};
static unsigned char F_9[7]={0x0E,0x11,0x11,0x0F,0x01,0x01,0x0E};
static unsigned char F_SPACE[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static unsigned char F_EXCL[7]={0x04,0x04,0x04,0x04,0x00,0x00,0x04};
static unsigned char F_DQUOTE[7]={0x0A,0x0A,0x00,0x00,0x00,0x00,0x00};
static unsigned char F_HASH[7]={0x0A,0x1F,0x0A,0x1F,0x0A,0x00,0x00};
static unsigned char F_DOLLAR[7]={0x04,0x0F,0x14,0x0E,0x05,0x1E,0x04};
static unsigned char F_PERCENT[7]={0x18,0x19,0x02,0x04,0x08,0x13,0x03};
static unsigned char F_AMP[7]={0x0C,0x12,0x14,0x08,0x15,0x12,0x0D};
static unsigned char F_QUOTE[7]={0x04,0x04,0x00,0x00,0x00,0x00,0x00};
static unsigned char F_LPAREN[7]={0x02,0x04,0x08,0x08,0x08,0x04,0x02};
static unsigned char F_RPAREN[7]={0x08,0x04,0x02,0x02,0x02,0x04,0x08};
static unsigned char F_STAR[7]={0x00,0x04,0x15,0x0E,0x15,0x04,0x00};
static unsigned char F_PLUS[7]={0x00,0x04,0x04,0x1F,0x04,0x04,0x00};
static unsigned char F_COMMA[7]={0x00,0x00,0x00,0x00,0x04,0x04,0x08};
static unsigned char F_DASH[7]={0x00,0x00,0x00,0x1F,0x00,0x00,0x00};
static unsigned char F_DOT[7]={0x00,0x00,0x00,0x00,0x00,0x04,0x04};
static unsigned char F_SLASH[7]={0x01,0x02,0x02,0x04,0x08,0x08,0x10};
static unsigned char F_COLON[7]={0x00,0x04,0x00,0x00,0x04,0x00,0x00};
static unsigned char F_SEMI[7]={0x00,0x04,0x00,0x04,0x04,0x08,0x00};
static unsigned char F_LT[7]={0x02,0x04,0x08,0x10,0x08,0x04,0x02};
static unsigned char F_EQUAL[7]={0x00,0x1F,0x00,0x1F,0x00,0x00,0x00};
static unsigned char F_GT[7]={0x10,0x08,0x04,0x02,0x04,0x08,0x10};
static unsigned char F_QUESTION[7]={0x0E,0x11,0x01,0x02,0x04,0x00,0x04};
static unsigned char F_AT[7]={0x0E,0x11,0x17,0x15,0x17,0x10,0x0E};
static unsigned char F_LBRACK[7]={0x0E,0x08,0x08,0x08,0x08,0x08,0x0E};
static unsigned char F_BACKSLASH[7]={0x10,0x08,0x04,0x02,0x02,0x01,0x00};
static unsigned char F_RBRACK[7]={0x0E,0x02,0x02,0x02,0x02,0x02,0x0E};
static unsigned char F_CARET[7]={0x04,0x0A,0x11,0x00,0x00,0x00,0x00};
static unsigned char F_UNDER[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x1F};
static unsigned char F_BACKTICK[7]={0x08,0x04,0x02,0x00,0x00,0x00,0x00};
static unsigned char F_LCURLY[7]={0x06,0x08,0x08,0x10,0x08,0x08,0x06};
static unsigned char F_BAR[7]={0x04,0x04,0x04,0x04,0x04,0x04,0x04};
static unsigned char F_RCURLY[7]={0x0C,0x02,0x02,0x01,0x02,0x02,0x0C};
static unsigned char F_TILDE[7]={0x00,0x0C,0x12,0x06,0x00,0x00,0x00};

static void igt(void){
    long i;
    for(i=0;i<256;i++) gt[i]=F_QUESTION;
    gt[65]=F_A;gt[66]=F_B;gt[67]=F_C;gt[68]=F_D;gt[69]=F_E;gt[70]=F_F;gt[71]=F_G;
    gt[72]=F_H;gt[73]=F_I;gt[74]=F_J;gt[75]=F_K;gt[76]=F_L;gt[77]=F_M;gt[78]=F_N;
    gt[79]=F_O;gt[80]=F_P;gt[81]=F_Q;gt[82]=F_R;gt[83]=F_S;gt[84]=F_T;gt[85]=F_U;
    gt[86]=F_V;gt[87]=F_W;gt[88]=F_X;gt[89]=F_Y;gt[90]=F_Z;
    gt[97]=F_a;gt[98]=F_b;gt[99]=F_c;gt[100]=F_d;gt[101]=F_e;gt[102]=F_f;gt[103]=F_g;
    gt[104]=F_h;gt[105]=F_i;gt[106]=F_j;gt[107]=F_k;gt[108]=F_l;gt[109]=F_m;gt[110]=F_n;
    gt[111]=F_o;gt[112]=F_p;gt[113]=F_q;gt[114]=F_r;gt[115]=F_s;gt[116]=F_t;gt[117]=F_u;
    gt[118]=F_v;gt[119]=F_w;gt[120]=F_x;gt[121]=F_y;gt[122]=F_z;
    gt[48]=F_0;gt[49]=F_1;gt[50]=F_2;gt[51]=F_3;gt[52]=F_4;gt[53]=F_5;gt[54]=F_6;
    gt[55]=F_7;gt[56]=F_8;gt[57]=F_9;
    gt[32]=F_SPACE;gt[33]=F_EXCL;gt[34]=F_DQUOTE;gt[35]=F_HASH;gt[36]=F_DOLLAR;
    gt[37]=F_PERCENT;gt[38]=F_AMP;gt[39]=F_QUOTE;gt[40]=F_LPAREN;gt[41]=F_RPAREN;
    gt[42]=F_STAR;gt[43]=F_PLUS;gt[44]=F_COMMA;gt[45]=F_DASH;gt[46]=F_DOT;
    gt[47]=F_SLASH;gt[58]=F_COLON;gt[59]=F_SEMI;gt[60]=F_LT;gt[61]=F_EQUAL;
    gt[62]=F_GT;gt[63]=F_QUESTION;gt[64]=F_AT;
    gt[91]=F_LBRACK;gt[92]=F_BACKSLASH;gt[93]=F_RBRACK;gt[94]=F_CARET;
    gt[95]=F_UNDER;gt[96]=F_BACKTICK;gt[123]=F_LCURLY;gt[124]=F_BAR;gt[125]=F_RCURLY;gt[126]=F_TILDE;
}

static unsigned char* gg(unsigned char c) { return gt[c]; }

#define PERM_ARENA_SIZE   (8u * 1024u * 1024u)
#define LEVEL_ARENA_SIZE  (4u * 1024u * 1024u)
#define FRAME_ARENA_SIZE  (1u * 1024u * 1024u)

static unsigned char g_perm_arena [PERM_ARENA_SIZE];
static unsigned char g_level_arena[LEVEL_ARENA_SIZE];
static unsigned char g_frame_arena[FRAME_ARENA_SIZE];

static unsigned int g_perm_used  = 0;
static unsigned int g_level_used = 0;
static unsigned int g_frame_used = 0;

void* perm_alloc(unsigned int n) {
    unsigned int aligned = (n + 7u) & ~7u;
    void *p;
    if (g_perm_used + aligned > PERM_ARENA_SIZE) return 0;
    p = (void*)(g_perm_arena + g_perm_used);
    g_perm_used += aligned;
    return p;
}
static void* level_alloc(unsigned int n) {
    unsigned int aligned = (n + 7u) & ~7u;
    void *p;
    if (g_level_used + aligned > LEVEL_ARENA_SIZE) return 0;
    p = (void*)(g_level_arena + g_level_used);
    g_level_used += aligned;
    return p;
}
static void* frame_alloc(unsigned int n) {
    unsigned int aligned = (n + 7u) & ~7u;
    void *p;
    if (g_frame_used + aligned > FRAME_ARENA_SIZE) return 0;
    p = (void*)(g_frame_arena + g_frame_used);
    g_frame_used += aligned;
    return p;
}
void perm_reset(void)  { g_perm_used  = 0; }
static void level_reset(void) { g_level_used = 0; }
static void frame_reset(void) { g_frame_used = 0; }

static unsigned int rd32(const unsigned char *p) {
    return (unsigned int)p[0] | ((unsigned int)p[1] << 8)
         | ((unsigned int)p[2] << 16) | ((unsigned int)p[3] << 24);
}
static unsigned int rd16(const unsigned char *p) {
    return (unsigned int)p[0] | ((unsigned int)p[1] << 8);
}
int strequ(const char *a, const char *b) {
    if (!a || !b) return 0;
    while (*a && *a == *b) { a++; b++; }
    return *a == *b;
}
char* perm_strdup(const char *s) {
    unsigned int n = 0;
    char *p;
    unsigned int i;
    while (s[n]) n++;
    p = (char*)perm_alloc(n + 1);
    if (!p) return 0;
    for (i = 0; i <= n; i++) p[i] = s[i];
    return p;
}

static inline void px(int o, int c) { fb[o] = (unsigned int)c; }

static inline int ma(int c, int b, int t) {
    unsigned int r = ((((c >> 16) & 0xFF) * t + ((b >> 16) & 0xFF) * (255 - t)) >> 8) << 16;
    unsigned int g = ((((c >> 8) & 0xFF) * t + ((b >> 8) & 0xFF) * (255 - t)) >> 8) << 8;
    unsigned int bl = (((c & 0xFF) * t + (b & 0xFF) * (255 - t)) >> 8);
    return (int)(r | g | bl);
}

static inline void pt(int x, int y, int c, int t) {
    if ((unsigned int)x >= (unsigned int)scw || (unsigned int)y >= (unsigned int)sch) return;
    int o = y * scw + x;
    if (t >= 255) { px(o, c); return; }
    if (t <= 0) return;
    px(o, ma(c, (int)fb[o], t));
}

static void rtt(int x, int y, int cx, int cy, int r, int *rx, int *ry) {
    if ((r & 63) == 0) { *rx = x; *ry = y; return; }
    static const short sn[17] = {0,25,49,73,97,120,142,162,181,198,212,225,236,244,250,254,256};
    int i = r & 63;
    int s, c;
    if (i <= 16)      { s =  sn[i];      c =  sn[16 - i]; }
    else if (i <= 32) { s =  sn[32 - i]; c = -sn[i - 16]; }
    else if (i <= 48) { s = -sn[i - 32]; c = -sn[48 - i]; }
    else              { s = -sn[64 - i]; c =  sn[i - 48]; }
    int dx = x - cx, dy = y - cy;
    *rx = cx + ((dx * c - dy * s) >> 8);
    *ry = cy + ((dx * s + dy * c) >> 8);
}

static void clr(int c, int t) {
    int i = 0, n = scw * sch;
    if (t >= 255) { for (; i < n; i++) px(i, c); return; }
    if (t <= 0) return;
    for (; i < n; i++) px(i, ma(c, (int)fb[i], t));
}

static void ch(int x, int y, unsigned char q, int col, int r, int t, int w, int h) {
    unsigned char *g = gg(q);
    int cx = x + w, cy = y + (h>>1);
    int j;
    for (j = 0; j < 7; j++) {
        unsigned char row = g[j];
        int i;
        for (i = 0; i < 5; i++) {
            if ((row >> (4 - i)) & 1) {
                int px0 = x + i*w;
                int py0 = y + j*h;
                int bx, by;
                for (by = 0; by < h; by++) {
                    for (bx = 0; bx < w; bx++) {
                        int rx, ry;
                        rtt(px0 + bx, py0 + by, cx, cy, r, &rx, &ry);
                        pt(rx, ry, col, t);
                    }
                }
            }
        }
    }
}

static void tx(int x, int y, const char *s, int col, int r, int t, int w, int h) {
    int i = 0, cx = x;
    for (; s[i]; i++) {
        ch(cx, y, (unsigned char)s[i], col, r, t, w, h);
        cx += 6*w;
    }
}

static void frc(int x, int y, int w, int h, int col, int r, int t) {
    int cx = x + w / 2, cy = y + h / 2;
    int j;
    for (j = 0; j < h; j++) {
        int i;
        for (i = 0; i < w; i++) {
            int rx, ry;
            rtt(x + i, y + j, cx, cy, r, &rx, &ry);
            pt(rx, ry, col, t);
        }
    }
}

static void ln(int x0, int y0, int x1, int y1, int col, int t) {
    int dx = x1 - x0, dy = y1 - y0;
    int sx = (dx >> 31) | 1, sy = (dy >> 31) | 1;
    dx = (dx ^ (dx >> 31)) - (dx >> 31);
    dy = (dy ^ (dy >> 31)) - (dy >> 31);
    dy = -dy;
    int e = dx + dy;
    for (;;) {
        pt(x0, y0, col, t);
        if (x0 == x1 && y0 == y1) return;
        int e2 = e << 1;
        if (e2 >= dy) { e += dy; x0 += sx; }
        if (e2 <= dx) { e += dx; y0 += sy; }
    }
}

static void orc(int x, int y, int w, int h, int col, int r, int t) {
    int cx = x + w / 2, cy = y + h / 2;
    int x_max = x + w - 1, y_max = y + h - 1;
    int a0, b0, a1, b1, a2, b2, a3, b3;
    rtt(x,     y,     cx, cy, r, &a0, &b0);
    rtt(x_max, y,     cx, cy, r, &a1, &b1);
    rtt(x_max, y_max, cx, cy, r, &a2, &b2);
    rtt(x,     y_max, cx, cy, r, &a3, &b3);
    ln(a0, b0, a1, b1, col, t);
    ln(a1, b1, a2, b2, col, t);
    ln(a2, b2, a3, b3, col, t);
    ln(a3, b3, a0, b0, col, t);
}

static void mupd(void) {
    int b = 0;
    plat_mouse(&msx, &msy, &b);
    msb = (unsigned char)b;
}

static int gmx(void) { return msx; }
static int gmy(void) { return msy; }

static void inpp(void) {
    register int i;
    for (i = 0; i < MXKY; i=i+1) {
        unsigned char now = (unsigned char)plat_kdown(i);
        g_in.down[i]     = now;
        g_in.pressed[i]  = now & ~g_in.prev[i];
        g_in.released[i] = g_in.prev[i] & ~now;
        g_in.prev[i]     = now;
    }
}

static int keyd(int vk)     { return g_in.down[vk];     }
static int keyp(int vk)  { return g_in.pressed[vk];  }
static int keyr(int vk) { return g_in.released[vk]; }

static unsigned int gp = 0;
static unsigned int gfs = 0;
static unsigned char gbf[65536];

static inline unsigned long long rdtsc(void) {
    unsigned int lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long)hi << 32) | lo;
}

static unsigned long long rng_state = 0;
static unsigned long long rngn(void) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 7;
    rng_state ^= rng_state << 17;
    return rng_state;
}
static unsigned __int128 rng(void) {
    unsigned long long a = rngn();
    unsigned long long b = rngn();
    return ((unsigned __int128)a << 64) | b;
}
static unsigned __int128 rndr(unsigned __int128 lo, unsigned __int128 hi) {
    if (lo > hi) { unsigned __int128 t = lo; lo = hi; hi = t; }
    return lo + (rng() % (hi - lo + 1));
}

static void dly(unsigned long long ms) {
    plat_sleep((unsigned int)ms);
}

static int lmn(void) {
    int v = 0, neg = 0;
    while (gp < gfs && (gbf[gp] == ' ' || gbf[gp] == '\t')) gp++;
    if (gp < gfs && gbf[gp] == '-') { neg = 1; gp++; }
    while (gp + 8 <= gfs) {
        unsigned long long w;
        unsigned char *q = (unsigned char *)&w;
        q[0]=gbf[gp]; q[1]=gbf[gp+1]; q[2]=gbf[gp+2]; q[3]=gbf[gp+3];
        q[4]=gbf[gp+4]; q[5]=gbf[gp+5]; q[6]=gbf[gp+6]; q[7]=gbf[gp+7];
        unsigned long long lo = 0x3030303030303030ULL;
        unsigned long long hi = 0x3939393939393939ULL;
        unsigned long long lt = (w | 0x8080808080808080ULL) - lo;
        unsigned long long gt = hi - (w | 0x8080808080808080ULL);
        unsigned long long ge = ~lt & 0x8080808080808080ULL & ~gt;
        if (ge != 0x8080808080808080ULL) break;
        v = v * 10 + (gbf[gp++] - '0');
        v = v * 10 + (gbf[gp++] - '0');
        v = v * 10 + (gbf[gp++] - '0');
        v = v * 10 + (gbf[gp++] - '0');
        v = v * 10 + (gbf[gp++] - '0');
        v = v * 10 + (gbf[gp++] - '0');
        v = v * 10 + (gbf[gp++] - '0');
        v = v * 10 + (gbf[gp++] - '0');
    }
    while (gp < gfs && gbf[gp] >= '0' && gbf[gp] <= '9') {
        v = v * 10 + (gbf[gp] - '0');
        gp++;
    }
    return neg ? -v : v;
}

static int lmh(void) {
    int v = 0;
    while (gp < gfs && (gbf[gp] == ' ' || gbf[gp] == '\t')) gp++;
    if (gp + 1 < gfs && gbf[gp] == '0' && (gbf[gp+1] == 'x' || gbf[gp+1] == 'X')) {
        gp += 2;
        while (gp + 8 <= gfs) {
            unsigned long long w;
            unsigned char *q = (unsigned char *)&w;
            q[0]=gbf[gp]; q[1]=gbf[gp+1]; q[2]=gbf[gp+2]; q[3]=gbf[gp+3];
            q[4]=gbf[gp+4]; q[5]=gbf[gp+5]; q[6]=gbf[gp+6]; q[7]=gbf[gp+7];
            unsigned long long H1 = 0x8080808080808080ULL;
            unsigned long long lo_d = (w | H1) - 0x3030303030303030ULL;
            unsigned long long hi_d = 0x3939393939393939ULL - (w | H1);
            unsigned long long is_d = ~lo_d & ~hi_d & H1;
            unsigned long long lo_l = (w | H1) - 0x6161616161616161ULL;
            unsigned long long hi_l = 0x6666666666666666ULL - (w | H1);
            unsigned long long is_l = ~lo_l & ~hi_l & H1;
            unsigned long long lo_u = (w | H1) - 0x4141414141414141ULL;
            unsigned long long hi_u = 0x4646464646464646ULL - (w | H1);
            unsigned long long is_u = ~lo_u & ~hi_u & H1;
            unsigned long long valid = is_d | is_l | is_u;
            if (valid != H1) break;
            {
                int i;
                for (i = 0; i < 8; i++) {
                    unsigned char c = (unsigned char)(w >> (i * 8));
                    int d;
                    if (c >= '0' && c <= '9') d = c - '0';
                    else if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
                    else d = c - 'A' + 10;
                    v = (v << 4) | d;
                }
                gp += 8;
            }
        }
        while (gp < gfs) {
            unsigned char c = gbf[gp];
            int d = -1;
            if (c >= '0' && c <= '9') d = c - '0';
            else if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
            else if (c >= 'A' && c <= 'F') d = c - 'A' + 10;
            if (d < 0) break;
            v = (v << 4) | d;
            gp++;
        }
    }
    return v;
}

static int lmt(const char *s) {
    unsigned int i = gp;
    unsigned int j = 0;
    while (i < gfs && (gbf[i]==' ' || gbf[i]=='\t')) i++;
    while (s[j] && i + j < gfs && gbf[i+j] == (unsigned char)s[j]) j++;
    if (!s[j]) { gp = i + j; return 1; }
    return 0;
}

static char *lms(void) {
    static char b[256];
    unsigned int n = 0;
    while (gp + 8 <= gfs) {
        unsigned long long w;
        unsigned char *q = (unsigned char *)&w;
        q[0]=gbf[gp]; q[1]=gbf[gp+1]; q[2]=gbf[gp+2]; q[3]=gbf[gp+3];
        q[4]=gbf[gp+4]; q[5]=gbf[gp+5]; q[6]=gbf[gp+6]; q[7]=gbf[gp+7];
        unsigned long long x = w ^ 0x2222222222222222ULL;
        x = (x - 0x0101010101010101ULL) & ~x & 0x8080808080808080ULL;
        if (x) break;
        gp += 8;
    }
    while (gp < gfs && gbf[gp] != '"') gp++;
    if (gp < gfs) gp++;
    while (gp < gfs && gbf[gp] != '"' && n < 255) b[n++] = gbf[gp++];
    b[n] = 0;
    if (gp < gfs) gp++;
    return b;
}

static int ldf(const char *fn) {
    gfs = 0;
    gfs = plat_load(fn, gbf, sizeof(gbf));
    return gfs > 0;
}

static int jumped=0, flag=0, nlb=0;
static unsigned int lbl[256];
static char lbn[256][32];
static char lwb[64];
static int yflag = 0;

static long long expr(void);
static char *lw(void);

static int flb(const char *s) {
    int i;
    for (i=0;i<nlb;i++) {
        unsigned int j=0;
        while (s[j] && lbn[i][j] && s[j]==lbn[i][j]) j++;
        if (!s[j] && !lbn[i][j]) return i;
    }
    return -1;
}

typedef struct { const char *name; void (*fn)(void); } CD;

static void cpxl(void) { register int x=expr(),y=expr(),c=lmh(),t=expr(); pt(x,y,c,t); }
static void clne(void) { register int a=expr(),b=expr(),c=expr(),d=expr(),e=lmh(),f=expr(); ln(a,b,c,d,e,f); }
static void cfrc(void) { register int a=expr(),b=expr(),c=expr(),d=expr(),e=lmh(),f=expr(),g=expr(); frc(a,b,c,d,e,f,g); }
static void corc(void) { register int a=expr(),b=expr(),c=expr(),d=expr(),e=lmh(),f=expr(),g=expr(); orc(a,b,c,d,e,f,g); }
static void ctxt(void) { register int x=expr(),y=expr(); char*s=lms(); register int c=lmh(),r=expr(),t=expr(),w=expr(),h=expr(); tx(x,y,s,c,r,t,w,h); }
static void csnd(void) { char*f=lms(); register int p=expr(),l=expr(); psnd(f,p,l); }
static void cclr(void) { register int c=lmh(); register int t=expr(); clr(c,t); }
static void ckyd(void) { int k = lmh(); flag = keyd(k) ? 1 : 0; }
static void ckyp(void) { int k = lmh(); flag = keyp(k) ? 1 : 0; }
static void ckyr(void) { int k = lmh(); flag = keyr(k) ? 1 : 0; }
static void chlt(void) { __asm__ volatile("hlt"); }
static void crst(void) { register long long a=expr(); dly(a); yflag = 1;}
static void crng(void);
static void cjmp(void) { char*n=lw(); int i=flb(n); if(i>=0){gp=lbl[i];jumped=1;} }
static void cjz(void) { char*n=lw(); if(!flag){int i=flb(n); if(i>=0){gp=lbl[i];jumped=1;}} }
static void cjnz(void) { char*n=lw(); if(flag){int i=flb(n); if(i>=0){gp=lbl[i];jumped=1;}} }
static void cceq(void) { register long long a=expr(),b=expr(); flag=(a==b); }
static void ccne(void) { register long long a=expr(),b=expr(); flag=(a!=b); }
static void ccge(void) { register long long a=expr(),b=expr(); flag=(a>=b); }
static void ccle(void) { register long long a=expr(),b=expr(); flag=(a<=b); }
static void ccgt(void) { register long long a=expr(),b=expr(); flag=(a>b); }
static void cclt(void) { register long long a=expr(),b=expr(); flag=(a<b); }
static void cext(void) {
#if defined(_WIN32)
    __asm__ __volatile__(
        "mov %0, %%ecx;"
        "sub $32, %%rsp;"
        "call ExitProcess;"
        "add $32, %%rsp;"
        :
        : "r"(0)
        : "rcx"
    );
#else
    __asm__ __volatile__("mov $60, %rax; xor %rdi, %rdi; syscall");
#endif
}

static CD cmd[] = {
    {"clr",cclr},{"pxl",cpxl},{"lne",clne},{"frc",cfrc},{"orc",corc},
    {"txt",ctxt},{"snd",csnd},{"ceq",cceq},{"cne",ccne},{"jmp",cjmp},{"jz",cjz},
    {"jnz",cjnz},{"kyd",ckyd},{"kyp",ckyp},{"kyr",ckyr},{"hlt",chlt},
    {"rst",crst},{"rng",crng},{"ext",cext},{"cge",ccge},{"cle",ccle},{"cgt",ccgt},{"clt",cclt},
};

static char *lw(void) {
    unsigned int n=0;
    while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
    while (gp<gfs && gbf[gp]!=' ' && gbf[gp]!='\t' && gbf[gp]!='\n' && gbf[gp]!='\r' && n<63) lwb[n++]=gbf[gp++];
    lwb[n]=0;
    return lwb;
}

static void scn(void) {
    unsigned int p=0, q;
    nlb=0;
    while (p<gfs && nlb<256) {
        unsigned int st=p;
        while (p + 8 <= gfs) {
            unsigned long long w;
            unsigned char *qq = (unsigned char *)&w;
            qq[0]=gbf[p]; qq[1]=gbf[p+1]; qq[2]=gbf[p+2]; qq[3]=gbf[p+3];
            qq[4]=gbf[p+4]; qq[5]=gbf[p+5]; qq[6]=gbf[p+6]; qq[7]=gbf[p+7];
            unsigned long long x = w ^ 0x0A0A0A0A0A0A0A0AULL;
            x = (x - 0x0101010101010101ULL) & ~x & 0x8080808080808080ULL;
            if (x) break;
            p += 8;
        }
        while (p<gfs && gbf[p]!='\n') p++;
        q=st;
        while (q<p && (gbf[q]==' '||gbf[q]=='\t')) q++;
        unsigned int ns=q;
        while (q<p && gbf[q]!=':' && gbf[q]!=' ' && gbf[q]!='\t') q++;
        if (q<p && gbf[q]==':') {
            unsigned int ln=q-ns;
            if (ln>0 && ln<32) {
                unsigned int k;
                for (k=0;k<ln;k++) lbn[nlb][k]=gbf[ns+k];
                lbn[nlb][ln]=0;
                lbl[nlb]=p+1;
                nlb++;
            }
        }
        if (p<gfs) p++;
    }
}

#define NSY 256
typedef struct { char nm[32]; unsigned int off; int w; int is_ptr; int stride; int mode; } SY;
static SY sy[NSY];
static int nsy = 0;
static unsigned int bit_top = 0;
static unsigned char g_bits[65536];

static unsigned long long rdbit(unsigned int off, int w) {
    if (w == 0) return 0;
    unsigned int byte = off >> 3;
    unsigned int bit = off & 7;
    if (bit == 0 && (w & 7) == 0) {
        int nb = w >> 3;
        unsigned long long v = 0;
        unsigned char *q = (unsigned char *)&v;
        int i;
        for (i = 0; i < nb; i++) q[i] = g_bits[byte + i];
        return v;
    }
    unsigned long long lo = 0, hi = 0;
    unsigned char *ql = (unsigned char *)&lo;
    unsigned char *qh = (unsigned char *)&hi;
    int i;
    for (i = 0; i < 8; i++) ql[i] = g_bits[byte + i];
    for (i = 0; i < 8; i++) qh[i] = g_bits[byte + 8 + i];
    lo >>= bit;
    if (bit) lo |= (hi << (64 - bit));
    if (w < 64) lo &= ((1ULL << w) - 1);
    return lo;
}

static void wrbit(unsigned int off, int w, unsigned long long v) {
    if (w == 0) return;
    unsigned int byte = off >> 3;
    unsigned int bit = off & 7;
    if (bit == 0 && (w & 7) == 0) {
        int nb = w >> 3;
        unsigned char *q = (unsigned char *)&v;
        int i;
        for (i = 0; i < nb; i++) g_bits[byte + i] = q[i];
        return;
    }
    unsigned long long lo = 0, hi = 0;
    unsigned char *ql = (unsigned char *)&lo;
    unsigned char *qh = (unsigned char *)&hi;
    int i;
    for (i = 0; i < 8; i++) ql[i] = g_bits[byte + i];
    for (i = 0; i < 8; i++) qh[i] = g_bits[byte + 8 + i];
    if (w < 64) v &= ((1ULL << w) - 1);
    lo &= ~(((1ULL << w) - 1) << bit);
    lo |= v << bit;
    if (bit && w + bit > 64) {
        int bits_in_hi = w + bit - 64;
        hi &= ~((1ULL << bits_in_hi) - 1);
        hi |= v >> (64 - bit);
    }
    for (i = 0; i < 8; i++) g_bits[byte + i] = ql[i];
    for (i = 0; i < 8; i++) g_bits[byte + 8 + i] = qh[i];
}

static int fnd(const char *n) {
    int i;
    for (i=0;i<nsy;i++) {
        unsigned int j=0;
        while (n[j] && sy[i].nm[j] && n[j]==sy[i].nm[j]) j++;
        if (!n[j] && !sy[i].nm[j]) return i;
    }
    return -1;
}

static long long or_expr(void);
static long long and_expr(void);
static long long add_expr(void);
static long long trm(void);
static long long atom(void);

static long long atom(void) {
    while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
    if (gbf[gp]=='*') {
        gp++;
        while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
        char nm[32];
        lw();
        unsigned int i=0;
        while (lwb[i] && i<31) { nm[i]=lwb[i]; i++; }
        nm[i]=0;
        int idx = fnd(nm);
        if (idx < 0) return 0;
        return (long long)sy[idx].off;
    }
    if (gbf[gp]=='#') {
        gp++;
        while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
        char nm[32];
        lw();
        unsigned int i=0;
        while (lwb[i] && i<31) { nm[i]=lwb[i]; i++; }
        nm[i]=0;
        int idx = fnd(nm);
        if (idx < 0) return 0;
        long long addr = (long long)rdbit(sy[idx].off, sy[idx].w);
        return (long long)rdbit((unsigned int)addr, sy[idx].stride);
    }
    if (gbf[gp]=='~') { gp++; return ~atom(); }
    if (gbf[gp]=='(') { gp++; long long v=expr(); while(gp<gfs&&gbf[gp]!=')')gp++; if(gp<gfs)gp++; return v; }
    if (gbf[gp]=='-' || (gbf[gp]>='0'&&gbf[gp]<='9')) return lmn();
    char nm[32];
    lw();
    unsigned int i=0;
    while (lwb[i] && i<31) { nm[i]=lwb[i]; i++; }
    nm[i]=0;
    if (nm[0]=='s' && nm[1]=='c' && nm[2]=='w' && nm[3]==0) return (long long)scw;
    if (nm[0]=='s' && nm[1]=='c' && nm[2]=='h' && nm[3]==0) return (long long)sch;
    if (nm[0]=='m' && nm[1]=='x' && nm[2]==0) return (long long)msx;
    if (nm[0]=='m' && nm[1]=='y' && nm[2]==0) return (long long)msy;
    if (nm[0]=='m' && nm[1]=='b' && nm[2]==0) return (long long)msb;
    if (nm[0]=='f' && nm[1]=='r' && nm[2]==0) return (long long)frm;
    int idx = fnd(nm);
    if (idx < 0) return 0;
    return (long long)rdbit(sy[idx].off, sy[idx].w);
}

static long long trm(void) {
    long long v = atom();
    for (;;) {
        while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
        if (gbf[gp]=='*') { gp++; v *= atom(); }
        else if (gbf[gp]=='/') { gp++; long long d=atom(); if(d) v/=d; }
        else if (gbf[gp]=='%') { gp++; long long d=atom(); if(d) v%=d; }
        else break;
    }
    return v;
}

static long long add_expr(void) {
    long long v = trm();
    for (;;) {
        while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
        if (gbf[gp]=='+') { gp++; v += trm(); }
        else if (gbf[gp]=='-') { gp++; v -= trm(); }
        else break;
    }
    return v;
}

static long long and_expr(void) {
    long long v = add_expr();
    for (;;) {
        while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
        if (gbf[gp]=='&' && gbf[gp+1]!='&') { gp++; v &= add_expr(); }
        else break;
    }
    return v;
}

static long long or_expr(void) {
    long long v = and_expr();
    for (;;) {
        while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
        if (gbf[gp]=='|' && gbf[gp+1]!='|') { gp++; v |= and_expr(); }
        else break;
    }
    return v;
}

static long long expr(void) { return or_expr(); }

static void cdec(int isreg) {
    char tp[8], nm[32];
    int isptr = 0, stride = 1, mode = 1;
    lw(); { unsigned int i=0; while(lwb[i]&&i<7){tp[i]=lwb[i];i++;} tp[i]=0; }
    while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
    if (gbf[gp]=='^') { isptr = 1; gp++; }
    lw(); { unsigned int i=0; while(lwb[i]&&i<31){nm[i]=lwb[i];i++;} nm[i]=0; }
    while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
    if (gbf[gp]=='{') {
        gp++;
        stride = lmn();
        while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
        if (gbf[gp]==',') { gp++; mode = lmn(); }
        else { mode = 1; }
        while (gp<gfs && gbf[gp]!='}') gp++;
        if (gp<gfs) gp++;
    }
    while (gp<gfs && gbf[gp]!='=' && gbf[gp]!='\n') gp++;
    if (gp<gfs) gp++;
    long long v = expr();
    int w = 0;
    { unsigned int i=1; while(tp[i]){ w=w*10+(tp[i]-'0'); i++; } }
    if (nsy < NSY && bit_top + w <= 524288) {
        unsigned int i=0;
        while (nm[i] && i<31) { sy[nsy].nm[i]=nm[i]; i++; }
        sy[nsy].nm[i]=0;
        sy[nsy].off = bit_top;
        sy[nsy].w = w;
        sy[nsy].is_ptr = isptr;
        sy[nsy].stride = stride;
        sy[nsy].mode = mode;
        wrbit(bit_top, w, (unsigned long long)v);
        bit_top += w;
        nsy++;
    }
}

static void casn(void) {
    char nm[32];
    int deref = 0;
    while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
    if (gbf[gp]=='#') { deref = 1; gp++; }
    lw();
    { unsigned int i=0; while(lwb[i]&&i<31){nm[i]=lwb[i];i++;} nm[i]=0; }
    while (gp<gfs && (gbf[gp]==' '||gbf[gp]=='\t')) gp++;
    int idx = fnd(nm);
    if (idx < 0) return;
    if (deref) {
        if (gbf[gp]=='=' && gbf[gp+1]!='=') {
            gp++;
            long long v = expr();
            long long addr = (long long)rdbit(sy[idx].off, sy[idx].w);
            wrbit((unsigned int)addr, sy[idx].stride, (unsigned long long)v);
        }
        return;
    }
    long long cur = (long long)rdbit(sy[idx].off, sy[idx].w);
    long long v = 0;
    if (gbf[gp]=='=' && gbf[gp+1]!='=') { gp++; v = expr(); }
    else if (gbf[gp]=='+' && gbf[gp+1]=='=') { gp+=2; v = cur + expr(); }
    else if (gbf[gp]=='-' && gbf[gp+1]=='=') { gp+=2; v = cur - expr(); }
    else if (gbf[gp]=='*' && gbf[gp+1]=='=') { gp+=2; v = cur * expr(); }
    else if (gbf[gp]=='/' && gbf[gp+1]=='=') { gp+=2; long long d=expr(); v = d ? cur / d : 0; }
    else if (gbf[gp]=='%' && gbf[gp+1]=='=') { gp+=2; long long d=expr(); v = d ? cur % d : 0; }
    else if (gbf[gp]=='=' && gbf[gp+1]=='=') { gp+=2; flag = (cur == expr()); return; }
    else if (gbf[gp]=='!' && gbf[gp+1]=='=') { gp+=2; flag = (cur != expr()); return; }
    else if (gbf[gp]=='<' ) { gp++; if (gbf[gp]=='=') { gp++; flag = (cur <= expr()); } else flag = (cur < expr()); return; }
    else if (gbf[gp]=='>' ) { gp++; if (gbf[gp]=='=') { gp++; flag = (cur >= expr()); } else flag = (cur > expr()); return; }
    else return;
    wrbit(sy[idx].off, sy[idx].w, (unsigned long long)v);
}

static void rnl(void) {
    unsigned int save, i;
    int f;
    if (gp == 0) scn();
    while (gp < gfs && !yflag) {
        save = gp;
        jumped = 0;
        f = 0;
        for (i=0;i<sizeof(cmd)/sizeof(cmd[0]);i++) {
            gp = save;
            if (lmt(cmd[i].name)) { cmd[i].fn(); f=1; break; }
        }
        if (!f) {
            gp = save;
            if (lmt("reg")) cdec(0);
            else if (lmt("mem")) cdec(1);
            else casn();
        }
        if (jumped) continue;
        gp = save;
        while (gp<gfs && gbf[gp]!='\n') gp++;
        if (gp<gfs) gp++;
    }
    yflag = 0;
}

static void crng(void) {
    char nm[32];
    lw();
    { unsigned int i=0; while(lwb[i]&&i<31){nm[i]=lwb[i];i++;} nm[i]=0; }
    register long long lo = expr();
    register long long hi = expr();
    int idx = fnd(nm);
    if (idx >= 0) wrbit(sy[idx].off, sy[idx].w, (unsigned long long)rndr((unsigned __int128)lo, (unsigned __int128)hi));
}

static volatile int lk = 0;
static inline __attribute__((always_inline)) void lka(void) {
    while (__atomic_exchange_n(&lk, 1, __ATOMIC_ACQUIRE));
}
static inline __attribute__((always_inline)) void lkr(void) {
    __atomic_store_n(&lk, 0, __ATOMIC_RELEASE);
}

static char gpth[1024];

static void gam(void) {
    inpp();
    mupd();
    frm = frm + 1;
    lka();
    plat_blit(fb);
    lkr();
}

static void* tun(void* p) {
    (void)p;
    for (;;) {
        lka();
        if (gpth[0]) {
            ldf(gpth);
            rnl();
        } else {
            orc(2, 2, scw-2, sch-2, 0x0011EE, 0, 255);
            tx(8, 16, "Drag a .kl file onto me!", 0x0000EF, 0, 255, 2, 2);
        }
        lkr();
        plat_sleep(16);
    }
    return 0;
}

static int e(int argc, char** argv) {
    (void)argc;
    rng_state = rdtsc() ^ 0x9E3779B97F4A7C15ULL;
    if (!plat_open(W, H, "Kryn")) return 1;
    fb = plat_fb();
    gpth[0] = 0;
    if (argv && argv[0]) {
        int i = 0, j = 0;
        while (argv[0][i] == '"') i++;
        while (argv[0][i] && argv[0][i] != '"' && j < 1023) gpth[j++] = argv[0][i++];
        gpth[j] = 0;
    }
    igt();
    plat_thread(tun);
    for (;;) {
        plat_pump();
        gam();
    }
    plat_shut();
    return 0;
}

#if defined(_WIN32)
int __stdcall WinMain(void* hi, void* hp, char* cmd, int show) {
    (void)hi; (void)hp; (void)show;
    char* a[1];
    a[0] = cmd;
    return e(1, a);
}
#else
int main(int argc, char** argv) {
    return e(argc, argv);
}
#endif
