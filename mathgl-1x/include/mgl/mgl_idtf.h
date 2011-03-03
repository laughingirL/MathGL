/***************************************************************************
 * mgl_idtf.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
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
//-----------------------------------------------------------------------------
#ifdef _MGL_W_H_
#warning "MathGL wrapper was enabled. So disable original MathGL classes"
#else
#ifndef _MGL_IDTF_H_
#define _MGL_IDTF_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
# include <list>
# include <vector>
# include <deque>
# include <string>
# include <sstream>
# include <iostream>
# include <fstream>

#ifdef WIN32
#include <windows.h>
#endif
#ifndef SIZE_MAX
#define SIZE_MAX size_t(-1)
#endif
#include "mgl/mgl_ab.h"
//-----------------------------------------------------------------------------
/// basic u3d classes and data structures
class mglGraphIDTF;
//-----------------------------------------------------------------------------
struct u3dNode
{
	mreal position[4][4];
	std::string parent;
	std::string resource;
	bool both_visible;
	std::string type;
	std::string name;
	u3dNode() : both_visible ( true ) {};
	void print ( std::ofstream& ostr );
};
//-----------------------------------------------------------------------------
struct u3dLight
{
	std::string type;
	mglColor color;
	mreal position[4][4];
	mreal intensity;
	mreal attenuation;
	std::string name;
	void print_light_resource ( std::ofstream& ostr );
	void print_node ( std::ofstream& ostr );
};
typedef std::list<u3dLight>  u3dLight_list;
//-----------------------------------------------------------------------------
/// Class contain color description and opacity. The color is used as diffuse and specular color (unlike other parts of MathGL, where specular color is assumed to be white). Ambient color is set to be 0.5 of the color in description. If emissive flag is set - the emissive color of material is set to the described color, otherwise to 1/8 of it. A flag controls if the vertexes can have colors assigned to them.
class u3dMaterial
{
public:
	mglColor diffuse;
	mglColor specular;
	mglColor emissive;
	mreal reflectivity;
	mreal opacity;
	bool vertex_color;
	std::string name;
	std::string texture;
	bool textumrealpha;

	u3dMaterial() : diffuse(BC), specular(BC), emissive(BC), reflectivity(0.25f), opacity(1.0f), vertex_color(false) {};
	bool operator== ( const u3dMaterial& Material )
	{
		return( this->diffuse	==  Material.diffuse &&
			this->specular	==  Material.specular &&
			this->emissive	==  Material.emissive &&
			this->opacity       ==  Material.opacity &&
			this->reflectivity  ==  Material.reflectivity &&
			this->vertex_color  ==  Material.vertex_color &&
			this->texture.empty() && Material.texture.empty() );
	}
	void print_material ( std::ofstream& ostr );
	void print_shader ( std::ofstream& ostr );
};
typedef std::deque<u3dMaterial> u3dMaterial_list;
//-----------------------------------------------------------------------------
/// This class is used for manipulation with TGA images.
class TGAImageMin
{
public:
	TGAImageMin();
	~TGAImageMin();

	bool Initialize( uint32_t width, uint32_t height, uint32_t channels );
	void Deallocate();

	bool Write( const char* pFileName ) const;

	uint32_t Width, Height;
	uint32_t Channels;
	uint8_t* RGBPixels; // RGBA
};
class u3dTexture
{
public:
	std::string name;
	TGAImageMin image;
	void print_texture ( const char *fname, std::ofstream& ostrtmp );
};
typedef std::list<u3dTexture>  u3dTexture_list;
//-----------------------------------------------------------------------------
struct u3dGroup
{
	std::string name;
	u3dGroup* parent;
	size_t NumberOfChildren;
	bool isauto;
	u3dGroup(): parent ( NULL ), NumberOfChildren ( 0 ), isauto ( false ) {};
};
typedef std::list<u3dGroup>  u3dGroup_list;
//-----------------------------------------------------------------------------
typedef std::vector<mglPoint> mglPoint_list;
typedef std::vector<mglColor> mglColor_list;
typedef std::vector<size_t> ModelMaterial_list;
/// Class describe an object in the scene. Base class for PointSet, LineSet, Mesh. Keeps a vector of vertexes used, colors assigned to them (if any), names of materials used in the model. Has a name and may belong to group of objects (have a parent).
class u3dModel
{
public:
	mglPoint_list Points;
	mglColor_list Colors;
	mreal position[4][4];
	mreal invpos[4][4];

	bool vertex_color;
	ModelMaterial_list ModelMaterials;
	bool both_visible;
	mglGraphIDTF *Graph;
	std::string name;
	u3dGroup* parent;
	u3dModel ( const std::string name, mglGraphIDTF *Graph, const bool& vertex_color );
	void print_node ( std::ofstream& ostr );
	void print_shading_modifier ( std::ofstream& ostr );
	size_t AddModelMaterial ( const mreal *c, bool emissive, bool vertex_color );
	size_t AddPoint ( const mreal *p );
	size_t AddPoint ( const mglPoint& p );
	size_t AddColor ( const mreal *c );
	size_t AddColor ( const mglColor& c );
};
//-----------------------------------------------------------------------------
typedef std::list<u3dModel>  u3dModel_list;
//-----------------------------------------------------------------------------
class u3dBall
{
public:
	mglPoint center;
	mreal radius;
	size_t material;

	mglGraphIDTF *Graph;
	std::string name;
	u3dGroup* parent;
	void print_node ( std::ofstream& ostr );
	void print_shading_modifier ( std::ofstream& ostr );
};
typedef std::vector<u3dBall> u3dBall_list;
//-----------------------------------------------------------------------------
struct u3dLine
{
	size_t pid1;
	size_t pid2;
	size_t mid;
};
typedef std::vector<u3dLine> u3dLine_list;
//-----------------------------------------------------------------------------
/// Class for any line segment indexes of both end points and index of used material are stored. Per-vertes colors not supported. Line patterns not supported.
class u3dLineSet : public u3dModel
{
public:
	u3dLineSet ( const std::string& name, mglGraphIDTF *Graph ) :
			u3dModel ( name, Graph, false )
	{
		this->both_visible = false;
	}
	u3dLine_list  Lines;
	void line_plot ( mreal *p1, mreal *p2, mreal *c1, mreal *c2 );
	void print_model_resource ( std::ofstream& ostrtmp );
	void AddLine ( size_t pid1, size_t pid2, size_t mid );
};
//-----------------------------------------------------------------------------
typedef std::list<u3dLineSet>  u3dLineSet_list;
//-----------------------------------------------------------------------------
/// Class does not extend u3dModel in any way, since in Adobe products points are always colored black.
class u3dPointSet : public u3dModel
{
public:
//	std::vector<size_t> pointShaders;
//	std::vector<size_t> pointColors; does not work due to Adobe bug
	u3dPointSet ( const std::string& name, mglGraphIDTF *Graph );
	void point_plot ( const mglPoint& p, const mglColor& c );
	void print_model_resource ( std::ofstream& ostrtmp );
};
//-----------------------------------------------------------------------------
typedef std::list<u3dPointSet>  u3dPointSet_list;
//-----------------------------------------------------------------------------
struct size_t3
{
	size_t a;
	size_t b;
	size_t c;
};
struct TexCoord2D
{
	mreal U;
	mreal V;
	TexCoord2D ( ) { }
	TexCoord2D ( mreal u, mreal v ) { U = u; V = v; }
};
typedef std::vector<TexCoord2D>  TexCoord2D_list;
/// Class contain meshes which are presented as a set of triangles. Indexes of vertexes, index of material, indexes of vertex colors are stored for each triangle. But mreally there are two modes, with and without per-vertex colors. If vertex colors are used, colors change smoothly along the surface, but transparency and lighting are disabled. If there are no vertex colors, the appearance is affected by materials, transparency and lighting are possible, but a large number of different materials results in slowly displayed models. Boolean parameter vertex_color_flag controls the mode. If you are drawing something in a small set of colors, it is advised to set it to false in order to get lighting for your model. (If exacly one color is used the per-vertex coloring is disabled automatically).
class u3dMesh : public u3dModel
{
public:
	TexCoord2D_list textureCoords;
	std::vector<size_t3> Triangles;
	std::vector<size_t> faceShaders;
	std::vector<size_t3> faceColors;
	bool disable_compression;
	size_t textureDimension;
	u3dMesh ( const std::string& name, mglGraphIDTF *Graph,
				const bool& vertex_color, const bool& disable_compression ) : u3dModel ( name, Graph, vertex_color )
	{
		this->disable_compression = disable_compression;
		this->textureDimension = 0;
	}
	void quad_plot ( mreal *pp0,mreal *pp1,mreal *pp2,mreal *pp3,
						mreal *cc0,mreal *cc1,mreal *cc2,mreal *cc3 );
	void quad_plot_n ( mreal *pp0,mreal *pp1,mreal *pp2,mreal *pp3,
						mreal *cc0,mreal *cc1,mreal *cc2,mreal *cc3,
						mreal *nn0,mreal *nn1,mreal *nn2,mreal *nn3 );
	void trig_plot ( mreal *pp0,mreal *pp1,mreal *pp2,
						mreal *cc0,mreal *cc1,mreal *cc2 );
	void trig_plot_n ( mreal *pp0,mreal *pp1,mreal *pp2,
						mreal *cc0,mreal *cc1,mreal *cc2,
						mreal *nn0,mreal *nn1,mreal *nn2 );
	void print_model_resource ( std::ofstream& ostrtmp );
	void AddTriangle ( size_t pid0, size_t pid1, size_t pid2,
						size_t cid0, size_t cid1, size_t cid2);
	void AddTriangle ( size_t pid0, size_t pid1, size_t pid2, size_t mid);
};
//-----------------------------------------------------------------------------
typedef std::list<u3dMesh>  u3dMesh_list;
//-----------------------------------------------------------------------------
/// Class implements the creation of different mathematical plots under OpenGL
class mglGraphIDTF : public mglGraphAB
{
public:
	mglGraphIDTF();
	~mglGraphIDTF();
	using mglGraph::Mark;
	using mglGraph::Ball;
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void DoubleSided(bool enable)  { double_sided_flag = enable ; }
	void VertexColor(bool enable)  { vertex_color_flag = enable ; }
	void TextureColor(bool enable) { textures_flag = enable ; }
	void Compression(bool enable)  { disable_compression_flag = !enable ; }
	void Unrotate(bool enable)     { unrotate_flag = enable ; }
	void BallIsPoint(bool enable)  { ball_is_point_flag = enable ; }
	/// Flush() finishes current PointSet, LineSet and Mesh. Use Flush() to separate unnamed objects in the scene. The named objects or object groups are separeted by StartGroup/EndGroup
	void Flush() { points_finished = lines_finished = mesh_finished = true; };
	void Light ( int n,mglPoint p, mglColor c=NC, mreal br=0.5, bool infty=true );
	void InPlot ( mreal x1,mreal x2,mreal y1,mreal y2, bool rel=false );
	void DefaultPlotParam();	///< Set default parameter for plotting
	void Rotate(mreal TetX,mreal TetZ,mreal TetY=0);
	void SetSize(int ,int ) {};
	void Clf ( mglColor Back=NC );
	/// Print string \a str in position \a p with font size \a size.
	void Putsw(mglPoint p,const wchar_t *text,const char *font=0,mreal size=-1,char dir=0,mreal shift=0);
	/// Print string \a str with font size \a size in position \a p along direction \a l.
	mreal Putsw(mglPoint p,mglPoint l,const wchar_t *text,char font='t',mreal size=-1);

	void Ball ( mreal x,mreal y,mreal z,mglColor col=RC,mreal alpha=1 );
	void WriteIDTF ( const char *fname,const char *descr=0 );
	void WriteU3D ( const char *fname,const char *descr=0 );
	void WritePDF ( const char *fname,const char *descr=0 );
	friend class u3dModel;
	friend class u3dMesh;
	friend class u3dBall;
	float diff_int;		///< diffuse color intensity
	float spec_int;		///< specular color intensity
	float emis_int;		///< emissive color intensity
	bool double_sided_flag;			///< show both sides in meshes
	bool vertex_color_flag;			///< use per-vertex color in meshes
	bool textures_flag;			///< use textures on surfaces
	bool disable_compression_flag;	///< disable mesh compression (normally is true). One don't need the number of polygons reduced automatically to save space, but you may have to unset it if you use unpatched (unfixed) U3D library.
	bool unrotate_flag;			///< do not rotate the scene as a whole, in this case you have to set camera position externally.
	bool ball_is_point_flag;
	/// Objects can belong to groups. StartGroup() and EndGroup() do what the names imply.
	void StartGroup ( const char *name );
	void StartAutoGroup ( const char *name );
	void EndGroup();
	u3dGroup* GetCurrentGroup();
protected:
	mglColor def_col;

	void ball ( mreal *p,mreal *c );
	void UnitBall ();

	void point_plot ( const mglPoint& p, const mglColor& c );
	void line_plot ( const mglPoint& p0, const mglPoint& p1 );
	void trig_plot ( const mglPoint& p0, const mglPoint& p1, const mglPoint& p2 );
	void quad_plot ( const mglPoint& p0, const mglPoint& p1, const mglPoint& p2, const mglPoint& p3 );
	void line_plot ( mreal *p1,mreal *p2,mreal *c1,mreal *c2,bool all=false );
	void line_plot_s ( mreal *p1,mreal *p2,mreal *c1,mreal *c2,bool all=false );
	void trig_plot ( mreal *p0,mreal *p1,mreal *p2,
						mreal *c0,mreal *c1,mreal *c2 );
	void trig_plot_n ( mreal *p0,mreal *p1,mreal *p2,
						mreal *c0,mreal *c1,mreal *c2,
						mreal *n0,mreal *n1,mreal *n2 );
	void quad_plot ( mreal *p0,mreal *p1,mreal *p2,mreal *p3,
						mreal *c0,mreal *c1,mreal *c2,mreal *c3 );
	void quad_plot_a ( mreal *p0,mreal *p1,mreal *p2,mreal *p3,
						mreal a0,mreal a1,mreal a2,mreal a3,mreal alpha );
	void quad_plot_n ( mreal *p0,mreal *p1,mreal *p2,mreal *p3,
						mreal *c0,mreal *c1,mreal *c2,mreal *c3,
						mreal *n0,mreal *n1,mreal *n2,mreal *n3 );
	void mark_plot ( mreal *pp, char type );
	void font_line ( mreal *p, unsigned char *r,bool thin=true );

	void surf_plot ( long n,long m,mreal *pp,mreal *cc,bool *tt );
	void cloud_plot( long nx, long ny, long nz, mreal *pp, mreal *a, mreal alpha );
	void arrow_plot ( mreal *p1,mreal *p2,char st );
	void quads_plot(long n, mreal *pp, mreal *cc, bool *tt);

	bool points_finished;	///< do not add any more to the current point set
	bool lines_finished;	///< do not add any more to the current line set
	bool mesh_finished;		///< do not add any more to the current mesh

	inline mreal fixalpha ( mreal alpha ) { return UseAlpha ? (alpha<0?-alpha:alpha) : 1.0f; };

	u3dMaterial_list Materials;		///< global list of all materials used in all models.
	size_t AddMaterial ( const u3dMaterial& Material );

	u3dTexture_list Textures;		///< global list of all textures
	u3dTexture& AddTexture();

	u3dPointSet_list  PointSets;	///< global list of u3dPointSets
	u3dPointSet& GetPointSet();
	u3dLineSet_list  LineSets;	///< global list of u3dLineSets
	u3dLineSet& GetLineSet();
	u3dMesh_list     Meshes;	///< global list of u3Meshes
	u3dMesh& GetMesh();

	u3dBall_list     Balls;		///< global list of u3Balls

	u3dLight_list Lights;
	void SetAmbientLight ( mglColor c=WC, mreal br=-1.0 );
	void AddLight ( mglPoint p, mglColor color=WC, mreal br=0.5, bool infty=true );

	u3dGroup*	CurrentGroup;
	u3dGroup_list	Groups;

	mreal* col2col ( const mreal *c, const mreal *n, mreal *r );
	void MakeTransformMatrix( mreal position[4][4], mreal invpos[4][4] );

	mreal rotX, rotY, rotZ;
};
//-----------------------------------------------------------------------------
#endif
#endif
//-----------------------------------------------------------------------------
