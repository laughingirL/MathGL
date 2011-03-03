/*
 * simple.cpp - simple demo for FTGL, the OpenGL font library
 *
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <math.h> // sin(), cos()
#include <stdlib.h> // exit()
#include <locale.h>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <FTGL/ftgl.h>

#include "FTVector.h"
#include "FTVectoriser.h"
/*
static FTFont *font[3];
static int fontindex = 0;
static int lastfps = 0;
static int frames = 0;
*/
typedef struct
{
	size_t first;
	size_t second;
	size_t third;
} FTTriangle;
typedef FTVector<FTTriangle> FTTriangleVector;
typedef FTVector<FTPoint> FTPointVector;

class FTScreenBuffer
{
public:
	FTScreenBuffer() : width(0), bBox(1048576.0f, 1048576.0f, 1048576.0f, -1048576.0f, -1048576.0f, -1048576.0f) {}
	void clear()
	{
		contourpoints.clear();
		points.clear();
		triangles.clear();
		width = 0;
		bBox = FTBBox ( 1048576.0f, 1048576.0f, 1048576.0f, -1048576.0f, -1048576.0f, -1048576.0f );
	}
	
	
	double width;
	FTBBox bBox;
	FTTriangleVector triangles;
	FTPointVector points;
	FTPointVector contourpoints;
};
FTScreenBuffer screenBuffer;

class TriangleGlyph
{
public:
/*	TriangleGlyph& operator =(const TriangleGlyph& v)
	{
        code = v.code;
        width = v.width;
        bBox = v.bBox;
        triangles = v.triangles;
        points = v.points;
        contourpoints = v.contourpoints;
		return *this;
	} */
//    ~TriangleGlyph()
//    {
//        triangles.clear();
//        points.clear();
//        contourpoints.clear();
//    }
    unsigned int code;
	
    double width;
    FTBBox bBox;
    FTTriangleVector triangles;
    FTPointVector points;
	FTPointVector contourpoints;
};
typedef FTVector<TriangleGlyph> TriangleGlyphVector;
TriangleGlyphVector TriangleFont;
FTBBox TriangleFontBBox(1048576.0, 1048576.0, 0, -1048576.0, -1048576.0, 0);

/**
 * FTvtkGlyph is a specialisation of FTGlyph for creating tessellated
 * polygon glyphs in memory.
 *
 */

class FTVectoriser;

class FTGL_EXPORT FTVtkGlyph : public FTGlyph
{
public:
	/**
	 * Constructor. Sets the Error to Invalid_Outline if the glyphs
	 * isn't an outline.
	 *
	 * @param glyph The Freetype glyph to be processed
	 */
	FTVtkGlyph(FT_GlyphSlot glyph);
	
	/**
	 * Destructor
	 */
	// virtual ~FTVtkGlyph();
	
	/**
	 * Render this glyph at the current pen position.
	 *
	 * @param pen  The current pen position.
	 * @param renderMode  Render mode to display
	 * @return  The advance distance for this glyph.
	 */
	virtual const FTPoint& Render(const FTPoint& pen, int renderMode);
	
	/**
	 * Return the advance width for this glyph.
	 *
	 * @return  advance width.
	 */
	virtual float Advance() const;
	
	/**
	 * Return the bounding box for this glyph.
	 *
	 * @return  bounding box.
	 */
	virtual const FTBBox& BBox() const;
	
	/**
	 * Queries for errors.
	 *
	 * @return  The current error code.
	 */
	virtual FT_Error Error() const;
	
private:
	/**
	 * The advance distance for this glyph
	 */
	FTPoint advance;
	
	/**
	 * The bounding box of this glyph.
	 */
	FTBBox bBox;
	
	/**
	 * Current error code. Zero means no error.
	 */
	FT_Error err;
	
	/**
	 * Private rendering variables.
	 */
	// unsigned int hscale, vscale;
	FTTriangleVector triangles;
	FTPointVector points;
	FTPointVector contourpoints;
	size_t AddPoint ( const FTPoint& Point );
	void AddTriangle ( const FTPoint& Point0, const FTPoint& Point1, const FTPoint& Point2 );
};

