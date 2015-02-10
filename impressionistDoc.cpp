// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//


#include <FL/fl_ask.H>

#include <algorithm>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "impBrush.h"

// Include individual brush headers here.
#include "pointBrush.h"
#include "scatteredPointBrush.h"
#include "singleLineBrush.h"
#include "scatteredLineBrush.h"
#include "filledCircleBrush.h"
#include "scatteredCircleBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucPreviewBackup = NULL;
	m_ucPaintingTemp = NULL;
	m_ucGradientAngles = NULL;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS] = new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES] = new SingleLineBrush(this, "Lines");
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]	= new FilledCircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS] = new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]	= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES] = new ScatteredCircleBrush( this, "Scattered Circles" );

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
	m_pCurrentAngle = SLIDER_VALUE;
}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Called by the UI when the user changes the angle input type.
// type: one of the defined angle input types.
//---------------------------------------------------------
int ImpressionistDoc::getAngleType()
{
	return m_pCurrentAngle;
}

//---------------------------------------------------------
// Called by the UI when the user changes the angle input type.
// type: one of the defined angle input types.
//---------------------------------------------------------
void ImpressionistDoc::setAngleType(int type)
{
	m_pCurrentAngle = type;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Sets the size of the brush.
//---------------------------------------------------------
void ImpressionistDoc::setSize(int size)
{
	m_pUI->setSize(size);
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getWidth()
{
	return m_pUI->getWidth();
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getAngle(Point target)
{
	if (m_pCurrentAngle == SLIDER_VALUE)
	{
		return m_pUI->getAngle();
	}
	else if (m_pCurrentAngle == RIGHT_MOUSE_BUTTON)
	{
		return m_pUI->getAngle();
	}
	else if (m_pCurrentAngle == CURSOR_MOVEMENT)
	{
		cursorCounter--;
		if (cursorCounter == 0)
		{
			cursorCounter = 5;
			int lineAngle = atan2(target.y - m_prevBrushPoint.y, target.x - m_prevBrushPoint.x) * 180 / M_PI;
			if (lineAngle < 0)
			{
				lineAngle += 360;
			}
			setAngle(lineAngle);
			m_prevBrushPoint = target;
		}
		return m_pUI->getAngle();
	}
	else if (m_pCurrentAngle == GRADIENT_ANGLE)
	{
		double angle = getGradientAngleAtPoint(target);
		if (angle > 360)
		{
			angle = angle - 360;
		}
		return angle;
	}
}

void ImpressionistDoc::setAngle(int angle)
{
	m_pUI->setAngle(angle);
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
float ImpressionistDoc::getAlpha()
{
	return (float)m_pUI->getAlpha();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	delete [] m_ucBitmap;
	delete [] m_ucPainting;
	delete [] m_ucPreviewBackup;
	delete [] m_ucPaintingTemp;
	delete [] m_ucGradientAngles;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting		= new unsigned char [width*height*3];
	m_ucPreviewBackup	= new unsigned char [width*height*3];
	m_ucPaintingTemp	= new unsigned char [width*height * 3];
	m_ucGradientAngles	= new unsigned char [width*height];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

// Apply the filter specified by filter_kernel to the 
// each pixel in the source buffer and place the resulting
// pixel in the destination buffer.  

// This is called from the UI when the
// "preview" or "apply" button is pressed in the filter dialog.



/*
 *	INPUT: 
 *		sourceBuffer:		the original image buffer, 
 *		srcBufferWidth:		the width of the image buffer
 *		srcBufferHeight:	the height of the image buffer
 *							the buffer is arranged such that 
 *							origImg[3*(row*srcBufferWidth+column)+0], 
 *							origImg[3*(row*srcBufferWidth+column)+1], 
 *							origImg[3*(row*srcBufferWidth+column)+2]
 *							are R, G, B values for pixel at (column, row).
 *		destBuffer:			the image buffer to put the resulting
 *							image in.  It is always the same size
 *							as the source buffer.
 *
 *      filterKernel:		the 2D filter kernel,
 *		knlWidth:			the width of the kernel
 *		knlHeight:			the height of the kernel
 *
 *		divisor, offset:	each pixel after filtering should be
 *							divided by divisor and then added by offset
 */
void ImpressionistDoc::applyFilter( const unsigned char* sourceBuffer,
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset )
{
	for (int pixelRow = 0; pixelRow < srcBufferHeight; pixelRow++)
	{
		for (int pixelColumn = 0; pixelColumn < srcBufferWidth; pixelColumn++)
		{
			double sumRed = 0, sumGreen = 0, sumBlue = 0;
			for (int filterRow = 0; filterRow < knlWidth; filterRow++)
			{
				for (int filterColumn = 0; filterColumn < knlHeight; filterColumn++)
				{
					int tempPixelRow = pixelRow - knlHeight/2 + filterRow;
					int tempPixelColumn = pixelColumn - knlWidth/2 + filterColumn;
					sumRed = sumRed + filterKernel[knlWidth * filterRow + filterColumn] * sourceBuffer[3 * (tempPixelRow*srcBufferWidth + tempPixelColumn) + 0];
					sumGreen = sumGreen + filterKernel[knlWidth * filterRow + filterColumn] * sourceBuffer[3 * (tempPixelRow*srcBufferWidth + tempPixelColumn) + 1];
					sumBlue = sumBlue + filterKernel[knlWidth * filterRow + filterColumn] * sourceBuffer[3 * (tempPixelRow*srcBufferWidth + tempPixelColumn) + 2];
				}
			}
			sumRed = sumRed / divisor + offset;
			sumGreen = sumGreen / divisor + offset;
			sumBlue = sumBlue / divisor + offset;
			destBuffer[3 * (pixelRow*srcBufferWidth + pixelColumn) + 0] = sumRed;
			destBuffer[3 * (pixelRow*srcBufferWidth + pixelColumn) + 1] = sumGreen;
			destBuffer[3 * (pixelRow*srcBufferWidth + pixelColumn) + 2] = sumBlue;
		}
	}
	m_pUI->m_paintView->refresh();
}

void ImpressionistDoc::applySobelCombinedFilter(const unsigned char* sourceBuffer,
	int srcBufferWidth, int srcBufferHeight,
	unsigned char* destBuffer,
	const double *filterKernelHorizontal,
	const double *filterKernelVertical,
	int knlWidth, int knlHeight,
	double divisor, double offset)
{
	for (int pixelRow = 0; pixelRow < srcBufferHeight; pixelRow++)
	{
		for (int pixelColumn = 0; pixelColumn < srcBufferWidth; pixelColumn++)
		{
			double sumRedH = 0, sumGreenH = 0, sumBlueH = 0;
			double sumRedV = 0, sumGreenV = 0, sumBlueV = 0;
			for (int filterRow = 0; filterRow < knlWidth; filterRow++)
			{
				for (int filterColumn = 0; filterColumn < knlHeight; filterColumn++)
				{
					int tempPixelRow = pixelRow - knlHeight / 2 + filterRow;
					int tempPixelColumn = pixelColumn - knlWidth / 2 + filterColumn;
					sumRedH = sumRedH + filterKernelHorizontal[knlWidth * filterRow + filterColumn] * sourceBuffer[3 * (tempPixelRow*srcBufferWidth + tempPixelColumn) + 0];
					sumGreenH = sumGreenH + filterKernelHorizontal[knlWidth * filterRow + filterColumn] * sourceBuffer[3 * (tempPixelRow*srcBufferWidth + tempPixelColumn) + 1];
					sumBlueH = sumBlueH + filterKernelHorizontal[knlWidth * filterRow + filterColumn] * sourceBuffer[3 * (tempPixelRow*srcBufferWidth + tempPixelColumn) + 2];

					sumRedV = sumRedV + filterKernelVertical[knlWidth * filterRow + filterColumn] * sourceBuffer[3 * (tempPixelRow*srcBufferWidth + tempPixelColumn) + 0];
					sumGreenV = sumGreenV + filterKernelVertical[knlWidth * filterRow + filterColumn] * sourceBuffer[3 * (tempPixelRow*srcBufferWidth + tempPixelColumn) + 1];
					sumBlueV = sumBlueV + filterKernelVertical[knlWidth * filterRow + filterColumn] * sourceBuffer[3 * (tempPixelRow*srcBufferWidth + tempPixelColumn) + 2];
				}
			}
			double sumRed = sqrt(sumRedH*sumRedH + sumRedV*sumRedV) + offset;
			double sumGreen = sqrt(sumGreenH*sumGreenH + sumGreenV*sumGreenV) + offset;
			double sumBlue = sqrt(sumBlueH*sumBlueH + sumBlueV*sumBlueV) + offset;
			destBuffer[3 * (pixelRow*srcBufferWidth + pixelColumn) + 0] = sumRed;
			destBuffer[3 * (pixelRow*srcBufferWidth + pixelColumn) + 1] = sumGreen;
			destBuffer[3 * (pixelRow*srcBufferWidth + pixelColumn) + 2] = sumBlue;
		}
	}
	m_pUI->m_paintView->refresh();
}

void ImpressionistDoc::generateGradientAngles()
{
	// Convert to grayscale
	unsigned char*	grayImage;
	grayImage = new unsigned char[m_nPaintWidth*m_nPaintHeight];
	unsigned char*	grayBlurImage;
	grayBlurImage = new unsigned char[m_nPaintWidth*m_nPaintHeight];
	for (int imageRow = 0; imageRow < m_nPaintHeight; imageRow++)
	{
		for (int imageCol = 0; imageCol < m_nPaintWidth; imageCol++)
		{
			double gray = 0.299*m_ucBitmap[3 * (imageRow*m_nPaintWidth + imageCol) + 0] + 0.587*m_ucBitmap[3 * (imageRow*m_nPaintWidth + imageCol) + 1] + 0.114*m_ucBitmap[3 * (imageRow*m_nPaintWidth + imageCol) + 2];
			grayImage[imageRow*m_nPaintWidth + imageCol] = gray;
		}
	}
	// Blur the gray scale image
	double blurFilter[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
	int knlWidth = 3, knlHeight = 3;
	for (int pixelRow = 0; pixelRow < m_nPaintHeight; pixelRow++)
	{
		for (int pixelColumn = 0; pixelColumn < m_nPaintWidth; pixelColumn++)
		{
			double sum = 0;
			for (int filterRow = 0; filterRow < knlWidth; filterRow++)
			{
				for (int filterColumn = 0; filterColumn < knlHeight; filterColumn++)
				{
					int tempPixelRow = pixelRow - knlHeight / 2 + filterRow;
					int tempPixelColumn = pixelColumn - knlWidth / 2 + filterColumn;
					sum = sum + blurFilter[knlWidth * filterRow + filterColumn] * grayImage[tempPixelRow*m_nPaintWidth + tempPixelColumn];
				}
			}
			sum = sum/16;
			grayBlurImage[pixelRow*m_nPaintWidth + pixelColumn] = sum;
		}
	}
	for (int pixelRow = 0; pixelRow < m_nPaintHeight; pixelRow++)
	{
		for (int pixelCol = 0; pixelCol < m_nPaintWidth; pixelCol++)
		{
			if (pixelRow < 0 || pixelCol < 0 || pixelRow>m_nPaintHeight || pixelCol>m_nPaintWidth)
			{
				continue;
			}
			double gradientX = grayBlurImage[(pixelRow*m_nPaintWidth + pixelCol) + 1] - grayBlurImage[(pixelRow*m_nPaintWidth + pixelCol)];
			double gradientY = grayBlurImage[(pixelRow*m_nPaintWidth + pixelCol)] - grayBlurImage[((pixelRow+1)*m_nPaintWidth + pixelCol)];
			double angle = atan2(gradientY, gradientX) * 180.0/M_PI;
			if (angle < 0)
				angle += 360;
			m_ucGradientAngles[(pixelRow*m_nPaintWidth + pixelCol)] = angle;
		}
	}
	delete [] grayImage;
	delete [] grayBlurImage;
}

float ImpressionistDoc::getGradientAngleAtPoint(Point target)
{
	return m_ucGradientAngles[target.x*m_nPaintWidth + target.y];
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}



