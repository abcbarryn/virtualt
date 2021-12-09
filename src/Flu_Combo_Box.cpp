// $Id: Flu_Combo_Box.cpp,v 1.2 2013/01/22 22:29:01 kpettit1 Exp $

/***************************************************************
 *                FLU - FLTK Utility Widgets 
 *  Copyright (C) 2002 Ohio Supercomputer Center, Ohio State University
 *
 * This file and its content is protected by a software license.
 * You should have received a copy of this license with this file.
 * If not, please contact the Ohio Supercomputer Center immediately:
 * Attn: Jason Bryan Re: FLU 1224 Kinnear Rd, Columbus, Ohio 43212
 * 
 ***************************************************************/



#include <stdio.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <string.h>
#include <stdlib.h>
#include <FL/math.h>

#include "FLU/Flu_Combo_Box.h"

Flu_Combo_Box :: Flu_Combo_Box( int X, int Y, int W, int H, const char* l )
  : Fl_Group( X, Y, W, H, l ), input( X+2, Y+2, W-20-1-2, H-4 )
{
  box( FL_DOWN_BOX );
  align( FL_ALIGN_LEFT );
  pop_height( 250 );

  _cbox = NULL;
  _valbox = FL_UP_BOX;

  input_callback( NULL );
  input.box( FL_FLAT_BOX );
  input.callback( input_cb, this );
  input.when( FL_WHEN_ENTER_KEY_ALWAYS );
  input.color( FL_WHITE, selection_color());
  input.textfont( FL_HELVETICA );
  input.textsize(  FL_NORMAL_SIZE );
  input.textcolor( FL_FOREGROUND_COLOR );

  editable( true );

  end();
}

Flu_Combo_Box::~Flu_Combo_Box()
{
}

void Flu_Combo_Box :: set_combo_widget( Fl_Widget *w )
{
  _cbox = w;
  this->add( w );
}

void Flu_Combo_Box :: input_cb( Fl_Widget*, void* v )
{
  // taken from Fl_Counter.cxx
  Flu_Combo_Box& t = *(Flu_Combo_Box*)v;

  if( strcmp( t.input.value(), t.value() )!=0 || t.input.when() & FL_WHEN_NOT_CHANGED)
    {
      if( t.when() )
	{
	  t.clear_changed();
	  if( t._inputCB )
	    t._inputCB( &t, t._inputCBD );
	  else
	    t.do_callback();
	}
      else
	{
	  t.set_changed();
	}
    }
}

void Flu_Combo_Box :: resize( int X, int Y, int W, int H )
{
  Fl_Group::resize( X, Y, W, H );
  input.resize( X+2, Y+2, W-20-1-2, H-4 );
}

void Flu_Combo_Box :: draw()
{
  int W = 18, H = h()-4;
  int X = x()+w()-W-2, Y = y()+2;

  fl_draw_box( box(), x(), y(), w(), h(), color() );

  // draw the arrow button
  fl_draw_box( (Fl_Boxtype)_valbox, X, Y, W, H, color() );
  fl_color( active_r() ? FL_FOREGROUND_COLOR : fl_inactive(FL_FOREGROUND_COLOR) );
  fl_polygon( X+W/2-4, Y+H/2-2, X+W/2+4, Y+H/2-2, X+W/2, Y+H/2+2 );

  draw_child( input );
  if (Fl::focus() == this) draw_focus();
}

Flu_Combo_Box::Popup :: Popup( Flu_Combo_Box *b, Fl_Widget *c, int H )
  : Fl_Double_Window( b->window()->x()+b->x()-2, b->window()->y()+b->y()+b->h()-2, b->w()+4, H, 0 )
{
  combo = b;
  dragging = false;
  selected = NULL;

  box( FL_BORDER_FRAME );
  border( 0 );
  add( c );
  end();
  //set_non_modal();
  set_modal();

  c->resize( 1, 1, w()-2, h()-2 );
}

Flu_Combo_Box::Popup :: ~Popup()
{
  while( children() )
    remove( child(0) );
}

void Flu_Combo_Box :: value( const char *v )
{
  if( _value( v ) )
    input.value( v );
}

void Flu_Combo_Box :: selected( const char *v )
{
  if( v )
    input.value( v );
  _popped = false;
  do_callback();
}

void Flu_Combo_Box :: selectall(void)
{
	const char *v = input.value();
	if (v)
		input.position(0, strlen(v));
	input.redraw();
}

int Flu_Combo_Box::Popup :: handle( int event )
{
  // FL_MOVE is generated while window is moving
  // this attempts to keep the popup window moving with the enclosing window
  if( event == FL_MOVE )
    position( combo->window()->x()+combo->x()-2, combo->window()->y()+combo->y()+combo->h()-2 );

  if( event == FL_DRAG )
    dragging = true;

  // if push outside the popup window, popdown
  if( event == FL_PUSH &&
      !Fl::event_inside( child(0) ) )
    {
      combo->_popped = false;
      return 0;
    }

  // if release after dragging outside the popup window, popdown
  if( event == FL_RELEASE && dragging && 
      !Fl::event_inside( child(0) ) )
    {
      combo->_popped = false;
      return 0;
    }

  if( event == FL_KEYDOWN )
    {
      if( Fl::event_key( FL_Escape ) )
	{
	  combo->_popped = false;
	  return 0;
	}
      else if( Fl::event_key( FL_Up ) )
	{
	  selected = combo->_previous();
	  return 1;
	}
      else if( Fl::event_key( FL_Down ) )
	{
	  selected = combo->_next();
	  return 1;
	}
      else if( Fl::event_key( FL_Enter ) || Fl::event_key( ' ' ) )
	{
	  if( selected )
	    {
	      combo->value( selected );
	      combo->selected( selected );
	    }
	  combo->_popped = false;
	  return 1;	  
	}
    }

  return Fl_Double_Window::handle( event );
}

int Flu_Combo_Box :: handle( int event )
{
  // is it time to popup?
  bool open = ( event == FL_PUSH ) && 
    (!Fl::event_inside( &input ) || ( !editable() && Fl::event_inside( &input ) ) );
  open |= ( event == FL_KEYDOWN ) && Fl::event_key( ' ' );

  if( open )
    {
      fl_cursor( FL_CURSOR_DEFAULT );

      _valbox = FL_THIN_DOWN_BOX;
      redraw();

      // remember old current group
      Fl_Group *c = Fl_Group::current();

      // set current group to 0 so this is a top level popup window
      Fl_Group::current( 0 );
      Popup *_popup = new Popup( this, _cbox, popHeight );

      // show it and make FLTK send all events there
      value( value() );
      _popup->show();
      Fl::grab( *_popup );
      Fl::focus( _cbox );
      _popped = true;
      Fl::pushed( _cbox );

      // wait for a selection to be made
      while( _popped )
	Fl::wait();

      // restore things and delete the popup
      _popup->hide();
      Fl::grab( 0 );
      delete _popup;
      Fl_Group::current( c );
      Fl::focus( this );

      _valbox = FL_UP_BOX;
      redraw();

      return 1;
    }

  if( input.handle(event) )
    {
      if( !editable() && ( event == FL_ENTER || event == FL_LEAVE ) )
	fl_cursor( FL_CURSOR_DEFAULT );
      return 1;
    }
  else
    return 0;
}
