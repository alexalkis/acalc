// -*- compile-command: "m68k-amigaos-gcc -s -noixemul -Os -o acalc acalc.c -lmpfr -lgmp" -*-
// for sas c try: sc math=standard link acalc.c
/*
**  A small calculator.  (I only wanted to check out gadtools! Sorry :P)
*/

#define USEMPFR

#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __SASC
#define M_E             2.7182818284590452354   /* e */
#define M_PI            3.14159265358979323846  /* pi */
#ifdef USEMPFR
#undef USEMPFR
#endif
#endif

#ifdef USEMPFR
#include <gmp.h>
#include <mpfr.h>
typedef mpfr_t num;
#define BITSACCURACY (80)
#else
typedef double num;
#endif

BYTE version[] = "\0$VER: acalc 1.0 (" __DATE__ " "GITVERSION")";

enum GdIds {
  GD_N0 = 0,
  GD_N1,
  GD_N2,
  GD_N3,
  GD_N4,
  GD_N5,
  GD_N6,
  GD_N7,
  GD_N8,
  GD_N9,
  GD_DISPLAY,
  GD_SIN,
  GD_COS,
  GD_PLUS,
  GD_AC,
  GD_TAN,
  GD_LOG,
  GD_MINUS,
  GD_CE,
  GD_SQRT,
  GD_LN,
  GD_MULT,
  GD_PI,
  GD_RECI,
  GD_POWER,
  GD_PERIOD,
  GD_EQUAL,
  GD_DIV,
  GD_E,
  GD_FACTORIAL,
  GD_SQUARE,
  GD_BACKSPACE,

  GD_CONSTANT_C,
  GD_CONSTANT_G,
  GD_CONSTANT_K,
  GD_CONSTANT_SECONDS_IN_YEAR,

  GD_MODE_SIMPLE,
  GD_MODE_SCIENTIFIC,
  GD_MODE_PROGRAMMERS,

  GD_EXIT
};

struct TextAttr topaz8 = {
  (STRPTR)"topaz.font", 8, 0, 1
};


struct NewMenu menu1[] =
{
  { NM_TITLE, "Project",               0 , 0,                           0, 0},
  {  NM_ITEM, "Mode",                  0 , 0,                           0, 0},
  {   NM_SUB, "Simple",                0 , CHECKIT|MENUTOGGLE,         ~1, (void *) GD_MODE_SIMPLE},
  {   NM_SUB, "Scientific",            0 , CHECKIT|MENUTOGGLE|CHECKED, ~2, (void *) GD_MODE_SCIENTIFIC},
  {   NM_SUB, "Programmer's",          0 , CHECKIT|MENUTOGGLE,         ~4, (void *) GD_MODE_PROGRAMMERS},        
  {  NM_ITEM, NM_BARLABEL,             0 , 0,                           0, 0},
  {  NM_ITEM, "Quit...",              "Q", 0,                           0, (void *) GD_EXIT},
 
  { NM_TITLE, "Constants",             0 , 0,                           0, 0            },
  {  NM_ITEM, "c-Speed of light",      0 , 0,                           0, (void *) GD_CONSTANT_C},
  {  NM_ITEM, "G-Gravitational",       0 , 0,                           0, (void *) GD_CONSTANT_G},
  {  NM_ITEM, "k-Coulomb's constant",  0 , 0,                           0, (void *) GD_CONSTANT_K},
  {  NM_ITEM, "Seconds in a year",     0 , 0,                           0, (void *) GD_CONSTANT_SECONDS_IN_YEAR},
  { NM_END, NULL, 0 , 0, 0, 0}
};


#define BSTARTX (8)
#define BSTARTY (28)
#define BWIDTH  (26)
#define BHEIGHT (13)
#define BSWIDTH (39)
#define BSHEIGHT BHEIGHT
#define BHAIR   (4)
#define BVAIR   (BHAIR)

#define TOTALNUMOFGADGETS (31)