FTVtkGlyph::FTVtkGlyph(FT_GlyphSlot glyph) : FTGlyph(glyph), err(0)
{
    if(glyph)
    {
        bBox = FTBBox(glyph);
        advance = FTPoint(glyph->advance.x / 64.0f,
                          glyph->advance.y / 64.0f);
    }
	
    if(ft_glyph_format_outline != glyph->format)
    {
        err = 0x14; // Invalid_Outline
        return;
    }
	
    FTVectoriser vectoriser ( glyph );
	
    if ( ( vectoriser.ContourCount() < 1 ) || ( vectoriser.PointCount() < 3 ) )
    {
		return;
    }
	
    // hscale = glyph->face->size->metrics.x_ppem * 64;
    // vscale = glyph->face->size->metrics.y_ppem * 64;
	
	for ( size_t c = 0; c < vectoriser.ContourCount(); ++c )
    {
        const FTContour* contour = vectoriser.Contour ( c );
		
        for ( size_t pointIndex = 0; pointIndex < contour->PointCount(); ++pointIndex )
        {
            FTPoint point = contour->Point ( pointIndex );
            contourpoints.push_back( ( 1/64.0 ) *  point );
        }
        contourpoints.push_back(FTPoint ( -1024.0, -1024.0, -1024.0 ));
    }
	
	vectoriser.MakeMesh(1.0, 0, 0);
	
    const FTMesh *mesh = vectoriser.GetMesh();
	
    for ( size_t index = 0; index < mesh->TesselationCount(); ++index )
    {
        const FTTesselation* subMesh = mesh->Tesselation ( index );
        GLenum polygonType = subMesh->PolygonType();
		
        if ( polygonType == GL_TRIANGLE_STRIP )
        {
			size_t PointCount = subMesh->PointCount();
			
			for ( size_t x = 2; x < PointCount; ++x )
			{
				if ( x % 2 )
				{
					AddTriangle ( subMesh->Point ( x-1 ), subMesh->Point ( x-2 ), subMesh->Point ( x ) );
				}
				else
				{
					AddTriangle ( subMesh->Point ( x-2 ), subMesh->Point ( x-1 ), subMesh->Point ( x ) );
				}
			}
        }
        else if ( polygonType == GL_TRIANGLE_FAN )
        {
            size_t PointCount = subMesh->PointCount();
			
            for ( size_t x = 2; x < PointCount; ++x )
            {
                AddTriangle ( subMesh->Point ( 0 ), subMesh->Point ( x-1 ), subMesh->Point ( x ) );
            }
        }
        else if ( polygonType == GL_TRIANGLES )
        {
            size_t PointCount = subMesh->PointCount();
			
            for ( size_t x = 0; x < PointCount; x += 3 )
            {
                AddTriangle ( subMesh->Point ( x+0 ), subMesh->Point ( x+1 ), subMesh->Point ( x+2 ) );
            }
        }
    }
}

float FTVtkGlyph::Advance() const
{
    return advance.Xf();
}


const FTBBox& FTVtkGlyph::BBox() const
{
    return bBox;
}


FT_Error FTVtkGlyph::Error() const
{
    return err;
}

const FTPoint& FTVtkGlyph::Render(const FTPoint& pen, int renderMode)
{
    if ( triangles.size() != 0 )
    {
        screenBuffer.width += advance.X();
        FTBBox tmpBox = bBox;
        tmpBox += pen;
        screenBuffer.bBox |= tmpBox;
		
        size_t pointsNumber = screenBuffer.points.size();
		
        for ( size_t i = 0 ; i < points.size(); i++ )
        {
            screenBuffer.points.push_back ( points[i] + pen );
        }
        for ( size_t i = 0 ; i < triangles.size(); i++ )
        {
            FTTriangle triangle;
            triangle.first  = triangles[i].first  + pointsNumber;
            triangle.second = triangles[i].second + pointsNumber;
            triangle.third  = triangles[i].third  + pointsNumber;
            screenBuffer.triangles.push_back ( triangle );
        }
    }
    for ( size_t i = 0 ; i < contourpoints.size(); i++ )
        screenBuffer.contourpoints.push_back ( contourpoints[i] + pen );
    return advance;
}


