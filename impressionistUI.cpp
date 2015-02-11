//
// impressionistUI.cpp
//
// The user interface part for the program.
//


#include <FL/fl_ask.H>

#include <algorithm>

#include <math.h>
#include <iostream>

#include "impressionistUI.h"
#include "impressionistDoc.h"
#include <sstream>

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    myWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//-------------------------------------------------------------
// Brings up the filter kernel dialog
// This is called by the UI when the filter kernel menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_filter_kernel(Fl_Menu_* o, void* v)
{
	if (whoami(o)->getDocument()->m_ucBitmap == NULL)
	{
		fl_alert("No Image Loaded");
	}
	else
	{
		whoami(o)->m_bPreviewMode = false;
		whoami(o)->m_filterKernelDialog->show();
	}
}


//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->hide();
	whoami(o)->m_filterKernelDialog->hide();
	whoami(o)->m_mainWindow->hide();
}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version 1.3.0 for CS 384G, Spring 2015");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(long)v;

	if (type == BRUSH_LINES || type == BRUSH_SCATTERED_LINES)
	{
		pDoc->m_pUI->m_BrushWidthSlider->activate();
		pDoc->m_pUI->m_BrushAngleSlider->activate();
	}
	else
	{
		pDoc->m_pUI->m_BrushWidthSlider->deactivate();
		pDoc->m_pUI->m_BrushAngleSlider->deactivate();
	}
	if (type == BRUSH_SPLINES)
	{
		pDoc->m_pUI->m_BrushWidthSlider->activate();
	}
	pDoc->setBrushType(type);
}

//-------------------------------------------------------------
// Sets the angle input type to use to the one chosen in the angle input 
// choice.  
// Called by the UI when a angle input method is chosen in the angle input choice
//-------------------------------------------------------------
void ImpressionistUI::cb_angleChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (long)v;

	pDoc->setAngleType(type);
	if (type == GRADIENT_ANGLE)
	{
		pDoc->generateGradientAngles();
	}
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

//-----------------------------------------------------------
// Updates the brush width to use from the value of the width
// slider
// Called by the UI when the width slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nWidth = int(((Fl_Slider *)o)->value());
}

//-----------------------------------------------------------
// Updates the brush angle to use from the value of the angle
// slider
// Called by the UI when the angle slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAngle = int(((Fl_Slider *)o)->value());
}

//------------------------------------------------------------
// Paints the picture automatically
// Called by the UI when the draw automatically button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_draw_automatically(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	pUI->m_paintView->drawAutomatically();
}

// ---------------------------
// Random angle button callback
// ---------------------------
void ImpressionistUI::cb_randomAngle(Fl_Widget* o, void* v)
{
	ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));
	if (pUI->m_bRandomAngle == TRUE) pUI->m_bRandomAngle = FALSE;
	else pUI->m_bRandomAngle = TRUE;
}

// ---------------------------
// Random size button callback
// ---------------------------
void ImpressionistUI::cb_randomSize(Fl_Widget* o, void* v)
{
	ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));
	if (pUI->m_bRandomSize == TRUE) pUI->m_bRandomSize = FALSE;
	else pUI->m_bRandomSize = TRUE;
}

//-----------------------------------------------------------
// Updates the brush spacing to use from the value of the spacing
// slider
// Called by the UI when the spacing slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_spacingSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSpacing = int(((Fl_Slider *)o)->value());
}

//-----------------------------------------------------------
// Updates the brush alpha to use from the value of the alpha
// slider
// Called by the UI when the alpha slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAlpha = double(((Fl_Slider *)o)->value());
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;
	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

