//
// scatteredPointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredPointBrush.h"
#include <cmath>

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, const char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize(1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);
	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int pointSize = pDoc->getSize();
	int noOfPoints = ceil(frand() * 80);
	Point tempTarget = target;
	Point tempSource = source;
	glBegin(GL_POINTS);
	for (int i = 0; i < 80; i++)
	{
		float randomXMov = frand() * pointSize;
		float randomYMov = frand() * pointSize;
		tempTarget.x = target.x + randomXMov - pointSize / 2.0;
		tempTarget.y = target.y + randomYMov - pointSize / 2.0;
		tempSource.x = source.x + randomXMov - pointSize / 2.0;
		tempSource.y = source.y + randomYMov - pointSize / 2.0;
		SetColor(tempSource);
		glVertex2d(tempTarget.x, tempTarget.y);
	}

	glEnd();
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

