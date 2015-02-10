//
// filledCircleBrush.h
//
// The header file for Point Brush. 
//

#ifndef FILLEDCIRCLEBRUSH_H
#define FILLEDCIRCLEBRUSH_H

#include "impBrush.h"

class FilledCircleBrush : public ImpBrush
{
public:
	FilledCircleBrush(ImpressionistDoc* pDoc = NULL, const char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif
