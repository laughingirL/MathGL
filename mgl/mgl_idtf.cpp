/***************************************************************************
 * mgl_idtf.cpp is part of Math Graphic Library
 * Copyright (C) 2008 Michail Vidiassov <balakin@appl.sci-nnov.ru>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <wchar.h>
#include <memory.h>
#if(!defined(PATH_MAX))
#define PATH_MAX	256
#endif
#ifdef WIN32
#define bzero(a,b) memset(a,0,b)
#endif
#include "mgl/mgl_idtf.h"
#include <iomanip>
#ifndef MAXFLOAT
#define MAXFLOAT	1e30
#endif
#define IDTFROUND(x) int((x)*1000000.0f+0.5f)/1000000.0f
// #define IDTFROUND(x) ldexpf(roundf(ldexpf((x),20)),-20)
// const static bool dbg = true;

/*
Here is the description of supported TGA format

DATA TYPE 2: Unmapped RGB

| Offset | Length |                     Description                            |
|--------|--------|------------------------------------------------------------|
|    0   |     1  |  Number of Characters in Identification Field.             |
|        |        |                                                            |
|        |        |  This field is a one-byte unsigned integer, specifying     |
|        |        |  the length of the Image Identification Field.  Its value  |
|        |        |  is 0 to 255.  A value of 0 means that no Image            |
|        |        |  Identification Field is included.                         |
|--------|--------|------------------------------------------------------------|
|    1   |     1  |  Color Map Type.                                           |
|        |        |                                                            |
|        |        |  This field contains 0.                                    |
|--------|--------|------------------------------------------------------------|
|    2   |     1  |  Image Type Code.                                          |
|        |        |                                                            |
|        |        |  This field will always contain a binary 2.                |
|        |        |  ( That's what makes it Data Type 2 ).                     |
|--------|--------|------------------------------------------------------------|
|    3   |     5  |  Color Map Specification.                                  |
|        |        |                                                            |
|        |        |  Ignored                                                   |
|--------|--------|------------------------------------------------------------|
|    8   |    10  |  Image Specification.                                      |
|        |        |                                                            |
|    8   |     2  |  X Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) X coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|   10   |     2  |  Y Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) Y coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|   12   |     2  |  Width of Image.                                           |
|        |        |  Integer ( lo-hi ) width of the image in pixels.           |
|   14   |     2  |  Height of Image.                                          |
|        |        |  Integer ( lo-hi ) height of the image in pixels.          |
|   16   |     1  |  Image Pixel Size.                                         |
|        |        |  Number of bits in a pixel.  This is 24 for Targa 24,      |
|        |        |  32 for Targa 32                                           |
|   17   |     1  |  Image Descriptor Byte.                                    |
|        |        |  Not used                                                  |
|--------|--------|------------------------------------------------------------|
|   18   | varies |  Image Identification Field.                               |
|        |        |                                                            |
|        |        |  Contains a free-form identification field of the length   |
|        |        |  specified in byte 1 of the image record.  It's usually    |
|        |        |  omitted ( length in byte 1 = 0 ), but can be up to 255    |
|        |        |  characters.  If more identification information is        |
|        |        |  required, it can be stored after the image data.          |
|--------|--------|------------------------------------------------------------|
| varies | varies |  Image Data Field.                                         |
|        |        |                                                            |
|        |        |  This field specifies (width) x (height) pixels.  Each     |
|        |        |  pixel specifies an RGB color value, which is stored as    |
|        |        |  an integral number of bytes.                              |
|        |        |  The 3 byte entry contains 1 byte each of blue, green,     |
|        |        |  and red.                                                  |
|        |        |  The 4 byte entry contains 1 byte each of blue, green,     |
|        |        |  red, and attribute.                                       |
--------------------------------------------------------------------------------

*/


typedef struct _TgaHeader
{
    uint8_t numIden;
    uint8_t colorMapType;
    uint8_t imageType;
    uint8_t colorMapSpec[5]; // not used, just here to take up space
    uint8_t origX[2];
    uint8_t origY[2];
    uint8_t width[2];
    uint8_t height[2];
    uint8_t bpp;
    uint8_t imageDes; // don't use, space eater
} TgaHeader;

TGAImage::TGAImage()
{
	Width = 0;
	Height = 0;
	Channels = 0;
	RGBPixels = NULL;
}

TGAImage::~TGAImage()
{
	Deallocate();
}

void TGAImage::Deallocate()
{
	if(RGBPixels)
	{
		delete[] RGBPixels;
	}

	RGBPixels = NULL;

	Width = 0;
	Height = 0;
	Channels = 0;
}

bool TGAImage::Write( const char* pFileName ) const
{
	TgaHeader header;
	uint8_t* BGRPixels = NULL; // BGRA
// fprintf(stderr, "name %s width %u height %u channels %u pixels %p\n", pFileName, Width, Height, Channels, RGBPixels );
    	bool ret = true;
	FILE* outFile = NULL;

	if( !RGBPixels )
	{
		ret = false;
	}

	if( ret )
	{
		outFile = fopen( pFileName, "wb" );

		if( !outFile )
			ret = false;
	}

	if( ret )
	{
		// first attemp to write TGA image
		BGRPixels = new uint8_t[ Width * Height * Channels ];
		if( NULL != BGRPixels && NULL != RGBPixels )
		{
			// R and B channels reordering
			uint32_t i;
			for( i = 0; i < Width * Height * Channels; i += Channels )
			{
				BGRPixels[i] = RGBPixels[i+2]; // R->B
				BGRPixels[i+1] = RGBPixels[i+1]; // G->G
				BGRPixels[i+2] = RGBPixels[i]; // B->R
				if( 4 == Channels )
					BGRPixels[i+3] = RGBPixels[i+3]; // A->A
			}
		}
		else
			ret = false;
	}

	if( ret )
	{
		memset(&header,0, sizeof(TgaHeader));
		header.imageType = 2;
		header.width[0] = Width % 256; header.width[1] = Width / 256;
		header.height[0] = Height % 256; header.height[1] = Height / 256;
		header.bpp = Channels*8;

		size_t count = fwrite( &header, sizeof(TgaHeader), 1, outFile );
		// if file header was not successfully written
		if( 1 != count )
		{
			ret = false;
		}
	}

	if( ret )
	{
		size_t count =
			fwrite( BGRPixels, Width * Height * Channels, 1, outFile );

		// if file data was not successfully written
		if( 1 != count )
		{
			ret = false;
		}
	}

	if( outFile )
	{
		fclose( outFile );
	}

	if( BGRPixels )
	{
		delete[] BGRPixels;
	}

	return ret;
}

bool TGAImage::Initialize( uint32_t width, uint32_t height, uint32_t channels )
{
	bool result = true;

	if(width < 1 || height < 1 || (channels != 3 && channels != 4))
	{
		result = false;
	}
	else
	{
		Height = height;
		Width = width;
		Channels = channels;

		RGBPixels = new uint8_t[ width * height * channels ];
		if( !RGBPixels )
			result = false;
	}

	return result;
}

//-----------------------------------------------------------------------------
/// Create mglGraph object in IDTF mode.
HMGL mgl_create_graph_idtf()
{	return new mglGraphIDTF;	}
/// Create mglGraph object in IDTF mode.
uintptr_t mgl_create_graph_idtf_()
{	return uintptr_t(new mglGraphIDTF);	}
//-----------------------------------------------------------------------------
// helper output routines
//-----------------------------------------------------------------------------
inline std::string i2s ( int x )
{
	std::ostringstream o;
	o << x;
	return o.str();
};
//-----------------------------------------------------------------------------
#define IDTFPrintVector3(x) \
  (x)[0] << " " << (x)[1] << " " << (x)[2]
//-----------------------------------------------------------------------------
#define IDTFPrintVector4(x) \
  (x)[0] << " " << (x)[1] << " " << (x)[2] << " " << (x)[3]
//-----------------------------------------------------------------------------
#define sign(x) ((x<0.0) ? (-1.0) : (1.0))

