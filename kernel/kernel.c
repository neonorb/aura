#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint-gcc.h>

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

#include "../utils/utils.h"

#include "log.c"

#include "gdt.c"
#include "idt.c"

#include "../modules/clock/clock.c"
#include "../modules/screen/screen.c"
#include "../modules/keyboard/keyboard.c"

#include "../implementation/implementation.c"

void kernel_main() {
	cli();

	screen_terminal_initialize();

	log("Setting up GDT");
	gdt_init();

	log("Setting up IDT");
	init_idt();

	log("Setting up clock");
	clock_initialize();

	log("Setting up keyboard");
	keyboard_initialize();

	//log("Initializing ACPI");
	//initAcpi();

	sti();

	log("Welcome to Aura!");

	implementation();

	fault(
			"                                  whatthefu                                         ");
	fault(
			"                              ckdidyoujustfucki                                     ");
	fault(
			"                          ngsayaboutme,youlittlebi                                  ");
	fault(
			"                  tch?I’llhaveyou           knowIgra                                ");
	fault(
			"               duatedtopofmy                  classin                               ");
	fault(
			"             theNavySeals,an                   dI’veb                               ");
	fault(
			"             eeninvolvedinnum                   erous                               ");
	fault(
			"             secretraids onAl-Q    uaeda,andIh  aveov                               ");
	fault(
			"             er300confirmedkills .Iamtrainedingo rill                               ");
	fault(
			"             awarfareandI’mthe  topsniperintheentireU                               ");
	fault(
			"            Sarme  dforces.You  arenothingtomebutjust                               ");
	fault(
			"           anothertarget.Iwillw ipeyouthefu ckoutwith                               ");
	fault(
			"          precisionthelikesof   whichhasneverbeenseen                               ");
	fault(
			"         beforeonthisEarth,markmyfuckingwords  .Yout                                ");
	fault(
			"        hinky          oucangetawaywithsa     yingth                                ");
	fault(
			"       atshi                      ttomeov     ertheI                                ");
	fault(
			"      nterne                                 t?Thin                                 ");
	fault(
			"     kagain                                 ,fucke                                  ");
	fault(
			"    r.Aswe                                  speakI                                  ");
	fault(
			"    amcon                      tact        ingmys                                   ");
	fault(
			"    ecre                      tnetw ork   ofspie                                    ");
	fault(
			"    sacr                      osstheUSAa  ndyou                         rIPisbein   ");
	fault(
			"   gtrac                      edrightno  wsoyo                        ubetterprepa  ");
	fault(
			"   refor                     thestorm,m aggot                       .Thest    ormt  ");
	fault(
			"   hatwi                     pesoutthe  pathe                     ticlitt    lethi  ");
	fault(
			"   ngyou                    callyourl  ife.Yo                   u’refuc     kingd   ");
	fault(
			"   ead,k                    id.Icanb   eanywhere,anytime,an   dIcanki     llyou     ");
	fault(
			"    inov                   ersevenh    undredways,andthat’sjustwith      mybar      ");
	fault(
			"    ehan                   ds.Noto     nlyam   Iexte   nsivelytra      inedin       ");
	fault(
			"    unar                  medcomba      t,b   utIhaveaccesstoth      eentir         ");
	fault(
			"    earse               nalof theUn         itedStatesMarineCo     rpsandI          ");
	fault(
			"     will             useit  toitsfu         llextenttowipeyourm   iserable         ");
	fault(
			"     assof            fthefaceofthec                     ontinent    ,youlittl      ");
	fault(
			"      eshit            .Ifonlyyouco              uldh       avekno  wnwh atunh      ");
	fault(
			"      olyret              ribu                   tion        yourli  ttle“cle       ");
	fault(
			"       ver”co                                mme              ntwas    abou         ");
	fault(
			"        ttobring                            down              upony     ou,m        ");
	fault(
			"           aybeyou                          woul              dhaveheldyourf        ");
	fault(
			" uck        ingtongue.                       Buty           oucouldn’t,youd         ");
	fault(
			"idn’t,a    ndnowyou’repaying                  the         price,y    o              ");
	fault(
			"ugoddamnidiot. Iwillshitfuryallovery           ouan    dyouwil                      ");
	fault(
			"ldro wninit.You’ref    uckingdead,kidd o.Wwhatthefuckdidyouj                        ");
	fault(
			" ustf  uckingsaya         boutme,youl ittlebitch?I’llhave                           ");
	fault(
			"  youk   nowIgr         aduatedtopof mycla ssintheNavy                              ");
	fault(
			"   Seals,andI           ’vebeeninvo  lved                                           ");
	fault(
			"    innumer              oussecre   trai                                            ");
	fault(
			"      dso                nAl-Qu    aeda                                             ");
	fault(
			"                          ,andIh  aveo                                              ");
	fault(
			"                           ver300conf                                               ");
	fault(
			"                             irmedki                                                ");
	fault(
			"                               lls                                                  "); // returning from here will clear interrupts, halt the system, and enter a jmp loop (boot.s)

	fault("");
	fault("We have just returned from the implmentation :(");
}
