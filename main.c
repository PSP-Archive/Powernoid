

//************************************ Powernoid by Art 2009 *********************************

#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspaudiolib.h>
#include <pspaudio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <png.h>
#include <math.h>
#include <pspgu.h>
#include <pspgum.h>
#include "graphics.h"
#include "intraFont.h"
#include "mikmod.h"
#include "Art.h"
#include "main.h"
#include "ballb.h"
#include "fball.h"
#include "missl.h"
#include "ping.h"
#include "popo.h"
#include "tss.h"
#include "sndgv.h"
#include "mbrick.h"
#include "pbrick.h"
#include "gbrick.h"
#include "dbrick.h"
#include "dwnbrick.h"
#include "powerup.h"
#include "buttons.h"
#include "ding.h"
#include "wbrick.h"
#include "plusone.h"

PSP_MODULE_INFO("Powernoid", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

sample_t* loop_buffer;				//
short mic_level;				//
unsigned int buf[5];				// free memory display buffers
unsigned int *pbuf = buf;			//
char filler[60];				// screen print character buffer
char fillerx[60];				// game over character buffer
int lives;					// number of lives variable
int stage;					// stage number
int score;					// score variable
int hiscore;					// highest score variable
int mscore;					// score multiplied
int endstage;					// end stage flag
int endgame;					// end game flag
int gpause;					// game pause for text screens
u8 rowa[9];					// brick arrays
u8 rowb[9];					//
u8 rowc[9];					//
u8 rowd[9];					//
u8 rowe[9];					//
u8 rowf[9];					//
u8 rowg[9];					//
u8 rowh[9];					//
u8 rowi[9];					//
u8 rowj[9];					//
u8 rowx[9];					// powerup bricks array
int del;					// delay counter
int shoot;					// shooting powerup flag
int wobble;					// wobbly ball bad powerup flag
int power;					// power ball powerup flag
int guard;					// guard powerup flag
int doubleb;					// double ball powerup
int advance;					// advance powerup
int fallpos;					//
int lost;					// lost life flag
int abullet;					// current bullet flags
int bbullet;					//
int bullety;					// bullet coordinates
int bulletx;					//
int xspeed;					// ball speed on x axis
int bx;						//
int by;						//
int bc;						//
int mballx;					// second ball coordinates
int mbally;					//
int mdirx;					// second ball direction flags
int mdiry;					//
int ballx;					// ball coordinates
int bally;					//
int dirx;					// direction flags
int diry;					//
int batx;					// bat position
int sam;					// sound routine variables
int samsel;					// sample select
int samlock;					// sound routine lock
int pxx;					// image processing variables
int pyy;					//
int i;						// counter
int kick;					// anti tamper status
int arta = 0x40;				// A
int artr = 0x74;				// r
int artt = 0x74;				// t
int ig;						// random number variables
int nosave;					// do not save score flag
u8 conf[10];					// config file array
u8 temp;					//
int cxtime;					//
u32 stagecol;					//
u32 seedcol;					//
u32 bricol;					//
u32 bapixel;					//
u32 bbpixel;					//
u32 dpixel;					//
u32 cpixel;					//
u32 pixel;					//
u32 badred;					//
u32 green;					//
u32 badredb;					//
u32 sample;					//
intraFont* ltn8;				//
SceCtrlData pad;				//
u32 image[480*100];				// banner image
int iindex;					// banner image index
int soff;					//
float size = 0.4;				// sine wave size
int exag = 136;					// sine wave loop exaggeration
int speed = 364;				// sine wave speed
int iyy;					// loop counter
int stimec;					// time counter for sine banner reverse
int sdir;					// fancy banner wave direction flag
float csize;					// banner font size
signed char charyoff;				// banner vertical line y offset
unsigned long j, t, m;				//
int y;						//
int cxxc;					// intro effect time counter
int cyyc;					//
int brickoff;					// moving brick offset
int introcnt;					// intro time counter
int lstage;					// random stage for extra life
int lcol;					// random column for extra life
int lrow;					// extra life brick row
int blstage;					// random stage for extra life
int blcol;					// random column for extra life
int blrow;					// extra life brick row
int ssample;					//
int done;					//

extern int _mm_errno;
extern BOOL _mm_critical;
extern char *_mm_errmsg[];
extern UBYTE md_musicvolume;
int mikModThreadID = -1;

const signed short aSin[360] = {
0 , 2 , 4 , 6 , 8 , 11 , 13 , 15 ,
17 , 19 , 22 , 24 , 26 , 28 , 30 , 32 ,
35 , 37 , 39 , 41 , 43 , 45 , 47 , 49 ,
51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 ,
67 , 69 , 71 , 72 , 74 , 76 , 78 , 79 ,
81 , 83 , 84 , 86 , 88 , 89 , 91 , 92 ,
94 , 95 , 97 , 98 , 100 , 101 , 102 , 104 ,
105 , 106 , 107 , 108 , 109 , 111 , 112 , 113 ,
114 , 115 , 116 , 116 , 117 , 118 , 119 , 120 ,
120 , 121 , 122 , 122 , 123 , 123 , 124 , 124 ,
125 , 125 , 125 , 126 , 126 , 126 , 126 , 126 ,
126 , 126 , 127 , 126 , 126 , 126 , 126 , 126 ,
126 , 126 , 125 , 125 , 125 , 124 , 124 , 123 ,
123 , 122 , 122 , 121 , 120 , 120 , 119 , 118 ,
117 , 116 , 116 , 115 , 114 , 113 , 112 , 111 ,
109 , 108 , 107 , 106 , 105 , 104 , 102 , 101 ,
100 , 98 , 97 , 95 , 94 , 92 , 91 , 89 ,
88 , 86 , 84 , 83 , 81 , 79 , 78 , 76 ,
74 , 72 , 71 , 69 , 67 , 65 , 63 , 61 ,
59 , 57 , 55 , 53 , 51 , 49 , 47 , 45 ,
43 , 41 , 39 , 37 , 35 , 32 , 30 , 28 ,
26 , 24 , 22 , 19 , 17 , 15 , 13 , 11 ,
8 , 6 , 4 , 2 , -1 , -3 , -5 , -7 ,
-9 , -12 , -14 , -16 , -18 , -20 , -23 , -25 ,
-27 , -29 , -31 , -33 , -36 , -38 , -40 , -42 ,
-44 , -46 , -48 , -50 , -52 , -54 , -56 , -58 ,
-60 , -62 , -64 , -66 , -68 , -70 , -72 , -73 ,
-75 , -77 , -79 , -80 , -82 , -84 , -85 , -87 ,
-89 , -90 , -92 , -93 , -95 , -96 , -98 , -99 ,
-101 , -102 , -103 , -105 , -106 , -107 , -108 , -109 ,
-110 , -112 , -113 , -114 , -115 , -116 , -117 , -117 ,
-118 , -119 , -120 , -121 , -121 , -122 , -123 , -123 ,
-124 , -124 , -125 , -125 , -126 , -126 , -126 , -127 ,
-127 , -127 , -127 , -127 , -127 , -127 , -127 , -127 ,
-127 , -127 , -127 , -127 , -127 , -127 , -126 , -126 ,
-126 , -125 , -125 , -124 , -124 , -123 , -123 , -122 ,
-121 , -121 , -120 , -119 , -118 , -117 , -117 , -116 ,
-115 , -114 , -113 , -112 , -110 , -109 , -108 , -107 ,
-106 , -105 , -103 , -102 , -101 , -99 , -98 , -96 ,
-95 , -93 , -92 , -90 , -89 , -87 , -85 , -84 ,
-82 , -80 , -79 , -77 , -75 , -73 , -72 , -70 ,
-68 , -66 , -64 , -62 , -60 , -58 , -56 , -54 ,
-52 , -50 , -48 , -46 , -44 , -42 , -40 , -38 ,
-36 , -33 , -31 , -29 , -27 , -25 , -23 , -20 ,
-18 , -16 , -14 , -12 , -9 , -7 , -5 , -3
};

void fancysetup(void);
void fancyprint(void);
void getimage(void);
void clearize(void);
void reset_bricks(void);
void reset_life(void);
void reset_colours(void);
void setup_bricks(void);
void check_bricks(void);
void check_end_stage(void);
void cheat_skipstage(void);
void end_screen(void);
void read_hiscore(void);
void save_hiscore(void);
void setup_start(void);
void rotateleft(void);
void rotateright(void);
void audioLoopStart(void);
void saveScreen(void);

int exit_callback(int arg1, int arg2, void *common) {
			sceKernelExitGame();
			return 0;
}

int CallbackThread(SceSize args, void *argp) {
			int cbid;
			cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
			sceKernelRegisterExitCallback(cbid);
			sceKernelSleepThreadCB();
			return 0;
}

int SetupCallbacks(void) {
			int thid = 0;
			thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
			if(thid >= 0) {sceKernelStartThread(thid, 0, 0);}
			return thid;
}
   
void audioOutputLoopCallback(void* buf, unsigned int length, void *userdata) {
   			sample_t* ubuf = (sample_t*) buf;
			int i;

			if (samsel == 0) {
			if (samlock == 1) {
			samsel = ssample; sam = 1;
			} // samlock
			} // ssample

			if (samlock == 1) {
			if (samsel == 1) {
			for (i = 0; i < 1024; i++) {
			ubuf[i].l = ping[sam+1] + (ping[sam]*256);
			ubuf[i].r = ubuf[i].l;
			sam++; sam++;
			if (sam > 3030) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			samlock = 0; samsel = 0;
			}
			}
			}

			if (samsel == 2) {
			for (i = 0; i < 1024; i++) {
			ubuf[i].l = popo[sam+1] + (popo[sam]*256);
			ubuf[i].r = ubuf[i].l;
			sam++; sam++;
			if (sam > 1166) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			samlock = 0; samsel = 0;
			}
			}
			}

			if (samsel == 3) {
			for (i = 0; i < 1024; i++) {
			ubuf[i].l = sndgv[sam+1] + (sndgv[sam]*256);
			ubuf[i].r = ubuf[i].l;
			sam++; sam++;
			if (sam > 57390) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			samlock = 0; samsel = 0;
			}
			}
			}

			if (samsel == 4) {
			for (i = 0; i < 1024; i++) {
			ubuf[i].l = powerup[sam+1] + (powerup[sam]*256);
			ubuf[i].r = ubuf[i].l;
			sam++; sam++; sam++; sam++;
			if (sam > 109320) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			samlock = 0;
			}
			}
			}

			if (samsel == 5) {
			for (i = 0; i < 1024; i++) {
			ubuf[i].l = tss[sam+1] + (tss[sam]*256);
			ubuf[i].r = ubuf[i].l;
			sam++; sam++;
			if (sam > 17508) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			samlock = 0; samsel = 0;
			}
			}
			}

			if (samsel == 6) {
			for (i = 0; i < 1024; i++) {
			ubuf[i].l = ding[sam+1] + (ding[sam]*256);
			ubuf[i].r = ubuf[i].l;
			sam++; sam++;
			if (sam > 65318) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			samlock = 0;
			}
			}
			}

			} else {
			int bbb;
			samsel = 0;
			for (bbb = 0; bbb < 1024; bbb++) {
			ubuf[bbb].l = 0xFFFF;
			ubuf[bbb].r = 0xFFFF;
			}
			}
}

			Image * Art;
			Image * ball;
			Image * fireball;
			Image * fire;
			Image * misbrick;
			Image * pwrbrick;
			Image * grdbrick;
			Image * dblbrick;
			Image * redbrick;
			Image * wobrick;
			Image * grnbrick;
			Image * grbbrick;
			Image * ibuttons;
			Image * frame;