static float mgl_globpos[4][4] = { {0.5, 0, 0, 0.5}, {0, 0.5, 0, 0.5}, {0, 0, 0.5, 0.5}, {0, 0, 0, 1} };
static float mgl_globinv[4][4] = { {2, 0, 0, -1}, {0, 2, 0, -1}, {0, 0, 2, -1}, {0, 0, 0, 1} };
const float mgl_definv[4][4] = { {2, 0, 0, -1}, {0, 2, 0, -1}, {0, 0, 2, -1}, {0, 0, 0, 1} };
const float mgl_idtrans[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

//-----------------------------------------------------------------------------
// u3d object methods
//-----------------------------------------------------------------------------
void u3dNode::print ( std::ofstream& ostr )
{
	ostr
	<< "NODE \"" << this->type << "\" {\n"
	<< "\tNODE_NAME \"" << this->name << "\"\n"
	<< "\tPARENT_LIST {\n"
	<< "\t\tPARENT_COUNT 1\n"
	<< "\t\tPARENT 0 {\n"
	<< "\t\t\tPARENT_NAME \"" << ( this->parent.empty() ? "<NULL>" : this->parent ) << "\"\n"
	<< "\t\t\tPARENT_TM {\n";
	for ( int i=0; i<4;i++ )
		ostr << "\t\t\t\t" << IDTFPrintVector4 ( position[i] ) << "\n";
	ostr << "\t\t\t}\n"
	<< "\t\t}\n"
	<< "\t}\n";
	if ( this->type != "GROUP" )
	{
		ostr << "\tRESOURCE_NAME \"" << ( this->resource.empty() ? this->name : this->resource ) << "\"\n";
	}
	if ( this->type == "MODEL" && both_visible )
	{
		ostr << "\tMODEL_VISIBILITY \"BOTH\"\n";
	}
	ostr << "}\n"
	<< "\n";
};

void u3dLight::print_light_resource ( std::ofstream& ostr )
{
	ostr
	<< "\t\tRESOURCE_NAME \"" << this->name << "\"\n"
	<< "\t\tLIGHT_TYPE \"" << this->type << "\"\n"
	<< "\t\tLIGHT_COLOR " << this->color.r << " "  << this->color.g << " " << this->color.b << "\n"
	<< "\t\tLIGHT_ATTENUATION " << this->attenuation << " 0 0\n"
	<< "\t\tLIGHT_INTENSITY " << this->intensity << "\n";
};
void u3dLight::print_node ( std::ofstream& ostr )
{
	u3dNode Node;
	Node.name = name;
//	for(int i=0; i<3; i++)
//		for(int j=0; j<3; j++)
//			Node.position[i][j]=this->position[i][j];
	memcpy ( Node.position,this->position, sizeof ( Node.position ) );
	Node.type = "LIGHT";
	Node.print ( ostr );
}

void mglGraphIDTF::SetAmbientLight ( mglColor c, float br )
{
	u3dLight Light;

	Light.name = "AmbientLight";
	Light.type = "AMBIENT";
	Light.attenuation = 1.0;
	Light.intensity = br >= 0.0 ? br : this->AmbBr;;
	Light.color = c;
	memcpy ( Light.position, mgl_idtrans, sizeof ( mgl_idtrans ) );
	Lights.push_back ( Light );
}
void mglGraphIDTF::AddLight ( mglPoint p, mglColor color, float br, bool infty )
{
	u3dLight Light;

	Light.color = color;
	Light.attenuation = 1.0;
	Light.intensity = br;
	if ( Light.name.empty() )
	{
		Light.name = "Light" + i2s ( Lights.size() );
	}

	memcpy ( Light.position, mgl_idtrans, sizeof ( mgl_idtrans ) );
	float a = p.x, b = p.y, c = p.z;
	if ( infty )
	{
		Light.type = "DIRECTIONAL";
		float n = sqrt ( a*a+b*b+c*c );
		if ( n != 0.0f )
		{
			a /= n; b /= n; c /=n;
		}
		else
			return;
		if ( sqrt ( a*a+b*b ) != 0.0 )
		{
//            -b/sqrt(a*a+b*b)   -a/sqrt(a*a+b*b)           0.0
//          -a*c/sqrt(a*a+b*b) -b*c/sqrt(a*a+b*b) sqrt(a*a+b*b)
//                    a                  b             c
			Light.position[0][0] =   -b/sqrt ( a*a+b*b );
			Light.position[0][1] =   -a/sqrt ( a*a+b*b );
			Light.position[1][0] = -a*c/sqrt ( a*a+b*b );
			Light.position[1][1] = -b*c/sqrt ( a*a+b*b );
			Light.position[1][2] = sqrt ( a*a+b*b );
			Light.position[2][0] = a;
			Light.position[2][1] = b;
			Light.position[2][2] = c;
		}
		else
		{
//          1 0 0
//          0 1 0
//          0 0 sign(c)
			Light.position[2][2] = sign ( c );
		}
	}
	else
	{
		Light.type = "POINT";
		Light.position[3][0] = a;
		Light.position[3][1] = b;
		Light.position[3][2] = c;
	}

	Lights.push_back ( Light );
};

void u3dMaterial::print_material ( std::ofstream& ostr )
{
	ostr
	<< "\t\tRESOURCE_NAME \"" << name << "\"\n"
	<< "\t\tMATERIAL_AMBIENT 0 0 0\n"
	<< "\t\tMATERIAL_DIFFUSE "   << diffuse.r  << " " << diffuse.g  << " " << diffuse.b  << "\n"
	<< "\t\tMATERIAL_SPECULAR "  << specular.r << " " << specular.g << " " << specular.b << "\n"
	<< "\t\tMATERIAL_EMISSIVE "  << emissive.r << " " << emissive.g << " " << emissive.b << "\n"
	<< "\t\tMATERIAL_REFLECTIVITY " << reflectivity << "\n"
	<< "\t\tMATERIAL_OPACITY " << opacity << "\n";
}

void u3dMaterial::print_shader ( std::ofstream& ostr )
{
	ostr << "\t\tRESOURCE_NAME \"" << this->name << "\"\n";
	if ( this->texture.empty()  && this->vertex_color )
	{
		ostr << "\t\tATTRIBUTE_USE_VERTEX_COLOR \"TRUE\"\n";
	}
	ostr << "\t\tSHADER_MATERIAL_NAME \"" << this->name << "\"\n";
	if ( this->texture.empty() )
		ostr
		<< "\t\tSHADER_ACTIVE_TEXTURE_COUNT 0\n";
	else
		ostr
		<< "\t\tSHADER_ACTIVE_TEXTURE_COUNT 1\n"
		<< "\t\tSHADER_TEXTURE_LAYER_LIST {\n"
		<< "\t\t\tTEXTURE_LAYER 0 {\n"
		<< "\t\t\t\tTEXTURE_LAYER_BLEND_FUNCTION \"REPLACE\"\n"
		<< "\t\t\t\tTEXTURE_LAYER_ALPHA_ENABLED \"" << (this->texturealpha ? "TRUE" : "FALSE") << "\"\n"
//		<< "\t\t\t\tTEXTURE_LAYER_REPEAT \"NONE\"\n"
		<< "\t\t\t\tTEXTURE_NAME \"" << this->texture << "\"\n"
		<< "\t\t\t}\n"
		<< "\t\t}\n";
}

size_t mglGraphIDTF::AddMaterial ( const u3dMaterial& Material )
{
	size_t mid;
	for ( mid = 0; mid < Materials.size(); mid++ )
	{
		if ( Materials[mid] == Material )
		{
			return mid;
		}
	}

	Materials.push_back ( Material );
	Materials.back().name = "Material" + i2s ( mid );
	return ( mid );
}

void u3dTexture::print_texture ( const char *fname, std::ofstream& ostr )
{
	char filename[PATH_MAX];
	bzero( filename, sizeof(filename) );
	const size_t fnlen = strlen(fname);
	if ( fnlen > 5 && strcasecmp(fname+fnlen-5, ".idtf") == 0 )
		strncpy(filename, fname, ( sizeof(filename)-1 > fnlen-5 ) ? (fnlen-5) : (sizeof(filename)-1) );
	else
		strncpy(filename, fname, sizeof(filename)-1);
	strncat(filename, this->name.c_str(), sizeof(filename)-strlen(filename)-5);
	strcat(filename, ".tga" );
	ostr
	<< "\t\tRESOURCE_NAME \"" << this->name << "\"\n"
	<< "\t\tTEXTURE_IMAGE_TYPE \"" << (this->image.Channels == 4 ? "RGBA" : "RGB") << "\"\n"
	<< "\t\tTEXTURE_IMAGE_COUNT 1\n"
	<< "\t\tIMAGE_FORMAT_LIST {\n"
	<< "\t\t        IMAGE_FORMAT 0 {\n"
	<< "\t\t                COMPRESSION_TYPE \"PNG\"\n"
	<< "\t\t                ALPHA_CHANNEL \"" << (this->image.Channels == 4 ? "TRUE" : "FALSE") << "\"\n"
	<< "\t\t                BLUE_CHANNEL \"TRUE\"\n"
	<< "\t\t                GREEN_CHANNEL \"TRUE\"\n"
	<< "\t\t                RED_CHANNEL \"TRUE\"\n"
	<< "\t\t        }\n"
	<< "\t\t}\n"
	<< "\t\tTEXTURE_PATH \"" << filename << "\"\n";
	this->image.Write( filename );
}

u3dTexture& mglGraphIDTF::AddTexture()
{
	u3dTexture Texture;
	Textures.push_back ( Texture );
	Textures.back().name = "Texture" + i2s ( Textures.size() );
	Textures.back().image.Width = 0;
	Textures.back().image.Height = 0;
	Textures.back().image.Channels = 0;
	Textures.back().image.RGBPixels = NULL;
	return ( Textures.back() );
}

// Get the last point set or start a new one if things have changed
u3dPointSet& mglGraphIDTF::GetPointSet()
{
	if ( points_finished )
	{
		u3dPointSet PointSet = u3dPointSet ( "PointSet" + i2s ( PointSets.size() ), this );
		PointSets.push_back ( PointSet );
		points_finished = false;
	}
	return PointSets.back();
}

// Get the last line set or start a new one if things have changed
u3dLineSet& mglGraphIDTF::GetLineSet()
{
	if ( lines_finished )
	{
		u3dLineSet LineSet = u3dLineSet ( "LineSet" + i2s ( LineSets.size() ), this );
		LineSets.push_back ( LineSet );
		lines_finished = false;
	}
	return LineSets.back();
}

// Get the last mesh or start a new one if things have changed
u3dMesh& mglGraphIDTF::GetMesh()
{
	if ( mesh_finished )
	{
		u3dMesh Mesh = u3dMesh ( "Mesh" + i2s ( Meshes.size() ), this, this->vertex_color_flag, this->disable_compression_flag );
		Meshes.push_back ( Mesh );
		mesh_finished = false;
	}
	return Meshes.back();
}

size_t u3dModel::AddPoint ( const float *p )
{
	return AddPoint( mglPoint ( p[0], p[1], p[2] ) );
}

size_t u3dModel::AddPoint ( const mglPoint& pnt )
{
	mglPoint point;
	point.x = invpos[0][0]*pnt.x+invpos[0][1]*pnt.y+invpos[0][2]*pnt.z+invpos[0][3];
	point.y = invpos[1][0]*pnt.x+invpos[1][1]*pnt.y+invpos[1][2]*pnt.z+invpos[1][3];
	point.z = invpos[2][0]*pnt.x+invpos[2][1]*pnt.y+invpos[2][2]*pnt.z+invpos[2][3];
// printf("%f %f %f - %f %f %f\n", pnt.x, pnt.y, pnt.z, point.x, point.y, point.z);
	for ( size_t i=0; i< this->Points.size(); i++ )
		if ( this->Points[i] == point )
			return i;
	this->Points.push_back ( point );
	return ( this->Points.size()-1 );
}

size_t u3dModel::AddColor ( const float *c )
{
	mglColor color = mglColor ( IDTFROUND(c[0]), IDTFROUND(c[1]), IDTFROUND(c[2]) );
	for ( size_t i=0; i< this->Colors.size(); i++ )
		if ( this->Colors[i] == color )
			return i;
	this->Colors.push_back ( color );
	return ( this->Colors.size()-1 );
};

size_t u3dModel::AddColor ( const mglColor& c )
{
	mglColor color = mglColor ( c.r, c.g, c.b );
	for ( size_t i=0; i< this->Colors.size(); i++ )
		if ( this->Colors[i] == color )
			return i;
	this->Colors.push_back ( color );
	return ( this->Colors.size()-1 );
};

void u3dMesh::AddTriangle ( size_t pid0, size_t pid1, size_t pid2,
                            size_t cid0, size_t cid1, size_t cid2 )
{
	size_t3 triangle;
	triangle.a = pid0;
	triangle.b = pid1;
	triangle.c = pid2;
	Triangles.push_back ( triangle );
	triangle.a = cid0;
	triangle.b = cid1;
	triangle.c = cid2;
	faceColors.push_back ( triangle );
};

void u3dMesh::AddTriangle ( size_t pid0, size_t pid1, size_t pid2, size_t mid)
{
	size_t3 triangle;
	triangle.a = pid0;
	triangle.b = pid1;
	triangle.c = pid2;
	Triangles.push_back ( triangle );
	faceShaders.push_back ( mid );
};
//-----------------------------------------------------------------------------
void mglGraphIDTF::MakeTransformMatrix( float position[4][4], float invpos[4][4] )
{
	const float s3=2*PlotFactor;
	position[0][0]=B[0]					/(s3*zoomx2);
	position[0][1]=B[1]					/(s3*zoomx2);
	position[0][2]=B[2]					/(s3*zoomx2);
	position[0][3]=(xPos - zoomx1*Width)/(zoomx2);
	position[1][0]=B[3]					/(s3*zoomy2);
	position[1][1]=B[4]					/(s3*zoomy2);
	position[1][2]=B[5]					/(s3*zoomy2);
	position[1][3]=(yPos - zoomy1*Height)/(zoomy2);
	position[2][0]=B[6]		      /(s3*sqrt(zoomx2*zoomy2));
	position[2][1]=B[7]		      /(s3*sqrt(zoomx2*zoomy2));
	position[2][2]=B[8]		      /(s3*sqrt(zoomx2*zoomy2));
	position[2][3]=(zPos)	      /(sqrt(zoomx2*zoomy2));
	position[3][0]=0.0f;
	position[3][1]=0.0f;
	position[3][2]=0.0f;
	position[3][3]=1.0f;
//
// From Mesa-2.2\src\glu\project.c
//

//
// Invert matrix m.  This algorithm contributed by Stephane Rehel
// <rehel@worldnet.fr>
//

/* Here's some shorthand converting standard (row,column) to index. */
#define m11 position[0][0]
#define m12 position[0][1]
#define m13 position[0][2]
#define m14 position[0][3]
#define m21 position[1][0]
#define m22 position[1][1]
#define m23 position[1][2]
#define m24 position[1][3]
#define m31 position[2][0]
#define m32 position[2][1]
#define m33 position[2][2]
#define m34 position[2][3]
#define m41 position[3][0]
#define m42 position[3][1]
#define m43 position[3][2]
#define m44 position[3][3]

	register double det;
	double tmp[16]; /* Allow out == in. */

	/* Inverse = adjoint / det. (See linear algebra texts.)*/

	tmp[0]= m22 * m33 - m23 * m32;
	tmp[1]= m23 * m31 - m21 * m33;
	tmp[2]= m21 * m32 - m22 * m31;

	/* Compute determinant as early as possible using these cofactors. */
	det= m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2];

	/* Run singularity test. */
	if (det == 0.0) {
		printf("invert_matrix: Warning: Singular matrix.\n");
		bzero ( invpos, sizeof ( invpos ) );
	}
	else {
		double d12, d13, d23, d24, d34, d41;
		register double im11, im12, im13, im14;

		det= 1. / det;

		/* Compute rest of inverse. */
		tmp[0] *= det;
		tmp[1] *= det;
		tmp[2] *= det;
		tmp[3]  = 0.;

		im11= m11 * det;
		im12= m12 * det;
		im13= m13 * det;
		im14= m14 * det;
		tmp[4] = im13 * m32 - im12 * m33;
		tmp[5] = im11 * m33 - im13 * m31;
		tmp[6] = im12 * m31 - im11 * m32;
		tmp[7] = 0.;

		/* Pre-compute 2x2 dets for first two rows when computing */
		/* cofactors of last two rows. */
		d12 = im11*m22 - m21*im12;
		d13 = im11*m23 - m21*im13;
		d23 = im12*m23 - m22*im13;
		d24 = im12*m24 - m22*im14;
		d34 = im13*m24 - m23*im14;
		d41 = im14*m21 - m24*im11;

		tmp[8] =  d23;
		tmp[9] = -d13;
		tmp[10] = d12;
		tmp[11] = 0.;

		tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23);
		tmp[13] =  (m31 * d34 + m33 * d41 + m34 * d13);
		tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12);
		tmp[15] =  1.;

		for (int r=0; r<4; r++)
			for (int c=0; c<4; c++)
				invpos[r][c] = tmp[c*4+r];
	}

#undef m11
#undef m12
#undef m13
#undef m14
#undef m21
#undef m22
#undef m23
#undef m24
#undef m31
#undef m32
#undef m33
#undef m34
#undef m41
#undef m42
#undef m43
#undef m44
// puts("pos");
// printf("%f %f %f %f\n", position[0][0], position[0][1], position[0][2], position[0][3]);
// printf("%f %f %f %f\n", position[1][0], position[1][1], position[1][2], position[1][3]);
// printf("%f %f %f %f\n", position[2][0], position[2][1], position[2][2], position[2][3]);
// printf("%f %f %f %f\n", position[3][0], position[3][1], position[3][2], position[3][3]);
// puts("invpos");
// printf("%f %f %f %f\n", invpos[0][0], invpos[0][1], invpos[0][2], invpos[0][3]);
// printf("%f %f %f %f\n", invpos[1][0], invpos[1][1], invpos[1][2], invpos[1][3]);
// printf("%f %f %f %f\n", invpos[2][0], invpos[2][1], invpos[2][2], invpos[2][3]);
// printf("%f %f %f %f\n", invpos[3][0], invpos[3][1], invpos[3][2], invpos[3][3]);
}
//-----------------------------------------------------------------------------
u3dModel::u3dModel ( const std::string name, mglGraphIDTF *Graph, const bool& vertex_color )
		: both_visible ( true )
{
	this->name = name;
	this->Graph = Graph;
	this->vertex_color = vertex_color;
	this->parent = Graph->GetCurrentGroup();
	if (this->parent) this->parent->NumberOfChildren++;
	Graph->MakeTransformMatrix(this->position, this->invpos);
}
//-----------------------------------------------------------------------------
size_t u3dModel::AddModelMaterial ( const float *c, bool emissive, bool vertex_color )
{
	u3dMaterial Material;
	if ( emissive )
	{
		Material.diffuse = mglColor ( IDTFROUND(c[0]), IDTFROUND(c[1]), IDTFROUND(c[2]) );
		Material.specular = BC;
		Material.emissive = mglColor ( IDTFROUND(c[0]), IDTFROUND(c[1]), IDTFROUND(c[2]) );
	}
	else
	{
		Material.diffuse = mglColor ( IDTFROUND(c[0]), IDTFROUND(c[1]), IDTFROUND(c[2]) );
		Material.specular = 0.5f*Material.diffuse;
		Material.emissive = BC;
	}
	Material.opacity = this->Graph->fixalpha ( c[3] );
	Material.vertex_color = vertex_color;
	for ( size_t mid=0; mid < this->ModelMaterials.size(); mid++ )
	{
		if ( Graph->Materials[this->ModelMaterials[mid]] == Material )
		{
			return ( mid );
		}
	}
	this->ModelMaterials.push_back ( Graph->AddMaterial ( Material ) );
	return ( this->ModelMaterials.size()-1 );
};
void u3dModel::print_node ( std::ofstream& ostr )
{
	u3dNode Node;
	Node.name = name;

	for ( int i=0; i<4;i++ )
		for ( int j=0; j<4;j++ )
				Node.position[i][j]	= position[0][i]*mgl_globinv[j][0]
							+ position[1][i]*mgl_globinv[j][1]
							+ position[2][i]*mgl_globinv[j][2]
							+ position[3][i]*mgl_globinv[j][3];
	Node.type = "MODEL";
	Node.both_visible = this->both_visible;
	if ( this->parent == NULL)
		Node.parent = "<NULL>";
	else
		Node.parent = this->parent->name;
	Node.print ( ostr );
}
void u3dModel::print_shading_modifier ( std::ofstream& ostr )
{
	ostr << "MODIFIER \"SHADING\" {\n"
	<< "\tMODIFIER_NAME \"" << this->name << "\"\n"
	<< "\tPARAMETERS {\n"
	<< "\t\tSHADER_LIST_COUNT " << this->ModelMaterials.size() << "\n"
	<< "\t\tSHADER_LIST_LIST {\n";
	int ShaderListNum = 0;
	for ( ModelMaterial_list::const_iterator it = this->ModelMaterials.begin(); it != this->ModelMaterials.end(); ++it )
	{
		ostr << "\t\t\tSHADER_LIST " << ShaderListNum++ << " {\n"
		<< "\t\t\t\tSHADER_COUNT 1\n"
		<< "\t\t\t\tSHADER_NAME_LIST {\n"
		<< "\t\t\t\t\tSHADER 0 NAME: \"" << Graph->Materials[*it].name << "\"\n"
		<< "\t\t\t\t}\n"
		<< "\t\t\t}\n";
	}
	ostr << "\t\t}\n"
	<< "\t}\n"
	<< "}\n"
	<< "\n";
}
void u3dBall::print_node ( std::ofstream& ostr )
{
	u3dNode Node;
	Node.name = name;
	Node.resource = "UnitBall";
	float position[4][4];
	memcpy ( position, mgl_idtrans, sizeof ( mgl_idtrans ) );
	position[0][0] = this->radius;
	position[1][1] = this->radius;
	position[2][2] = this->radius;
	position[0][3] = this->center.x;
	position[1][3] = this->center.y;
	position[2][3] = this->center.z;
	for ( int i=0; i<4;i++ )
		for ( int j=0; j<4;j++ )
				Node.position[i][j]	= position[0][i]*mgl_globinv[j][0]
							+ position[1][i]*mgl_globinv[j][1]
							+ position[2][i]*mgl_globinv[j][2]
							+ position[3][i]*mgl_globinv[j][3];
	Node.type = "MODEL";
	Node.both_visible = false;
	if ( this->parent == NULL)
		Node.parent = "<NULL>";
	else
		Node.parent = this->parent->name;
	Node.print ( ostr );
}

