#ifndef __CONFIG_H__
#define __CONFIG_H__

//STRUCT CONSTS
#define mxBLK 25
#define mxCNT 30
#define mxREG 20
#define ENTER_KEY 13
#define BACKSPACE_KEY 8
#define ESCAPE_KEY 27
#define maxLEN 50

//BLOCK IDS
#define START    1
#define STOP     2
#define DECISION 3
#define EXPR     4
#define READ     5
#define WRITE    6

//GRAPHICS
#define width  800
#define height 800
#define blkW  70
#define blkH 40
#define DELAY 15
#define ANIM_DELAY 100
#define BLK_STROKE WHITE
#define CNNT_STROKE MAGENTA 
#define CNNT_R 7     
#define EDGE_R 7
#define BG BLACK
#define BOUND 21 
#define DECLEFTBIAS 10
#define DECRIGHTBIAS 15
#define OVERLAPBIAS 20
#define BOUND_TXT 40//30
#define HTXT LIGHTMAGENTA
#define SLOW_STEP 200
#define RUN_STEP 50
#define FAST_STEP 10
int STEP_DELAY = SLOW_STEP;

#define c3c4side 15
#define c1c2side 20
#define KEY_WAIT 10000000

#define intermW 306
#define intermH 70
#define filemenuW 236

#endif
