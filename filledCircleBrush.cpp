//
// filledCircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "filledCircleBrush.h"
#include <cmath>

extern float frand();

FilledCircleBrush::FilledCircleBrush(ImpressionistDoc* pDoc, const char* name) :
ImpBrush(pDoc, name)
{
}

void FilledCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);
	BrushMove(source, target);
}

void FilledCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("FilledCircleBrush::BrushMove  document is NULL\n");
		return;
	}
	int radius = pDoc->getSize();
	int noOfTriangles = 15;
	glBegin(GL_TRIANGLE_FAN);
	SetColor(source);

	glVertex2d(target.x, target.y);
	for (int i = 0; i <= noOfTriangles; i++)
	{
		float x = target.x + radius / 2.0 * cos(2 * M_PI * i / noOfTriangles);
		float y = target.y + radius / 2.0 * sin(2 * M_PI * i / noOfTriangles);
		glVertex2f(x, y);
	}
	glEnd();
}

void FilledCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

