//
// scatteredLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredLineBrush.h"
#include <cmath>

extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, const char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int lineWidth = pDoc->getWidth();

	glLineWidth((float)lineWidth);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);
	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredLineBrush::BrushMove  document is NULL\n");
		return;
	}
	int lineLength = pDoc->getSize();
	float angle = (float)pDoc->getAngle(target) * M_PI / 180.0;
	int noOfLines = ceil(frand() * 6);
	Point tempTarget = target;
	Point tempSource = source;

	glBegin(GL_LINES);
	
	for (int i = 0; i < noOfLines; i++)
	{
		float randomXMov = frand() * lineLength;
		float randomYMov = frand() * lineLength;
		tempTarget.x = target.x + randomXMov - lineLength / 2.0;
		tempTarget.y = target.y + randomYMov - lineLength / 2.0;
		tempSource.x = source.x + randomXMov - lineLength / 2.0;
		tempSource.y = source.y + randomYMov - lineLength / 2.0;
		SetColor(tempSource);

		int xOff = cos(angle)*(float)lineLength / 2;
		int yOff = sin(angle)*(float)lineLength / 2;
		glVertex2d(tempTarget.x - xOff, tempTarget.y - yOff);
		glVertex2d(tempTarget.x + xOff, tempTarget.y + yOff);
	}

	glEnd();
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