void Print(unsigned long x,unsigned long y,unsigned long color,const char *str) {

			x = 0;
			while (x < 478) {
			charyoff = 16 + (aSin[(j+(x*8*m)*exag) % 360] >> 3);
			charyoff = charyoff * size;
			for (iyy=0; iyy<50; iyy++) {
			pixel = image[iindex];
			if (pixel != 0) {
			if (charyoff > 6) {pixel = pixel >> 8;}
			} // pixel
			putPixelScreen(pixel, x, iyy+charyoff);
			iindex++;
			}
			x++;
			}
}

void my_error_handler(void) {return;}

static int AudioChannelThread(int args, void *argp) {
			while (!done) {
			MikMod_Update();
			sceKernelDelayThread(1);
			}
			return (0);
}

			extern UWORD md_mode;
			extern UBYTE md_reverb;
			extern UBYTE md_pansep;

			MODULE *mf = NULL; // for mod

int main(void) {
			int maxchan = 128;
			MikMod_RegisterErrorHandler(my_error_handler);
			MikMod_RegisterAllDrivers();
			MikMod_RegisterAllLoaders();

			md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_SNDFX|DMODE_SOFT_MUSIC; 
			md_reverb = 0;
			md_pansep = 128;
			if (MikMod_Init("")) {
			sceKernelExitGame();
			return 0;
			}
  
			MikMod_SetNumVoices(-1, 8);
			MikMod_EnableOutput();

  if ((mikModThreadID = sceKernelCreateThread("MikMod" ,(void*)&AudioChannelThread,0x12,0x10000,0,NULL)) > 0) {
			sceKernelStartThread(mikModThreadID, 0 , NULL);
			}

			mf = Player_Load("./music.xm", maxchan, 0);
			md_musicvolume = 17;

			setup_start();						//
			speed = 364;						// set sine speed
while (1) {
			clearScreen(0);
			setup_bricks();						// draw bricks

			fillScreenRect(0xFFEFDDCA, batx, 264, 80, 8);		// draw bat

			if (guard == 1) {					// draw guard
			fillScreenRect(0xFFEFDDCA, 0, 264, 40, 8);		//
			fillScreenRect(0xFFEFDDCA, 440, 264, 40, 8);		//
			} // guard						//

			if (shoot == 1) {					// bullet collision detect
			if (abullet == 1) {					//
			bapixel = getPixelScreen(bulletx+5, bullety+5);		//
			} // abullet						//
			if (bbullet == 1) {					//
			bbpixel = getPixelScreen(bulletx+71, bullety+5);	//
			} // bbullet						//
			} // shoot						//

			if (diry == 0) {					// collision detect ball one
			if (dirx == 0) {					//
			cpixel = getPixelScreen(ballx+8, bally+8);		//
			} else {						//
			cpixel = getPixelScreen(ballx, bally+8);		//
			}							//
			} else {						//
			if (dirx == 0) {					//
			cpixel = getPixelScreen(ballx+8, bally);		//
			} else {						//
			cpixel = getPixelScreen(ballx, bally);			//
			}							//
			} // diry						//
			if (mdiry == 0) {					// collision detect ball two
			if (mdirx == 0) {					//
			dpixel = getPixelScreen(mballx+8, mbally+8);		//
			} else {						//
			dpixel = getPixelScreen(mballx, mbally+8);		//
			}							//
			} else {						//
			if (mdirx == 0) {					//
			dpixel = getPixelScreen(mballx+8, mbally);		//
			} else {						//
			dpixel = getPixelScreen(mballx, mbally);		//
			}							//
			} // mdiry						//

			check_bricks();						// brick collision detect

			if (gpause == 0) {					// draw ball
			if (power == 0) {
			blitAlphaImageToScreen(0, 0, 9, 9, ball, ballx,bally);	//
			} else {
			blitAlphaImageToScreen(0, 0, 9, 9, fireball, ballx,bally);
			}
			if (doubleb == 1) {					//
			if (power == 0) {
			blitAlphaImageToScreen(0, 0, 9, 9, ball, mballx,mbally);// draw second ball
			} else {
			blitAlphaImageToScreen(0, 0, 9, 9, fireball, mballx,mbally);// draw second ball
			}
			} // doubleb						//
			} else {						//
			if (power == 0) {
			blitImageToScreen(0, 0, 9, 9, ball, ballx,bally);	// draw ball
			} else {
			blitImageToScreen(0, 0, 9, 9, fireball, ballx,bally);	// draw ball
			}
			if (doubleb == 1) {					//
			if (power == 0) {
			blitImageToScreen(0, 0, 9, 9, ball, mballx,mbally);	// draw second ball
			} else {
			blitImageToScreen(0, 0, 9, 9, fireball, mballx,mbally);	// draw second ball
			}
			} // doubleb						//
			} // gpause						//

			if (shoot == 1) {					// draw bullets
			if (abullet == 1) {					//
			blitAlphaImageToScreen(0, 0, 9, 9, fire, bulletx,bullety);
			} // abullet						//
			if (bbullet == 1) {					//
			blitAlphaImageToScreen(0, 0, 9, 9, fire, bulletx + 71,bullety);
			} // bbullet						//
			} // shoot						//

			if (cpixel == 0xFFEFDDCA) {				// handle bat collision
			samlock = 1; // re-enable player routine		//
			ssample = 2; // select pop sample			//
			bally--;						//
			if (diry == 0) {diry = 1;} else {diry = 0;}		//
			if (bally > 220) {
			xspeed = 3;						//
			if (ballx < batx + 20) {				//
			if (dirx == 0) {dirx = 1;}				//
			}							//
			if (ballx > batx + 60) {				//
			if (dirx == 1) {dirx = 0;}				//
			}							//
			if (ballx > batx + 25) {				//
			if (ballx < batx + 55) {				//
			xspeed = 2;						//
			}}							//
			if (ballx > batx + 35) {				//
			if (ballx < batx + 45) {				//
			xspeed = 1;						//
			}}							//
			} // bally
			if (advance == 1) {fallpos++;fallpos++;fallpos++;}	//
			} // cpixel						//

			if (dpixel == 0xFFEFDDCA) {				// handle bat collision for second ball
			if (ssample != 3) {					//
			samlock = 1; // re-enable player routine		//
			ssample = 2; // select pop sample			//
			} // ssample						//
			mbally--;						//
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}		//
			} // dpixel						//

			if (gpause == 0) {					// no game movement if paused

			if (doubleb == 1) {
			if (mdirx == 0) {
			mballx++;mballx++;

			if (wobble == 1) {
			mballx = mballx + charyoff/8;
			} // wobble

			if (mballx > 469) {mdirx = 1;}
			} else {
			mballx--;mballx--;

			if (wobble == 1) {
			mballx = mballx + charyoff/8;
			} // wobble

			if (mballx < 2) {mdirx = 0;}
			} // mdirx

			if (mdiry == 0) {
			mbally++;
			if (mbally > 262) {mdiry = 1;
			mballx = 236;
			mbally = 0;
			if (doubleb == 0) {
			if (lives > 1) {
			lives--;
			} else {
			endgame = 1; gpause = 440; lives--;
			} // lives
			} // double b
			doubleb = 0;
			}
			} else {
			mbally--;
			if (mbally < 1) {mdiry = 0;}
			} // mdiry
			} // doubleb

			if (dirx == 0) {
			ballx = ballx + xspeed;
			if (wobble == 1) {
			ballx = ballx + charyoff/8;
			} // wobble
			if (ballx > 468) {dirx = 1;}
			} else {
			ballx = ballx - xspeed;
			if (wobble == 1) {
			ballx = ballx + charyoff/8;
			} // wobble
			if (ballx < 3) {dirx = 0;}
			} // dirx

			if (advance == 1) {
			if (lost == 1) {
			lost = 0;
			reset_life();
			shoot = 0;	// reset powerups
			power = 0;	//
			guard = 0;	//
			doubleb = 0;	//
			wobble = 0;	//
			samlock = 1; // re-enable player routine
			ssample = 3; // select gv sample
			gpause = 220;
			}}

			if (diry == 0) {
			bally = bally + 4;
			if (bally > 260) {diry = 1;

			if (doubleb == 0) {
			reset_life();
			shoot = 0;	// reset powerups
			power = 0;	//
			guard = 0;	//
			doubleb = 0;	//
			wobble = 0;	//
			samlock = 1; // re-enable player routine
			ssample = 3; // select gv sample
			gpause = 220;
			} else {
			diry = mdiry; dirx = mdirx;
			bally = mbally; ballx = mballx;
			} // doubleb
			if (doubleb == 0) {
			if (lives > 1) {
			lives--;
			} else {
			endgame = 1; gpause = 440; lives--;
			} // lives
			} // doubleb
			doubleb = 0;
			}
			} else {
			bally = bally - 4;
			if (bally < 4) {diry = 0;}
			} // diry

			if (shoot == 1) {
			if (abullet == 1 || bbullet == 1) {
			if (bullety > 10) {
			bullety = bullety - 10;
			} else {
			abullet = 0; bbullet = 0;
			} // bullety
			} // bullet
			} // shoot

			} else {	// if game movement is paused

			mscore = score * 10;

			guStart();
			sprintf(filler, "STAGE: %d",stage);
			intraFontSetStyle(ltn8, 1.0f,0xFFEFDDCA,0,INTRAFONT_ALIGN_CENTER);
			intraFontPrint(ltn8, 240, 200, filler);
			sprintf(filler, "BALLS: %d",lives);
			intraFontSetStyle(ltn8, 1.0f,0xFFEFDDCA,0,INTRAFONT_ALIGN_CENTER);
			intraFontPrint(ltn8, 240, 240, filler);
			sprintf(filler, "SCORE: %d",mscore);
			intraFontSetStyle(ltn8, 1.0f,0xFFEFDDCA,0,INTRAFONT_ALIGN_CENTER);
			intraFontPrint(ltn8, 240, 220, filler);
			sceGuFinish();
			sceGuSync(0,0);
			if (gpause == 1) {
			if (endgame == 1) {

			if (mscore > hiscore) {hiscore = mscore; save_hiscore();}

			end_screen();
			stage = 1; reset_colours(); reset_bricks(); reset_life(); mbally = 0; lives = 3;
			} // endgame
			initGraphics();
			} // gpause
			} // gpause

			check_end_stage();
			if (endstage == 1) {
			stage++; abullet = 0; bbullet = 0;
			if (stage == 16) {stage = 1;}
			reset_colours();
			reset_bricks();
			reset_life();
			gpause = 220;
			} // endstage

			sceCtrlSetSamplingMode(1); 
			sceCtrlPeekBufferPositive(&pad, 1);

			if(pad.Buttons & PSP_CTRL_CROSS) {
			if (shoot == 1) {
			if (gpause == 0) {
			if (abullet == 0 && bbullet == 0) {
			abullet = 1; bbullet = 1; bulletx = batx; bullety = 252;
			samlock = 1; // re-enable player routine
			ssample = 5; // select fire bullet sample
			} // bullet
			} // gpause
			} // shoot
			}

			if (gpause == 0) {

			if(pad.Buttons & PSP_CTRL_LEFT) {
			if (batx > 5) {batx = batx - 6;}
			}
			if(pad.Buttons & PSP_CTRL_RIGHT) {
			if (batx < 395) {batx = batx + 6;}
			}
			if(pad.Lx > 235) {
			if (batx < 395) {batx = batx + 6;}
			}
			if(pad.Lx < 20) {
			if (batx > 5) {batx = batx - 6;}
			}
			} // gpause

			if(pad.Buttons & PSP_CTRL_HOME) {gpause = 200;}

			if(pad.Buttons & PSP_CTRL_DOWN) {		//
			md_musicvolume = 0;				// in game music off
			}

			if(pad.Buttons & PSP_CTRL_UP) {			//
			md_musicvolume = 17;				// in game music on

	//	cheat_skipstage(); gpause = 100;		// skip stage

			if(pad.Buttons & PSP_CTRL_TRIANGLE) {		//
			doubleb = 1; power = 1; guard = 1; shoot = 1; 	// activate all good powerups
			nosave = 1;					//
			samlock = 1; // re-enable player routine	//
			ssample = 4; // select powerup sample		//
			}						//
			if(pad.Buttons & PSP_CTRL_CIRCLE) {		//
			wobble = 1; advance = 1; 			// activate all bad powerups
			nosave = 1;					//
			samlock = 1; // re-enable player routine	//
			ssample = 4; // select powerup sample		//
			}
			}						//

			sceDisplayWaitVblankStart();
			flipScreen();
			if (gpause > 0) {gpause--;}

			} // while
			return 0;
}

