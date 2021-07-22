
PSPSDK                 = $(shell psp-config --pspsdk-path)
PSPDIR                 = $(shell psp-config --psp-prefix)

INCDIR                 := ./include
CFLAGS                 = -G0 -Wall -O2 -fno-strict-aliasing
LIBS                   = -lmikmod -lpspaudiolib -lpspaudio -lpspgum -lpspgu -lpsppower -lpng -lz -lm -lpspumd

OBJS                   = main.o graphics.o framebuffer.o intraFont.o

TARGET                 = Powernoid
EXTRA_TARGETS          = EBOOT.PBP
PSP_EBOOT_ICON = ICON0.png
PSP_EBOOT_PIC1 = PIC1.PNG
PSP_EBOOT_TITLE        = Powernoid
EXTRA_CLEAN            =

include $(PSPSDK)/lib/build.mak

