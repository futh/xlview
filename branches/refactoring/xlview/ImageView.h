#ifndef XLVIEW_IMAGE_VIEW_H
#define XLVIEW_IMAGE_VIEW_H
#include "libxl/include/ui/Control.h"
#include "DisplayImage.h"
#include "ImageLoader.h"
#include "ImageManager.h"

#define ID_VIEW  99

class CDisplayParameter {
	bool loaded;
	bool suitable;
	int zoomTo;
	int zoomNow;
	int srcX;
	int srcY;
	CSize realSize;
	// CSize zoomSize;
	CRect rcView;

	int frameIndex;

	void _DrawSuitable (HDC, CImagePtr);
	void _DrawZoom (HDC, CImagePtr);

	void _CalacuteParameter ();

public:
	CDisplayParameter ();
	~CDisplayParameter ();
	void reset (CRect);

	void setRealSize (CSize);
	void setLoaded ();
	void setViewRect (CRect);

	bool isLoaded () const { return loaded; }
	CSize getRealSize () const { return realSize; }
	CSize getZoomToSize () const;
	CSize getZoomNowSize () const;
	int getZoomTo () const { return zoomTo; }
	int getZoomNow () const { return zoomNow; }

	bool showLarger ();
	bool onTimer ();

	void draw (HDC, CImagePtr);
	void drawLoading (HDC);
	void drawParameter (HDC);
};

//////////////////////////////////////////////////////////////////////////
// CImageView
class CImageView 
	: public xl::ui::CControl
	, public CImageManager::IObserver
	, public xl::CUserLock
{
protected:
	CImageManager     *m_pImageManager;

	CDisplayParameter   m_disp;
	CImagePtr          m_imageThumbnail; // cloned
	CImagePtr          m_imageZoomed; // cloned
	CImagePtr          m_imageRealSize; // point to m_pImageManager->m_image[curr]->getRealSizeImage();

	void _OnIndexChanged (int index);
	void _OnImageLoaded (int index);

	//////////////////////////////////////////////////////////////////////////
	// thread related
	bool m_exiting;
	HANDLE m_hZoomEvent;
	HANDLE m_hZoomThread;
	static unsigned __stdcall _ZoomThread (void *);
	void _CreateThreads ();
	void _TerminateThreads ();
	void _BeginZoom ();
public:
	CImageView(CImageManager *pImageManager);
	virtual ~CImageView(void);

	void showLarger ();

	//////////////////////////////////////////////////////////////////////////
	// virtual

	// xl::ui::CControl
	virtual void onSize ();
	virtual void drawMe (HDC hdc);

	virtual void onLButtonDown (CPoint pt, xl::uint key);
	virtual void onLButtonUp (CPoint pt, xl::uint key);
	virtual void onMouseMove (CPoint pt, xl::uint key);
	virtual void onTimer (xl::uint id);
	virtual void onLostCapture ();

	// CImageManager::IObserver
	virtual void onEvent (EVT evt, void *param);
};

#endif