void u3dBall::print_shading_modifier ( std::ofstream& ostr )
{
	ostr << "MODIFIER \"SHADING\" {\n"
	<< "\tMODIFIER_NAME \"" << this->name << "\"\n"
	<< "\tPARAMETERS {\n"
	<< "\t\tSHADER_LIST_COUNT 1\n"
	<< "\t\tSHADER_LIST_LIST {\n"
	<< "\t\t\tSHADER_LIST 0 {\n"
		<< "\t\t\t\tSHADER_COUNT 1\n"
		<< "\t\t\t\tSHADER_NAME_LIST {\n"
		<< "\t\t\t\t\tSHADER 0 NAME: \"" << this->Graph->Materials[this->material].name << "\"\n"
		<< "\t\t\t\t}\n"
		<< "\t\t\t}\n"
	<< "\t\t}\n"
	<< "\t}\n"
	<< "}\n"
	<< "\n";
}

u3dPointSet::u3dPointSet ( const std::string& name, mglGraphIDTF *Graph ) :
		u3dModel ( name, Graph, true )
{
	const float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	this->AddModelMaterial ( color, true, true );
	this->both_visible = false;
}
void u3dPointSet::point_plot ( const mglPoint& p, const mglColor& c )
{
	Points.push_back( p );
	Colors.push_back( c );
}