size_t FTVtkGlyph::AddPoint ( const FTPoint& Point )
{
    FTPoint NewPoint = Point;
    NewPoint = ( 1/64.0 ) * NewPoint;
    for ( size_t i = 0; i < points.size(); i++ )
    {
        if ( points[i] == NewPoint )
        {
            return ( i );
        }
    }
    points.push_back( NewPoint );
    return ( points.size()-1 );
}

void FTVtkGlyph::AddTriangle ( const FTPoint& Point0, const FTPoint& Point1, const FTPoint& Point2 )
{
    if ( Point0.X() == Point1.X() &&
		Point0.X() == Point2.X()   ||
		Point0.Y() == Point1.Y() &&
		Point0.Y() == Point2.Y() )
    {
        /* degenerate triangle */
    }
    else
    {
        FTTriangle triangle;
        triangle.first  = AddPoint ( Point0 );
        triangle.second = AddPoint ( Point1 );
        triangle.third  = AddPoint ( Point2 );
        triangles.push_back(triangle);
    }
}

class FTVtkFont : public FTFont
{
public:
	FTVtkFont(char const *fontFilePath) : FTFont(fontFilePath) { GlyphLoadFlags(FT_LOAD_NO_HINTING); }
	
private:
	virtual FTGlyph* MakeGlyph(FT_GlyphSlot slot)
	{
		return new FTVtkGlyph(slot);
	}
};

// default font
const char *default_font = "STIXGeneral.otf";
const char *filename=NULL;
// default point size
int point_size =14;
// default scale down factor to translate (FT)GL units to vtk units
int scaleFactor=10;