//------------------------------------------------
// Return the brush width
//------------------------------------------------
int ImpressionistUI::getWidth()
{
	return m_nWidth;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setWidth(int width)
{
	m_nWidth = width;

	if (width <= 40)
		m_BrushWidthSlider->value(m_nWidth);
}

//------------------------------------------------
// Return the brush angle
//------------------------------------------------
int ImpressionistUI::getAngle()
{
	return m_nAngle;
}

//-------------------------------------------------
// Set the brush angle
//-------------------------------------------------
void ImpressionistUI::setAngle(int angle)
{
	m_nAngle = angle;

	if (angle <= 360)
		m_BrushAngleSlider->value(m_nAngle);
}

//------------------------------------------------
// Return the spacing for automatic drawing
//------------------------------------------------
int ImpressionistUI::getSpacing()
{
	return m_nSpacing;
}

//-------------------------------------------------
// Set the spacing for automatic drawing
//-------------------------------------------------
void ImpressionistUI::setSpacing(int spacing)
{
	m_nSpacing = spacing;
}

//------------------------------------------------
// Return the random angle boolean
//------------------------------------------------
bool ImpressionistUI::getRandomAngle()
{
	return m_bRandomAngle;
}

//-------------------------------------------------
// Set the random angle boolean
//-------------------------------------------------
void ImpressionistUI::setRandomAngle(bool angle)
{
	m_bRandomAngle = angle;
}

//------------------------------------------------
// Return the random size boolean
//------------------------------------------------
bool ImpressionistUI::getRandomSize()
{
	return m_bRandomSize;
}

//-------------------------------------------------
// Set the random size boolean
//-------------------------------------------------
void ImpressionistUI::setRandomSize(bool size)
{
	m_bRandomSize = size;
}

//------------------------------------------------
// Return the brush alpha
//------------------------------------------------
double ImpressionistUI::getAlpha()
{
	return m_nAlpha;
}

//-------------------------------------------------
// Set the brush alpha
//-------------------------------------------------
void ImpressionistUI::setAlpha(double alpha)
{
	m_nAlpha = alpha;

	if (alpha <= 1)
		m_BrushAlphaSlider->value(m_nAlpha);
}



// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes },
		{ "&Filter Kernel...", FL_CTRL + 'f', (Fl_Callback *)ImpressionistUI::cb_filter_kernel },
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Splines",			FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SPLINES },
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {0}
};

// Angle input choice menu definition
Fl_Menu_Item ImpressionistUI::angleTypeMenu[NUM_ANGLE_TYPE+1] = {
	{ "Slider", FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)SLIDER_VALUE },
	{ "Right Mouse Button", FL_ALT + 'r', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)RIGHT_MOUSE_BUTTON },
	{ "Cursor Movement", FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)CURSOR_MOVEMENT },
	{ "Perpendicular Gradient", FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)GRADIENT_ANGLE },
	{ 0 }
};

// Filter Type input choice menu definition
Fl_Menu_Item ImpressionistUI::filterTypeMenu[NUM_FILTER_TYPE+1] = {
	{ "Mean Filter", FL_ALT + 'm', (Fl_Callback *)ImpressionistUI::cb_filterTypeChoice, (void *)MEAN_FILTER },
	{ "Gaussian Filter", FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_filterTypeChoice, (void *)GAUSSIAN_FILTER },
	{ "Laplacian Filter", FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_filterTypeChoice, (void *)LAPLACIAN_FILTER },
	{ "Original Filter", FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_filterTypeChoice, (void *)ORIGINAL_FILTER },
	{ 0 }
};

// Filter Type input choice menu definition
Fl_Menu_Item ImpressionistUI::sobelFilterTypeMenu[NUM_SOBEL_FILTER + 1] = {
	{ "Horizontal Filter", FL_ALT + 'm', (Fl_Callback *)ImpressionistUI::cb_sobelFilterChoice, (void *)SOBEL_HORIZONTAL },
	{ "Vertical Filter", FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_sobelFilterChoice, (void *)SOBEL_VERITCAL },
	{ "Combined Filter", FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_sobelFilterChoice, (void *)SOBEL_COMBINED },
	{ 0 }
};

// Filter Size input choice menu definition
Fl_Menu_Item ImpressionistUI::filterSizeMenu[NUM_SIZE_TYPE + 1] = {
	{ "3x3", FL_ALT + 't', (Fl_Callback *)ImpressionistUI::cb_filterSizeChoice, (void *)SIZE_3 },
	{ "5x5", FL_ALT + 'f', (Fl_Callback *)ImpressionistUI::cb_filterSizeChoice, (void *)SIZE_5 },
	{ "7x7", FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_filterSizeChoice, (void *)SIZE_7 },
	{ "9x9", FL_ALT + 'n', (Fl_Callback *)ImpressionistUI::cb_filterSizeChoice, (void *)SIZE_9 },
	{ 0 }
};