/* Data for gadget structures */
struct NewGadget Gadgetdata[] = {
  BSTARTX, 11, 256-2*BSTARTX, 13, NULL, &topaz8, GD_DISPLAY, PLACETEXT_LEFT, NULL, NULL, 

  /* first row */
  BSTARTX,                                          BSTARTY, BSWIDTH, BSHEIGHT, (UBYTE *)"sin", &topaz8, GD_SIN, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR),                          BSTARTY, BSWIDTH, BSHEIGHT, (UBYTE *)"cos", &topaz8, GD_COS, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR,                  BSTARTY, BWIDTH, BHEIGHT, (UBYTE *)"7", &topaz8, GD_N7, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR),   BSTARTY, BWIDTH, BHEIGHT, (UBYTE *)"8", &topaz8, GD_N8, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR)*2, BSTARTY, BWIDTH, BHEIGHT, (UBYTE *)"9", &topaz8, GD_N9, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*3, BSTARTY, BWIDTH, BHEIGHT, (UBYTE *)"+", &topaz8, GD_PLUS, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*4, BSTARTY, BWIDTH, BHEIGHT, (UBYTE *)"AC", &topaz8, GD_AC, PLACETEXT_IN, NULL,NULL,

  /* second row */
  BSTARTX,                                          BSTARTY+(BHEIGHT+BVAIR)  , BSWIDTH, BSHEIGHT, (UBYTE *)"tan", &topaz8, GD_TAN, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR),                          BSTARTY+(BHEIGHT+BVAIR)  , BSWIDTH, BSHEIGHT, (UBYTE *)"log", &topaz8, GD_LOG, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR,                  BSTARTY+(BHEIGHT+BVAIR)  , BWIDTH, BHEIGHT, (UBYTE *)"4", &topaz8, GD_N4, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR),   BSTARTY+(BHEIGHT+BVAIR)  , BWIDTH, BHEIGHT, (UBYTE *)"5", &topaz8, GD_N5, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR)*2, BSTARTY+(BHEIGHT+BVAIR)  , BWIDTH, BHEIGHT, (UBYTE *)"6", &topaz8, GD_N6, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*3, BSTARTY+(BHEIGHT+BVAIR)  , BWIDTH, BHEIGHT, (UBYTE *)"-", &topaz8, GD_MINUS, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*4, BSTARTY+(BHEIGHT+BVAIR)  , BWIDTH, BHEIGHT, (UBYTE *)"CE", &topaz8, GD_CE, PLACETEXT_IN, NULL,NULL,

  /* third row */
  BSTARTX,                                          BSTARTY+(BHEIGHT+BVAIR)*2, BSWIDTH, BSHEIGHT, (UBYTE *)"sqrt", &topaz8, GD_SQRT, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR),                          BSTARTY+(BHEIGHT+BVAIR)*2, BSWIDTH, BSHEIGHT, (UBYTE *)"ln", &topaz8, GD_LN, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR,                  BSTARTY+(BHEIGHT+BVAIR)*2, BWIDTH, BHEIGHT, (UBYTE *)"1", &topaz8, GD_N1, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR),   BSTARTY+(BHEIGHT+BVAIR)*2, BWIDTH, BHEIGHT, (UBYTE *)"2", &topaz8, GD_N2, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR)*2, BSTARTY+(BHEIGHT+BVAIR)*2, BWIDTH, BHEIGHT, (UBYTE *)"3", &topaz8, GD_N3, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*3, BSTARTY+(BHEIGHT+BVAIR)*2, BWIDTH, BHEIGHT, (UBYTE *)"*", &topaz8, GD_MULT, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*4, BSTARTY+(BHEIGHT+BVAIR)*2, BWIDTH, BHEIGHT, (UBYTE *)"pi", &topaz8, GD_PI, PLACETEXT_IN, NULL,NULL,

  /* fourth row */
  BSTARTX,                                          BSTARTY+(BHEIGHT+BVAIR)*3, BSWIDTH, BSHEIGHT, (UBYTE *)"1/x", &topaz8, GD_RECI, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR),                          BSTARTY+(BHEIGHT+BVAIR)*3, BSWIDTH, BSHEIGHT, (UBYTE *)"x^y", &topaz8, GD_POWER, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR,                  BSTARTY+(BHEIGHT+BVAIR)*3, BWIDTH, BHEIGHT, (UBYTE *)"0", &topaz8, GD_N0, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR),   BSTARTY+(BHEIGHT+BVAIR)*3, BWIDTH, BHEIGHT, (UBYTE *)".", &topaz8, GD_PERIOD, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR)*2, BSTARTY+(BHEIGHT+BVAIR)*3, BWIDTH, BHEIGHT, (UBYTE *)"=", &topaz8, GD_EQUAL, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*3, BSTARTY+(BHEIGHT+BVAIR)*3, BWIDTH, BHEIGHT, (UBYTE *)"/", &topaz8, GD_DIV, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*4, BSTARTY+(BHEIGHT+BVAIR)*3, BWIDTH, BHEIGHT, (UBYTE *)"e", &topaz8, GD_E, PLACETEXT_IN, NULL,NULL,

  /* fifth row */
  BSTARTX,                                          BSTARTY+(BHEIGHT+BVAIR)*4, BSWIDTH, BSHEIGHT, (UBYTE *)"x!", &topaz8, GD_FACTORIAL, PLACETEXT_IN, NULL,NULL,
  BSTARTX+(BSWIDTH+BHAIR),                          BSTARTY+(BHEIGHT+BVAIR)*4, BSWIDTH, BSHEIGHT, (UBYTE *)"x^2", &topaz8, GD_SQUARE, PLACETEXT_IN, NULL,NULL,
  //  BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR,                  BSTARTY+(BHEIGHT+BVAIR)*4, BWIDTH, BHEIGHT, (UBYTE *)"0", &topaz8, GD_N0, PLACETEXT_IN, NULL,NULL,
  //BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR),   BSTARTY+(BHEIGHT+BVAIR)*4, BWIDTH, BHEIGHT, (UBYTE *)".", &topaz8, GD_PERIOD, PLACETEXT_IN, NULL,NULL,
  //BSTARTX+(BSWIDTH+BHAIR)*2+BHAIR+(BWIDTH+BHAIR)*2, BSTARTY+(BHEIGHT+BVAIR)*4, BWIDTH, BHEIGHT, (UBYTE *)"=", &topaz8, GD_EQUAL, PLACETEXT_IN, NULL,NULL,
  //BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*3, BSTARTY+(BHEIGHT+BVAIR)*4, BWIDTH, BHEIGHT, (UBYTE *)"/", &topaz8, GD_DIV, PLACETEXT_IN, NULL,NULL,
  //BSTARTX+(BSWIDTH+BHAIR)*2+2*BHAIR+(BWIDTH+BHAIR)*4, BSTARTY+(BHEIGHT+BVAIR)*4, BWIDTH, BHEIGHT, (UBYTE *)"e", &topaz8, GD_E, PLACETEXT_IN, NULL,NULL,
};

