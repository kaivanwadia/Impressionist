#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>

// Each brush type has an associated constant.
enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES,
	BRUSH_CIRCLES,
	BRUSH_SCATTERED_POINTS,
	BRUSH_SCATTERED_LINES,
	BRUSH_SCATTERED_CIRCLES,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
};

// Each angle type has an associated constant
enum
{
	SLIDER_VALUE = 0,
	RIGHT_MOUSE_BUTTON,
	CURSOR_MOVEMENT,
	NUM_ANGLE_TYPE
};

// Each filter type has an associated constant
enum
{
	MEAN_FILTER = 0,
	GAUSSIAN_FILTER,
	LAPLACIAN_FILTER,
	ORIGINAL_FILTER,
	NUM_FILTER_TYPE
};

// Each sobel filter type has an associated constant
enum
{
	SOBEL_HORIZONTAL = 0,
	SOBEL_VERITCAL,
	SOBEL_COMBINED,
	NUM_SOBEL_FILTER
};

// Each filter size type has an associated constant
enum
{
	SIZE_3 = 0,
	SIZE_5,
	SIZE_7,
	SIZE_9,
	NUM_SIZE_TYPE
};

class ImpressionistDoc; // Pre-declaring class

class Point 
{
public:
	Point() {};
	Point(int xx, int yy) { x = xx; y = yy; };

	int x, y;
};

class ImpBrush 
{
protected:
	ImpBrush::ImpBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );

public:
	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin( const Point source, const Point target ) = 0;
	virtual void BrushMove( const Point source, const Point target ) = 0;
	virtual void BrushEnd( const Point source, const Point target ) = 0;

	// according to the source image and the position, determine the draw color
	void SetColor( const Point source );

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument( void );

	// Return the name of the brush (not used in this version).
	const char* BrushName( void );
	
	static int			c_nBrushCount;	// How many brushes we have,
	static ImpBrush**	c_pBrushes;		// and what they are.

private:
	ImpressionistDoc*	m_pDoc;

	// Brush's name (not used in this version).
	const char*			m_pBrushName;
};

#endif

