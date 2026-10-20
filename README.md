Kuil is a visual language, with assembly like code's logic.

It is still not fully finished, but I put real time on it.

Commands
<span style="color:blue">Windows</span>

cmd
gcc kl.c kw.c -O0 -s -lkernel32 -luser32 -lgdi32 -lwinmm -o e.exe
<span style="color:red">macOS</span>

bash
gcc kl.c km.c -O0 -s -framework Cocoa -framework CoreGraphics -pthread -o e
<span style="color:green">Linux</span>

bash
gcc kl.c kx.c -O0 -s -lX11 -lpthread -o e