/* Extra information for gadgets using Tags */
ULONG GadgetTags[] = {
  (GTST_MaxChars), 256, (STRINGA_Justification), (GACT_STRINGRIGHT), (TAG_DONE)
};

/* GLOBALS */
#define MAXLEN 80
char input[MAXLEN+1];
int len;
int hasinput;
int hasdecimal;
int power_state;

num base, factorSoFar, sumSoFar, res, operand;
int pendingMultiplicativeOperator;
int pendingAdditiveOperator;
int waitingForOperand;
struct Window *wp;
struct Gadget *display;
struct Gadget *gadgets[TOTALNUMOFGADGETS];
struct Gadget *glist = NULL;
struct Menu *menuStrip;

/* Prototyping */
void ReAddGadgets(void);
void Process(enum GdIds id);
enum GdIds HandleKey(char c);
void ClearEntry(void);
void ClearAll(void);
void EqualClicked(void);
void AdditiveOperator(enum GdIds id);
void MultiplicativeOperator(enum GdIds id);
void abortOperation(void);
void displayNum(num d);
int calculate(num rightOperand, enum GdIds pendingOperator);
void EventLoop(void);

int main(int argc, char **argv) {
  APTR visual;
  struct Screen *pubScreen;
  struct Gadget *gad1;
  int i;

#ifdef USEMPFR
  mpfr_init2(base, BITSACCURACY);
  mpfr_init2(factorSoFar, BITSACCURACY);
  mpfr_init2(sumSoFar, BITSACCURACY);
  mpfr_init2(res, BITSACCURACY);
  mpfr_init2(operand, BITSACCURACY);
#endif
  
  /* Lock screen and get visual info for gadtools */
  if (pubScreen = LockPubScreen(NULL)) {
    if (visual = GetVisualInfo(pubScreen, TAG_DONE)) {
      int adjusty = (pubScreen->WBorTop+pubScreen->Font->ta_YSize+1) - 9; /* Infer window top border height from screen's info */
      /* Create the gadget list */
      if (gad1 = CreateContext(&glist)) {
        /* Create gadgets specify gadget kind, a Gadget, NewGadget data and extra tag info */
        for (i=0; i < TOTALNUMOFGADGETS; i++) {
          Gadgetdata[i].ng_VisualInfo = visual;
          Gadgetdata[i].ng_TopEdge += adjusty;  /* adjust gadget's top accordingly to what screen says */
          if (gadgets[i] = gad1 = CreateGadgetA(
                  (i == 0) ? STRING_KIND : BUTTON_KIND,
                  gad1,
                  &Gadgetdata[i],
                  (i == 0) ? (struct TagItem *)&GadgetTags[i]: NULL)) {
            if (i == 0) {
              display = gad1;
            }
          } else {
            if (argc > 0) printf("Failed to create gadget %d.\n", i);
          }
        }
        /* Open window and specify gadget list (glist) */
        if (wp = (struct Window *)
            OpenWindowTags(NULL,
                           WA_Left, 10, WA_Top, 15,
                           WA_Width, 256, WA_Height, 96+17+adjusty,
                           WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_GADGETUP | IDCMP_VANILLAKEY | IDCMP_MENUPICK,
                           WA_Flags, WFLG_DRAGBAR    | WFLG_DEPTHGADGET |
                           WFLG_CLOSEGADGET | WFLG_ACTIVATE | WFLG_SMART_REFRESH,
                           WA_NewLookMenus, TRUE,
                           WA_Gadgets, glist,
                           WA_Title, "ACalc",
                           WA_PubScreenName, "Workbench",
                           TAG_DONE)) {
          if (menuStrip = CreateMenus(menu1, TAG_END)) {
            if (LayoutMenus(menuStrip, visual, GTMN_NewLookMenus, TRUE, TAG_END)) {
              /* Attach menu to window */
              if (SetMenuStrip(wp, menuStrip)) {
                GT_RefreshWindow(wp, NULL); /* Update window */
                ClearAll();
                GT_SetGadgetAttrs(display, wp, NULL, GTST_String, "0", TAG_END);
                EventLoop();
                ClearMenuStrip(wp);
              }
            }
            FreeMenus(menuStrip);
          }
          CloseWindow(wp);
        }
        FreeGadgets(glist);
      }
      FreeVisualInfo(visual);
    }
    UnlockPubScreen(NULL, pubScreen);
  }

#ifdef USEMPFR
  mpfr_free_cache();
  mpfr_clear(base);
  mpfr_clear(factorSoFar);
  mpfr_clear(sumSoFar);
  mpfr_clear(res);
  mpfr_clear(operand);
#endif
  return(0);
}