//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_nWidth = 10;
	m_nAngle = 0;
	m_nAlpha = 1;
	m_bPreviewMode = false;
	m_filterSize = 0;
	m_applyOnPainting = false;
	filterType = NUM_FILTER_TYPE;
	filterSizeType = NUM_SIZE_TYPE;
	sobelFilterType = SOBEL_HORIZONTAL;
	m_bRandomAngle = false;
	m_bRandomSize = false;
	m_nSpacing = 1;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		// Add a brush angle method choice to the dialog
		m_AngleTypeChoice = new Fl_Choice(130, 50, 150, 25, "&Angle Input Type");
		m_AngleTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_AngleTypeChoice->menu(angleTypeMenu);
		m_AngleTypeChoice->callback(cb_angleChoice);

		// Add brush size slider to the dialog 
		// Slider for size
		m_BrushSizeSlider = new Fl_Value_Slider(10, 120, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		// Slider for width
		m_BrushWidthSlider = new Fl_Value_Slider(10, 145, 300, 20, "Line Width");
		m_BrushWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushWidthSlider->labelfont(FL_COURIER);
		m_BrushWidthSlider->labelsize(12);
		m_BrushWidthSlider->minimum(1);
		m_BrushWidthSlider->maximum(40);
		m_BrushWidthSlider->step(1);
		m_BrushWidthSlider->value(m_nWidth);
		m_BrushWidthSlider->align(FL_ALIGN_RIGHT);
		m_BrushWidthSlider->callback(cb_widthSlides);
		m_BrushWidthSlider->deactivate();

		// Slider for angle
		m_BrushAngleSlider = new Fl_Value_Slider(10, 170, 300, 20, "Line Angle");
		m_BrushAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAngleSlider->labelfont(FL_COURIER);
		m_BrushAngleSlider->labelsize(12);
		m_BrushAngleSlider->minimum(0);
		m_BrushAngleSlider->maximum(360);
		m_BrushAngleSlider->step(1);
		m_BrushAngleSlider->value(m_nAngle);
		m_BrushAngleSlider->align(FL_ALIGN_RIGHT);
		m_BrushAngleSlider->callback(cb_angleSlides);
		m_BrushAngleSlider->deactivate();

		// Slider for alpha
		m_BrushAlphaSlider = new Fl_Value_Slider(10, 195, 300, 20, "Alpha");
		m_BrushAlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAlphaSlider->labelfont(FL_COURIER);
		m_BrushAlphaSlider->labelsize(12);
		m_BrushAlphaSlider->minimum(0);
		m_BrushAlphaSlider->maximum(1);
		m_BrushAlphaSlider->step(0.01);
		m_BrushAlphaSlider->value(m_nAlpha);
		m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
		m_BrushAlphaSlider->callback(cb_alphaSlides);

		m_DrawAutomatically = new Fl_Button(10, 230, 150, 25, "&Draw Automatically");
		m_DrawAutomatically->user_data((void*)(this));
		m_DrawAutomatically->callback(cb_draw_automatically);

		m_RandomAngle = new Fl_Light_Button(220, 230, 120, 25, "Random Angle");
		m_RandomAngle->user_data((void*)(this));
		m_RandomAngle->callback(cb_randomAngle);

		m_RandomSize = new Fl_Light_Button(220, 270, 120, 25, "Random Size");
		m_RandomSize->user_data((void*)(this));
		m_RandomSize->callback(cb_randomSize);

		m_SpacingSlider = new Fl_Value_Slider(10, 270, 150, 20, "Spacing");
		m_SpacingSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_SpacingSlider->type(FL_HOR_NICE_SLIDER);
		m_SpacingSlider->labelfont(FL_COURIER);
		m_SpacingSlider->labelsize(12);
		m_SpacingSlider->minimum(1);
		m_SpacingSlider->maximum(20);
		m_SpacingSlider->step(1);
		m_SpacingSlider->value(2);
		m_SpacingSlider->align(FL_ALIGN_RIGHT);
		m_SpacingSlider->callback(cb_spacingSlides);

		m_brushDialog->end();

	// brush dialog definition
	m_filterKernelDialog = new Fl_Window(500, 600, "Filter Design UI");
		m_filterKernelGroup = new Fl_Group(2, 5, 460, 265);
			m_filterKernelGroup->box(FL_BORDER_BOX);

			m_filterKernelGroup->add(new Fl_Input(10, 10, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(60, 10, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(110, 10, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(160, 10, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(210, 10, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(260, 10, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(310, 10, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(360, 10, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(410, 10, 40, 25, ""));

			m_filterKernelGroup->add(new Fl_Input(10, 38, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(60, 38, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(110, 38, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(160, 38, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(210, 38, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(260, 38, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(310, 38, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(360, 38, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(410, 38, 40, 25, ""));

			m_filterKernelGroup->add(new Fl_Input(10, 66, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(60, 66, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(110, 66, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(160, 66, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(210, 66, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(260, 66, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(310, 66, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(360, 66, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(410, 66, 40, 25, ""));

			m_filterKernelGroup->add(new Fl_Input(10, 94, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(60, 94, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(110, 94, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(160, 94, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(210, 94, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(260, 94, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(310, 94, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(360, 94, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(410, 94, 40, 25, ""));

			m_filterKernelGroup->add(new Fl_Input(10, 122, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(60, 122, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(110, 122, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(160, 122, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(210, 122, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(260, 122, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(310, 122, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(360, 122, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(410, 122, 40, 25, ""));

			m_filterKernelGroup->add(new Fl_Input(10, 150, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(60, 150, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(110, 150, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(160, 150, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(210, 150, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(260, 150, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(310, 150, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(360, 150, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(410, 150, 40, 25, ""));

			m_filterKernelGroup->add(new Fl_Input(10, 178, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(60, 178, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(110, 178, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(160, 178, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(210, 178, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(260, 178, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(310, 178, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(360, 178, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(410, 178, 40, 25, ""));

			m_filterKernelGroup->add(new Fl_Input(10, 206, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(60, 206, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(110, 206, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(160, 206, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(210, 206, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(260, 206, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(310, 206, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(360, 206, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(410, 206, 40, 25, ""));

			m_filterKernelGroup->add(new Fl_Input(10, 234, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(60, 234, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(110, 234, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(160, 234, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(210, 234, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(260, 234, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(310, 234, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(360, 234, 40, 25, ""));
			m_filterKernelGroup->add(new Fl_Input(410, 234, 40, 25, ""));
		m_filterKernelGroup->end();
		

		// Divide by input dialog
		m_divideByInput = new Fl_Input(120, 275, 145, 25, "Divide By:");
		m_divideByInput->user_data((void*)(this));   // record self to be used by static callback functions
		m_divideByInput->callback(cb_divideByInput);

		// Original or Painting Image Light Button
		m_imageChoiceButton = new Fl_Light_Button(280, 285, 80, 25, "&Painting");
		m_imageChoiceButton->user_data((void*)(this));   // record self to be used by static callback functions
		m_imageChoiceButton->callback(cb_imageChoiceButton);

		// Offset input dialog
		m_offsetInput = new Fl_Input(120, 305, 145, 25, "Offset:");
		m_offsetInput->user_data((void*)(this));   // record self to be used by static callback functions
		m_offsetInput->callback(cb_offsetInput);

		// Preview Button
		m_previewButton = new Fl_Button(7, 345, 273, 25, "Preview");
		m_previewButton->user_data((void*)(this));   // record self to be used by static callback functions
		m_previewButton->callback(cb_previewButton);

		// Apply Button
		m_applyButton = new Fl_Button(7, 380, 130, 25, "Apply");
		m_applyButton->user_data((void*)(this));   // record self to be used by static callback functions
		m_applyButton->callback(cb_applyButton);

		// Cancel Button
		m_cancelButton = new Fl_Button(150, 380, 130, 25, "Undo");
		m_cancelButton->user_data((void*)(this));   // record self to be used by static callback functions
		m_cancelButton->callback(cb_cancelButton);

		// Add a filter type choice to the dialog
		m_FilterSizeChoice = new Fl_Choice(80, 410, 200, 25, "Filter Size");
		m_FilterSizeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_FilterSizeChoice->menu(filterSizeMenu);
		m_FilterSizeChoice->value(SIZE_5);
		m_FilterSizeChoice->callback(cb_filterSizeChoice);

		// Add a filter type choice to the dialog
		m_FilterTypeChoice = new Fl_Choice(80, 440, 200, 25, "Filter Type");
		m_FilterTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_FilterTypeChoice->menu(filterTypeMenu);
		m_FilterTypeChoice->value(ORIGINAL_FILTER);
		m_FilterTypeChoice->callback(cb_filterTypeChoice);

		// Add a filter type choice to the dialog
		m_SobelFilterTypeChoice = new Fl_Choice(80, 470, 200, 25, "Sobel Filter");
		m_SobelFilterTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_SobelFilterTypeChoice->menu(sobelFilterTypeMenu);
		m_SobelFilterTypeChoice->callback(cb_sobelFilterChoice);

		// Preview sobel filter Button
		m_previewSobelButton = new Fl_Button(285, 470, 95, 25, "Preview Sobel");
		m_previewSobelButton->user_data((void*)(this));   // record self to be used by static callback functions
		m_previewSobelButton->callback(cb_previewSobelButton);

		// Apply sobel filter Button
		m_applySobelButton = new Fl_Button(390, 470, 90, 25, "Apply Sobel");
		m_applySobelButton->user_data((void*)(this));   // record self to be used by static callback functions
		m_applySobelButton->callback(cb_applySobelButton);



	m_filterKernelDialog->end();

	this->updateFilterGroupUsingSizeType(SIZE_5);
	this->updateFilterGroupUsingFilterType(ORIGINAL_FILTER);

}

//Filter Callbacks and other functions

// ---------------------------
// Image choice button callback
// ---------------------------
void ImpressionistUI::cb_imageChoiceButton(Fl_Widget* o, void* v)
{
	ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));
	if (pUI->m_applyOnPainting == TRUE) pUI->m_applyOnPainting = FALSE;
	else pUI->m_applyOnPainting = TRUE;
}

// ---------------------------
// Divide by Input Callback
// ---------------------------
void ImpressionistUI::cb_divideByInput(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->scale = atoi(((Fl_Input *)o)->value());
}

// ---------------------------
// Offset Input Callback
// ---------------------------
void ImpressionistUI::cb_offsetInput(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->offset = atoi(((Fl_Input *)o)->value());
}

// ---------------------------
// Apply Button Callback
// ---------------------------
void ImpressionistUI::cb_applyButton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	int count = 0;
	for (int i = 0; i < FLT_HEIGHT*FLT_WIDTH; i++)
	{
		pUI->fltKernel[i] = 0;
	}
	for (int i = 0; i < pUI->m_filterSize; i++)
	{
		for (int j = 0; j < pUI->m_filterSize; j++)
		{
			int filterNo = i*FLT_WIDTH + j;
			Fl_Input* tempInt = (Fl_Input *)pUI->m_filterKernelGroup->array()[filterNo];
			float x = atof(tempInt->value());
			pUI->fltKernel[count] = x;
			count++;
		}
	}
	pUI->applyFilter();

}

// ---------------------------
// Function to apply the filter
// ---------------------------
void ImpressionistUI::applyFilter()
{
	if (this->m_bPreviewMode)
	{
		int size = m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth * 3;
		memcpy(m_pDoc->m_ucPreviewBackup, m_pDoc->m_ucPainting, size);
		this->m_bPreviewMode = false;
		return;
	}
	if (this->m_applyOnPainting)
	{
		int size = m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth * 3;
		memcpy(m_pDoc->m_ucPaintingTemp, m_pDoc->m_ucPainting, size);
		m_pDoc->applyFilter(m_pDoc->m_ucPaintingTemp, m_pDoc->m_nPaintWidth, m_pDoc->m_nPaintHeight, m_pDoc->m_ucPainting, this->fltKernel, this->m_filterSize, this->m_filterSize, this->scale, this->offset);
	}
	else
	{
		m_pDoc->applyFilter(m_pDoc->m_ucBitmap, m_pDoc->m_nPaintWidth, m_pDoc->m_nPaintHeight, m_pDoc->m_ucPainting, this->fltKernel, this->m_filterSize, this->m_filterSize, this->scale, this->offset);
	}
	this->m_paintView->refresh();
}

// ---------------------------
// Preview Button Callback
// ---------------------------
void ImpressionistUI::cb_previewButton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	int count = 0;
	for (int i = 0; i < pUI->m_filterSize; i++)
	{
		for (int j = 0; j < pUI->m_filterSize; j++)
		{
			int filterNo = i*FLT_WIDTH + j;
			Fl_Input* tempInt = (Fl_Input *)pUI->m_filterKernelGroup->array()[filterNo];
			float x = atof(tempInt->value());
			pUI->fltKernel[count] = x;
			count++;
		}
	}
	pUI->previewFilter();
}

// ---------------------------
// Function to preview the filter
// ---------------------------
void ImpressionistUI::previewFilter()
{
	this->m_bPreviewMode = true;
	int size = m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth * 3;
	memcpy(m_pDoc->m_ucPreviewBackup, m_pDoc->m_ucPainting, size);
	if (this->m_applyOnPainting)
	{
		memcpy(m_pDoc->m_ucPaintingTemp, m_pDoc->m_ucPainting, size);
		m_pDoc->applyFilter(m_pDoc->m_ucPaintingTemp, m_pDoc->m_nPaintWidth, m_pDoc->m_nPaintHeight, m_pDoc->m_ucPainting, this->fltKernel, this->m_filterSize, this->m_filterSize, this->scale, this->offset);
	}
	else
	{	
		m_pDoc->applyFilter(m_pDoc->m_ucBitmap, m_pDoc->m_nPaintWidth, m_pDoc->m_nPaintHeight, m_pDoc->m_ucPainting, this->fltKernel, this->m_filterSize, this->m_filterSize, this->scale, this->offset);
	}
	this->m_paintView->refresh();
}

// ---------------------------
// Cancel Button Callback
// ---------------------------
void ImpressionistUI::cb_cancelButton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	pUI->cancelFilter();
}

// ---------------------------
// Function to cancel the filter
// ---------------------------
void ImpressionistUI::cancelFilter()
{
	if (this->m_bPreviewMode)
	{
		int size = m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth * 3;
		memcpy(m_pDoc->m_ucPainting, m_pDoc->m_ucPreviewBackup, size);
	}
	this->m_bPreviewMode = false;
	this->m_paintView->refresh();
	//this->m_filterKernelDialog->hide();
}

//------------------------------------------------
// Returns the the filter kernel scale
//------------------------------------------------
double ImpressionistUI::getScale()
{
	return scale;
}

//------------------------------------------------
// Returns the the filter kernel offset
//------------------------------------------------
double ImpressionistUI::getOffset()
{
	return offset;
}

//-------------------------------------------------------------
// Sets the type of filter to use to the one chosen in the filter 
// type choice.  
// Called by the UI when a filter type is chosen in the filter type choice
//-------------------------------------------------------------
void ImpressionistUI::cb_filterTypeChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));

	int type = (long)v;
	pUI->updateFilterGroupUsingFilterType(type);
}

// -----------------------------------------------------------
// Function to update the filter group based on the filter type
// -----------------------------------------------------------
void ImpressionistUI::updateFilterGroupUsingFilterType(int filType)
{
	if (filType == this->filterType)
	{
		return;
	}
	this->filterType = filType;
	if (this->filterType == ORIGINAL_FILTER)
	{
		this->updateFilterGroupToOriginalFilter();
	}
	else if (this->filterType == MEAN_FILTER)
	{
		this->updateFilterGroupToMeanFilter();
	}
	else if (this->filterType == GAUSSIAN_FILTER)
	{
		if (this->m_filterSize > 7)
		{
			this->updateFilterGroupUsingSizeType(SIZE_7);
			this->m_FilterSizeChoice->value(SIZE_7);
		}
		this->updateFilterGroupToGaussianFilter();
	}
	else if (this->filterType == LAPLACIAN_FILTER)
	{
		if (this->m_filterSize > 7)
		{
			this->updateFilterGroupUsingSizeType(SIZE_7);
			this->m_FilterSizeChoice->value(SIZE_7);
		}
		this->updateFilterGroupToLaplacianFilter();
	}
}

//-------------------------------------------------------------
// Sets the size of filter to use to the one chosen in the filter 
// size choice.  
// Called by the UI when a filter size is chosen in the filter size choice
//-------------------------------------------------------------
void ImpressionistUI::cb_filterSizeChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));

	int type = (long)v;
	pUI->updateFilterGroupUsingSizeType(type);
}

// -----------------------------------------------------------
// Function to update the filter group based on the size type
// -----------------------------------------------------------
void ImpressionistUI::updateFilterGroupUsingSizeType(int sizeType)
{
	if (sizeType == this->filterSizeType)
	{
		return;
	}
	this->filterSizeType = sizeType;
	if (this->filterSizeType == SIZE_3)
	{
		this->m_filterSize = 3;
	}
	else if (this->filterSizeType == SIZE_5)
	{
		this->m_filterSize = 5;
	}
	else if (this->filterSizeType == SIZE_7)
	{
		this->m_filterSize = 7;
	}
	else if (this->filterSizeType == SIZE_9)
	{
		this->m_filterSize = 9;
	}
	for (int i = 0; i < FLT_WIDTH*FLT_HEIGHT; i++)
	{
		this->m_filterKernelGroup->array()[i]->hide();
	}
	for (int i = 0; i < this->m_filterSize; i++)
	{
		for (int j = 0; j < this->m_filterSize; j++)
		{
			int filterNo = i*FLT_WIDTH + j;
			this->m_filterKernelGroup->array()[filterNo]->show();
		}
	}
}

// -----------------------------------------------------------
// Function to update the filter group to the original filter type
// -----------------------------------------------------------
void ImpressionistUI::updateFilterGroupToOriginalFilter()
{
	int middleElement = this->m_filterSize / 2;
	for (int i = 0; i < this->m_filterSize; i++)
	{
		for (int j = 0; j < this->m_filterSize; j++)
		{
			int filterNo = i*FLT_WIDTH + j;
			if (i == middleElement && j == middleElement)
			{
				((Fl_Input *)this->m_filterKernelGroup->array()[filterNo])->value("1");
			}
			else
			{
				((Fl_Input *)this->m_filterKernelGroup->array()[filterNo])->value("0");
			}
		}
	}
	this->m_divideByInput->value("1");
	this->m_offsetInput->value("0");
	this->scale = 1;
	this->offset = 0;
}

// -----------------------------------------------------------
// Function to update the filter group to the mean filter type
// -----------------------------------------------------------
void ImpressionistUI::updateFilterGroupToMeanFilter()
{
	for (int i = 0; i < this->m_filterSize; i++)
	{
		for (int j = 0; j < this->m_filterSize; j++)
		{
			int filterNo = i*FLT_WIDTH + j;
			((Fl_Input *)this->m_filterKernelGroup->array()[filterNo])->value("1");
		}
	}
	std::stringstream str;
	str<<(this->m_filterSize*this->m_filterSize);
	this->m_divideByInput->value(str.str().c_str());
	this->m_offsetInput->value("0");
	this->scale = this->m_filterSize*this->m_filterSize;
	this->offset = 0;
}

// -----------------------------------------------------------
// Function to update the filter group to the gaussian filter type
// -----------------------------------------------------------
void ImpressionistUI::updateFilterGroupToGaussianFilter()
{
	int count = 0;
	double tempScale = 0;
	for (int i = 0; i < this->m_filterSize; i++)
	{
		for (int j = 0; j < this->m_filterSize; j++)
		{
			int filterNo = i*FLT_WIDTH + j;
			char str[5];
			if (this->m_filterSize == 3)
			{
				std::stringstream str;
				str<<(gaussianFilter3x3[count]);
				((Fl_Input *)this->m_filterKernelGroup->array()[filterNo])->value(str.str().c_str());
				tempScale += gaussianFilter3x3[count];
			}
			else if (this->m_filterSize == 5)
			{
				std::stringstream str;
				str<<(gaussianFilter5x5[count]);
				((Fl_Input *)this->m_filterKernelGroup->array()[filterNo])->value(str.str().c_str());
				tempScale += gaussianFilter3x3[count];
			}
			else if (this->m_filterSize == 7)
			{
				std::stringstream str;
				str<<(gaussianFilter7x7[count]);
				((Fl_Input *)this->m_filterKernelGroup->array()[filterNo])->value(str.str().c_str());
				tempScale += gaussianFilter3x3[count];
			}
			count++;
		}
	}
	std::stringstream scale;
	scale<<tempScale;
	this->m_divideByInput->value(scale.str().c_str());
	this->m_offsetInput->value("0");
	this->scale = tempScale;
	this->offset = 0;
}

// -----------------------------------------------------------
// Function to update the filter group to the laplacian filter type
// -----------------------------------------------------------
void ImpressionistUI::updateFilterGroupToLaplacianFilter()
{
	int count = 0;
	for (int i = 0; i < this->m_filterSize; i++)
	{
		for (int j = 0; j < this->m_filterSize; j++)
		{
			int filterNo = i*FLT_WIDTH + j;
			char str[5];
			if (this->m_filterSize == 3)
			{
				std::stringstream str;
				str<<(laplacianFilter3x3[count]);
				((Fl_Input *)this->m_filterKernelGroup->array()[filterNo])->value(str.str().c_str());
			}
			else if (this->m_filterSize == 5)
			{
				std::stringstream str;
				str<<(laplacianFilter5x5[count]);
				((Fl_Input *)this->m_filterKernelGroup->array()[filterNo])->value(str.str().c_str());
			}
			else if (this->m_filterSize == 7)
			{
				std::stringstream str;
				str<<(laplacianFilter7x7[count]);
				((Fl_Input *)this->m_filterKernelGroup->array()[filterNo])->value(str.str().c_str());
			}
			count++;
		}
	}
	this->m_divideByInput->value("1");
	this->m_offsetInput->value("0");
	this->scale = 1;
	this->offset = 0;
}

//-------------------------------------------------------------
// Sets the type of sobel filter to use to the one chosen in the sobel 
// filter type choice.
// Called by the UI when a sobel filter is chosen in the filter type choice
//-------------------------------------------------------------
void ImpressionistUI::cb_sobelFilterChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));

	int type = (long)v;
	pUI->sobelFilterType = type;
}

// ---------------------------
// Apply Sobel Button Callback
// ---------------------------
void ImpressionistUI::cb_applySobelButton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();
	if (pUI->m_bPreviewMode)
	{
		int size = pDoc->m_nPaintHeight * pDoc->m_nPaintWidth * 3;
		memcpy(pDoc->m_ucPreviewBackup, pDoc->m_ucPainting, size);
		pUI->m_bPreviewMode = false;
		return;
	}
	int size = pDoc->m_nPaintHeight * pDoc->m_nPaintWidth * 3;
	if (pUI->sobelFilterType == SOBEL_HORIZONTAL)
	{
		double sobelFilter[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
		if (pUI->m_applyOnPainting)
		{
			memcpy(pDoc->m_ucPaintingTemp, pDoc->m_ucPainting, size);
			pDoc->applyFilter(pDoc->m_ucPaintingTemp, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilter, 3, 3, 1, pUI->offset);
		}
		else
		{
			pDoc->applyFilter(pDoc->m_ucBitmap, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilter, 3, 3, 1, pUI->offset);
		}
	}
	else if (pUI->sobelFilterType == SOBEL_VERITCAL)
	{
		double sobelFilter[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
		if (pUI->m_applyOnPainting)
		{
			memcpy(pDoc->m_ucPaintingTemp, pDoc->m_ucPainting, size);
			pDoc->applyFilter(pDoc->m_ucPaintingTemp, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilter, 3, 3, 1, pUI->offset);
		}
		else
		{
			pDoc->applyFilter(pDoc->m_ucBitmap, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilter, 3, 3, 1, pUI->offset);
		}
	}
	else if (pUI->sobelFilterType == SOBEL_COMBINED)
	{
		double sobelFilterHorizontal[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
		double sobelFilterVertical[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
		if (pUI->m_applyOnPainting)
		{
			memcpy(pDoc->m_ucPaintingTemp, pDoc->m_ucPainting, size);
			pDoc->applySobelCombinedFilter(pDoc->m_ucPaintingTemp, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilterHorizontal, sobelFilterVertical, 3, 3, 1, pUI->offset);
		}
		else
		{
			pDoc->applySobelCombinedFilter(pDoc->m_ucBitmap, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilterHorizontal, sobelFilterVertical, 3, 3, 1, pUI->offset);
		}
	}
	pUI->m_divideByInput->value("1");
	pUI->scale = 1;
	pUI->m_paintView->refresh();
}

// ---------------------------
// Preview Sobel Button Callback
// ---------------------------
void ImpressionistUI::cb_previewSobelButton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();
	// TODO : Preview Sobel filter
	pUI->m_bPreviewMode = true;
	int size = pDoc->m_nPaintHeight * pDoc->m_nPaintWidth * 3;
	memcpy(pDoc->m_ucPreviewBackup, pDoc->m_ucPainting, size);
	if (pUI->sobelFilterType == SOBEL_HORIZONTAL)
	{
		double sobelFilter[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
		if (pUI->m_applyOnPainting)
		{
			memcpy(pDoc->m_ucPaintingTemp, pDoc->m_ucPainting, size);
			pDoc->applyFilter(pDoc->m_ucPaintingTemp, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilter, 3, 3, 1, pUI->offset);
		}
		else
		{
			pDoc->applyFilter(pDoc->m_ucBitmap, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilter, 3, 3, 1, pUI->offset);
		}
	}
	else if (pUI->sobelFilterType == SOBEL_VERITCAL)
	{
		double sobelFilter[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
		if (pUI->m_applyOnPainting)
		{
			memcpy(pDoc->m_ucPaintingTemp, pDoc->m_ucPainting, size);
			pDoc->applyFilter(pDoc->m_ucPaintingTemp, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilter, 3, 3, 1, pUI->offset);
		}
		else
		{
			pDoc->applyFilter(pDoc->m_ucBitmap, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilter, 3, 3, 1, pUI->offset);
		}
	}
	else if (pUI->sobelFilterType == SOBEL_COMBINED)
	{
		double sobelFilterHorizontal[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
		double sobelFilterVertical[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
		if (pUI->m_applyOnPainting)
		{
			memcpy(pDoc->m_ucPaintingTemp, pDoc->m_ucPainting, size);
			pDoc->applySobelCombinedFilter(pDoc->m_ucPaintingTemp, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilterHorizontal, sobelFilterVertical, 3, 3, 1, pUI->offset);
		}
		else
		{
			pDoc->applySobelCombinedFilter(pDoc->m_ucBitmap, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, sobelFilterHorizontal, sobelFilterVertical, 3, 3, 1, pUI->offset);
		}
	}
	pUI->m_divideByInput->value("1");
	pUI->scale = 1;
	pUI->m_paintView->refresh();
}

ImpressionistUI::~ImpressionistUI()
{
}