*Kuil is a visual-language, with assembly like code's logic.*

[It is still not fully finished, but I put real time on it]



Commands:


Windows (in CMD or PowerShell):

gcc kl.c kw.c -O0 -s -lkernel32 -luser32 -lgdi32 -lwinmm -o e.exe


Linux (in a terminal, with libx11-dev installed):

gcc kl.c kx.c -O0 -s -lX11 -lpthread -o e


macOS (in Terminal, with Xcode Command Line Tools installed):

gcc kl.c km.c -O0 -s -framework Cocoa -framework CoreGraphics -pthread -o e