void ReAddGadgets(void) {
  struct Gadget *pgad;
  int i;

  if (glist) {
    RemoveGList(wp, glist, -1);
    FreeGadgets(glist);
  }

  pgad = CreateContext(&glist);  /* ContextData ! */

  for (i=0; i < TOTALNUMOFGADGETS; i++) {
    if (gadgets[i] = pgad = CreateGadgetA(
            (i == 0) ? STRING_KIND : BUTTON_KIND,
            pgad,
            &Gadgetdata[i],
            (i == 0) ? (struct TagItem *)&GadgetTags[i]: NULL)) {
      if (i == 0) {
        display = pgad;
      }
    } else {
      printf("Failed to create gadget %d.\n", i);
    }
  }
  AddGList(wp, glist, -1, -1, NULL);
  RefreshGList(glist, wp, NULL, -1);
  GT_RefreshWindow(wp, NULL);
}

void EventLoop(void) {
  ULONG signals;
  struct IntuiMessage *intuiMsg;
  ULONG                class;
  UWORD                icode;
  UWORD menuNumber, menuNum, itemNum, subNum;
  struct Gadget       *gad;
  int exit = FALSE;

  while (exit == FALSE) {
    signals = Wait(1 << wp->UserPort->mp_SigBit);
    while (intuiMsg = (struct IntuiMessage *)GT_GetIMsg(wp->UserPort)) {
      class = intuiMsg->Class;
      icode = intuiMsg->Code;

      if (class == IDCMP_GADGETUP) {
        gad = (struct Gadget*) intuiMsg->IAddress;
        Process((enum GdIds) gad->GadgetID);
      } else if (class == IDCMP_VANILLAKEY) {
        Process(HandleKey((char)icode));
      } else if (class == IDCMP_MENUPICK) { /* Menu selected? */
        struct MenuItem *item;
        ULONG ud;

        menuNumber = icode;
        while(menuNumber != MENUNULL) {
          /* Split code into menus, items, subitems */
          menuNum = MENUNUM(menuNumber);
          itemNum = ITEMNUM(menuNumber);
          subNum = SUBNUM(menuNumber);
          item = ItemAddress(menuStrip, menuNumber);
          /* Find userdata of menuitem */
          ud = (ULONG) GTMENUITEM_USERDATA(item);
          if (ud == GD_EXIT)
            exit = TRUE;
          else
            Process((enum GdIds) ud);
          #ifndef NDEBUG
          printf("Menu: %hu, item: %hu, subitem: %hu (UserData: %d)\n", menuNum, itemNum, subNum, ud);
          #endif
          menuNumber = item->NextSelect;
        }
      } else if (class == IDCMP_CLOSEWINDOW) {
        exit = TRUE;
      } else if (class == IDCMP_REFRESHWINDOW) {
        GT_BeginRefresh(wp);
        GT_EndRefresh(wp, TRUE);
      }
      GT_ReplyIMsg(intuiMsg);
    }
  }
}

