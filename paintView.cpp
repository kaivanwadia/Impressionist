//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintView.h"
#include "impBrush.h"
#include <cmath>

extern float frand();

#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define CURSOR_MOVE			7
#define DRAW_AUTOMATICALLY	8


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	m_bDrawAutomatically = false;
}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();
	}

	if (m_bDrawAutomatically)
	{
		isAnEvent = 0;
		Point source(0, 0);
		Point target(0, 0);
		int count = 0;
		m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
		for (int row = 0; row < m_pDoc->m_nPaintHeight; row++)
		{
			for (int column = 0; column < m_pDoc->m_nPaintWidth; column++)
			{
				if (count != 0)
				{
					count--;
					continue;
				}
				if (m_pDoc->getRandomSize())
				{
					int size = frand() * 39 + 1;
					m_pDoc->setSize(size);
				}
				if (m_pDoc->getRandomAngle())
				{
					int angle = frand() * 359 + 1;
					m_pDoc->setAngle(angle);
				}
				count = m_pDoc->getSpacing();
				source.x = column;
				source.y = row;
				target.x = column;
				target.y = row;
				m_pDoc->m_pCurrentBrush->BrushMove(source, target);
			}
		}
		m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
		m_bDrawAutomatically = false;
		SaveCurrentContent();
		RestoreContent();
	}
	else if ( m_pDoc->m_ucPainting && isAnEvent)
	{
		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			if (m_pDoc->getAngleType() == RIGHT_MOUSE_BUTTON)
			{
				m_rightClickOrigin = target;
			}
			break;
		case RIGHT_MOUSE_DRAG:
			if (m_pDoc->getAngleType() == RIGHT_MOUSE_BUTTON)
			{
				RestoreContent();
				glBegin(GL_LINES);
				glColor3f(1.0f, 0, 0);
				glVertex2f((float)m_rightClickOrigin.x, (float)m_rightClickOrigin.y);
				glVertex2f((float)target.x, (float)target.y);
				glEnd();
			}
			break;
		case RIGHT_MOUSE_UP:
			if (m_pDoc->getAngleType() == RIGHT_MOUSE_BUTTON)
			{
				RestoreContent();
				Point distance = m_rightClickOrigin;
				distance.x -= target.x;
				distance.y -= target.y;
				int size = (int)sqrt(distance.x*distance.x + distance.y*distance.y);
				m_pDoc->setSize(size);
				int lineAngle = atan2(target.y - m_rightClickOrigin.y, target.x - m_rightClickOrigin.x) * 180 / M_PI;
				if (lineAngle < 0)
				{
					lineAngle = lineAngle + 360;
				}
				m_pDoc->setAngle(lineAngle);
			}
			break;
		default:
			printf("Unknown event!!\n");
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA
}

void PaintView::drawAutomatically()
{
	isAnEvent = 1;
	m_bDrawAutomatically = true;
	redraw();
}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		m_pDoc->m_pUI->m_origView->drawRedDotAtPoint(Point(coord.x, m_nEndRow - coord.y));
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		m_pDoc->m_pUI->m_origView->drawRedDotAtPoint(Point(coord.x, m_nEndRow - coord.y));
		break;
	default:
		return 0;
		break;

	}
	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

