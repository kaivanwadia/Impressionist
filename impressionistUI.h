//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_File_Chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>

#include "impressionist.h"
#include "originalView.h"
#include "paintView.h"

#include "impBrush.h"

const int FLT_WIDTH = 9;
const int FLT_HEIGHT = 9;

const int gaussianFilter3x3[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
const int gaussianFilter5x5[25] = { 1, 4, 6, 4, 1, 4, 16, 24, 16, 4, 6, 24, 36, 24, 6, 4, 16, 24, 16, 4, 1, 4, 6, 4, 1 };
const int gaussianFilter7x7[49] = { 1, 6, 15, 20, 15, 6, 1, 6, 36, 90, 120, 90, 36, 6, 15, 90, 225, 300, 225, 90, 15, 20, 120, 300, 400, 300, 120, 20, 15, 90, 225, 300, 225, 90, 15, 6, 36, 90, 120, 90, 36, 6, 1, 6, 15, 20, 15, 6, 1 };

const int laplacianFilter3x3[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
const int laplacianFilter5x5[25] = { 0, 0, 1, 0, 0, 0, 1, 2, 1, 0, 1, 2, -16, 2, 1, 0, 1, 2, 1, 0, 0, 0, 1, 0, 0 };
const int laplacianFilter7x7[49] = { 0, 0, 1, 1, 1, 0, 0, 0, 1, 3, 3, 3, 1, 0, 1, 3, 0, -7, 0, 3, 1, 1, 3, -7, -24, -7, 3, 1, 1, 3, 0, -7, 0, 3, 1, 0, 1, 3, 3, 3, 1, 0, 0, 0, 1, 1, 1, 0, 0 };

class ImpressionistUI {
public:
	ImpressionistUI();
	~ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice*			m_AngleTypeChoice;

	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_BrushWidthSlider;
	Fl_Slider*			m_BrushAngleSlider;
	Fl_Slider*			m_BrushAlphaSlider;
	Fl_Button*          m_ClearCanvasButton;
	Fl_Button*			m_DrawAutomatically;

	//For Filter kernel
	Fl_Window*			m_filterKernelDialog;
	Fl_Input*			m_divideByInput;
	Fl_Input*			m_offsetInput;
	Fl_Button*			m_applyButton;
	Fl_Button*			m_cancelButton;
	Fl_Button*			m_previewButton;
	Fl_Button*			m_applySobelButton;
	Fl_Button*			m_previewSobelButton;
	Fl_Group*			m_filterKernelGroup;
	Fl_Box*				m_filterBox;
	Fl_Choice*			m_FilterTypeChoice;
	Fl_Choice*			m_SobelFilterTypeChoice;
	Fl_Choice*			m_FilterSizeChoice;
	Fl_Light_Button*	m_imageChoiceButton;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute

	int					getSize();
	void				setSize(int size);
	int					getWidth();
	void				setWidth(int width);
	int					getAngle();
	void				setAngle(int angle);
	double				getAlpha();
	void				setAlpha(double alpha);

	// Interface to get filter kernel attributes
	double				getScale();
	double				getOffset();

	// Callbacks for the image filter dialogue (different from
	// the other callbacks because they aren't static)
	void				initFltDesignUI(void);
	void				updateFilter(void);

	void				cancelFilter(void);
	void				applyFilter(void);
	void				previewFilter(void);

	void				updateFilterGroupUsingSizeType(int sizeType);
	void				updateFilterGroupUsingFilterType(int filType);
	void				updateFilterGroupToOriginalFilter();
	void				updateFilterGroupToMeanFilter();
	void				updateFilterGroupToGaussianFilter();
	void				updateFilterGroupToLaplacianFilter();


private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nSize;
	int		m_nWidth;
	int		m_nAngle;
	double	m_nAlpha;
	bool	m_bPreviewMode;
	int		m_filterSize;
	bool	m_applyOnPainting;

	// These attributes are set by the filter kernel UI
	int		sobelFilterType;
	int		filterType;
	int		filterSizeType;
	double	fltKernel[FLT_WIDTH*FLT_HEIGHT];		//the kernel of the image filter
	double	scale,offset;						//the divisor and offset parameter, 
												//applied after convolving to transform 
												//the filtered image into the range [0,255]


	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];
	static Fl_Menu_Item		angleTypeMenu[NUM_ANGLE_TYPE+1];
	static Fl_Menu_Item		filterTypeMenu[NUM_FILTER_TYPE + 1];
	static Fl_Menu_Item		sobelFilterTypeMenu[NUM_SOBEL_FILTER + 1];
	static Fl_Menu_Item		filterSizeMenu[NUM_SIZE_TYPE + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_angleChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void	cb_widthSlides(Fl_Widget* o, void* v);
	static void	cb_angleSlides(Fl_Widget* o, void* v);
	static void	cb_alphaSlides(Fl_Widget* o, void* v);
	static void	cb_draw_automatically(Fl_Widget* o, void* v);

	//Filter Kernel Callbacks
	static void	cb_filter_kernel(Fl_Menu_* o, void* v);
	static void	cb_divideByInput(Fl_Widget* o, void* v);
	static void	cb_offsetInput(Fl_Widget* o, void* v);
	static void cb_applyButton(Fl_Widget* o, void* v);
	static void cb_previewButton(Fl_Widget* o, void* v);
	static void cb_cancelButton(Fl_Widget* o, void* v);
	static void	cb_filterTypeChoice(Fl_Widget* o, void* v);
	static void	cb_filterSizeChoice(Fl_Widget* o, void* v);
	static void	cb_sobelFilterChoice(Fl_Widget* o, void* v);
	static void cb_applySobelButton(Fl_Widget* o, void* v);
	static void cb_previewSobelButton(Fl_Widget* o, void* v);
	static void cb_imageChoiceButton(Fl_Widget* o, void* v);
};

#endif