enum GdIds HandleKey(char c)
{
  if (c >= '0' && c <= '9') return c-'0';
  if (c == '.' || c == ',') return GD_PERIOD;
  if (c == 8) return GD_BACKSPACE;
  if (c == '+') return GD_PLUS;
  if (c == '-') return GD_MINUS;
  if (c == '*') return GD_MULT;
  if (c == '/') return GD_DIV;
  if (c == '!') return GD_FACTORIAL;
  if (c == 13 || c == '=') return GD_EQUAL;  // enter/return is ='s shortcut
  if (c == 127) return GD_AC;          // Delete key is a clear all shortcut
  // printf("%d\n",(int)c );
}

void getNumFromInput(num *n, char *str) {
  #ifdef USEMPFR
  char *end;
  int ret = mpfr_strtofr(*n, str, &end, 10, MPFR_RNDN);
  // mpfr_out_str(stdout, 10, 0, *n, MPFR_RNDD);putchar('\n');
  #else
  *n = atof(str);
  #endif
}

void Process(enum GdIds id) {
  unsigned long int fac;

  switch (id) {
    case GD_N0:
    case GD_N1:
    case GD_N2:
    case GD_N3:
    case GD_N4:
    case GD_N5:
    case GD_N6:
    case GD_N7:
    case GD_N8:
    case GD_N9:
      if (id == GD_N0 && len == 1 && input[0] == '0')
        return;
      if (waitingForOperand) {
        ClearEntry();
        waitingForOperand = FALSE;
      }
      if (len < MAXLEN) {
        if (len == 1 && input[0] == '0')
          --len;
        input[len++] = id + '0';
        input[len]='\0';
        hasinput = TRUE;
        getNumFromInput(&res, input);
      }
      break;
    case GD_PERIOD:
      if (len < MAXLEN && !hasdecimal) {
        input[len++] = '.';
        input[len]='\0';
        hasinput = TRUE;
        hasdecimal = TRUE;
        GT_SetGadgetAttrs(display, wp, NULL, GTST_String, input, TAG_END);
        return;
      }
      break;
    case GD_BACKSPACE:
      if (waitingForOperand)
        return;
      if (len > 0) {
        if (input[len-1] == '.')
          hasdecimal = FALSE;
        input[--len]='\0';
        if (len == 0) {
          waitingForOperand = TRUE;
          hasinput = FALSE;
          ClearEntry();
        } else {
          getNumFromInput(&res, input);
        }
      }
      break;
    case GD_SQRT:
      #ifdef USEMPFR
      mpfr_sqrt(res, res, MPFR_RNDN);
      #else
      res = sqrt(res);
      #endif
      break;
    case GD_SIN:
      #ifdef USEMPFR
      mpfr_sin(res, res, MPFR_RNDN);
      #else
      res = sin(res);
      #endif
      //Gadgetdata[1].ng_GadgetText = "new";
      //ReAddGadgets();
      break;
    case GD_COS:
      #ifdef USEMPFR
      mpfr_cos(res, res, MPFR_RNDN);
      #else
      res = cos(res);
      #endif
      break;
    case GD_TAN:
      #ifdef USEMPFR
      mpfr_tan(res, res, MPFR_RNDN);
      #else
      res = tan(res);
      #endif
      break;
    case GD_LOG:
      #ifdef USEMPFR
      mpfr_log10(res, res, MPFR_RNDN);
      #else
      res = log10(res);
      #endif
      break;
    case GD_LN:
      #ifdef USEMPFR
      mpfr_log(res, res, MPFR_RNDN);
      #else
      res = log(res);
      #endif
      break;
    case GD_RECI:
      #ifdef USEMPFR
      mpfr_ui_div(res, 1, res, MPFR_RNDN);
      #else
      if (res != 0.0) {
        res = 1.0/res;
      }
      #endif
      break;
    case GD_POWER:
      if (power_state == 0) {
        #ifdef USEMPFR
        mpfr_set(base, res, MPFR_RNDN);
        #else
        base = res;
        #endif
        power_state = 1;
        ClearEntry();
      } else {
        #ifdef USEMPFR
        mpfr_pow(res, base, res, MPFR_RNDN);
        #else
        res = pow(base, res);
        #endif
        power_state = 0;
      }
      break;
    case GD_FACTORIAL:
      #ifdef USEMPFR
      fac = mpfr_get_ui(res, MPFR_RNDN);
      //printf("fac of %d\n", fac);
      mpfr_fac_ui(res, fac, MPFR_RNDN);
      #else
      if (res < 0) {
        res = sqrt(-1);
        break;
      }
      fac = (unsigned long int)res;
      if (fac > 170) {
        res = 1.0/0;
        break;
      }
      res = 1.0;
      if (fac == 0 || fac == 1)
        break;
      while (fac > 1) {
        res *=fac;
        --fac;
      }
      #endif
      break;
    case GD_SQUARE:
      #ifdef USEMPFR
      mpfr_mul(res, res, res, MPFR_RNDN);
      #else
      res *=res;
      #endif
      break;
    case GD_CE:
      ClearEntry();
      break;
    case GD_AC:
      ClearAll();
      break;
    case GD_PLUS:
    case GD_MINUS:
      AdditiveOperator(id);
      break;
    case GD_MULT:
    case GD_DIV:
      MultiplicativeOperator(id);
      break;
    case GD_EQUAL:
      EqualClicked();
      break;
    case GD_PI:
      #ifdef USEMPFR
      mpfr_const_pi(res, MPFR_RNDN);
      #else
      res = M_PI;
      #endif
      break;
    case GD_E:
      #ifdef USEMPFR
      mpfr_set_ui(res, 1, MPFR_RNDN);
      mpfr_exp(res, res, MPFR_RNDN);
      #else
      res = M_E;
      #endif
      break;
    case GD_CONSTANT_C:
      #ifdef USEMPFR
      mpfr_set_d(res, 299792458.0, MPFR_RNDN);
      #else
      res = 299792458.0;
      #endif
      break;
    case GD_CONSTANT_G:
      #ifdef USEMPFR
      mpfr_set_d(res, 6.67408e-11, MPFR_RNDN);
      #else
      res = 6.67408e-11;
      #endif
      break;
    case GD_CONSTANT_K:
      #ifdef USEMPFR
      mpfr_set_d(res, 8.9875517873681764e9, MPFR_RNDN);
      #else
      res = 8.9875517873681764e9;
      #endif
      break;
    case GD_CONSTANT_SECONDS_IN_YEAR:
      #ifdef USEMPFR
      mpfr_set_d(res, 31557600, MPFR_RNDN);
      #else
      res = 31557600;
      #endif
      break;
    default:
      #ifndef NDEBUG
      printf("Unknown id for case. %d\n", id);
      #else
      ;
      #endif
  }
  // printf("id: %d input: %s (%f)\n", id, input,atof(input));
  displayNum(res);
  GT_SetGadgetAttrs(display, wp, NULL, GTST_String, input, TAG_END);
}

