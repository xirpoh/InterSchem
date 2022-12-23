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
#define WIDTH 800
#define HEIGHT 800
#define width  1200//800
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

//nou
#define cpwidth	300
#define cpheight height/2
#define padding 10

#define prop 1.5
#define buttonh cpwidth/10 *prop
#define buttonw cpwidth/10 *prop
#define outpadding 10
#define buttonx (width-cpwidth-buttonw-outpadding)
#define buttony outpadding
#define cpx buttonx+buttonw
#define cpy buttony+buttonh
#define codpaneltopoffset cpy+padding
#define codpanelleftoffset cpx+padding

#define lbuttonw  cpwidth/10 *prop
#define lbuttonh  cpwidth/10 *prop
#define lbuttonx (buttonx+buttonw)  
#define lbuttony (buttony)  
#define rbuttonw  cpwidth/10 *prop
#define rbuttonh  cpwidth/10 *prop
#define rbuttonx (lbuttonx+lbuttonw)  
#define rbuttony (buttony)

#define ubuttonw  cpwidth/10 *prop
#define ubuttonh  cpwidth/10 *prop
#define ubuttonx (buttonx)  
#define ubuttony (buttony+buttonh)  
#define dbuttonw  cpwidth/10 *prop
#define dbuttonh  cpwidth/10 *prop
#define dbuttonx (ubuttonx)  
#define dbuttony (ubuttony+ubuttonh)

#define inputx rbuttonx+rbuttonw+5
#define inputy buttony
#define inputw cpwidth/10*5
#define inputh buttonh
#define inputbg 255,255,255
#define inputfg 0,0,0
#define inputinpadding 5
#define inputoutpadding 5
#define updowncut inputh/10 

#define dlbuttonx cpx + cpwidth - buttonw * 0.75
#define dlbuttony  buttony + buttonh * 0.25
#define dlbuttonw  buttonw*0.5
#define dlbuttonh  buttonh*0.5

#define hcpw 40
#define hcph 40
#define hcpx buttonx
#define hcpy buttony+buttonh+cpheight-hcpw

#define shcpw 40
#define shcph 40
#define shcpx cpx+cpwidth-shcpw
#define shcpy buttony

#define cpbg 4,28,48
#define error 10

#endif