void setup_bricks() {
			charyoff = (aSin[(j+(8*m)*134) % 360] >> 3);		// increment sine table
			j += speed; j %= 360;					// 

			int vspace;
			int vstart;
			vstart = 30;
			vspace = 10;
			brickoff = 0;

			if (stage == 2) {vspace = 20; vstart = 10; brickoff = charyoff;}
			if (stage == 3 || stage == 10) {vstart = 50;}
			if (stage == 4 || stage == 6) {vstart = 60;}
			if (stage == 7 || stage == 8) {vstart = 70;}

			if (stage == 14) {			// draw barrier
			fillScreenRect(0xFFEFDDCA, 192, 161, 95, 8);
			fillScreenRect(0xFFEFDDCA, 0, 161, 47, 8);
			fillScreenRect(0xFFEFDDCA, 432, 161, 47, 8);
			vstart = 70;
			} // stage

			if (stage == 9) {			// draw barrier
			fillScreenRect(0xFFEFDDCA, 48, 91, 383, 8);
			vstart = 40;
			} // stage

			if (stage == 13) {			// draw barrier
			fillScreenRect(0xFFEFDDCA, 48+charyoff, 141, 383, 8);
			} // stage

			if (stage == 5) {			// draw barrier
			fillScreenRect(0xFFEFDDCA, 144+charyoff*2, 31, 191, 8);
			vstart = 50;
			} // stage

			if (stage == 11) {			// draw barrier
			fillScreenRect(0xFFEFDDCA, 48+charyoff*2, 141, 95, 8);
			fillScreenRect(0xFFEFDDCA, 336+charyoff*2, 141, 95, 8);
			vstart = 30;
			} // stage

			if (stage == 12) {			// draw barrier
			fillScreenRect(0xFFEFDDCA, 48+charyoff*2, 141, 95, 8);
			fillScreenRect(0xFFEFDDCA, 336+charyoff*2, 141, 95, 8);
			fillScreenRect(0xFFEFDDCA, 144+charyoff, 31, 191, 8);
			vstart = 40;
			} // stage
			
			lrow = vstart + 20;
			if (stage == 2) {lrow = lrow + 20;}
			blrow = lrow;

			if (advance == 1) {vstart = 40 + fallpos;}
			lost = 0;

			seedcol = stagecol; bc = 48; bricol = seedcol;
			bc += brickoff;
			for (i=0; i<8; i++) { //
			if (rowa[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol; bc = 48; vstart = vstart + vspace;
			bc -= brickoff;
			for (i=0; i<8; i++) { //
			if (rowb[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol; bc = 48; vstart = vstart + vspace;
			bc += brickoff;
			for (i=0; i<8; i++) { //
			if (rowc[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol; bc = 48; vstart = vstart + vspace;
			bc -= brickoff;
			for (i=0; i<8; i++) { //
			if (rowd[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol; bc = 48; vstart = vstart + vspace;
			bc += brickoff;
			for (i=0; i<8; i++) { //
			if (rowe[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol; bc = 48; vstart = vstart + vspace;
			bc -= brickoff;
			for (i=0; i<8; i++) { //
			if (rowf[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol; bc = 48; vstart = vstart + vspace;
			bc += brickoff;
			for (i=0; i<8; i++) { //
			if (rowg[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol; bc = 48; vstart = vstart + vspace;
			bc -= brickoff;
			for (i=0; i<8; i++) { //
			if (rowh[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol; bc = 48; vstart = vstart + vspace;
			bc += brickoff;
			for (i=0; i<8; i++) { //
			if (rowi[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol; bc = 48; vstart = vstart + vspace;
			bc -= brickoff;
			for (i=0; i<8; i++) { //
			if (rowj[i] == 1) {fillScreenRect(bricol, bc, vstart, 47, 9);
			if (vstart > 256) {lost = 1;}
			}
			bc = bc + 48; bricol = bricol + 0xFF100000;
			} // i
			
			if (rowx[0] == 1) {blitImageToScreen(0, 0, 47, 9, misbrick, 216,110);}
			if (rowx[1] == 1) {blitImageToScreen(0, 0, 47, 9, pwrbrick, 216,10);}
			if (rowx[2] == 1) {blitImageToScreen(0, 0, 47, 9, grdbrick, 216,100);}
			if (rowx[3] == 1) {blitImageToScreen(0, 0, 47, 9, dblbrick, 216,80);}
			if (rowx[4] == 1) {blitImageToScreen(0, 0, 47, 9, redbrick, 240,120);}
			if (rowx[5] == 1) {blitImageToScreen(0, 0, 47, 9, wobrick, 240,150);}

			if (rowx[6] == 1) {
			blitImageToScreen(0, 0, 47, 9, grnbrick, 48+brickoff+lcol*48,lrow);
 			rowc[lcol] = 0;
			}
			if (rowx[7] == 1) {
			blitImageToScreen(0, 0, 47, 9, grbbrick, 48+brickoff+blcol*48,blrow);
 			rowc[blcol] = 0;
			}
}

void check_bricks() {
			seedcol = stagecol;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowa[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowa[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowa[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowa[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowb[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowb[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowb[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowb[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowc[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowc[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowc[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowc[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowd[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowd[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowd[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowd[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowe[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowe[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowe[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowe[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowf[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowf[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowf[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowf[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowg[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowg[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowg[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowg[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowh[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowh[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowh[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowh[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowi[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowi[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowi[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowi[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i
			seedcol = seedcol + 0x00000FEF;
			bricol = seedcol;
			for (i=0; i<8; i++) { //
			if (cpixel == bricol) {rowj[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (dpixel == bricol) {rowj[i] = 0; score++;
			samlock = 1; // re-enable player routine
			ssample = 1; // select ping sample
			if (power == 0) {
			if (mdiry == 0) {mdiry = 1;} else {mdiry = 0;}
			} // power
			}
		if (bapixel == bricol) {rowj[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		abullet = 0;
		} // bapixel
		if (bbpixel == bricol) {rowj[i] = 0; score++;
		samlock = 1; // re-enable player routine
		ssample = 1; // select ping sample
		bbullet = 0;
		} // bbpixel
			bc += 48; bricol += 0xFF100000;
			} // i

			if (bapixel == 0xFFEFDDCA) {abullet = 0;} // bullet hit barrier
			if (bbpixel == 0xFFEFDDCA) {bbullet = 0;} //
			bapixel = 0; bbpixel = 0;

			if (cpixel == 0xF733EE33 || dpixel == 0xF733EE33) {rowx[7] = 0; score = score + 10;
			lives++;
			samlock = 1; // re-enable player routine
			ssample = 6; // select extra life sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}

			if (cpixel == 0xF833EE33 || dpixel == 0xF833EE33) {rowx[6] = 0; score = score + 10;
			lives++;
			samlock = 1; // re-enable player routine
			ssample = 6; // select extra life sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}

			if (cpixel == badredb) {rowx[5] = 0; score = score + 10;
			wobble = 1;	// activate wobbly ball disadvantage
			samlock = 1; // re-enable player routine
			ssample = 4; // select powerup sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}

			if (cpixel == badred) {rowx[4] = 0; score = score + 10;
			advance = 1;	// activate advancing bricks disadvantage
			samlock = 1; // re-enable player routine
			ssample = 4; // select powerup sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}

			if (cpixel == 0xFEEFDDCA || dpixel == 0xFEEFDDCA) {rowx[0] = 0; score = score + 10;
			shoot = 1;	// activate shooting powerup
			samlock = 1; // re-enable player routine
			ssample = 4; // select powerup sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
			if (cpixel == 0xFDEFDDCA || dpixel == 0xFDEFDDCA) {rowx[1] = 0; score = score + 10;
			power = 1;	// activate power ball powerup
			samlock = 1; // re-enable player routine
			ssample = 4; // select powerup sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}

			if (cpixel == 0xFE667766 || dpixel == 0xFE667766) {rowx[0] = 0; score = score + 10;
			shoot = 1;	// activate shooting powerup
			samlock = 1; // re-enable player routine
			ssample = 4; // select powerup sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}

			if (cpixel == 0xFCEFDDCA || dpixel == 0xFCEFDDCA) {rowx[2] = 0; score = score + 10;
			guard = 1;	// activate guard powerup
			samlock = 1; // re-enable player routine
			ssample = 4; // select powerup sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}

			if (cpixel == 0xFC777777 || dpixel == 0xFC777777) {rowx[2] = 0; score = score + 10;
			guard = 1;	// activate guard powerup
			samlock = 1; // re-enable player routine
			ssample = 4; // select powerup sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}

			if (cpixel == 0xFBEFDDCA || dpixel == 0xFBEFDDCA) {rowx[3] = 0; score = score + 10;
			doubleb = 1;	// activate double ball powerup
			mballx = ballx;
			mbally = bally;
			mdiry = diry;
			if (dirx == 0) {mdirx = 1;} else {mdirx = 0;}
			samlock = 1; // re-enable player routine
			ssample = 4; // select powerup sample
			if (power == 0) {
			if (diry == 0) {diry = 1;} else {diry = 0;}
			} // power
			}
}

void check_end_stage() {
			endstage = 1;
			for (i=0; i<8; i++) { //
			if (rowa[i] == 1) {endstage = 0;}
			if (rowb[i] == 1) {endstage = 0;}
			if (rowc[i] == 1) {endstage = 0;}
			if (rowd[i] == 1) {endstage = 0;}
			if (rowe[i] == 1) {endstage = 0;}
			if (rowf[i] == 1) {endstage = 0;}
			if (rowg[i] == 1) {endstage = 0;}
			if (rowh[i] == 1) {endstage = 0;}
			if (rowi[i] == 1) {endstage = 0;}
			if (rowj[i] == 1) {endstage = 0;}
			}
}

void end_screen() {
			SceCtrlData xpad;
			stage = 1;
			reset_colours();
			reset_bricks();
       			if (NULL != mf) {
			Player_Stop();
			}
			MikMod_DisableOutput();
			int gos = 0; cyyc = 1;
			while (1) {
			sceCtrlSetSamplingMode(1); 
			sceCtrlPeekBufferPositive(&xpad, 1);
			clearScreen(0);
			guStart();
			intraFontSetStyle(ltn8, 1.0f,0xFFEFDDCA,0,INTRAFONT_ALIGN_CENTER);
			intraFontPrint(ltn8, 240, 200, fillerx);
			sprintf(filler, "HI SCORE: %d",hiscore);
			intraFontPrint(ltn8, 240, 220, filler);
			sceGuFinish();
			sceGuSync(0,0);
			setup_bricks();
			blitImageToScreen(75, 0, 76, 42, ibuttons, 191,226);
			sceDisplayWaitVblankStart();
			flipScreen();

			if (cyyc == 1) {check_end_stage();}
			if (cyyc == 6) {
			if (endstage == 1) {
			stage++;
			gos = -1;
			if (stage > 15) {stage = 1;}
			reset_colours();
			reset_bricks();
			} // endstage
			}

			if(xpad.Buttons & PSP_CTRL_CROSS) {
			samlock = 1; // re-enable player routine
			ssample = 2; // select pop sample
			gpause = 220;
			mscore = 0; score = 0;
			initGraphics(); endgame = 0;

			srand(j);
			lstage = (rand() % (14-0)); lstage++;
			srand(time(NULL));
			lcol = (rand() % (7-0));
			blstage = (rand() % (4-0)); blstage++;
			blcol = (rand() % (7-0));

			if (blcol == lcol) {
			blcol++;
			if (blcol == 8) {blcol--;blcol--;}
			} // blcol
			MikMod_EnableOutput();
       			if (NULL != mf) {
			mf->wrap = 1;
			Player_Start(mf);
      			}
			break;
			} // cross

			if (cyyc == 0) {cxtime = (rand() % (8-0));}
			if (cyyc == 7) {ig = (rand() % (10-0));
			if (gos < 7) {gos++;}
			}
			if (cyyc == 2) {
			rowa[gos] = 0;
			rowb[gos] = 0;
			rowc[gos] = 0;
			}
			if (cyyc == 3) {
			rowd[gos] = 0;
			rowe[gos] = 0;
			rowf[gos] = 0;
			} // cyyc
			if (cyyc == 4) {
			rowg[gos] = 0;
			rowh[gos] = 0;
			rowi[gos] = 0;
			rowj[gos] = 0;
			} // cyyc

			cyyc++;
			if (cyyc == 8) {cyyc = 0;}

			} // while
}

void reset_bricks() {
			for(i=0; i<8; i++) {	// reset all bricks
			rowa[i] = 1; rowb[i] = 1;
			rowc[i] = 1; rowd[i] = 1;
			rowe[i] = 1; rowf[i] = 1;
			rowg[i] = 1; rowh[i] = 1;
			rowi[i] = 1; rowj[i] = 1;
			rowx[i] = 0;
			} // i

			if (stage == 1) {
			rowh[3] = 0; rowh[4] = 0; rowx[2] = 1;
			} // stage

			if (stage == 2) {
			for(i=0; i<8; i++) {rowj[i] = 0;} // turn off bottom row
			} // stage

			if (stage == 3) {
			rowx[0] = 1;
			rowj[1] = 0; rowj[6] = 0; rowg[4] = 0; rowg[5] = 0;
			rowi[1] = 0; rowi[6] = 0; rowh[4] = 0; rowh[5] = 0;
			rowh[1] = 0; rowh[6] = 0; rowh[2] = 0; rowh[3] = 0;
			rowg[1] = 0; rowg[6] = 0; rowg[2] = 0; rowg[3] = 0;
			} // stage

			if (stage == 4) {
			for(i=0; i<8; i++) {
			rowa[i] = 0; rowb[i+1] = 0;
			rowc[i] = 0; rowd[i+1] = 0;
			rowe[i] = 0; rowf[i+1] = 0;
			rowg[i] = 0; rowh[i+1] = 0;
			rowi[i] = 0; rowj[i+1] = 0;
			i++;
			} // i
			} // stage

			if (stage == 5) {rowx[1] = 1;} // include power ball powerup

			if (stage == 6) {
			rowc[1] = 0; rowc[6] = 0; rowd[1] = 0; rowd[6] = 0;
			rowe[1] = 0; rowe[6] = 0; rowf[1] = 0; rowf[6] = 0; rowi[5] = 0;
			rowg[1] = 0; rowg[6] = 0; rowh[1] = 0; rowh[6] = 0; rowb[5] = 0;
			rowb[1] = 0; rowb[6] = 0; rowb[2] = 0; rowb[3] = 0; rowb[4] = 0;
			rowi[1] = 0; rowi[6] = 0; rowi[2] = 0; rowi[3] = 0; rowi[4] = 0;
			} // stage

			if (stage == 7) {
			rowd[3] = 0; rowd[4] = 0; rowx[2] = 1;
			} // stage

			if (stage == 8) {
			rowb[0] = 0; rowb[7] = 0; rowb[1] = 0; rowb[6] = 0; rowa[0] = 0; rowa[7] = 0;
			rowi[0] = 0; rowi[7] = 0; rowi[1] = 0; rowi[6] = 0; rowj[0] = 0; rowj[7] = 0;
			rowc[0] = 0; rowc[7] = 0; rowc[1] = 0; rowc[6] = 0; rowc[2] = 0; rowc[5] = 0;
			rowh[0] = 0; rowh[7] = 0; rowh[1] = 0; rowh[6] = 0; rowh[2] = 0; rowh[5] = 0;
			rowd[0] = 0; rowd[7] = 0; rowd[1] = 0; rowd[6] = 0; rowd[2] = 0; rowd[5] = 0;
			rowg[0] = 0; rowg[7] = 0; rowg[1] = 0; rowg[6] = 0; rowg[2] = 0; rowg[5] = 0;
			} // stage

			if (stage == 9) {
			rowe[0] = 0; rowe[7] = 0; rowe[1] = 0; rowe[6] = 0; rowe[2] = 0; rowe[5] = 0;
			rowf[0] = 0; rowf[7] = 0; rowf[1] = 0; rowf[6] = 0; rowf[2] = 0; rowf[5] = 0;
			rowf[3] = 0; rowf[4] = 0; rowe[3] = 0; rowe[4] = 0; rowx[3] = 1;
			} // stage

			if (stage == 11) {
			rowa[0] = 0; rowa[7] = 0; rowj[0] = 0; rowj[7] = 0;
			rowa[1] = 0; rowa[6] = 0; rowj[1] = 0; rowj[6] = 0;
			rowa[2] = 0; rowa[5] = 0; rowj[2] = 0; rowj[5] = 0;
			rowb[0] = 0; rowb[7] = 0; rowi[0] = 0; rowi[7] = 0;
			rowb[1] = 0; rowb[6] = 0; rowi[1] = 0; rowi[6] = 0;
			rowc[0] = 0; rowc[7] = 0; rowh[0] = 0; rowh[7] = 0; rowx[1] = 1;
			} // stage

			if (stage == 13) {
			rowi[3] = 0; rowi[4] = 0; rowx[0] = 1;
			} // stage

			if (stage == 14) {
			rowa[3] = 0; rowa[4] = 0; rowb[3] = 0; rowb[4] = 0;
			rowc[3] = 0; rowc[4] = 0; rowd[3] = 0; rowd[4] = 0;
			rowe[3] = 0; rowe[4] = 0; rowf[3] = 0; rowf[4] = 0;
			rowg[3] = 0; rowg[4] = 0; rowh[3] = 0; rowh[4] = 0;
			rowi[3] = 0; rowi[4] = 0; rowj[3] = 0; rowj[4] = 0;
			} // stage

			if (stage == 15) {rowj[4] = 0; rowx[4] = 1;}

			if (stage == 6) {rowj[4] = 0; rowx[5] = 1;}

			if (lstage == stage) {rowx[6] = 1;}
			if (blstage == stage) {rowx[7] = 1;}
}

void reset_colours() {
			if (stage == 1) {stagecol = 0xFF00A7FF;}
			if (stage == 2) {stagecol = 0xEE173166;}
			if (stage == 3) {stagecol = 0xFF88C199;}
			if (stage == 4) {stagecol = 0xCC123155;}
			if (stage == 5) {stagecol = 0x662F1165;}
			if (stage == 6) {stagecol = 0x66334C6A;}
			if (stage == 7) {stagecol = 0xDD112ABC;}
			if (stage == 8) {stagecol = 0xF12A2A2A;}
			if (stage == 9) {stagecol = 0xDD662FFC;}
			if (stage ==10) {stagecol = 0xAD11AF1C;}
			if (stage ==11) {stagecol = 0x3A21412A;}
			if (stage ==12) {stagecol = 0xAACB2FDC;}
			if (stage ==13) {stagecol = 0xF1332DDA;}
			if (stage ==14) {stagecol = 0x7DB67F71;}
			if (stage ==15) {stagecol = 0xBB11BBDC;}
}

void reset_life() {
			mballx = 236; mbally = 0;
			ballx = 236; bally = 251;
			batx = 200;
			dirx = 0; diry = 0;
			abullet = 0; bbullet = 0; bullety = 252;
			fallpos = 0; advance = 0;
}

void cheat_skipstage() {
			for(i=0; i<8; i++) {	// reset all bricks
			rowa[i] = 0;
			rowb[i] = 0;
			rowc[i] = 0;
			rowd[i] = 0;
			rowe[i] = 0;
			rowf[i] = 0;
			rowg[i] = 0;
			rowh[i] = 0;
			rowi[i] = 0;
			rowj[i] = 0;
			} // i
}

void clearize() {
			pyy = 0;
			pxx = 0;
			pixel = getPixelImage(pxx, pyy, ball);
			sample = pixel;
			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<9; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, ball);
			if (pixel == sample) {
			putPixelImage(0x00000000, pxx, pyy, ball);
			}
			} // width
			} // height

			pyy = 0;
			pxx = 0;
			pixel = getPixelImage(pxx, pyy, fireball);
			sample = pixel;
			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<9; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, fireball);
			if (pixel == sample) {
			putPixelImage(0x00000000, pxx, pyy, fireball);
			}
			} // width
			} // height

			pyy = 0;
			pxx = 0;
			pixel = getPixelImage(pxx, pyy, fire);
			sample = pixel;
			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<9; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, fire);
			if (pixel == sample) {
			putPixelImage(0x00000000, pxx, pyy, fire);
			}
			} // width
			} // height

			pyy = 0;
			pxx = 0;
			pixel = getPixelImage(pxx, pyy, misbrick);
			sample = pixel;
			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<47; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, misbrick);
			if (pixel == sample) {
			putPixelImage(0xFEEFDDCA, pxx, pyy, misbrick);
			}
			} // width
			} // height

			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<47; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, misbrick);
			putPixelImage(0xFEEFDDCA, pxx, pyy, misbrick);
			pxx++;
			} // width
			} // height

			pyy = 0;
			pxx = 0;
			pixel = getPixelImage(pxx, pyy, misbrick);
			sample = pixel;
			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<47; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, misbrick);
			if (pixel != sample) {
			putPixelImage(0xFE667766, pxx, pyy, misbrick);
			}
			} // width
			} // height

			pyy = 0;
			pxx = 0;
			pixel = getPixelImage(pxx, pyy, grdbrick);
			sample = pixel;
			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<47; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, grdbrick);
			if (pixel == sample) {
			putPixelImage(0xFCEFDDCA, pxx, pyy, grdbrick);
			} else {
			putPixelImage(0xFC777777, pxx, pyy, grdbrick);
			}

			} // width
			} // height

			pyy = 0;
			pxx = 0;
			pixel = getPixelImage(pxx, pyy, pwrbrick);
			sample = pixel;
			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<47; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, pwrbrick);
			if (pixel == sample) {
			putPixelImage(0xFDEFDDCA, pxx, pyy, pwrbrick);
			}
			} // width
			} // height

			pyy = 0;
			pxx = 0;
			pixel = getPixelImage(pxx, pyy, dblbrick);
			sample = pixel;
			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<47; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, dblbrick);
			if (pixel == sample) {
			putPixelImage(0xFBEFDDCA, pxx, pyy, dblbrick);
			}
			} // width
			} // height

			pyy = 1;
			pxx = 1;
			pixel = getPixelImage(pxx, pyy, Art);
			sample = pixel;
			for (pyy=0; pyy<57; pyy++) { // image height
			for (pxx=0; pxx<100; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, Art);
			if (pixel == sample) {
			putPixelImage(0x00000000, pxx, pyy, Art);
			} else {
			putPixelImage(0xFFEFDDCA, pxx, pyy, Art);
			}
			} // width
			} // height

			pyy = 1;
			pxx = 1;
			pixel = getPixelImage(pxx, pyy, redbrick);
			badred = pixel;

			pixel = getPixelImage(pxx, pyy, wobrick);
			badredb = pixel;

			pixel = getPixelImage(pxx, pyy, grnbrick);
			green = pixel;

			for (pyy=0; pyy<9; pyy++) { // image height
			for (pxx=0; pxx<47; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, grnbrick);
			if (pixel == green) {
			putPixelImage(0xF833EE33, pxx, pyy, grnbrick);
			putPixelImage(0xF733EE33, pxx, pyy, grbbrick);
			}
			} // width
			} // height
}

void setup_start() {
			kick = artr + artt + arta;
			if (kick == 0x128) {
			artr--;
			Artpic[5] = 0x64;
			Artpic[25] = 0x6D;
			Artpic[37] = 0x1D;
			Artpic[12] = 0x27;
			Artpic[7] = 0x64;
			Artpic[26] = 0x6E;
			arta++;
			Artpic[0] = 0xE7;
			Artpic[6] = 0x74;
			artr--;
			Artpic[1] = 0x3E;

			for(i=0; i<316; i++) {
			Artpic[i] = Artpic[i] ^ 0x6E;	// simple xor decrypt Art logo graphic
			}
			Art = loadImageMemory(Artpic, 316);
			ball = loadImageMemory(ballb, 186);
			fireball = loadImageMemory(fball, 189);
			fire = loadImageMemory(missl, 205);
			pwrbrick = loadImageMemory(pbrick, 263);
			misbrick = loadImageMemory(mbrick, 264);
			grdbrick = loadImageMemory(gbrick, 203);
			dblbrick = loadImageMemory(dbrick, 242);
			redbrick = loadImageMemory(dwnbrick, 242);
			wobrick = loadImageMemory(wbrick, 212);
			grnbrick = loadImageMemory(plusone, 205);
			grbbrick = loadImageMemory(plusone, 205);
			ibuttons = loadImageMemory(buttons, 6554);
			} // kick = 128
			clearize();

			intraFontInit();
			ltn8 = intraFontLoad("flash0:/font/ltn8.pgf",INTRAFONT_CACHE_ASCII); 
			if(!ltn8) sceKernelExitGame();

			SetupCallbacks();
			loop_buffer = NULL;
			pspAudioInit();
			audioLoopStart();
			samlock = 1; // re-enable player routine
			ssample = 4; // select powerup sample
			read_hiscore();
			initGraphics();
			stage = 10;
			reset_colours();
			reset_bricks();
			fancysetup();
			ig = 7; cxtime = 0;

			srand(speed);
			lstage = (rand() % (14-0)); lstage++;
			srand(time(NULL));
			lcol = (rand() % (7-0));
			blstage = (rand() % (4-0)); blstage++;
			srand(time(NULL));
			blcol = (rand() % (7-0));

			if (blcol == lcol) {
			blcol++;
			if (blcol == 8) {blcol--;blcol--;}
			} // blcol

			while (1) {
			sceCtrlSetSamplingMode(1); 
			sceCtrlPeekBufferPositive(&pad, 1);
			clearScreen(0);
			introcnt++;
			if (introcnt > 800) {introcnt = 0;}
			if (del > 40) {
			if (introcnt < 400) {
			guStart();
			fancyprint();
			sprintf(filler, "Inspired by MegaBall for the Amiga!");
			intraFontSetStyle(ltn8, 1.0f,0xFFEFDDCA,0,INTRAFONT_ALIGN_CENTER);
			intraFontPrint(ltn8, 240, 85, filler);
			intraFontSetStyle(ltn8, 1.1f,0xFFEFDDCA,0,INTRAFONT_ALIGN_CENTER);
			sprintf(filler, "11/01/09");
			intraFontSetStyle(ltn8, 1.0f,0xFFEFDDCA,0,INTRAFONT_ALIGN_LEFT);
			intraFontPrint(ltn8, 280, 125, filler);
			check_end_stage();
			if (endstage == 1) {
			sprintf(filler, "Intrafont library by BenHur");
			intraFontSetStyle(ltn8, 1.0f,0xFFEFDDCA,0,INTRAFONT_ALIGN_CENTER);
			intraFontPrint(ltn8, 240, 160, filler);
			sprintf(filler, "HI SCORE: %d",hiscore);
			intraFontPrint(ltn8, 240, 210, filler);
			} // endstage
			sceGuFinish();
			sceGuSync(0,0);
			blitImageToScreen(0, 7, 96, 48, Art, 180,90);
			} else {
			guStart();
			fancyprint();
			intraFontSetStyle(ltn8, 0.8f,0xFFEFDDCA,0,INTRAFONT_ALIGN_CENTER);
			intraFontPrint(ltn8, 270, 88, "Shooting Paddle");
			intraFontPrint(ltn8, 270, 103, "Power Ball");
			intraFontPrint(ltn8, 270, 118, "Side Guards");
			intraFontPrint(ltn8, 270, 133, "Multiball");
			intraFontPrint(ltn8, 270, 148, "Wobbly Ball");
			intraFontPrint(ltn8, 270, 163, "Falling Bricks");
			intraFontPrint(ltn8, 270, 178, "Extra Ball");
			intraFontSetStyle(ltn8, 1.0f,0xFFEFDDCA,0,INTRAFONT_ALIGN_CENTER);
			sprintf(filler, "HI SCORE: %d",hiscore);
			intraFontPrint(ltn8, 240, 210, filler);
			sceGuFinish();
			sceGuSync(0,0);
			blitImageToScreen(0, 0, 47, 9, misbrick, 160,80);
			blitImageToScreen(0, 0, 47, 9, pwrbrick, 160,95);
			blitImageToScreen(0, 0, 47, 9, grdbrick, 160,110);
			blitImageToScreen(0, 0, 47, 9, dblbrick, 160,125);
			blitImageToScreen(0, 0, 47, 9, wobrick, 160,140);
			blitImageToScreen(0, 0, 47, 9, redbrick, 160,155);
			blitImageToScreen(0, 0, 47, 9, grnbrick, 160,170);
			} // introcnt
			} // del

			setup_bricks();

			blitImageToScreen(75, 0, 76, 42, ibuttons, 191,226);
			sceDisplayWaitVblankStart();
			flipScreen();

			if(pad.Buttons & PSP_CTRL_CROSS) {
			clearScreen(0);
			sceDisplayWaitVblankStart();
			flipScreen();
			initGraphics();

			samlock = 1; // re-enable player routine
			ssample = 2; // select pop sample
			stage = 1;
			reset_colours();
			reset_bricks();
     			if (NULL != mf) {
			mf->wrap = 1;
			Player_Start(mf);
			}
			break;
			} // cross
			if (endstage == 0) {
			del++;

			if (del > 40) {
			rowa[cxtime] = 0; rowe[cxtime] = 0; rowi[cxtime] = 0;
			rowc[cxtime] = 0; rowg[cxtime] = 0;
			rowb[ig] = 0; rowf[ig] = 0; rowj[ig] = 0;
			rowd[ig] = 0; rowh[ig] = 0;
			cxxc++;
			if (cxxc == 3) {
			if (ig > 0) {
			cxxc = 0;
			ig--;
			cxtime++;
			}}
			} // del

			} // endstage
			} // while

			initGraphics();			// allow alpha graphics after intrafont printing
			sprintf(fillerx, "GAME OVER");
			mballx = 236;
			mbally = 0;
			ballx = 240;
			bally = 252;
			batx = 200;
			dirx = 0;
			diry = 0;
			lives = 3;
			bullety = 252;
			gpause = 30;
			endgame = 0;

			clearScreen(0);
			sceDisplayWaitVblankStart();
			flipScreen();
			clearScreen(0);
			sceDisplayWaitVblankStart();
			flipScreen();
}

void rotateright() {
			temp = (conf[5] << 7);			// bitwise rotate right once
			conf[5] = (conf[4] << 7) | (conf[5] >> 1);
			conf[4] = (conf[3] << 7) | (conf[4] >> 1);
			conf[3] = (conf[2] << 7) | (conf[3] >> 1);
			conf[2] = (conf[1] << 7) | (conf[2] >> 1);
			conf[1] = (conf[0] << 7) | (conf[1] >> 1);
			conf[0] = temp | (conf[0] >> 1);
}

void rotateleft() {
			temp = (conf[0] >> 7);			// bitwise rotate left once
			conf[0] = (conf[1] >> 7) | (conf[0] << 1);
			conf[1] = (conf[2] >> 7) | (conf[1] << 1);
			conf[2] = (conf[3] >> 7) | (conf[2] << 1);
			conf[3] = (conf[4] >> 7) | (conf[3] << 1);
			conf[4] = (conf[5] >> 7) | (conf[4] << 1);
			conf[5] = temp | (conf[5] << 1);
}

void read_hiscore() {
			int stg;
			int checksum;
			int ic;
			stg = sceIoOpen("./powernoid.bin", PSP_O_RDONLY, 0777);
			sceIoRead(stg,conf,10);
			sceIoClose(stg);
			rotateright(); rotateright();
			hiscore = hiscore + conf[0] * 100000;
			hiscore = hiscore + conf[1] * 10000;
			hiscore = hiscore + conf[2] * 1000;
			hiscore = hiscore + conf[3] * 100;
			hiscore = hiscore + conf[4] * 10;
			hiscore = hiscore + conf[5];
			checksum = 0x3F;
			for (ic = 0; ic < 5; ic++) {checksum = checksum ^ conf[ic];}
			if (conf[6] != checksum) {hiscore = 0;}
			checksum = 0x52;
			for (ic = 0; ic < 6; ic++) {checksum = checksum ^ conf[ic];}
			if (conf[7] != checksum) {hiscore = 0;}
			checksum = 0x24;
			for (ic = 2; ic < 7; ic++) {checksum = checksum ^ conf[ic];}
			if (conf[8] != checksum) {hiscore = 0;}
			checksum = 0x1C;
			for (ic = 0; ic < 8; ic++) {checksum = checksum + conf[ic];}
			if (conf[9] != checksum) {hiscore = 0;}
}

void save_hiscore() {
			int sstg;
			int hix;
			hix = hiscore;
			int checksum;
			int ic;
			conf[0] = hix / 100000;
			hix = hix - conf[0] * 100000;
			conf[1] = hix / 10000;
			hix = hix - conf[1] * 10000;
			conf[2] = hix / 1000;
			hix = hix - conf[2] * 1000;
			conf[3] = hix / 100;
			hix = hix - conf[3] * 100;
			conf[4] = hix / 10;
			hix = hix - conf[4] * 10;
			conf[5] = hix;
			checksum = 0x3F;
			for (ic = 0; ic < 5; ic++) {checksum = checksum ^ conf[ic];}
			conf[6] = checksum;
			checksum = 0x52;
			for (ic = 0; ic < 6; ic++) {checksum = checksum ^ conf[ic];}
			conf[7] = checksum;
			checksum = 0x24;
			for (ic = 2; ic < 7; ic++) {checksum = checksum ^ conf[ic];}
			conf[8] = checksum;
			checksum = 0x1C;
			for (ic = 0; ic < 8; ic++) {checksum = checksum + conf[ic];}
			conf[9] = checksum;
			rotateleft(); rotateleft();
			if (nosave == 0) {
			sstg = sceIoOpen("./powernoid.bin", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
			sceIoWrite(sstg,conf,10);
			sceIoClose(sstg);
			} // nosave
			nosave = 0;
}

void audioLoopStart() {
			loop_buffer = (sample_t*) malloc(PSP_NUM_AUDIO_SAMPLES * sizeof(sample_t));
			sceKernelDelayThread(200000);	
			pspAudioSetChannelCallback(0, audioOutputLoopCallback, NULL);
}

void getimage() {
			iindex = 0;
			for (pxx=0; pxx<480; pxx++) { // image width
			for (pyy=222; pyy<272; pyy++) { // image height
			pixel = getPixelScreen(pxx, pyy);
			image[iindex] = pixel;
			iindex++;
			} // width
			} // height
			iindex = 0;
			for (pxx=0; pxx<480; pxx++) { // image width
			for (pyy=0; pyy<50; pyy++) { // image height
			putPixelImage(image[iindex], pxx, pyy, frame);
			iindex++;
			} // width
			} // height
}

void fancysetup() {
			charyoff=0;
			j=0;
			m=1;
			y=15;
			frame = createImage(480, 50);
			fillImageRect(0xDDDDDDDD, 0, 0, 480, 50, frame);
			clearScreen(0);
			guStart();
			intraFontSetStyle(ltn8, 2.0,0xFFFF3300,0,INTRAFONT_ALIGN_CENTER | 12);
			intraFontPrint(ltn8, 240, 262, "POWERNOID");
			sceGuFinish();
			sceGuSync(0,0);
			getimage();
}

void fancyprint() {
			iindex = 0;
			Print(5,y,0,"POWERNOID");
			stimec++;

			if (stimec > 7) {
			stimec = 0;
			if (sdir == 0) {speed++;} else {speed--;}

			if (speed == 360) {
			if (sdir == 0) {speed++;} else {speed--;}
			} // speed

			if (speed > 378) {sdir = 1;}
			if (speed < 342) {sdir = 0;}
			}

			j += speed;
			j %= 360;
}

void saveScreen() {
			int posX;
			int posY;
			Color pixel;
			Color* vram = getVramDisplayBuffer();
			Image* screenShot;
			screenShot = createImage(480,272);
			for (posY=0; posY<272; posY++) {
			for(posX=0; posX<480; posX++) {
			pixel = vram[PSP_LINE_SIZE * posY + posX];
			putPixelImage(pixel,posX,posY,screenShot);
			}
			}
			saveImage("ms0:/Snapshot.png",screenShot->data,480,272,PSP_LINE_SIZE,0);
			freeImage(screenShot);
}