void ClearEntry(void) {
  len = 1;
  input[len-1] = '0';
  input[len] = '\0';
  #ifdef USEMPFR
  mpfr_set_ui(res, 0, MPFR_RNDN);
  #else
  res = 0.0;
  #endif
}


void displayNum(num d) {
  #ifdef USEMPFR
  // mpfr_out_str(stdout, 10, 0, d, MPFR_RNDD); putchar('\n');
  int ret = mpfr_sprintf(input, "%.20Rg", d);
  // printf("input len = %d \"%s\" ret=%d\n", strlen(input), input, ret);
  #else
  /* commented out, doesn't work :( */
  /* int inf = isinf(d); */
  /* if ( inf == -1 ) { */
  /*   strcpy(input, "-inf"); */
  /* } else if ( inf == 1) { */
  /*   strcpy(input, "inf"); */
  /* } else  */
  sprintf(input, "%.15g",d);
  // 15 is the DBL_DIG in machine/float.h (gcc 3.4.0)
  #endif
}

void ClearAll(void) {
  ClearEntry();
  power_state = hasdecimal = hasinput = FALSE;
  pendingAdditiveOperator = 0;
  pendingMultiplicativeOperator = 0;
  waitingForOperand = TRUE;
  #ifdef USEMPFR
  mpfr_set_ui(base, 0, MPFR_RNDN);
  mpfr_set_ui(sumSoFar, 0, MPFR_RNDN);
  mpfr_set_ui(factorSoFar, 0, MPFR_RNDN);
  mpfr_set_ui(res, 0, MPFR_RNDN);
  #else
  res = base = sumSoFar = factorSoFar = 0.0;
  #endif
}