//
//  Main program entry point: set up GLUT window, load fonts, run GLUT loop.
//
int main ( int argc, char **argv )
{
    setlocale ( LC_ALL, "en_US.UTF-8" );
    if ( argc < 2 )
    {
//        std::cerr << "Usage: " << argv[0] << " <filename.ttf> point_size downscale_factor\n" << std::endl;
//        exit ( -1 );
		filename = default_font;
    }
	else
		filename = argv[ 1 ];
	
    {
        FILE * fp = fopen ( filename, "r" );
		
        if ( fp == NULL )
        {
            std::cerr << "Couldn't open file '" << argv[ 1 ] << "'" << std::endl;
            exit ( -1 );
        }
        fclose ( fp );
    }
    if ( argc >= 3 )
    {
        point_size = atoi ( argv[ 2 ] );
    }
    if ( argc >= 4 )
    {
        scaleFactor = atoi ( argv[ 3 ] );
    }
	static FTFont *fontT;
    fontT = new FTVtkFont ( filename );
    if ( fontT->Error() )
    {
        std::cerr << "ERROR: Unable to open font " << filename << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "Reading font from " << filename << std::endl;
		
        if ( !fontT->FaceSize ( point_size ) )
        {
            std::cerr << "ERROR: Unable to set font face size " << point_size << std::endl;
        }
    }
	
    /* Set up some strings with the characters to draw. */
    wchar_t wstring[2];
    register unsigned int chr;
	
    wstring[1] = '\0';
    
    double ww = 0;    // maximal width
	
    for ( chr=' '; chr<0xFFFF; chr++ )
    // for ( chr=' '; chr<0xFF; chr++ )
    {
// #undef SMALL_SET
// #ifdef SMALL_SET
        // if(chr/256!=0 && chr/256!=0x03 && chr/256!=0x21 && chr/256!=0x21)
     //   if(chr!=0x21)
     //       continue;
// #endif
		
		// print one character string
        wstring[0] = chr;
		// clear the screen buffer
        screenBuffer.clear();
        fontT->Render ( wstring );
		// if something gets printed, copy output to glyph structure and add it to font
        if ( screenBuffer.triangles.size() > 0 )
        {
            {
			TriangleGlyph glyph;
            glyph.code = chr;
            glyph.width = screenBuffer.width;
            ww = ww>glyph.width ? ww:glyph.width;
            glyph.bBox = screenBuffer.bBox;
            TriangleFontBBox |= glyph.bBox;
			glyph.points = screenBuffer.points;
		    glyph.triangles = screenBuffer.triangles;
            glyph.contourpoints = screenBuffer.contourpoints;
            TriangleFont.push_back ( glyph );
            }
			/*{
            LineGlyph glyph;
            glyph.code = chr;
            glyph.width = screenBuffer.width;
            // ww = ww>glyph.width ? ww:glyph.width;
            glyph.bBox = screenBuffer.bBox;
            LineFontBBox |= glyph.bBox;
            for ( size_t i = 0 ; i < screenBuffer.contourpoints.size(); i++ )
                glyph.points.push_back ( screenBuffer.contourpoints[i] );
            LineFont.push_back ( glyph );
			}*/
        }
    }
	
    // print out the result
	
    FILE *fout;
    /// Saving in "binary" format
    char fname[128];
    sprintf(fname,"%s.vfm",filename);
    fout   = fopen ( fname,"w" );
    // some general comments
    fprintf ( fout, "# font %s has %zu glyphs \n", filename, TriangleFont.size() );
    double d1 = 500./fontT->Ascender(), d2 = -500./fontT->Descender(), d3 = 1000./ww, dx;
    dx = d1<d2 ? d1:d2;        dx = dx<d3 ? dx:d3;
    unsigned i, *ids, *posl, *post, len, cur, numg=TriangleFont.size();
    short *wdt, *numl, *numt, *buf, *xx, *yy, *tt;
    
    for ( chr =0, len=0; chr < numg; chr++ )
        len += TriangleFont[chr].contourpoints.size();
    for ( chr =0; chr < numg; chr++ )
        len += 3*TriangleFont[chr].points.size();
    
    ids = new unsigned[numg];
    wdt = new short[numg];
    numl= new short[numg];
    posl= new unsigned[numg];
    numt= new short[numg];
    post= new unsigned[numg];
    buf = new short[2*len];
	printf("Allocated %d\n",2*len);
    // prepare wire font data
    for ( chr =0, cur=0; chr < numg; chr++ )
    {
        const TriangleGlyph& glyph = TriangleFont[chr];
        // removing the points in the same line
        xx = new short[glyph.contourpoints.size()];
        yy = new short[glyph.contourpoints.size()];
        for ( i = 0 ; i < glyph.contourpoints.size(); i++ )
        {
            xx[i] = glyph.contourpoints[i].X()>-500. ? int(dx*glyph.contourpoints[i].X()+0.5) : 0x3fff;
            yy[i] = glyph.contourpoints[i].Y()>-500. ? int(dx*glyph.contourpoints[i].Y()+0.5) : 0x3fff;
        }
        unsigned last = 0;
        for ( i = 0 ; i < glyph.contourpoints.size(); i++ )
        {
            if(i>0 && i<glyph.contourpoints.size())
            {
                if(((xx[i]-xx[last-1])*(yy[i+1]-yy[last-1]) == (yy[i]-yy[last-1])*(xx[i+1]-xx[last-1])) &&
				   xx[i]!=0x3fff && yy[i]!=0x3fff && xx[i+1]!=0x3fff && yy[i+1]!=0x3fff)
                    continue;
            }
            xx[last] = xx[i];    yy[last] = yy[i];    last++;
        }
        if(xx[last-1]==0x3fff || yy[last-1]==0x3fff)    last--;
        if(xx[last-1]==xx[0] && yy[last-1]==yy[0])    last--;
        
        // write glyph to tables
        ids[chr] = glyph.code;    numl[chr] = last;    posl[chr] = cur;
        wdt[chr] = short(glyph.width*dx);
        for ( i = 0 ; i < last; i++ )    {    buf[cur++] = xx[i];    buf[cur++] = yy[i];    }
        delete []xx;    delete []yy;
    }
    // prepare solid font data
    for ( chr =0; chr < TriangleFont.size(); chr++ )
    {
        const TriangleGlyph& glyph = TriangleFont[chr];
        // removing the points in the same line
        xx = new short[glyph.points.size()];
        yy = new short[glyph.points.size()];
        tt = new short[6*glyph.triangles.size()];
        for ( i = 0 ; i < glyph.points.size(); i++ )
        {
            xx[i] = short(dx*glyph.points[i].X()+0.5);
            yy[i] = short(dx*glyph.points[i].Y()+0.5);
        }
        unsigned last = 0, ii, ll;
        for ( i = 0 ; i < glyph.triangles.size(); i++ )
        {
            ii = 6*i;    ll = 6*last;
            tt[ii]   = xx[glyph.triangles[i].first];
            tt[ii+1] = yy[glyph.triangles[i].first];
            tt[ii+2] = xx[glyph.triangles[i].second];
            tt[ii+3] = yy[glyph.triangles[i].second];
            tt[ii+4] = xx[glyph.triangles[i].third];
            tt[ii+5] = yy[glyph.triangles[i].third];
            if(i>0 && i<glyph.triangles.size())
            {
                if( tt[ii]==tt[ii+2] && tt[ii+1]==tt[ii+3] )    continue;
                if( tt[ii]==tt[ii+4] && tt[ii+1]==tt[ii+5] )    continue;
                if( tt[ii+2]==tt[ii+4] && tt[ii+3]==tt[ii+5] )    continue;
            }
            memcpy(tt+ll,tt+ii,6*sizeof(int));    last++;
        }
        // write glyph to tables
        numt[chr] = last;    post[chr] = cur;
        for ( i = 0 ; i < last; i++ )
        {
            ii = 6*i;
            buf[cur++] = tt[ii];    buf[cur++] = tt[ii+1];    buf[cur++] = tt[ii+2];
            buf[cur++] = tt[ii+3];    buf[cur++] = tt[ii+4];    buf[cur++] = tt[ii+5];
        }
        delete []xx;    delete []yy;    delete []tt;
    }
	printf("Used: %d\n",cur);
#ifdef SMALL_SET
    // write general font parameters
    fprintf ( fout, "short mgl_numg=%d, mgl_fact=%g, mgl_cur=%d\n", numg, dx, cur);
    // now write general tables
    fprintf(fout,"int mgl_gen_fnt[%d][6] = {\n",numg);
    for(i=0;i<numg;i++)
        fprintf(fout,"\t{%u, %d, %d, %u, %d, %u},\n",ids[i], wdt[i], numl[i], posl[i], numt[i], post[i]);
    fprintf(fout,"};\nshort mgl_buf_fnt[%d] = {\n",cur);
    // now write glyph descriptions
    for(i=0;i<cur;i++)
    {    fprintf(fout,"%d,",buf[i]);    if(i%512==0)    fprintf(fout,"\n");    }
    fprintf(fout,"};\n");
#else
    // write general font parameters
    fprintf ( fout, "%d %g %d\n", numg, dx, cur);
    // now write general tables
    for(i=0;i<numg;i++)
        fprintf(fout,"%u %d %d %u %d %u\n",ids[i], wdt[i], numl[i], posl[i], numt[i], post[i]);
    // now write glyph descriptions
    for(i=0;i<cur;i++)    fprintf(fout,"%d\t",buf[i]);
    fprintf(fout,"\n");
#endif
	/*    // now write general tables
	 fwrite(ids,1,numg*sizeof(short),fout);
	 fwrite(wdt,1,numg*sizeof(short),fout);
	 // now write tables for wire fonts
	 fwrite(numl,1,numg*sizeof(short),fout);
	 fwrite(posl,1,numg*sizeof(unsigned),fout);
	 // now write tables for solid fonts
	 fwrite(numt,1,numg*sizeof(short),fout);
	 fwrite(post,1,numg*sizeof(unsigned),fout);
	 // now write glyph descriptions
	 fwrite(buf,1,cur*sizeof(short),fout);*/
    fclose ( fout );
    delete []ids;    delete []numl;    delete []numt;    delete []wdt;
    delete []buf;    delete []posl;    delete []post;
	
    return EXIT_SUCCESS;
}

