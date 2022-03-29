#include <vcl.h>

#pragma hdrstop

#include <math.h>

#include "Unit1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#pragma resource "*.dfm"

TForm1 *Form1;

//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)

: TForm(Owner)

{

}

int i=0, x1=0, y1=0, x0=0, y0=0, sx=0, sy=0;

float x=0, y=0, dx=0, dy=0, l=0;

//---------------------------------------------------------------------------

void __fastcall TForm1::gridClick(TObject *Sender)

{ Canvas->Pen->Color = (TColor)RGB(255,255,255); // заливаем фон

Canvas->Brush->Color = (TColor)RGB(255,255,255);

Canvas->Rectangle(0,0,800,800);

x0=0, y0=0, x=800; y=600;

Canvas->Pen->Width=1; //ширина сетки

Canvas->Pen->Color=(TColor)RGB(0,0,0); //цвет сетки

while (x0 <= x) {Canvas->MoveTo(x0,0); Canvas->LineTo(x0,600); x0+=15; }

while (y0 <= y) {Canvas->MoveTo(0,y0); Canvas->LineTo(800,y0); y0+=15; }

Canvas->Pen->Width=3;

Canvas->MoveTo(405,0); Canvas->LineTo(405,800); //оси координат

Canvas->MoveTo(0,300); Canvas->LineTo(800,300);

}

//---------------------------------------------------------------------------

void __fastcall TForm1::cdaClick(TObject *Sender)

{try{x0=StrToInt(xx1->Text); y0=StrToInt(yy1->Text);

x1=StrToInt(xx2->Text); y1=StrToInt(yy2->Text);}

catch (EConvertError &e){

ShowMessage("Проверьте правильность заполнения полей ввода");//ошибка при вводе в edit не числа

return;}

Canvas->Pen->Width=0;

Canvas->Pen->Color=(TColor)RGB(0,0,0);

Canvas->Brush->Color = (TColor)RGB(3,225,78);

x=abs(x1-x0); y=abs(y1-y0); //начальные координаты

if(x>=y)l=x; else l=y; //определение l

{try{dx=(x1-x0)/l; dy=(y1-y0)/l; //приращения

}

catch (EInvalidOp &e){

ShowMessage("Длина не может быть равно 0");//ошибка при вводе в edit не числа

return;}

if(dx>0) sx=1; else sx=-1; //знаки приращений

if(dy>0) sy=1; else sy=-1;

x=x0+0.5*sx; y=y0+0.5*sy;

for(i=0; i!=l; i++){ //рисуем пикселы

Canvas->Rectangle(floor(x)*15+405,-floor(y)*15+285,floor(x)*15+421,-floor(y)*15+301);

x=x+dx; y=y+dy;}

}}

//---------------------------------------------------------------------------

void __fastcall TForm1::paintClick(TObject *Sender)

{

{try{x0=StrToInt(xx1->Text); y0=StrToInt(yy1->Text);

x1=StrToInt(xx2->Text); y1=StrToInt(yy2->Text);}

catch (EConvertError &e){

ShowMessage("Проверьте правильность заполнения полей ввода");//ошибка при вводе в edit не числа

return;}

Canvas->Pen->Width=1;

Canvas->Pen->Color=(TColor)RGB(255,0,0);

Canvas->MoveTo(x0*15+405,-y0*15+300); Canvas->LineTo(x1*15+405,-y1*15+300);

}

}

//---------------------------------------------------------------------------

void __fastcall TForm1::brezClick(TObject *Sender)

{

{try{x0=StrToInt(xx1->Text); y0=StrToInt(yy1->Text);

x1=StrToInt(xx2->Text); y1=StrToInt(yy2->Text);}

catch (EConvertError &e){

ShowMessage("Проверьте правильность заполнения полей ввода");//ошибка при вводе в edit не числа

return;}

int a=0, b=0, e=0;

Canvas->Pen->Width=0;

Canvas->Pen->Color=(TColor)RGB(0,0,0);

Canvas->Brush->Color = (TColor)RGB(0,153,225);

x=x0; y=y0; //начальные координаты

dx=abs(x1-x0); dy=abs(y1-y0);

if((x1-x0)>=0) sx=1; else sx=-1;

if((y1-y0)>=0) sy=1; else sy=-1;

if(dy>dx){a=dx; dx=dy; dy=a; b=1;} else b=0;

e=2*dy-dx;

for(i=0; i!=dx; i++){

Canvas->Rectangle(x*15+405,-y*15+285,x*15+421,-y*15+301);

while(e>=0){

if(b==1)x=x+sx; else y=y+sy;

e=e-2*dx;}

if(b==1) y=y+sy; else x=x+sx;

e=e+2*dy;}

}

}

//---------------------------------------------------------------------------