void AdditiveOperator(enum GdIds id) {
  #ifdef USEMPFR
  mpfr_set(operand, res, MPFR_RNDN);
  #else
  operand = res;
  #endif

  if (pendingMultiplicativeOperator) {
    if (!calculate(operand, pendingMultiplicativeOperator)) {
      abortOperation();
      return;
    }
    displayNum(factorSoFar);
    #ifdef USEMPFR
    mpfr_set(operand, factorSoFar, MPFR_RNDN);
    mpfr_set_ui(factorSoFar, 0, MPFR_RNDN);
    #else
    operand = factorSoFar;
    factorSoFar = 0.0;
    #endif
    pendingMultiplicativeOperator = 0;
  }

  if (pendingAdditiveOperator) {
    if (!calculate(operand, pendingAdditiveOperator)) {
      abortOperation();
      return;
    }
    #ifdef USEMPFR
    mpfr_set(res, sumSoFar, MPFR_RNDN);
    #else
    res = sumSoFar;
    #endif
  } else {
    #ifdef USEMPFR
    mpfr_set(sumSoFar, operand, MPFR_RNDN);
    #else
    sumSoFar = operand;
    #endif
  }
  pendingAdditiveOperator = id;
  waitingForOperand = TRUE;
}

void MultiplicativeOperator(enum GdIds id) {

  #ifdef USEMPFR
  mpfr_set(operand, res, MPFR_RNDN);
  #else
  operand = res;
  #endif

  if (pendingMultiplicativeOperator) {
    if (!calculate(operand, pendingMultiplicativeOperator)) {
      abortOperation();
      return;
    }
    #ifdef USEMPFR
    mpfr_set(res, factorSoFar, MPFR_RNDN);
    #else
    res = factorSoFar;
    #endif
  } else {
    #ifdef USEMPFR
    mpfr_set(factorSoFar, operand, MPFR_RNDN);
    #else
    factorSoFar = operand;
    #endif
  }

  pendingMultiplicativeOperator = id;
  waitingForOperand = TRUE;
}

