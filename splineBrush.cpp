//
// splineBrush.cpp
//
// The implementation of Spline Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "splineBrush.h"
#include <cmath>

extern float frand();

SplineBrush::SplineBrush(ImpressionistDoc* pDoc, const char* name) :
ImpBrush(pDoc, name)
{
}

void SplineBrush::BrushBegin(const Point source, const Point target)
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

void SplineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	int size = pDoc->getSize();
	if (pDoc == NULL) {
		printf("SingleLineBrush::BrushMove  document is NULL\n");
		return;
	}
	glBegin(GL_LINES);
	SetColor(source);
	glVertex2d(target.x - 2 * size, target.y + 0);
	glVertex2d(target.x - 1 * size, target.y + 0.25 * 8);

	glVertex2d(target.x - 1 * size, target.y + 0.25 * 8);
	glVertex2d(target.x - 0 * size, target.y + 1 * 8);

	glVertex2d(target.x + 0 * size, target.y + 1 * 8);
	glVertex2d(target.x + 1 * size, target.y + 0.25 * 8);

	glVertex2d(target.x + 1 * size, target.y + 0.25 * 8);
	glVertex2d(target.x + 2 * size, target.y + 0);
	glEnd();
}

void SplineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}