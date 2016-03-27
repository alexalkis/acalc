// -*- compile-command: "m68k-amigaos-gcc -s -noixemul -Os -o acalc acalc.c -lm" -*-

/*
**  A small calculator.  (I only wanted to check out gadtools! Sorry :P)
*/
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <intuition/intuition.h>
#include <libraries/gadtools.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

  GD_BACKSPACE,

  GD_CONSTANT_C,
  GD_CONSTANT_G,
  GD_CONSTANT_K,

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
double base;
double factorSoFar, sumSoFar;
int pendingMultiplicativeOperator;
int pendingAdditiveOperator;
int waitingForOperand;
struct Window *wp;
struct Gadget *display;
struct Gadget *gadgets[29];
struct Menu *menuStrip;

/* Prototyping */
void Process(enum GdIds id);
enum GdIds HandleKey(char c);
void ClearEntry(void);
void ClearAll(void);
void EqualClicked(void);
void AdditiveOperator(enum GdIds id);
void MultiplicativeOperator(enum GdIds id);
void abortOperation(void);
void displayNum(double d);
int calculate(double rightOperand, enum GdIds pendingOperator);
void EventLoop(void);

int main(int argc, char **argv) {
  APTR visual;
  struct Screen *pubScreen;
  struct Gadget *glist = NULL, *gad1;
  int i;

  /* Lock screen and get visual info for gadtools */
  if (pubScreen = LockPubScreen(NULL)) {
    if (visual = GetVisualInfo(pubScreen, TAG_DONE)) {
      /* Create the gadget list */
      if (gad1 = CreateContext(&glist)) {
        /* Create gadgets specify gadget kind, a Gadget, NewGadget data and extra tag info */
        for (i=0; i < 29; i++) {
          Gadgetdata[i].ng_VisualInfo = visual;
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
        if (wp = (struct Window *)  OpenWindowTags(NULL,
                                                   WA_Left, 10, WA_Top, 15,
                                                   WA_Width, 256, WA_Height, 96,
                                                   WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_GADGETUP | IDCMP_VANILLAKEY | IDCMP_MENUPICK,
                                                   WA_Flags, WFLG_DRAGBAR    | WFLG_DEPTHGADGET |
                                                   WFLG_CLOSEGADGET | WFLG_ACTIVATE | WFLG_SMART_REFRESH,
                                                   WA_NewLookMenus, TRUE,
                                                   WA_Gadgets, glist,
                                                   WA_Title, "ACalc",
                                                   WA_PubScreenName, "Workbench",
                                                   TAG_DONE)) {
          if (menuStrip = CreateMenus(menu1,TAG_END)) {
            if (LayoutMenus(menuStrip, visual, GTMN_NewLookMenus, TRUE, TAG_END)) {
              /* Attach menu to window */
              if (SetMenuStrip (wp, menuStrip)) {
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
  return(0);
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
        menuNum = MENUNUM(menuNumber);  /* Split code into menus, items, subitems */
        if (menuNum != 31) {
          itemNum = ITEMNUM(menuNumber);
          subNum = SUBNUM(menuNumber);

          item = ItemAddress( menuStrip, menuNumber );

          /* Find userdata of menuitem */
          ud = (ULONG) GTMENUITEM_USERDATA( item );

          if (ud == GD_EXIT)
            exit = TRUE;
          else
            Process((enum GdIds) ud);
          //printf("Menu: %hu, item: %hu, subitem: %hu (UserData: %d)\n", menuNum, itemNum, subNum, ud);
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
  if (c == 13 || c == '=') return GD_EQUAL;  // enter/return is ='s shortcut
  if (c == 127) return GD_AC;                // Delete key is a clear all shortcut
  //printf("%d\n",(int)c );
}


void Process(enum GdIds id) {
  double res;

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
      }
      break;
    case GD_PERIOD:
      if (len < MAXLEN && !hasdecimal) {
        input[len++] = '.';
        input[len]='\0';
        hasinput = TRUE;
        hasdecimal = TRUE;
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
        }
      }
      break;
    case GD_SQRT:
      res = sqrt(atof(input));
      displayNum(res);
      break;
    case GD_SIN:
      res = sin(atof(input));
      displayNum(res);
      //      gadgets[1]->GadgetText->IText="Ok";
      //      GT_SetGadgetAttrs(gadgets[1], wp, NULL, GTTX_Text ,"Ok", TAG_END);
      break;
    case GD_COS:
      res = cos(atof(input));
      displayNum(res);
      break;
    case GD_TAN:
      res = tan(atof(input));
      displayNum(res);
      break;
    case GD_LOG:
      res = log10(atof(input));
      displayNum(res);
      break;
    case GD_LN:
      res = log(atof(input));
      displayNum(res);
      break;
    case GD_RECI:
      res = atof(input);
      if (res != 0.0) {
        res = 1.0/res;
        displayNum(res);
      }
      break;
    case GD_POWER:
      res = atof(input);
      if (power_state == 0) {
        base = res;
        power_state = 1;
        ClearEntry();
      } else {
        res = pow(base, res);
        power_state = 0;
        displayNum(res);
      }
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
      res = M_PI;
      displayNum(res);
      break;
    case GD_E:
      res = M_E;
      displayNum(res);
      break;
    case GD_CONSTANT_C:
      res = 299792458.0;
      displayNum(res);
      break;
    case GD_CONSTANT_G:
      res = 6.67408e-11;
      displayNum(res);
      break;
    case GD_CONSTANT_K:
      res = 8.9875517873681764e9;
      displayNum(res);
      break;
    default:
      printf("Unknown id for case. %d\n",id);
  }
  //printf("id: %d input: %s (%f)\n", id, input,atof(input));
  GT_SetGadgetAttrs(display, wp, NULL, GTST_String, input, TAG_END);
}

void ClearEntry(void) {
  len = 1;
  input[len-1] = '0';
  input[len] = '\0';
}


void displayNum(double d) {
  /* commented out, doesn't work :( */
  /* int inf = isinf(d); */
  /* if ( inf == -1 ) { */
  /*   strcpy(input, "-inf"); */
  /* } else if ( inf == 1) { */
  /*   strcpy(input, "inf"); */
  /* } else  */
  sprintf(input, "%.15g",d);  // 15 is the DBL_DIG in machine/float.h (gcc 3.4.0)
}

void ClearAll(void) {
  ClearEntry();
  power_state = hasdecimal = hasinput = FALSE;
  pendingAdditiveOperator = 0;
  pendingMultiplicativeOperator = 0;
  waitingForOperand = TRUE;
  sumSoFar = factorSoFar = 0.0;
}

void AdditiveOperator(enum GdIds id) {
  double operand = atof(input);
  if (pendingMultiplicativeOperator) {
    if (!calculate(operand, pendingMultiplicativeOperator)) {
      abortOperation();
      return;
    }
    displayNum(factorSoFar);
    operand = factorSoFar;
    factorSoFar = 0.0;
    pendingMultiplicativeOperator = 0;
  }

  if (pendingAdditiveOperator) {
    if (!calculate(operand, pendingAdditiveOperator)) {
      abortOperation();
      return;
    }
    displayNum(sumSoFar);
  } else {
    sumSoFar = operand;
  }
  pendingAdditiveOperator = id;
  waitingForOperand = TRUE;
}

void MultiplicativeOperator(enum GdIds id) {
  double operand = atof(input);
  if (pendingMultiplicativeOperator) {
    if (!calculate(operand, pendingMultiplicativeOperator)) {
      abortOperation();
      return;
    }
    displayNum(factorSoFar);
  } else {
    factorSoFar = operand;
  }

  pendingMultiplicativeOperator = id;
  waitingForOperand = TRUE;
}

void EqualClicked(void) {
  double operand = atof(input);

  if (pendingMultiplicativeOperator) {
    if (!calculate(operand, pendingMultiplicativeOperator)) {
      abortOperation();
      return;
    }
    operand = factorSoFar;
    factorSoFar = 0.0;
    pendingMultiplicativeOperator = 0;
  }

  if (pendingAdditiveOperator) {
    if (!calculate(operand, pendingAdditiveOperator)) {
      abortOperation();
      return;
    }
    pendingAdditiveOperator = 0;
  } else {
    sumSoFar = operand;
  }

  displayNum(sumSoFar);
  sumSoFar = 0;
  waitingForOperand = TRUE;
}

void abortOperation(void) {
  ClearAll();
  strcpy(input, "####");
}

int calculate(double rightOperand, enum GdIds pendingOperator) {
  if (pendingOperator == GD_PLUS) {
    sumSoFar += rightOperand;
  } else if (pendingOperator == GD_MINUS) {
    sumSoFar -= rightOperand;
  } else if (pendingOperator == GD_MULT) {
    factorSoFar *= rightOperand;
  } else if (pendingOperator == GD_DIV) {
    if (rightOperand == 0.0)
      return FALSE;
    factorSoFar /= rightOperand;
  }
  return TRUE;
}