void EqualClicked(void) {
  #ifdef USEMPFR
  mpfr_set(operand, res, MPFR_RNDN);
  #else
  operand = res;
  #endif

  if (pendingMultiplicativeOperator) {
    if (!calculate(operand, pendingMultiplicativeOperator)) {
      abortOperation();
      return;
    }
    #ifdef USEMPFR
    mpfr_set(operand, factorSoFar, MPFR_RNDN);
    mpfr_set_ui(factorSoFar, 0, MPFR_RNDN);
    #else
    operand = factorSoFar;
    factorSoFar = 0.0;
    #endif
    pendingMultiplicativeOperator = 0;
  }

  if (pendingAdditiveOperator) {
    if (!calculate(operand, pendingAdditiveOperator)) {
      abortOperation();
      return;
    }
    pendingAdditiveOperator = 0;
  } else {
    #ifdef USEMPFR
    mpfr_set(sumSoFar, operand, MPFR_RNDN);
    #else
    sumSoFar = operand;
    #endif
  }

  #ifdef USEMPFR
  mpfr_set(res, sumSoFar, MPFR_RNDN);
  mpfr_set_ui(sumSoFar, 0, MPFR_RNDN);
  #else
  res = sumSoFar;
  sumSoFar = 0;
  #endif
  waitingForOperand = TRUE;
}

void abortOperation(void) {
  ClearAll();
  strcpy(input, "####");
}

int calculate(num rightOperand, enum GdIds pendingOperator) {
  if (pendingOperator == GD_PLUS) {
    #ifdef USEMPFR
    mpfr_add(sumSoFar, sumSoFar, rightOperand, MPFR_RNDN);
    #else
    sumSoFar += rightOperand;
    #endif
  } else if (pendingOperator == GD_MINUS) {
    #ifdef USEMPFR
    mpfr_sub(sumSoFar, sumSoFar, rightOperand, MPFR_RNDN);
    #else
    sumSoFar -= rightOperand;
    #endif
  } else if (pendingOperator == GD_MULT) {
    #ifdef USEMPFR
    mpfr_mul(factorSoFar, factorSoFar, rightOperand, MPFR_RNDN);
    #else
    factorSoFar *= rightOperand;
    #endif
  } else if (pendingOperator == GD_DIV) {
    #ifdef USEMPFR
    mpfr_div(factorSoFar, factorSoFar, rightOperand, MPFR_RNDN);
    #else
    if (rightOperand == 0.0)
      return FALSE;
    factorSoFar /= rightOperand;
    #endif
  }
  return TRUE;
}
