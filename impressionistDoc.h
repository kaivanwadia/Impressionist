// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas

	void applyFilter( const unsigned char* sourceBuffer, //The filter kernel
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset );

	void applySobelCombinedFilter(const unsigned char* sourceBuffer, //The filter kernel
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernelHorizontal,
		const double *filterKernelVertical,
		int knlWidth, int knlHeight,
		double divisor, double offset);

	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setAngleType(int type);			// called by the UI to set the angleType
	int		getAngleType();
	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	int		getWidth();						// get the UI width
	void	setWidth(int width);				// set the UI width
	int		getAngle(Point target);						// get the UI angle
	void	setAngle(int angle);				// set the UI angle
	float	getAlpha();						// get the UI alpha
	void	setAlpha(double alpha);				// set the UI alpha
	char*	getImageName();					// get the current image name

	float	getGradientAngleAtPoint(Point target); //get the gradient at a specific point
	void	generateGradientAngles(); //Compute the gray scaled blurred image for gradient calculations
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;

	// Used by the filtering code so that we can
	// preview filters before applying.
	unsigned char*	m_ucPreviewBackup;
	unsigned char*	m_ucPaintingTemp;
	unsigned char*	m_ucGradientAngles;



	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	int			m_pCurrentAngle;
	// Size of the brush.
	int m_nSize;

	int cursorCounter;
	Point m_prevBrushPoint;

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );



private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
