//
// singleLineBrush.cpp
//
// The implementation of Single Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "singleLineBrush.h"
#include <cmath>

extern float frand();

SingleLineBrush::SingleLineBrush(ImpressionistDoc* pDoc, const char* name) :
ImpBrush(pDoc, name)
{
}

void SingleLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	pDoc->m_prevBrushPoint = target;
	pDoc->cursorCounter = 1;

	int lineWidth = pDoc->getWidth();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);
	glLineWidth(lineWidth);
	BrushMove(source, target);
}

void SingleLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int size = pDoc->getSize();
	float angle = (float)pDoc->getAngle(target) * M_PI / 180.0;
	if (pDoc == NULL) {
		printf("SingleLineBrush::BrushMove  document is NULL\n");
		return;
	}
	glBegin( GL_LINES );
	SetColor(source);
	int xOff = cos(angle)*(float)size / 2;
	int yOff = sin(angle)*(float)size / 2;
	glVertex2d(target.x - xOff, target.y - yOff);
	glVertex2d(target.x + xOff, target.y + yOff);
	glEnd();
}

void SingleLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}