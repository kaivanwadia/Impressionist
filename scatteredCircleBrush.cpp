//
// scatteredCircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredCircleBrush.h"
#include <cmath>

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, const char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);
	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}
	int noOfCircles = ceil(frand() * 8);
	int radius = pDoc->getSize();
	int noOfTriangles = 15;
	Point tempTarget = target;
	Point tempSource = source;
	for (int i = 0; i < noOfCircles; i++)
	{
		float randomXMov = frand() * radius;
		float randomYMov = frand() * radius;
		tempTarget.x = target.x + randomXMov - radius / 2.0;
		tempTarget.y = target.y + randomYMov - radius / 2.0;
		tempSource.x = source.x + randomXMov - radius / 2.0;
		tempSource.y = source.y + randomYMov - radius / 2.0;
		glBegin(GL_TRIANGLE_FAN);
		SetColor(tempSource);

		glVertex2d(tempTarget.x, tempTarget.y);
		for (int i = 0; i <= noOfTriangles; i++)
		{
			float x = tempTarget.x + radius / 2.0 * cos(2 * M_PI * i / noOfTriangles);
			float y = tempTarget.y + radius / 2.0 * sin(2 * M_PI * i / noOfTriangles);
			glVertex2f(x, y);
		}
		glEnd();
	}
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

