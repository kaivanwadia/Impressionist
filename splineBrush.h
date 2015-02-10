//
// splineBrush.h
//
// The header file for Spline Brush. 
//

#ifndef SPLINEBRUSH_H
#define SPLINEBRUSH_H

#include "impBrush.h"

class SplineBrush : public ImpBrush
{
public:
	SplineBrush(ImpressionistDoc* pDoc = NULL, const char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif
