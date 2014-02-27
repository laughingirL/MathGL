// This is just simplest sample for parallel use of MathGL windows
// Function calc perform long "calculations" and update window on request
// Initialization and main loop are placed in main() function, as well as parallel call of calc() function

#include <unistd.h>
#if defined(USE_FLTK)
#include <mgl2/fltk.h>
mglWnd *gr=NULL;
#elseif defined(USE_QT)
#include <mgl2/qt.h>
mglWnd *gr=NULL;
#else
#include <mgl2/glut.h>
mglGLUT *gr=NULL;
#endif
//-----------------------------------------------------------------------------
void *calc(void *)
{
	mglPoint pnt;
	for(int i=0;i<10;i++)	// do calculation
	{
		sleep(1);           // which can be very long
		pnt = mglPoint(2*mgl_rnd()-1,2*mgl_rnd()-1);
		if(gr)
		{
			gr->Clf();			// make new drawing
			gr->Line(mglPoint(),pnt,"Ar2");
			char str[10] = "i=0";	str[3] = '0'+i;
			gr->Puts(mglPoint(),"");
			gr->Update();		// update window
		}
	}
	exit(0);
}
//-----------------------------------------------------------------------------
int main(int argc,char **argv)
{
	static pthread_t thr;
	pthread_create(&thr,0,calc,0);
	pthread_detach(thr);
#if defined(USE_FLTK)
	gr = new mglFLTK;
#elseif defined(USE_QT)
	gr = new mglQT;
#else
	gr = new mglGLUT;
#endif
	gr->Run();	return 0;
}
//-----------------------------------------------------------------------------
