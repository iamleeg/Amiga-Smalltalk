#include "Interpreter_Error.h"

#include <proto/intuition.h>

#include <stdlib.h>

void Interpreter_error(char *message)
{
  struct IntuiText body, postext, negtext;
  BOOL response;

  body.IText = message;
  body.FrontPen = 0;
  body.BackPen = 1;
  body.DrawMode = JAM1;
  body.LeftEdge = 0;
  body.TopEdge = 0;
  body.ITextFont = NULL;
  body.NextText = NULL;

  postext.IText = "Continue";
  postext.FrontPen = 0;
  postext.BackPen = 1;
  postext.DrawMode = JAM1;
  postext.LeftEdge = 0;
  postext.TopEdge = 0;
  postext.ITextFont = NULL;
  postext.NextText = NULL;
  
  negtext.IText = "Stop";
  negtext.FrontPen = 0;
  negtext.BackPen = 1;
  negtext.DrawMode = JAM1;
  negtext.LeftEdge = 0;
  negtext.TopEdge = 0;
  negtext.ITextFont = NULL;
  negtext.NextText = NULL;

  response = AutoRequest(NULL, &body, &postext, &negtext, 0, 0, 200, 80);
  if (response == FALSE) {
    exit(-1);
  }
}