void u3dPointSet::print_model_resource ( std::ofstream& ostrtmp )
{
	size_t numPoints    = this->Points.size();
	if ( numPoints == 0 )	return;
// Convert pointset to mesh
	size_t duplpoints = this->Points.size() % 3;
	if ( duplpoints > 0 )
	{
		this->Points.push_back( this->Points.back() );
		this->Colors.push_back( this->Colors.back() );
		numPoints++;
	}
	if ( duplpoints == 1 )
	{
		this->Points.push_back( this->Points.back() );
		this->Colors.push_back( this->Colors.back() );
		numPoints++;
	}

	size_t numTriangles = this->Points.size() / 3;

	// is there just one color in the model?
	bool onecolor = true;
	size_t numColors    = this->Colors.size();
	for ( size_t cid=0; cid < numColors; cid++ )
	{
		if ( Colors[0].r != Colors[cid].r || Colors[0].g != Colors[cid].g || Colors[0].b != Colors[cid].b )
		{
			onecolor = false;
			break;
		}
	}
	if ( onecolor ) // if there is just one color in the model - make the corresponding material
	{
		this->ModelMaterials.pop_back();
		float c[4] = { this->Colors[0].r, this->Colors[0].g, this->Colors[0].b, 1.0f };
		this->AddModelMaterial ( c, true, false );
		this->Colors.clear();
		numColors = 0;
		this->vertex_color = false;
	}
	bool colored  = this->Colors.size() > 0;

	ostrtmp
	<< "\t\tRESOURCE_NAME \"" << this->name << "\"\n"
	<< "\t\tMODEL_TYPE \"MESH\"\n"
	<< "\t\tMESH {\n"
	<< "\t\t\tFACE_COUNT " << numTriangles << "\n"
	<< "\t\t\tMODEL_POSITION_COUNT " << numPoints << "\n"
	<< "\t\t\tMODEL_BASE_POSITION_COUNT " << numPoints << "\n"
	<< "\t\t\tMODEL_NORMAL_COUNT 0\n"
	<< "\t\t\tMODEL_DIFFUSE_COLOR_COUNT " << numColors << "\n"
	<< "\t\t\tMODEL_SPECULAR_COLOR_COUNT 0\n"
	<< "\t\t\tMODEL_TEXTURE_COORD_COUNT 0\n"
	<< "\t\t\tMODEL_BONE_COUNT 0\n"
	<< "\t\t\tMODEL_SHADING_COUNT 1\n"
	<< "\t\t\tMODEL_SHADING_DESCRIPTION_LIST {\n"
	<< "\t\t\t\tSHADING_DESCRIPTION 0 {\n"
	<< "\t\t\t\t\tTEXTURE_LAYER_COUNT 0\n"
	<< "\t\t\t\t\tSHADER_ID 0\n"
	<< "\t\t\t\t}\n"
	<< "\t\t\t}\n";

	ostrtmp << "\t\t\tMESH_FACE_POSITION_LIST {\n";
	for ( size_t id=0; id < numTriangles; id++ )
	{
		ostrtmp << "\t\t\t\t"
		<< 3*id+0 << " "
		<< 3*id+1 << " "
		<< 3*id+2 << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tMESH_FACE_SHADING_LIST {\n";
	for ( size_t id=0; id < numTriangles; id++ )
	{
		ostrtmp << "\t\t\t\t0\n";
	}
	ostrtmp << "\t\t\t}\n";

	if ( colored )
	{
		ostrtmp << "\t\t\tMESH_FACE_DIFFUSE_COLOR_LIST {\n";
		for ( size_t id=0; id < numTriangles; id++ )
		{
			ostrtmp << "\t\t\t\t"
			<< 3*id+0 << " "
			<< 3*id+1 << " "
			<< 3*id+2 << "\n";
		}
		ostrtmp << "\t\t\t}\n";
	}

	ostrtmp << "\t\t\tMODEL_POSITION_LIST {\n";
	for ( size_t pid=0; pid < numPoints; pid++ )
	{
		ostrtmp << "\t\t\t\t"
		<< ( this->Points[pid].x ) << " "
		<< ( this->Points[pid].y ) << " "
		<< ( this->Points[pid].z ) << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	if ( colored )
	{
		ostrtmp << "\t\t\tMODEL_DIFFUSE_COLOR_LIST {\n";
		for ( size_t cid=0; cid < numColors; cid++ )
		{
			ostrtmp << "\t\t\t\t"
			<< this->Colors[cid].r << " "
			<< this->Colors[cid].g << " "
			<< this->Colors[cid].b << "\n";
		}
		ostrtmp << "\t\t\t}\n";
	}

	ostrtmp << "\t\t\tMODEL_BASE_POSITION_LIST {\n";
	for ( size_t pid=0; pid < numPoints; pid++ )
	{
		ostrtmp << "\t\t\t\t" << pid << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t}\n";
}
/*
{
	size_t numMaterials = this->ModelMaterials.size();
	size_t numPoints = this->Points.size();

	if ( numPoints == 0 )	return;

	ostrtmp
	<< "\t\tRESOURCE_NAME \"" << this->name << "\"\n"
	<< "\t\tMODEL_TYPE \"POINT_SET\"\n"
	<< "\t\tPOINT_SET {\n"
	<< "\t\t\tPOINT_COUNT " << numPoints << "\n"
	<< "\t\t\tMODEL_POSITION_COUNT " << numPoints << "\n"
	<< "\t\t\tMODEL_NORMAL_COUNT 0\n"
	<< "\t\t\tMODEL_DIFFUSE_COLOR_COUNT 0\n"
	<< "\t\t\tMODEL_SPECULAR_COLOR_COUNT 0\n"
	<< "\t\t\tMODEL_TEXTURE_COORD_COUNT 0\n"
	<< "\t\t\tMODEL_SHADING_COUNT " << numMaterials << "\n"
	<< "\t\t\tMODEL_SHADING_DESCRIPTION_LIST {\n";
	for ( size_t id=0; id < numMaterials; id++ )
	{
		ostrtmp
		<< "\t\t\t\tSHADING_DESCRIPTION " << id << " {\n"
		<< "\t\t\t\t\tTEXTURE_LAYER_COUNT 0\n"
		<< "\t\t\t\t\tSHADER_ID " << id << "\n"
		<< "\t\t\t\t}\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tPOINT_POSITION_LIST {\n";
	for ( size_t id=0; id < numPoints; id++ )
	{
		ostrtmp << "\t\t\t\t" << i2s ( id ) << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tPOINT_SHADING_LIST {\n";
	for ( size_t id=0; id < numPoints; id++ )
	{
		ostrtmp << "\t\t\t\t 0\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tMODEL_POSITION_LIST {\n";
	for ( size_t pid=0; pid < numPoints; pid++ )
	{
		ostrtmp << "\t\t\t\t"
		<< ( this->Points[pid].x )  << " "
		<< ( this->Points[pid].y )  << " "
		<< ( this->Points[pid].z )  << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t}\n";
}
*/
void u3dLineSet::AddLine ( size_t pid1, size_t pid2, size_t mid )
{
	u3dLine line = {pid1, pid2, mid};
	for ( size_t lid=0; lid < this->Lines.size(); lid++ )
	{
		if ( this->Lines[lid].mid == mid && (
			( this->Lines[lid].pid1 == pid1 && this->Lines[lid].pid2 == pid2 )
			||
			( this->Lines[lid].pid1 == pid2 && this->Lines[lid].pid2 == pid1 )
			) )
		{
			return;
		}
	}
	this->Lines.push_back ( line );
}

void u3dLineSet::line_plot ( float *p1, float *p2, float *c1, float *c2 )
{
	float color[4];
	size_t pid1 = this->AddPoint ( p1 );
	size_t pid2 = this->AddPoint ( p2 );
	color[0] = ( c1[0] + c2[0] ) /2.0f;
	color[1] = ( c1[1] + c2[1] ) /2.0f;
	color[2] = ( c1[2] + c2[2] ) /2.0f;
	color[3] = 1.0f;
	size_t mid =  this->AddModelMaterial ( color, true, false );
	u3dLine line = {pid1, pid2, mid};
	this->Lines.push_back ( line );
}

void u3dLineSet::print_model_resource ( std::ofstream& ostrtmp )
{
	size_t numMaterials = this->ModelMaterials.size();
	size_t numPoints = this->Points.size();
	size_t numLines  = this->Lines.size();

	if ( numLines == 0 )	return;

	ostrtmp
	<< "\t\tRESOURCE_NAME \"" << this->name << "\"\n"
	<< "\t\tMODEL_TYPE \"LINE_SET\"\n"
	<< "\t\tLINE_SET {\n"
	<< "\t\t\tLINE_COUNT " << numLines << "\n"
	<< "\t\t\tMODEL_POSITION_COUNT " << numPoints << "\n"
	<< "\t\t\tMODEL_NORMAL_COUNT 0\n"
	<< "\t\t\tMODEL_DIFFUSE_COLOR_COUNT 0\n"
	<< "\t\t\tMODEL_SPECULAR_COLOR_COUNT 0\n"
	<< "\t\t\tMODEL_TEXTURE_COORD_COUNT 0\n"
	<< "\t\t\tMODEL_SHADING_COUNT " << numMaterials << "\n"
	<< "\t\t\tMODEL_SHADING_DESCRIPTION_LIST {\n";
	for ( size_t id=0; id < numMaterials; id++ )
	{
		ostrtmp
		<< "\t\t\t\tSHADING_DESCRIPTION " << id << " {\n"
		<< "\t\t\t\t\tTEXTURE_LAYER_COUNT 0\n"
		<< "\t\t\t\t\tSHADER_ID " << id << "\n"
		<< "\t\t\t\t}\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tLINE_POSITION_LIST {\n";
	for ( size_t id=0; id < numLines; id++ )
	{
		ostrtmp << "\t\t\t\t" << i2s ( this->Lines[id].pid1 ) << " " << i2s ( this->Lines[id].pid2 ) << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tLINE_SHADING_LIST {\n";
	for ( size_t id=0; id < numLines; id++ )
	{
		ostrtmp << "\t\t\t\t" << this->Lines[id].mid << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tMODEL_POSITION_LIST {\n";
	for ( size_t pid=0; pid < numPoints; pid++ )
	{
		ostrtmp << "\t\t\t\t"
		<< ( this->Points[pid].x ) << " "
		<< ( this->Points[pid].y ) << " "
		<< ( this->Points[pid].z ) << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t}\n";
}
//-----------------------------------------------------------------------------
void u3dMesh::quad_plot ( float *pp0,float *pp1,float *pp2,float *pp3,
                          float *cc0,float *cc1,float *cc2,float *cc3 )
{
	this->trig_plot ( pp0,pp1,pp2,cc0,cc1,cc2 );
	this->trig_plot ( pp1,pp3,pp2,cc1,cc3,cc2 );
}
//-----------------------------------------------------------------------------
void u3dMesh::quad_plot_n ( float *pp0,float *pp1,float *pp2,float *pp3,
                            float *cc0,float *cc1,float *cc2,float *cc3,
                            float *nn0,float *nn1,float *nn2,float *nn3 )
{
	this->trig_plot_n ( pp0,pp1,pp2,cc0,cc1,cc2,nn0,nn1,nn2 );
	this->trig_plot_n ( pp1,pp3,pp2,cc1,cc3,cc2,nn0,nn2,nn3 );
}
//-----------------------------------------------------------------------------
void u3dMesh::trig_plot ( float *pp0,float *pp1,float *pp2,
                          float *cc0,float *cc1,float *cc2 )
{
	size_t pid0 = this->AddPoint ( pp0 );
	size_t pid1 = this->AddPoint ( pp1 );
	size_t pid2 = this->AddPoint ( pp2 );

	if ( !this->vertex_color )
	{
		const float color[4] =
		{
			 ( cc0[0]+cc1[0]+cc2[0] ) /3.0f,
			 ( cc0[1]+cc1[1]+cc2[1] ) /3.0f,
			 ( cc0[2]+cc1[2]+cc2[2] ) /3.0f,
			 ( cc0[3]+cc1[3]+cc2[3] ) /3.0f
		};
		size_t mid = this->AddModelMaterial ( color, false, false);
		this->AddTriangle ( pid0, pid1, pid2, mid );
	}
	else
	{
		if ( ModelMaterials.size() == 0 )
		{
			const float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			this->AddModelMaterial ( color, false, true );
		}
		size_t cid0 = this->AddColor ( cc0 );
		size_t cid1 = this->AddColor ( cc1 );
		size_t cid2 = this->AddColor ( cc2 );
		this->AddTriangle ( pid0, pid1, pid2, cid0, cid1, cid2 );
	}
}
//-----------------------------------------------------------------------------
void u3dMesh::trig_plot_n ( float *pp0,float *pp1,float *pp2,
                            float *cc0,float *cc1,float *cc2,
                            float *nn0,float *nn1,float *nn2 )
{
	size_t pid0 = this->AddPoint ( pp0 );
	size_t pid1 = this->AddPoint ( pp1 );
	size_t pid2 = this->AddPoint ( pp2 );

	if ( !this->vertex_color )
	{
		const float color[4] =
		{
			 ( cc0[0]+cc1[0]+cc2[0] ) /3.0f,
			 ( cc0[1]+cc1[1]+cc2[1] ) /3.0f,
			 ( cc0[2]+cc1[2]+cc2[2] ) /3.0f,
			 ( cc0[3]+cc1[3]+cc2[3] ) /3.0f
		};
		size_t mid = this->AddModelMaterial ( color, false, false );
		this->AddTriangle ( pid0, pid1, pid2, mid );
	}
	else
	{
		if ( ModelMaterials.size() == 0 )
		{
			const float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			this->AddModelMaterial ( color, false, true );
		}
		float cc[3];
		size_t cid0 = this->AddColor ( this->Graph->col2col ( cc0, nn0, cc ) );
		size_t cid1 = this->AddColor ( this->Graph->col2col ( cc1, nn1, cc ) );
		size_t cid2 = this->AddColor ( this->Graph->col2col ( cc2, nn2, cc ) );
		this->AddTriangle ( pid0, pid1, pid2, cid0, cid1, cid2 );
	}
}
//-----------------------------------------------------------------------------
void u3dMesh::print_model_resource ( std::ofstream& ostrtmp )
{
	size_t numMaterials = this->ModelMaterials.size();
	size_t numPoints    = this->Points.size();
	size_t numTriangles = this->Triangles.size();
	size_t numTexCoords = this->textureCoords.size();
	bool textured = this->textureDimension > 0;
	bool shaded = this->faceShaders.size() > 0;

	if ( numTriangles == 0 )	return;

	if ( this->Colors.size() == 1 ) // if there is just one color in the model - make the corresponding material
	{
		this->ModelMaterials.pop_back();
		float c[4] = { this->Colors[0].r, this->Colors[0].g, this->Colors[0].b, 1.0f };
		this->AddModelMaterial ( c, false, false );
		this->Colors.clear();
	}
	bool colored  = this->Colors.size() > 0;
	size_t numColors    = this->Colors.size();

	ostrtmp
	<< "\t\tRESOURCE_NAME \"" << this->name << "\"\n"
	<< "\t\tMODEL_TYPE \"MESH\"\n"
	<< "\t\tMESH {\n"
	<< "\t\t\tFACE_COUNT " << numTriangles << "\n"
	<< "\t\t\tMODEL_POSITION_COUNT " << numPoints << "\n";
	if ( this->disable_compression )
	{
		ostrtmp << "\t\t\tMODEL_BASE_POSITION_COUNT " << numPoints << "\n";
	}
	ostrtmp
	<< "\t\t\tMODEL_NORMAL_COUNT 0\n"
	<< "\t\t\tMODEL_DIFFUSE_COLOR_COUNT " << numColors << "\n"
	<< "\t\t\tMODEL_SPECULAR_COLOR_COUNT 0\n"
	<< "\t\t\tMODEL_TEXTURE_COORD_COUNT " << numTexCoords << "\n"
	<< "\t\t\tMODEL_BONE_COUNT 0\n"
	<< "\t\t\tMODEL_SHADING_COUNT " << numMaterials << "\n"
	<< "\t\t\tMODEL_SHADING_DESCRIPTION_LIST {\n";
	for ( size_t cid=0; cid < numMaterials; cid++ )
	{
		ostrtmp
		<< "\t\t\t\tSHADING_DESCRIPTION " << cid << " {\n";
		if ( textured )
			ostrtmp
			<< "\t\t\t\t\tTEXTURE_LAYER_COUNT 1\n"
			<< "\t\t\t\t\tTEXTURE_COORD_DIMENSION_LIST {\n"
			<< "\t\t\t\t\t\tTEXTURE_LAYER 0 DIMENSION: " << this->textureDimension << "\n"
			<< "\t\t\t\t\t}\n";
		else
			ostrtmp
			<< "\t\t\t\t\tTEXTURE_LAYER_COUNT 0\n";
		ostrtmp
		<< "\t\t\t\t\tSHADER_ID " << cid << "\n"
		<< "\t\t\t\t}\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tMESH_FACE_POSITION_LIST {\n";
	for ( size_t id=0; id < numTriangles; id++ )
	{
		ostrtmp << "\t\t\t\t"
		<< this->Triangles[id].a << " "
		<< this->Triangles[id].b << " "
		<< this->Triangles[id].c << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tMESH_FACE_SHADING_LIST {\n";
	for ( size_t id=0; id < numTriangles; id++ )
	{
		if ( shaded )
			ostrtmp << "\t\t\t\t" << this->faceShaders[id] << "\n";
		else
			ostrtmp << "\t\t\t\t0\n";
	}
	ostrtmp << "\t\t\t}\n";

	if ( textured )
	{
		ostrtmp << "\t\t\tMESH_FACE_TEXTURE_COORD_LIST {\n";
		for ( size_t id=0; id < numTriangles; id++ )
		{
			ostrtmp << "\t\t\t\tFACE 0 {\n"
			<< "\t\t\t\t\tTEXTURE_LAYER 0 TEX_COORD: "
			<< this->faceColors[id].a << " "
			<< this->faceColors[id].b << " "
			<< this->faceColors[id].c << "\n"
			<< "\t\t\t\t}\n";
		}
		ostrtmp << "\t\t\t}\n";
	}

	if ( colored )
	{
		ostrtmp << "\t\t\tMESH_FACE_DIFFUSE_COLOR_LIST {\n";
		for ( size_t id=0; id < numTriangles; id++ )
		{
			ostrtmp << "\t\t\t\t"
			<< this->faceColors[id].a << " "
			<< this->faceColors[id].b << " "
			<< this->faceColors[id].c << "\n";
		}
		ostrtmp << "\t\t\t}\n";
	}

	ostrtmp << "\t\t\tMODEL_POSITION_LIST {\n";
	for ( size_t pid=0; pid < numPoints; pid++ )
	{
		ostrtmp << "\t\t\t\t"
		<< ( this->Points[pid].x ) << " "
		<< ( this->Points[pid].y ) << " "
		<< ( this->Points[pid].z ) << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	if ( colored )
	{
		ostrtmp << "\t\t\tMODEL_DIFFUSE_COLOR_LIST {\n";
		for ( size_t cid=0; cid < numColors; cid++ )
		{
			ostrtmp << "\t\t\t\t"
			<< this->Colors[cid].r << " "
			<< this->Colors[cid].g << " "
			<< this->Colors[cid].b << "\n";
		}
		ostrtmp << "\t\t\t}\n";
	}

	if ( textured )
	{
		ostrtmp << "\t\t\tMODEL_TEXTURE_COORD_LIST {\n";
		for ( size_t cid=0; cid < numTexCoords; cid++ )
		{
			ostrtmp << "\t\t\t\t"
			<< this->textureCoords[cid].U << " "
			<< ( this->textureDimension == 2 ? this->textureCoords[cid].V : 0.0 ) << " "
			<< "0.0 0.0\n";
		}
		ostrtmp << "\t\t\t}\n";
	}

	if ( this->disable_compression )
	{
		ostrtmp << "\t\t\tMODEL_BASE_POSITION_LIST {\n";
		for ( size_t pid=0; pid < numPoints; pid++ )
		{
			ostrtmp << "\t\t\t\t" << pid << "\n";
		}
		ostrtmp << "\t\t\t}\n";
	}

	ostrtmp << "\t\t}\n";
}
//-----------------------------------------------------------------------------
mglGraphIDTF::mglGraphIDTF() : mglGraphAB ( 1,1 ),
		vertex_color_flag ( true ), disable_compression_flag ( true ), unrotate_flag ( false ), ball_is_point_flag ( false ),
		points_finished ( true ), lines_finished ( true ), mesh_finished ( true ),
		CurrentGroup ( NULL )
{	Width = Height = Depth = 1;	}
//-----------------------------------------------------------------------------
mglGraphIDTF::~mglGraphIDTF() {}
//-----------------------------------------------------------------------------
void mglGraphIDTF::Light ( int n, mglPoint p, mglColor c, float br, bool infty )
{
	if ( n<0 || n>9 )	{       SetWarn ( mglWarnLId );	return;	}
	nLight[n] = true;	aLight[n] = 3;	bLight[n] = br;
	rLight[3*n] = p.x;	rLight[3*n+1] = p.y;	rLight[3*n+2] = p.z;
	cLight[3*n] = c.r;	cLight[3*n+1] = c.g;	cLight[3*n+2] = c.b;
	iLight[n] = infty;
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::Clf ( mglColor Back )
{
	CurrPal = 0;
	if ( Back==NC )	Back=mglColor ( 1,1,1 );
	Groups.clear();
	CurrentGroup = NULL;
	LineSets.clear();
	PointSets.clear();
	Meshes.clear();
	Balls.clear();
	Materials.clear();
	Textures.clear();
	points_finished = true;
	lines_finished = true;
	mesh_finished = true;
}

void mglGraphIDTF::StartGroup ( const char *name )
{
	points_finished = true;
	lines_finished = true;
	mesh_finished = true;
	if ( name == NULL || strlen(name) == 0 || strcmp(name,"<NULL>") == 0 )
	{
		CurrentGroup = NULL;
		return;
	}
	for ( u3dGroup_list::iterator it = Groups.begin(); it != Groups.end(); ++it )
	{
		if ( name == it->name )
		{
			CurrentGroup = &(*it);
			return;
		}
	}
	u3dGroup Group;
	Group.name = name;
	Group.parent = CurrentGroup;
	if (Group.parent)
	{
		Group.parent->NumberOfChildren++;
	}
	Groups.push_back ( Group );
	CurrentGroup = &Groups.back();
}
void mglGraphIDTF::StartAutoGroup ( const char *name )
{
	points_finished = true;
	lines_finished = true;
	mesh_finished = true;
	u3dGroup Group;
	Group.name = name;
	Group.parent = CurrentGroup;
	if (Group.parent)
	{
		Group.parent->NumberOfChildren++;
	}
	Group.isauto = true;
	Groups.push_back ( Group );
	CurrentGroup = &Groups.back();
}
void mglGraphIDTF::EndGroup()
{
	points_finished = true;
	lines_finished = true;
	mesh_finished = true;
	if (CurrentGroup == NULL )
		return;
	CurrentGroup = CurrentGroup->parent;
}
u3dGroup* mglGraphIDTF::GetCurrentGroup()
{
	return CurrentGroup;
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::UnitBall ( )
{
	const size_t ThetaResolution = 10; // 4
	const size_t PhiResolution   = 10; // 3
	mglPoint pnt;
	mglPoint nrm;
	const mglPoint Center = mglPoint ( 0, 0, 0 );
	const float Radius = 1.0f;
	u3dMesh Mesh = u3dMesh ( "UnitBall" , this, false, true );
	Mesh.both_visible=false;

	float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	Mesh.AddModelMaterial ( color, false, false );

// tetrahedron
//	Mesh.Points.push_back ( Radius * mglPoint ( 1,  1,  1) );
//	Mesh.Points.push_back ( Radius * mglPoint (-1, -1,  1) );
//	Mesh.Points.push_back ( Radius * mglPoint (-1,  1, -1) );
//	Mesh.Points.push_back ( Radius * mglPoint ( 1, -1, -1) );
//	size_t3 triangle;
//	triangle.a = 0;
//	triangle.b = 1;
//	triangle.c = 3;
//	Mesh.Triangles.push_back ( triangle );
//	triangle.a = 0;
//	triangle.b = 2;
//	triangle.c = 1;
//	Mesh.Triangles.push_back ( triangle );
//	triangle.a = 1;
//	triangle.b = 2;
//	triangle.c = 3;
//	Mesh.Triangles.push_back ( triangle );
//	triangle.a = 0;
//	triangle.b = 2;
//	triangle.c = 3;
//	Mesh.Triangles.push_back ( triangle );
//	Meshes.push_back ( Mesh );
//return;

	// Create north pole
	nrm.x = nrm.y = 0.0; nrm.z = 1.0;
	pnt = Center + Radius * nrm;
	Mesh.Points.push_back ( pnt );

	// Create south pole
	nrm.x = nrm.y = 0.0; nrm.z = -1.0;
	pnt = Center + Radius * nrm;
	Mesh.Points.push_back ( pnt );

	// Create intermediate points
	for ( size_t i=0; i < ThetaResolution; i++ )
	{
		double deltaTheta = 2*M_PI/ThetaResolution;
		double theta = i*deltaTheta;;

		for ( size_t j=1; j<PhiResolution-1; j++ )
		{
			double deltaPhi = M_PI/ ( PhiResolution-1 );
			double phi = j*deltaPhi;
			nrm.x = sin ( phi ) * cos ( theta );
			nrm.y = sin ( phi ) * sin ( theta );
			nrm.z = cos ( phi );
			pnt = Center + Radius * nrm;
			Mesh.Points.push_back ( pnt );
		}
	}

	// Generate mesh connectivity
	size_t phiResolution = PhiResolution - 2;
	size_t base = phiResolution * ThetaResolution;
	const size_t numPoles = 2;

	// around north pole
	for ( size_t i=0; i < ThetaResolution; i++ )
	{
		size_t3 triangle;
		triangle.a =  phiResolution*i + numPoles;
		triangle.b = ( phiResolution* ( i+1 ) % base ) + numPoles;
		triangle.c =  0;
		Mesh.Triangles.push_back ( triangle );
	}

	// around south pole
	size_t numOffset = phiResolution - 1 + numPoles;
	for ( size_t i=0; i < ThetaResolution; i++ )
	{
		size_t3 triangle;
		triangle.b = phiResolution*i + numOffset;
		triangle.a = ( ( phiResolution* ( i+1 ) ) % base ) + numOffset;
		triangle.c = numPoles - 1;
		Mesh.Triangles.push_back ( triangle );
	}

	// bands in-between poles
	for ( size_t i=0; i < ThetaResolution; i++ )
	{
		for ( size_t j=0; j < ( phiResolution-1 ); j++ )
		{
			size_t3 triangle;
			triangle.a = phiResolution*i + j + numPoles;
			triangle.b = triangle.a + 1;
			triangle.c = ( ( phiResolution* ( i+1 ) +j ) % base ) + numPoles + 1;
			Mesh.Triangles.push_back ( triangle );
			triangle.b = triangle.c;
			triangle.c = triangle.b-1;
			Mesh.Triangles.push_back ( triangle );
		}
	}
	Meshes.push_back ( Mesh );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::Ball ( float x,float y,float z,mglColor col,float alpha )
{
	if(alpha==0)	return;
	if(alpha<0)	{	alpha = -alpha;	}
	else		{	if(!ScalePoint(x,y,z))	return;	}
	if ( !col.Valid() )	col = mglColor ( 1.,0.,0. );
	alpha = Transparent ? alpha : 1.0f;
	alpha = UseAlpha ? alpha : 1.0f;
	u3dBall ball;

	float p[3] = {x,y,z};
	if (ball_is_point_flag)
	{
		point_plot ( mglPoint ( p[0],p[1],p[2] ), col );
		return;
	}
	PostScale ( p,1 );
	ball.center = mglPoint ( p[0],p[1],p[2] );
	if (PenWidth != 0.0f)
		ball.radius = fabs(PenWidth)/500.f;
	else
		ball.radius = fabs(BaseLineWidth)/500.f;
// fprintf(stderr, "PenWidth %f BaseLineWidth %f radius %f\n", PenWidth, BaseLineWidth, ball.radius);
	ball.Graph = this;
	ball.parent = this->GetCurrentGroup();
	if (ball.parent) ball.parent->NumberOfChildren++;
	ball.name = "Ball" + i2s(Balls.size()) ;

	u3dMaterial Material;
	Material.diffuse = col;
	Material.specular = 0.5*col;
	Material.emissive = 0.125*col;
	Material.reflectivity = 0.1f;
	Material.opacity = alpha;
	Material.vertex_color = false;
	ball.material = this->AddMaterial ( Material );

	Balls.push_back(ball);
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::ball ( float *p,float *c )
{
//	point_plot ( mglPoint ( p[0], p[1], p[2] ) );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::mark_plot ( float *pp, char type )
{
	float x=pp[0],y=pp[1],z=pp[2];
#define pnt(x, y)  ( p + ss*mglPoint((float)(x), (float)(y), 0.0f))
	mglPoint p = mglPoint ( x, y, z );
	mglPoint p1;
	mglPoint p2;
	float ss=MarkSize*0.35*font_factor;
	if ( type=='.' || ss==0 )
	{
		point_plot ( p, mglColor( CDef[0], CDef[1], CDef[2] ) );
	}
	else
	{
		float pw = PenWidth;	PenWidth = BaseLineWidth;
		unsigned pd = PDef;	PDef = 0xffff;
		switch ( type )
		{
			case '+':
				ss = ss*1.1;
				line_plot ( pnt ( -1, 0 ), pnt ( 1, 0 ) );
				line_plot ( pnt ( 0,-1 ), pnt ( 0, 1 ) );
				break;
			case 'x':
				line_plot ( pnt ( -1,-1 ), pnt ( 1, 1 ) );
				line_plot ( pnt ( 1,-1 ), pnt ( -1, 1 ) );
				break;
			case 's':
				line_plot ( pnt ( -1,-1 ), pnt ( 1,-1 ) );
				line_plot ( pnt ( 1,-1 ), pnt ( 1, 1 ) );
				line_plot ( pnt ( 1 ,1 ), pnt ( -1, 1 ) );
				line_plot ( pnt ( -1, 1 ), pnt ( -1,-1 ) );
				break;
			case 'd':
				ss = ss*1.1;
				line_plot ( pnt ( 0,-1 ), pnt ( 1, 0 ) );
				line_plot ( pnt ( 1, 0 ), pnt ( 0, 1 ) );
				line_plot ( pnt ( 0, 1 ), pnt ( -1, 0 ) );
				line_plot ( pnt ( -1, 0 ), pnt ( 0,-1 ) );
				break;
			case '*':
				ss = ss*1.1;
				line_plot ( pnt ( -1, 0 ), pnt ( 1, 0 ) );
				line_plot ( pnt ( -0.6,-0.8 ), pnt ( 0.6, 0.8 ) );
				line_plot ( pnt ( -0.6, 0.8 ), pnt ( 0.6,-0.8 ) );
				break;
			case '^':
				ss = ss*1.1;
				line_plot ( pnt ( 0, 1 ), pnt ( 0.7,-0.5 ) );
				line_plot ( pnt ( 0.7,-0.5 ), pnt ( -0.7,-0.5 ) );
				line_plot ( pnt ( -0.7,-0.5 ), pnt ( 0,   1 ) );
				break;
			case 'v':
				ss = ss*1.1;
				line_plot ( pnt ( 0,  -1 ), pnt ( 0.7, 0.5 ) );
				line_plot ( pnt ( 0.7, 0.5 ), pnt ( -0.7, 0.5 ) );
				line_plot ( pnt ( -0.7, 0.5 ), pnt ( 0,  -1 ) );
				break;
			case 'o':
				p1 = pnt ( 1, 0 );
				for ( int i=1;i<32;i++ )
				{
					p2 = pnt ( cos ( i*M_PI/16. ), sin ( i*M_PI/16. ) );
					line_plot ( p1, p2 );
					p1 = p2;
				}
				p2 = pnt ( 1, 0 );
				line_plot ( p1, p2 );
				break;
			case 'O':
				p1 = pnt ( 1, 0 );
				for ( int i=1;i<32;i++ )
				{
					p2 = pnt ( cos ( i*M_PI/16. ), sin ( i*M_PI/16. ) );
					trig_plot ( p,p1,p2 );
					p1 = p2;
				}
				p2 = pnt ( 1, 0 );
				trig_plot ( p,p1,p2 );
				break;
		}
		PDef = pd;
		PenWidth = pw;
	}
#undef pnt
}
//-----------------------------------------------------------------------------
float* mglGraphIDTF::col2col ( const float *c, const float *n, float *r )
{
	register long i,j;
	static float u[3];
	mglColor b = mglColor ( c[0], c[1], c[2] );
	if ( r==0 ) r = u;
	if ( UseLight && n && this->vertex_color_flag )
	{
		float nn;
		b = AmbBr*b;
		mglPoint Normal = mglPoint ( n[0], n[1], n[2] ); // Normal to surface after transform
		nn = Norm ( Normal );
		if ( nn != 0.0f )
			for ( i=0;i<10;i++ )
			{
				if ( !nLight[i] )	continue;
				j = 3*i;
				mglPoint PLight = mglPoint ( pLight[j], pLight[j+1], pLight[j+2] );
				nn = 2* ( Normal*PLight ) / ( Normal*Normal );
				mglPoint d = PLight - nn * Normal;
				nn = 1 + d.z/Norm ( d );

				nn = exp ( -aLight[i]*nn ) *bLight[i]*2;
				b = b + nn * mglColor ( cLight[j], cLight[j+1], cLight[j+2] );
			}
		b.r = b.r<1 ? b.r : 1;
		b.g = b.g<1 ? b.g : 1;
		b.b = b.b<1 ? b.b : 1;
	}
	r[0] = b.r;	r[1] = b.g;	r[2] = b.b;
	return r;
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::quad_plot ( float *pp0,float *pp1,float *pp2,float *pp3,
                               float *cc0,float *cc1,float *cc2,float *cc3 )
{
	GetMesh().quad_plot ( pp0, pp1, pp2, pp3, cc0, cc1, cc2, cc3 );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::cloud_plot(long nx,long ny,long nz,float *pp,float *a,float alpha)
{
	register long i,j,k,i0;
	if(!pp || !a || !DrawFace || alpha==0)	return;
	float *aa=new float[nx*ny*nz];
	float *cc=new float[4*nx*ny*nz];
	bool *tt=new bool[nx*ny*nz];
	if(!aa || !tt)
	{	delete []aa;	delete []tt;	return;	}
	for(i=0;i<nx*ny*nz;i++)
	{
		register float t,s;
		register long k;
		const long n = NumCol-1;
		mglColor c;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]) && !isnan(a[i]);
		aa[i] = GetA(a[i]);
		s = aa[i];
		t = (alpha/4)* ( alpha>0 ? ( s+1.f ) * ( s+1.f ) : ( 1.f-s ) * ( s-1.f ) );
		s = n* ( s+1.f ) /2.f;	k = long ( s );	s -= k;
		if ( k<n )	c = cmap[k]* ( 1.f-s ) + cmap[k+1]*s;	else	c = cmap[n];
		cc[4*i+0] = c.r; cc[4*i+1] = c.g; cc[4*i+2] = c.b; cc[4*i+3] = t;
	}

	bool textures_flag_old = textures_flag;
	textures_flag = true;

	std::string GroupName;
	GroupName = CurrentGroup->name;
	GroupName.append( "_xSections" );
	StartAutoGroup( GroupName.c_str() );
	for(i=0;i<nx;i++)
	{
		float *p=new float[3*ny*nz];
		float *c=new float[4*ny*nz];
		bool *t=new bool[ny*nz];
		long i1;
		for(j=0;j<ny;j++) for(k=0;k<nz;k++)
		{
			i0=k*nx*ny+j*nx+i;
			i1=k*ny+j;
			t[i1]=tt[i0];
			p[3*i1+0]=pp[3*i0+0];
			p[3*i1+1]=pp[3*i0+1];
			p[3*i1+2]=pp[3*i0+2];
			c[4*i1+0]=cc[4*i0+0];
			c[4*i1+1]=cc[4*i0+1];
			c[4*i1+2]=cc[4*i0+2];
			c[4*i1+3]=cc[4*i0+3];
		}
		surf_plot( ny, nz, p, c, t );
		delete []p;	delete []t;	delete []c;
	}
	EndGroup();
	GroupName = CurrentGroup->name;
	GroupName.append( "_ySections" );
	StartAutoGroup( GroupName.c_str() );
	for(j=0;j<ny;j++)
	{
		float *p=new float[3*nx*nz];
		float *c=new float[4*nx*nz];
		bool *t=new bool[nx*nz];
		long i1;
		for(i=0;i<nx;i++) for(k=0;k<nz;k++)
		{
			i0=k*nx*ny+j*nx+i;
			i1=k*nx+i;
			t[i1]=tt[i0];
			p[3*i1+0]=pp[3*i0+0];
			p[3*i1+1]=pp[3*i0+1];
			p[3*i1+2]=pp[3*i0+2];
			c[4*i1+0]=cc[4*i0+0];
			c[4*i1+1]=cc[4*i0+1];
			c[4*i1+2]=cc[4*i0+2];
			c[4*i1+3]=cc[4*i0+3];
		}
		surf_plot( nx, nz, p, c, t );
		delete []p;	delete []t;	delete []c;
	}
	EndGroup();
	GroupName = CurrentGroup->name;
	GroupName.append( "_zSections" );
	StartAutoGroup( GroupName.c_str() );
	for(k=0;k<nz;k++)
	{
		float *p=new float[3*nx*ny];
		float *c=new float[4*nx*ny];
		bool *t=new bool[nx*ny];
		long i1;
		for(i=0;i<nx;i++) for(j=0;j<ny;j++)
		{
			i0=k*nx*ny+j*nx+i;
			i1=j*nx+i;
			t[i1]=tt[i0];
			p[3*i1+0]=pp[3*i0+0];
			p[3*i1+1]=pp[3*i0+1];
			p[3*i1+2]=pp[3*i0+2];
			c[4*i1+0]=cc[4*i0+0];
			c[4*i1+1]=cc[4*i0+1];
			c[4*i1+2]=cc[4*i0+2];
			c[4*i1+3]=cc[4*i0+3];
		}
		surf_plot( nx, ny, p, c, t );
		delete []p;	delete []t;	delete []c;
	}
	EndGroup();

/* One more way, "cell projections", direct port to idtf of the mglGraphAB alghorithm
        but the file size turns out to be big and there are problems when looking thru
        transparent objects on other parts of the same objects.

	GroupName = CurrentGroup->name;
	GroupName.append( "_Cells" );
	StartAutoGroup( GroupName.c_str() );
	{
		register float t,s;
		register long k;
		const long n = NumCol-1;
		mglColor c;
		size_t *pid=new size_t[nx*ny*nz];

		PostScale ( pp,nx*ny*nz );
		mesh_finished = true;
		u3dMesh& Mesh = GetMesh();
		mesh_finished = true;
		Mesh.textureDimension = 1;
		Mesh.vertex_color = false;
		u3dTexture& texture = AddTexture ();
		texture.image.Initialize ( NumCol+2, 1, 4 );
		for ( i=0;i<NumCol;i++ )
		{
			texture.image.RGBPixels[4*(i+1)+0] = cmap[i].r*255;
			texture.image.RGBPixels[4*(i+1)+1] = cmap[i].g*255;
			texture.image.RGBPixels[4*(i+1)+2] = cmap[i].b*255;
			s = (i*2.f/n)-1.f;
			t = (alpha/4)* ( alpha>0 ? ( s+1.f ) * ( s+1.f ) : ( 1.f-s ) * ( s-1.f ) );
			texture.image.RGBPixels[4*(i+1)+3] = t*255;
		}
		texture.image.RGBPixels[4*0+0] = texture.image.RGBPixels[4*1+0];
		texture.image.RGBPixels[4*0+1] = texture.image.RGBPixels[4*1+0];
		texture.image.RGBPixels[4*0+2] = texture.image.RGBPixels[4*1+0];
		texture.image.RGBPixels[4*0+3] = texture.image.RGBPixels[4*1+0];
		texture.image.RGBPixels[4*(NumCol+1)+0] = texture.image.RGBPixels[4*NumCol+0];
		texture.image.RGBPixels[4*(NumCol+1)+1] = texture.image.RGBPixels[4*NumCol+0];
		texture.image.RGBPixels[4*(NumCol+1)+2] = texture.image.RGBPixels[4*NumCol+0];
		texture.image.RGBPixels[4*(NumCol+1)+3] = texture.image.RGBPixels[4*NumCol+0];
		u3dMaterial Material;
		Material.color = mglColor ( 1.0f, 1.0f, 1.0f );
		Material.opacity = 1.0f;
		Material.emissive = false;
		Material.vertex_color = false;
		Material.texture = texture.name;
		Material.texturealpha = true;
		Mesh.ModelMaterials.push_back ( AddMaterial ( Material ) );
		for ( i=0;i<nx*ny*nz;i++ )
		{
			if ( tt[i] )
			{
			mglPoint point;
			point.x = Mesh.invpos[0][0]*pp[3*i]+Mesh.invpos[0][1]*pp[3*i+1]+Mesh.invpos[0][2]*pp[3*i+2]+Mesh.invpos[0][3];
			point.y = Mesh.invpos[1][0]*pp[3*i]+Mesh.invpos[1][1]*pp[3*i+1]+Mesh.invpos[1][2]*pp[3*i+2]+Mesh.invpos[1][3];
			point.z = Mesh.invpos[2][0]*pp[3*i]+Mesh.invpos[2][1]*pp[3*i+1]+Mesh.invpos[2][2]*pp[3*i+2]+Mesh.invpos[2][3];
			pid[i] = Mesh.Points.size();
			Mesh.Points.push_back ( point );
			}
			else
			{
				pid[i] = SIZE_MAX;
			}
			Mesh.Colors.push_back ( mglColor ( (1.+NumCol*((aa[i]+1.f)/2.f))/(NumCol+2), 0, 0 ) );

		}
#define AddTri(i1, i2, i3)	\
			Mesh.AddTriangle(pid[i0+(i1)], pid[i0+(i2)], pid[i0+(i3)], pid[i0+(i1)], pid[i0+(i2)], pid[i0+(i3)])
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
		{
			i0 = i+nx*(j+ny*k);
			if(!tt[i0])	continue;
			if(i<nx-1 && j<ny-1 && tt[i0+1] && tt[i0+nx] && tt[i0+nx+1])
			{
				AddTri ( 0, 1, nx );
				AddTri ( 1, nx+1, nx );
			}
			if(i<nx-1 && k<nz-1 && tt[i0+1] && tt[i0+nx*ny] && tt[i0+nx*ny+1])
			{
				AddTri ( 0, 1, nx*ny );
				AddTri ( 1, nx*ny+1, nx*ny );
			}
			if(k<nz-1 && j<ny-1 && tt[i0+nx*ny] && tt[i0+nx] && tt[i0+nx*ny+nx])
			{
				AddTri ( 0, nx, nx*ny );
				AddTri ( nx, nx*ny+nx, nx*ny );
			}
		}
#undef AddTri
		delete []pid;
	}
	EndGroup();
*/
	textures_flag = textures_flag_old;
	delete []aa;	delete []tt;	delete []cc;
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::quad_plot_a ( float *p0,float *p1,float *p2,float *p3,
                                 float a0,float a1,float a2,float a3,float alpha )
{
	register float t,s;
	register long k;
	long n = NumCol-1;
	mglColor c;
	float c0[4], c1[4], c2[4], c3[4];

	s = a0;
	t = alpha* ( alpha>0 ? ( s+1.f ) * ( s+1.f ) : ( 1.f-s ) * ( s-1.f ) );
	s = n* ( s+1.f ) /2.f;	k = long ( s );	s -= k;
	if ( k<n )	c = cmap[k]* ( 1.f-s ) + cmap[k+1]*s;	else	c = cmap[n];
	c0[0] = c.r; c0[1] = c.g; c0[2] = c.b; c0[3] = t;

	s = a1;
	t = alpha* ( alpha>0 ? ( s+1.f ) * ( s+1.f ) : ( 1.f-s ) * ( s-1.f ) );
	s = n* ( s+1.f ) /2.f;	k = long ( s );	s -= k;
	if ( k<n )	c = cmap[k]* ( 1.f-s ) + cmap[k+1]*s;	else	c = cmap[n];
	c1[0] = c.r; c1[1] = c.g; c1[2] = c.b; c1[3] = t;

	s = a3;
	t = alpha* ( alpha>0 ? ( s+1.f ) * ( s+1.f ) : ( 1.f-s ) * ( s-1.f ) );
	s = n* ( s+1.f ) /2.f;	k = long ( s );	s -= k;
	if ( k<n )	c = cmap[k]* ( 1.f-s ) + cmap[k+1]*s;	else	c = cmap[n];
	c2[0] = c.r; c2[1] = c.g; c2[2] = c.b; c2[3] = t;

	s = a2;
	t = alpha* ( alpha>0 ? ( s+1.f ) * ( s+1.f ) : ( 1.f-s ) * ( s-1.f ) );
	s = n* ( s+1.f ) /2.f;	k = long ( s );	s -= k;
	if ( k<n )	c = cmap[k]* ( 1.f-s ) + cmap[k+1]*s;	else	c = cmap[n];
	c3[0] = c.r; c3[1] = c.g; c3[2] = c.b; c3[3] = t;

	quad_plot ( p0, p1, p2, p3, c0, c1, c2, c3 );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::quad_plot ( const mglPoint& p0, const mglPoint& p1, const mglPoint& p2, const mglPoint& p3 )
{
//	if ( dbg ) fprintf ( stderr, "quad_plot\n p0 %f %f %f\n p1 %f %f %f\n p2 %f %f %f\n p3 %f %f %f\n",
//		                     p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z );
	float pp0[3] = {p0.x, p0.y, p0.z};
	float pp1[3] = {p1.x, p1.y, p1.z};
	float pp2[3] = {p2.x, p2.y, p2.z};
	float pp3[3] = {p3.x, p3.y, p3.z};
	GetMesh().quad_plot ( pp0, pp1, pp2, pp3, CDef, CDef, CDef, CDef );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::quad_plot_n ( float *pp0,float *pp1,float *pp2,float *pp3,
                                 float *cc0,float *cc1,float *cc2,float *cc3,
                                 float *nn0,float *nn1,float *nn2,float *nn3 )
{
	GetMesh().quad_plot_n ( pp0, pp1, pp2, pp3, cc0, cc1, cc2, cc3, nn0, nn1, nn2, nn3 );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::trig_plot ( float *pp0,float *pp1,float *pp2,
                               float *cc0,float *cc1,float *cc2 )
{
	GetMesh().trig_plot ( pp0, pp1, pp2, cc0, cc1, cc2 );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::trig_plot ( const mglPoint& p0, const mglPoint& p1, const mglPoint& p2 )
{
	float pp0[3] = {p0.x, p0.y, p0.z};
	float pp1[3] = {p1.x, p1.y, p1.z};
	float pp2[3] = {p2.x, p2.y, p2.z};
	GetMesh().trig_plot ( pp0, pp1, pp2, CDef, CDef, CDef );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::trig_plot_n ( float *pp0,float *pp1,float *pp2,
                                 float *cc0,float *cc1,float *cc2,
                                 float *nn0,float *nn1,float *nn2 )
{
	GetMesh().trig_plot_n ( pp0, pp1, pp2, cc0, cc1, cc2, nn0, nn1, nn2 );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::point_plot ( const mglPoint& p, const mglColor& c )
{
	GetPointSet().point_plot ( p, c );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::line_plot_s ( float *p1,float *p2,float *c1,float *c2,bool all )
{
	GetLineSet().line_plot ( p1, p2, c1, c2 );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::line_plot ( const mglPoint& p0, const mglPoint& p1 )
{
// if (dbg) fprintf(stderr, "line_plot p0 %f %f %f p1 %f %f %f\n", p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);
	float pp0[3] = {p0.x, p0.y, p0.z};
	float pp1[3] = {p1.x, p1.y, p1.z};
	GetLineSet().line_plot ( pp0, pp1, CDef, CDef );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::line_plot ( float *pp0,float *pp1,float *cc0,float *cc1,bool all )
{
	if ( !DrawFace )	{	line_plot_s ( pp0,pp1,cc0,cc1,all );	return;	}
	if ( PDef == 0x0000 ) 	{	return;	}
	if ( pp0[0] == pp1[0] && pp0[1] == pp1[1] && pp0[2] == pp1[2] ) 	{	return;	}
	line_plot_s ( pp0,pp1,cc0,cc1,all );
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::surf_plot ( long n,long m,float *pp,float *cc,bool *tt )
{
	register long i,j,i0;
	float *c,*ns,d1[3],d2[3];
	long k=3*n;
	size_t *cid=NULL;
	size_t *pid=NULL;
	if ( !pp || n<2 || m<2 )	return;
	PostScale ( pp,n*m );	LightScale();
	if ( !DrawFace )	{	wire_plot ( n,m,pp,cc,tt );	return;	}
	ns = new float[3*n*m];
	for ( i=0;i<n-1;i++ )	for ( j=0;j<m-1;j++ )
		{
			i0 = 3* ( i+n*j );
			d1[0] = pp[i0+3]-pp[i0];	d2[0] = pp[i0+k]-pp[i0];
			d1[1] = pp[i0+4]-pp[i0+1];	d2[1] = pp[i0+k+1]-pp[i0+1];
			d1[2] = pp[i0+5]-pp[i0+2];	d2[2] = pp[i0+k+2]-pp[i0+2];
			ns[i0]  = ( d2[2]*d1[1]-d2[1]*d1[2] );		// normal vector
			ns[i0+1]= ( d2[0]*d1[2]-d2[2]*d1[0] );
			ns[i0+2]= ( d2[1]*d1[0]-d2[0]*d1[1] );
		}
	memcpy ( ns+3* ( m-1 ) *n,ns+3* ( m-2 ) *n,3*n*sizeof ( float ) );
	for ( i=0;i<m;i++ )
	{
		i0 = 3* ( n-2+i*n );
		ns[i0+3]=ns[i0];
		ns[i0+4]=ns[i0+1];
		ns[i0+5]=ns[i0+2];
	}
// Let us check if the surface is is actually in one color
	bool onecolor=false;
	const float *thecol=NULL;  // the color of the surface
	if ( !(vertex_color_flag && UseLight) )
	if ( cc )
	{
		onecolor=true;
		for ( i=4;i<n*m;i++ )
		{
			if (cc[0] != cc[i] || cc[1] != cc[i+1] || cc[2] != cc[i+2] || (UseAlpha && cc[3] != cc[i+3]))
			{
				onecolor=false;
				break;
			}
		}
		if (onecolor)
		{
			thecol=cc;
			cc = NULL;
		}
	}
	else
	{
		onecolor=true;
		thecol=CDef;
	}

	mesh_finished = true;
	u3dMesh& Mesh = GetMesh();
	mesh_finished = true;
	if ( textures_flag && cc )
	{
		Mesh.textureDimension = 2;
		Mesh.vertex_color = false;
	}
// Let us check if the surface is flat
#define FLT_EPS	(0.+2e-07)
	bool flat = false;
	if ( textures_flag && cc )
	{
		flat = true;
		const mglPoint dx = mglPoint(pp[3*1+0]-pp[0], pp[3*1+1]-pp[1], pp[3*1+2]-pp[2]);
		const mglPoint dy = mglPoint(pp[3*n+0]-pp[0], pp[3*n+1]-pp[1], pp[3*n+2]-pp[2]);
		for ( j=0;j<m;j++ ) for ( i=0;i<n;i++ )
		{
			i0=i+n*j;
			if ( !tt[i0] ||
				( i>0 && Norm( mglPoint(pp[3*(i0)+0]-pp[3*(i0-1)+0], pp[3*(i0)+1]-pp[3*(i0-1)+1], pp[3*(i0)+2]-pp[3*(i0-1)+2])  - dx ) > FLT_EPS ) ||
				( j>0 && Norm( mglPoint(pp[3*(i0)+0]-pp[3*(i0-n)+0], pp[3*(i0)+1]-pp[3*(i0-n)+1], pp[3*(i0)+2]-pp[3*(i0-n)+2])  - dy ) > FLT_EPS ) )
			{
//	fprintf(stderr,"points %f %f %f x-1 %f %f %f y-1 %f %f %f tt %u\n", pp[3*i0+0], pp[3*i0+1], pp[3*i0+2], pp[3*(i0-1)+0], pp[3*(i0-1)+1], pp[3*(i0-1)+2], pp[3*(i0-n)+0], pp[3*(i0-n)+1], pp[3*(i0-n)+2], tt[i0] );
				flat = false;
				break;
			}
		}
//fprintf(stderr, "flat %u\n", flat );
	}
	if (flat)
	{
		u3dTexture& texture = AddTexture ();
		texture.image.Initialize ( n, m, (UseAlpha?4:3) );
		for ( i=0;i<n*m;i++ )
		{
			texture.image.RGBPixels[(UseAlpha?4:3)*i+0] = cc[4*i+0]*255.0f;
			texture.image.RGBPixels[(UseAlpha?4:3)*i+1] = cc[4*i+1]*255.0f;
			texture.image.RGBPixels[(UseAlpha?4:3)*i+2] = cc[4*i+2]*255.0f;
			if (UseAlpha)
				texture.image.RGBPixels[4*i+3] = cc[4*i+3]*255.0f;
		}
		u3dMaterial Material;
		Material.diffuse = BC;
		Material.specular = BC;
		Material.emissive = WC;
		Material.reflectivity = 0.0f;
		Material.opacity = 1.0f;
		Material.vertex_color = false;
		Material.texture = texture.name;
		Material.texturealpha = UseAlpha;
		Mesh.ModelMaterials.push_back ( AddMaterial ( Material ) );
		TexCoord2D texCoord;
                texCoord.U =       0.5f/n; texCoord.V =       0.5f/m; Mesh.textureCoords.push_back( texCoord );
		texCoord.U = 1.f - 0.5f/n; texCoord.V =       0.5f/m; Mesh.textureCoords.push_back( texCoord );
		texCoord.U =       0.5f/n; texCoord.V = 1.f - 0.5f/m; Mesh.textureCoords.push_back( texCoord );
		texCoord.U = 1.f - 0.5f/n; texCoord.V = 1.f - 0.5f/m; Mesh.textureCoords.push_back( texCoord );
		i=0;
		{
			mglPoint point;
			point.x = Mesh.invpos[0][0]*pp[3*i]+Mesh.invpos[0][1]*pp[3*i+1]+Mesh.invpos[0][2]*pp[3*i+2]+Mesh.invpos[0][3];
			point.y = Mesh.invpos[1][0]*pp[3*i]+Mesh.invpos[1][1]*pp[3*i+1]+Mesh.invpos[1][2]*pp[3*i+2]+Mesh.invpos[1][3];
			point.z = Mesh.invpos[2][0]*pp[3*i]+Mesh.invpos[2][1]*pp[3*i+1]+Mesh.invpos[2][2]*pp[3*i+2]+Mesh.invpos[2][3];
			Mesh.Points.push_back ( point );
		}
		i=n-1;
		{
			mglPoint point;
			point.x = Mesh.invpos[0][0]*pp[3*i]+Mesh.invpos[0][1]*pp[3*i+1]+Mesh.invpos[0][2]*pp[3*i+2]+Mesh.invpos[0][3];
			point.y = Mesh.invpos[1][0]*pp[3*i]+Mesh.invpos[1][1]*pp[3*i+1]+Mesh.invpos[1][2]*pp[3*i+2]+Mesh.invpos[1][3];
			point.z = Mesh.invpos[2][0]*pp[3*i]+Mesh.invpos[2][1]*pp[3*i+1]+Mesh.invpos[2][2]*pp[3*i+2]+Mesh.invpos[2][3];
			Mesh.Points.push_back ( point );
		}
		i=n*m-n;
		{
			mglPoint point;
			point.x = Mesh.invpos[0][0]*pp[3*i]+Mesh.invpos[0][1]*pp[3*i+1]+Mesh.invpos[0][2]*pp[3*i+2]+Mesh.invpos[0][3];
			point.y = Mesh.invpos[1][0]*pp[3*i]+Mesh.invpos[1][1]*pp[3*i+1]+Mesh.invpos[1][2]*pp[3*i+2]+Mesh.invpos[1][3];
			point.z = Mesh.invpos[2][0]*pp[3*i]+Mesh.invpos[2][1]*pp[3*i+1]+Mesh.invpos[2][2]*pp[3*i+2]+Mesh.invpos[2][3];
			Mesh.Points.push_back ( point );
		}
		i=n*m-1;
		{
			mglPoint point;
			point.x = Mesh.invpos[0][0]*pp[3*i]+Mesh.invpos[0][1]*pp[3*i+1]+Mesh.invpos[0][2]*pp[3*i+2]+Mesh.invpos[0][3];
			point.y = Mesh.invpos[1][0]*pp[3*i]+Mesh.invpos[1][1]*pp[3*i+1]+Mesh.invpos[1][2]*pp[3*i+2]+Mesh.invpos[1][3];
			point.z = Mesh.invpos[2][0]*pp[3*i]+Mesh.invpos[2][1]*pp[3*i+1]+Mesh.invpos[2][2]*pp[3*i+2]+Mesh.invpos[2][3];
			Mesh.Points.push_back ( point );
		}
		Mesh.AddTriangle( 0, 1, 2, 0, 1, 2);
		Mesh.AddTriangle( 1, 3, 2, 1, 3, 2);
		delete []ns;
		if (cid) delete []cid;
		if (pid) delete []pid;
		return;
	}
#undef FLT_EPS
	pid = new size_t[n*m];
	for ( i=0;i<n*m;i++ )
	{
		if ( tt[i] )
		{
			mglPoint point;
			point.x = Mesh.invpos[0][0]*pp[3*i]+Mesh.invpos[0][1]*pp[3*i+1]+Mesh.invpos[0][2]*pp[3*i+2]+Mesh.invpos[0][3];
			point.y = Mesh.invpos[1][0]*pp[3*i]+Mesh.invpos[1][1]*pp[3*i+1]+Mesh.invpos[1][2]*pp[3*i+2]+Mesh.invpos[1][3];
			point.z = Mesh.invpos[2][0]*pp[3*i]+Mesh.invpos[2][1]*pp[3*i+1]+Mesh.invpos[2][2]*pp[3*i+2]+Mesh.invpos[2][3];
			pid[i] = Mesh.Points.size();
			Mesh.Points.push_back ( point );
		}
		else
		{
			pid[i] = SIZE_MAX;
		}
		float nn = sqrt ( ns[3*i]*ns[3*i]+ns[3*i+1]*ns[3*i+1]+ns[3*i+2]*ns[3*i+2] );
		if ( nn != 0.0 ) { ns[3*i]/=nn; ns[3*i+1]/=nn; ns[3*i+2]/=nn; }
	}
// Do lighting
	if ( !textures_flag && vertex_color_flag && (cc || UseLight) )
	{
		cid = new size_t[n*m];
		const float color [4] = {1.0f, 1.0f, 1.0f, 1.0f};
		Mesh.AddModelMaterial ( color, false, true );
		for ( i=0;i<n*m;i++ )
		{
			float col [4];
			if (cc)
			{
				col2col ( cc+4*i, ns+3*i, col );
			}
			else
			{
				col2col ( thecol, ns+3*i, col );
			}
			cid[i] = Mesh.AddColor ( col );
		}
	}
// fprintf(stderr,"point %f %f %f color %f %f %f %f enabled %u\n", pp[3*i+0], pp[3*i+1], pp[3*i+2], cc[4*i+0], cc[4*i+1], cc[4*i+2], cc[4*i+3], tt[i] );
// Make texture
	if ( textures_flag && cc )
	{
		u3dTexture& texture = AddTexture ();
		texture.image.Initialize ( n, m, (UseAlpha?4:3) );
		for ( i=0;i<n*m;i++ )
		{
			texture.image.RGBPixels[(UseAlpha?4:3)*i+0] = cc[4*i+0]*255.0f;
			texture.image.RGBPixels[(UseAlpha?4:3)*i+1] = cc[4*i+1]*255.0f;
			texture.image.RGBPixels[(UseAlpha?4:3)*i+2] = cc[4*i+2]*255.0f;
			if (UseAlpha)
				texture.image.RGBPixels[4*i+3] = cc[4*i+3]*255.0f;
		}
		u3dMaterial Material;
		Material.diffuse = WC;
		Material.specular = 0.5*WC;
		Material.emissive = BC;
		Material.reflectivity = 0.25f;
		Material.opacity = 1.0f;
		Material.vertex_color = false;
		Material.texture = texture.name;
		Material.texturealpha = UseAlpha;
		Mesh.ModelMaterials.push_back ( AddMaterial ( Material ) );
		for ( j=0;j<m;j++ ) for ( i=0;i<n;i++ )
			if ( tt[i+n*j] )
				Mesh.textureCoords.push_back ( TexCoord2D ( (0.5 + i)/n, (0.5 + j)/m ) );
	}
// if only one color is used
	if (!cc && !(vertex_color_flag && UseLight))
	{
		Mesh.AddModelMaterial ( thecol, false, false );
		Mesh.vertex_color = false;
	}
#define AddTri(i1, i2, i3)	\
		if ( Mesh.textureDimension > 0 && cc )							\
		{										\
			Mesh.AddTriangle(pid[i0+(i1)], pid[i0+(i2)], pid[i0+(i3)], pid[i0+(i1)], pid[i0+(i2)], pid[i0+(i3)]);	\
		}										\
		else if ( Mesh.vertex_color && (cc || UseLight) )				\
		{										\
			Mesh.AddTriangle(pid[i0+(i1)], pid[i0+(i2)], pid[i0+(i3)], cid[i0+(i1)], cid[i0+(i2)], cid[i0+(i3)]);	\
		}										\
		else										\
		{										\
			if (cc)									\
			{									\
				float col [4];							\
				size_t mid;							\
				col[0] = (c[4*(i1)+0] + c[4*(i2)+0] + c[4*(i3)+0])/3.0f;	\
				col[1] = (c[4*(i1)+1] + c[4*(i2)+1] + c[4*(i3)+1])/3.0f;	\
				col[2] = (c[4*(i1)+2] + c[4*(i2)+2] + c[4*(i3)+2])/3.0f;	\
				col[3] = (c[4*(i1)+3] + c[4*(i2)+3] + c[4*(i3)+3])/3.0f;	\
				mid = Mesh.AddModelMaterial(col, false, false);			\
				Mesh.AddTriangle(pid[i0+(i1)], pid[i0+(i2)], pid[i0+(i3)], mid);	\
			}									\
			else									\
			{									\
				Mesh.AddTriangle(pid[i0+(i1)], pid[i0+(i2)], pid[i0+(i3)], 0);	\
			}									\
		}
	for ( j=0;j<m-1;j++ )	for ( i=0;i<n-1;i++ )
	{
		i0 = i+n*j;	c = cc+4*i0;
		if ( !tt || ( tt[i0] && tt[i0+1] && tt[i0+n] && tt[i0+1+n] ) )
		{
			AddTri ( 0, 1, n )
			AddTri ( 1, n+1, n )
		}
		else if ( tt[i0] && tt[i0+1] && tt[i0+n] )
		{	AddTri ( 0, 1, n )	}
		else if ( tt[i0] && tt[i0+1] && tt[i0+n+1] )
		{	AddTri ( 0, 1, n+1 )	}
		else if ( tt[i0] && tt[i0+n+1] && tt[i0+n] )
		{	AddTri ( 0, n+1, n )	}
		else if ( tt[i0+n+1] && tt[i0+1] && tt[i0+n] )
		{	AddTri ( n+1, 1, n )	}
	}
#undef AddTri
	delete []ns;
	if (cid) delete []cid;
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::arrow_plot ( float *p1,float *p2,char st )
{
//	 mglGraphAB::arrow_plot(p1, p2, st);
	if ( !strchr ( "AVKSDTIO",st ) )	return;
	float ss = 0.35*ArrowSize*font_factor;
	mglPoint p = mglPoint ( p1[0], p1[1], p1[2] );
	mglPoint l = mglPoint ( p2[0], p2[1], p2[2] )-p;	// unit vector in the direction of the arrow
	if ( Norm ( l ) == 0.0 ) return;
	l = l/Norm ( l );
	mglPoint k = !l;		// unit vector orthigonal to l and parallel to "screen"
	k = k/Norm ( k );
	mglPoint m = l^k;		// unit vector orthogonal to l and k
	l = ss*l;
	k = ss*k;
	m = ss*m;

	bool ul = UseLight;		UseLight = false;
	switch ( st )
	{
		case 'I':
			line_plot ( p-k,p+k );		break;
		case 'D':
			trig_plot ( p-k,p+l,p+k );
			trig_plot ( p+k,p-l,p-k );		break;
		case 'S':
			trig_plot ( p-l-k,p+l-k,p+l+k );
			trig_plot ( p+l+k,p-l+k,p-l-k );	break;
		case 'T':
			trig_plot ( p-l-k,p-l+k,p+l );	break;
		case 'A':
			k = 0.5*k; m = 0.5*m;
			trig_plot ( p,p+2.*l+k+m, p+2.*l+k-m );
			trig_plot ( p,p+2.*l+k-m, p+2.*l-k-m );
			trig_plot ( p,p+2.*l-k-m, p+2.*l-k+m );
			trig_plot ( p,p+2.*l-k+m, p+2.*l+k+m );
			break;
		case 'K':
			trig_plot ( p,p+2.*l-k,p+1.5*l );
			trig_plot ( p,p+1.5*l,p+2.*l+k );
			line_plot ( p-k,p+k );		break;
		case 'V':
			trig_plot ( p,p-2.*l-k,p-1.5*l );
			trig_plot ( p,p-1.5*l,p-2.*l+k );	break;
		case 'O':
			mglPoint p1, p2;
			p1 = p+l;
			for ( int i=1; i<32; i++ )
			{
				p2 = p + cos ( i*M_PI/16. ) *l + sin ( i*M_PI/16. ) *k;
				trig_plot ( p,p1,p2 );
				p1 = p2;
			}
			p2 = p+l;
			trig_plot ( p,p1,p2 );
	}
	UseLight = ul;
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::InPlot ( float x1,float x2,float y1,float y2, bool rel )
{
	mglGraphAB::InPlot ( x1,x2,y1,y2, rel);
	points_finished = lines_finished = mesh_finished = true;
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::WriteIDTF ( const char *fname,const char *descr )
{
	std::ofstream ostr ( fname );

	Lights.clear();
	if ( UseLight )
	{
		SetAmbientLight();
		LightScale();
		for ( int i=0; i<10; i++ )
			if ( nLight[i] )
			{
				if ( unrotate_flag )
					AddLight ( mglPoint ( rLight[3*i], rLight[3*i+1], rLight[3*i+2] ),
				           mglColor ( cLight[3*i], cLight[3*i+1], cLight[3*i+2] ),
				           bLight[i], iLight[i] );
				else
					AddLight ( mglPoint ( pLight[3*i], pLight[3*i+1], pLight[3*i+2] ),
				           mglColor ( cLight[3*i], cLight[3*i+1], cLight[3*i+2] ),
				           bLight[i], iLight[i] );
			}
	}
// Cleanup
// Remove empty models
	for ( u3dPointSet_list::iterator it = PointSets.begin(); it != PointSets.end(); ++it )
	{
		if (it->Points.empty())
		{
			if ( it->parent )
				it->parent->NumberOfChildren--;
			PointSets.erase(it);
		}
	}
	for ( u3dLineSet_list::iterator it = LineSets.begin(); it != LineSets.end(); ++it )
	{
		if (it->Points.empty() || it->Lines.empty())
		{
			if ( it->parent )
				it->parent->NumberOfChildren--;
			LineSets.erase(it);
		}
	}
	for ( u3dMesh_list::iterator it = Meshes.begin(); it != Meshes.end(); ++it )
	{
		if (it->Points.empty() || it->Triangles.empty())
		{
			if ( it->parent )
				it->parent->NumberOfChildren--;
			Meshes.erase(it);
		}
	}
// Remove automatically created groups that are the only object in manually created ones
	for ( u3dGroup_list::iterator it = Groups.begin(); it != Groups.end(); ++it )
	{
		if (it->isauto && it->parent && !it->parent->isauto && it->parent->NumberOfChildren == 1)
		{
			it->name = it->parent->name;
			it->parent->NumberOfChildren = 0;
			it->parent = it->parent->parent;
			it->isauto = false;
		}
	}
// Reduce groups with just one model in them to just models
	for ( u3dPointSet_list::iterator it = PointSets.begin(); it != PointSets.end(); ++it )
	{
		if (it->parent && it->parent->NumberOfChildren == 1)
		{
			it->name = it->parent->name;
			it->parent->NumberOfChildren = 0;
			it->parent = it->parent->parent;
		}
	}
	for ( u3dLineSet_list::iterator it = LineSets.begin(); it != LineSets.end(); ++it )
	{
		if (it->parent && it->parent->NumberOfChildren == 1)
		{
			it->name = it->parent->name;
			it->parent->NumberOfChildren = 0;
			it->parent = it->parent->parent;
		}
	}
	for ( u3dMesh_list::iterator it = Meshes.begin(); it != Meshes.end(); ++it )
	{
		if (it->parent && it->parent->NumberOfChildren == 1)
		{
			it->name = it->parent->name;
			it->parent->NumberOfChildren = 0;
			it->parent = it->parent->parent;
		}
	}

// Make inverse coordinate transform with the model, if so desired
	if (unrotate_flag)
		MakeTransformMatrix( mgl_globpos, mgl_globinv);
	else
		memcpy( mgl_globinv, mgl_definv, sizeof(mgl_definv));

	ostr << std::fixed << std::setprecision(6)
	<< "FILE_FORMAT \"IDTF\"\n"
	<< "FORMAT_VERSION 100\n"
	<< "\n"
	<< "NODE \"VIEW\" {\n"
	<< "	NODE_NAME \"DefaultView\"\n"
	<< "    PARENT_LIST {\n"
	<< "		PARENT_COUNT 1\n"
	<< "		PARENT 0 {\n"
	<< "			PARENT_NAME \"<NULL>\"\n"
	<< "			PARENT_TM {\n"
	<< "				1 0 0 0\n"
	<< "				0 1 0 0\n"
	<< "				0 0 1 0\n"
	<< "				0 0 2 1\n"
	<< "			}\n"
	<< "		}\n"
	<< "	}\n"
	<< "	RESOURCE_NAME \"SceneViewResource\"\n"
	<< "	VIEW_DATA {\n"
	<< "		VIEW_TYPE \"ORTHO\"\n"
	<< "		VIEW_PROJECTION 2\n"
	<< "		VIEW_PORT_WIDTH 2\n"
	<< "		VIEW_PORT_HEIGHT 2\n"
	<< "	}\n"
	<< "}\n"
	<< "\n";
	for ( u3dGroup_list::iterator it = Groups.begin(); it != Groups.end(); ++it )
	{
		if ( it->name == "<NULL>" || it->NumberOfChildren == 0 )
			continue;
		u3dNode Node;
		Node.name = it->name;
		if ( it->parent == NULL)
			Node.parent = "<NULL>";
		else
			Node.parent = it->parent->name;
		memcpy ( Node.position, mgl_idtrans, sizeof ( mgl_idtrans ) );
		Node.type = "GROUP";
		Node.print ( ostr );
	}
	for ( u3dLight_list::iterator it = Lights.begin(); it != Lights.end(); ++it )
	{
		it->print_node ( ostr );
	}
	for ( u3dPointSet_list::iterator it = PointSets.begin(); it != PointSets.end(); ++it )
	{
		it->print_node ( ostr );
	}
	for ( u3dLineSet_list::iterator it = LineSets.begin(); it != LineSets.end(); ++it )
	{
		it->print_node ( ostr );
	}
	for ( u3dMesh_list::iterator it = Meshes.begin(); it != Meshes.end(); ++it )
	{
		it->print_node ( ostr );
	}
	for ( u3dBall_list::iterator it = Balls.begin(); it != Balls.end(); ++it )
	{
		it->print_node ( ostr );
	}
	ostr
	<< "\n"
	<< "RESOURCE_LIST \"VIEW\" {\n"
	<< "	RESOURCE_COUNT 1\n"
	<< "	RESOURCE 0 {\n"
	<< "		RESOURCE_NAME \"SceneViewResource\"\n"
	<< "		VIEW_PASS_COUNT 1\n"
	<< "		VIEW_ROOT_NODE_LIST {\n"
	<< "			ROOT_NODE 0 {\n"
	<< "				ROOT_NODE_NAME \"<NULL>\"\n"
	<< "			}\n"
	<< "		}\n"
	<< "	}\n"
	<< "}\n"
	<< "\n";

	unsigned int ResourceCount;

	// Write lights
	if ( !Lights.empty() )
	{
		ostr << "RESOURCE_LIST \"LIGHT\" {\n"
		<< "\tRESOURCE_COUNT " << Lights.size() << "\n";
		ResourceCount = 0;
		for ( u3dLight_list::iterator it = Lights.begin(); it != Lights.end(); ++it )
		{
			ostr << "\tRESOURCE " << ResourceCount++ << " {\n";
			it->print_light_resource ( ostr );
			ostr << "\t}\n";
		}
		ostr << "}\n"
		<< "\n";
	}

	if ( Balls.size() != 0 )
	{
		UnitBall();
	}

	// Write models
	ostr << "RESOURCE_LIST \"MODEL\" {\n"
	<< "\tRESOURCE_COUNT " << ( PointSets.size() + LineSets.size() + Meshes.size() ) << "\n";
	ResourceCount = 0;
	for ( u3dPointSet_list::iterator it = PointSets.begin(); it != PointSets.end(); ++it )
	{
		ostr << "\tRESOURCE " << ResourceCount++ << " {\n";
		it->print_model_resource ( ostr );
		ostr << "\t}\n";
	}
	for ( u3dLineSet_list::iterator it = LineSets.begin(); it != LineSets.end(); ++it )
	{
		ostr << "\tRESOURCE " << ResourceCount++ << " {\n";
		it->print_model_resource ( ostr );
		ostr << "\t}\n";
	}
	for ( u3dMesh_list::iterator it = Meshes.begin(); it != Meshes.end(); ++it )
	{
		ostr << "\tRESOURCE " << ResourceCount++ << " {\n";
		it->print_model_resource ( ostr );
		ostr << "\t}\n";
	}
	ostr << "}\n"
	<< "\n";

	if ( Balls.size() != 0 )
	{
		Meshes.pop_back();
	}

	// Write shaders
	ostr << "RESOURCE_LIST \"SHADER\" {\n"
	<< "\tRESOURCE_COUNT " << Materials.size() << "\n";
	ResourceCount = 0;
	for ( u3dMaterial_list::iterator it = Materials.begin(); it != Materials.end(); ++it )
	{
		ostr << "\tRESOURCE " << ResourceCount++ << " {\n";
		it->print_shader ( ostr );
		ostr << "\t}\n";
	}
	ostr << "}\n"
	<< "\n";

	// Write materials
	ostr << "RESOURCE_LIST \"MATERIAL\" {\n"
	<< "\tRESOURCE_COUNT " << Materials.size() << "\n";
	ResourceCount = 0;
	for ( u3dMaterial_list::iterator it = Materials.begin(); it != Materials.end(); ++it )
	{
		ostr << "\tRESOURCE " << ResourceCount++ << " {\n";
		it->print_material ( ostr );
		ostr << "\t}\n";
	}
	ostr << "}\n"
	<< "\n";

	// Write textures
	if ( Textures.size() != 0 )
	{
		ostr << "RESOURCE_LIST \"TEXTURE\" {\n"
		<< "\tRESOURCE_COUNT " << Textures.size() << "\n";
		ResourceCount = 0;
		for ( u3dTexture_list::iterator it = Textures.begin(); it != Textures.end(); ++it )
		{
			ostr << "\tRESOURCE " << ResourceCount++ << " {\n";
			it->print_texture ( fname, ostr );
			ostr << "\t}\n";
		}
		ostr << "}\n"
		<< "\n";
	}

	// Write shading modifiers
	for ( u3dPointSet_list::iterator it = PointSets.begin(); it != PointSets.end(); ++it )
	{
		it->print_shading_modifier ( ostr );
	}
	for ( u3dLineSet_list::iterator it = LineSets.begin(); it != LineSets.end(); ++it )
	{
		it->print_shading_modifier ( ostr );
	}
	for ( u3dMesh_list::iterator it = Meshes.begin(); it != Meshes.end(); ++it )
	{
		it->print_shading_modifier ( ostr );
	}
	for ( u3dBall_list::iterator it = Balls.begin(); it != Balls.end(); ++it )
	{
		it->print_shading_modifier ( ostr );
	}

	ostr.close ();

}
//-----------------------------------------------------------------------------
void mglGraphIDTF::quads_plot(long n,float *pp,float *cc,bool *tt)
{
	register long i;
	float *p=NULL, *c=NULL;
	u3dMesh *pMesh = NULL;;
	u3dModel *pModel = NULL;;
	u3dLineSet *pLineSet = NULL;
	if(DrawFace)
	{
		pModel = pMesh = &GetMesh();
	}
	else
	{
		pModel = pLineSet = &GetLineSet();
	}
	PostScale(pp,4*n);	LightScale();
	if(cc)
	{

		size_t mid = SIZE_MAX;
		if(DrawFace)
		{
			if ( pMesh->vertex_color )
			{
				const float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
				mid = pMesh->AddModelMaterial ( color, false, true );
			}
		}

		for(i=0;i<n;i++)
		{
			if(tt && (!tt[4*i] || !tt[4*i+1] || !tt[4*i+2] || !tt[4*i+3]))
				continue;
			p = pp+12*i;	c = cc+16*i;

			size_t pid0 = pModel->AddPoint ( p );
			size_t pid1 = pModel->AddPoint ( p+3 );
			size_t pid2 = pModel->AddPoint ( p+6 );
			size_t pid3 = pModel->AddPoint ( p+9 );

			if(DrawFace)
			{
				if ( pMesh->vertex_color )
				{
					size_t cid0 = pMesh->AddColor ( c );
					size_t cid1 = pMesh->AddColor ( c+4 );
					size_t cid2 = pMesh->AddColor ( c+8 );
					size_t cid3 = pMesh->AddColor ( c+12 );
					pMesh->AddTriangle ( pid0, pid1, pid3, cid0, cid1, cid3 );
					pMesh->AddTriangle ( pid1, pid2, pid3, cid1, cid2, cid3 );
				}
				else
				{
					float color[4];
					color[0] = ( c[0]+ c[4]+c[12] ) /3.0f;
					color[1] = ( c[1]+ c[5]+c[13] ) /3.0f;
					color[2] = ( c[2]+ c[6]+c[14] ) /3.0f;
					color[3] = ( c[3]+ c[7]+c[15] ) /3.0f;
					mid = pMesh->AddModelMaterial ( color, false, false );
					pMesh->AddTriangle ( pid0, pid1, pid3, mid );
					color[0] = ( c[4]+ c[8]+c[12] ) /3.0f;
					color[1] = ( c[5]+ c[9]+c[13] ) /3.0f;
					color[2] = ( c[6]+c[10]+c[14] ) /3.0f;
					color[3] = ( c[7]+c[11]+c[15] ) /3.0f;
					mid = pMesh->AddModelMaterial ( color, false, false );
					pMesh->AddTriangle ( pid1, pid2, pid3, mid );
				}
			}
			else
			{
				float color[4];
				color[3] = 1.0f;
				color[0] = ( c[ 0+0] + c[ 4+0] ) /2.0f;
				color[1] = ( c[ 0+1] + c[ 4+1] ) /2.0f;
				color[2] = ( c[ 0+2] + c[ 4+2] ) /2.0f;
				mid =  pLineSet->AddModelMaterial ( color, true, false );
				pLineSet->AddLine( pid0, pid1, mid );
				color[0] = ( c[ 0+0] + c[12+0] ) /2.0f;
				color[1] = ( c[ 0+1] + c[12+1] ) /2.0f;
				color[2] = ( c[ 0+2] + c[12+2] ) /2.0f;
				mid =  pLineSet->AddModelMaterial ( color, true, false );
				pLineSet->AddLine( pid0, pid3, mid );
				color[0] = ( c[12+0] + c[ 8+0] ) /2.0f;
				color[1] = ( c[12+1] + c[ 8+1] ) /2.0f;
				color[2] = ( c[12+2] + c[ 8+2] ) /2.0f;
				mid =  pLineSet->AddModelMaterial ( color, true, false );
				pLineSet->AddLine( pid3, pid2, mid );
				color[0] = ( c[ 4+0] + c[ 8+0] ) /2.0f;
				color[1] = ( c[ 4+1] + c[ 8+1] ) /2.0f;
				color[2] = ( c[ 4+2] + c[ 8+2] ) /2.0f;
				mid =  pLineSet->AddModelMaterial ( color, true, false );
				pLineSet->AddLine( pid1, pid2, mid );
			}
		}
	}
	else
	{

		size_t mid = SIZE_MAX;
		size_t cid = SIZE_MAX;
		if(DrawFace)
		{
			if ( pMesh->vertex_color )
			{
				const float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
				mid = pMesh->AddModelMaterial ( color, false, true );
				cid = pMesh->AddColor ( CDef );
			}
			else
			{
				mid = pMesh->AddModelMaterial ( CDef, false, false );
			}
		}
		else
		{
			mid = pLineSet->AddModelMaterial ( CDef, true, false );
		}

		for(i=0;i<n;i++)
		{
			if(tt && (!tt[4*i] || !tt[4*i+1] || !tt[4*i+2] || !tt[4*i+3]))
				continue;
			p = pp+12*i;

			size_t pid0 = pModel->AddPoint ( p );
			size_t pid1 = pModel->AddPoint ( p+3 );
			size_t pid2 = pModel->AddPoint ( p+6 );
			size_t pid3 = pModel->AddPoint ( p+9 );

			if(DrawFace)
			{
				if ( pMesh->vertex_color )
				{
					pMesh->AddTriangle ( pid0, pid1, pid3, cid, cid, cid );
					pMesh->AddTriangle ( pid1, pid2, pid3, cid, cid, cid );
				}
				else
				{
					pMesh->AddTriangle ( pid0, pid1, pid3, mid );
					pMesh->AddTriangle ( pid1, pid2, pid3, mid );
				}
			}
			else
			{
				pLineSet->AddLine( pid0, pid1, mid );
				pLineSet->AddLine( pid0, pid3, mid );
				pLineSet->AddLine( pid3, pid2, mid );
				pLineSet->AddLine( pid1, pid2, mid );
			}
		}
	}
}
//-----------------------------------------------------------------------------
// I do not remember what problem this workaround was initially for
void mglGraphIDTF::Putsw(mglPoint p,const wchar_t *text,const char *font,float size,char dir,float shift)
{
	bool disable_compression_sav = disable_compression_flag;
	bool vertex_color_sav = vertex_color_flag;
	disable_compression_flag = true;
	vertex_color_flag = false;
	mglGraphAB::Putsw(p, text, font, size, dir, shift);
	disable_compression_flag = disable_compression_sav;
	vertex_color_flag = vertex_color_sav;
}
float mglGraphIDTF::Putsw(mglPoint p,mglPoint l,const wchar_t *text,char font,float size)
{
	bool disable_compression_sav = disable_compression_flag;
	bool vertex_color_sav = vertex_color_flag;
	disable_compression_flag = true;
	vertex_color_flag = false;
	float ret = mglGraphAB::Putsw(p, l, text, font, size);
	disable_compression_flag = disable_compression_sav;
	vertex_color_flag = vertex_color_sav;
	return ret;
}
