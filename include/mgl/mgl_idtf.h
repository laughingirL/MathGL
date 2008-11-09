/* mgl_idtf.h is part of Math Graphic Library
 * Copyright (C) 2008 Michail Vidiassov and Alexey Balakin <mathgl.abalakin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
//-----------------------------------------------------------------------------
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

#ifdef _MSC_VER
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
	float position[4][4];
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
	float position[4][4];
	float intensity;
	float attenuation;
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
	mglColor color;
	float opacity;
	bool emissive;
	bool vertex_color;
	std::string name;
	u3dMaterial() : opacity ( 1.0f ), emissive ( false ), vertex_color ( false ) {};
	bool operator== ( const u3dMaterial& Material )
	{
		return ( this->color         ==  Material.color
					&& this->opacity       ==  Material.opacity
					&& this->emissive      ==  Material.emissive
					&& this->vertex_color  ==  Material.vertex_color );
	}
	void print_material ( std::ofstream& ostr );
	void print_shader ( std::ofstream& ostr );
};
typedef std::deque<u3dMaterial> u3dMaterial_list;
//-----------------------------------------------------------------------------
typedef std::vector<mglPoint> mglPoint_list;
//-----------------------------------------------------------------------------
typedef std::vector<size_t> ModelMaterial_list;
//-----------------------------------------------------------------------------
struct u3dGroup
{
	std::string name;
	u3dGroup* parent;
	size_t NumberOfChildren;
	bool isauto;
	u3dGroup(): parent ( NULL ), NumberOfChildren ( 0 ), isauto ( false ) {};
};
//-----------------------------------------------------------------------------
typedef std::list<u3dGroup>  u3dGroup_list;
//-----------------------------------------------------------------------------
/// Class describe an object in the scene. Base class for PointSet, LineSet, Mesh. Keeps a vector of vertexes used, colors assigned to them (if any), names of materials used in the model. Has a name and may belong to group of objects (have a parent).
class u3dModel
{
public:
	mglPoint_list Points;
	std::vector<mglColor> PointColors;
	float position[4][4];
	float invpos[4][4];

	bool vertex_color;
	ModelMaterial_list ModelMaterials;
	bool both_visible;
	mglGraphIDTF *Graph;
	std::string name;
	u3dGroup* parent;
	u3dModel ( const std::string name, mglGraphIDTF *Graph, const bool& vertex_color );
	void print_node ( std::ofstream& ostr );
	void print_shading_modifier ( std::ofstream& ostr );
	size_t AddModelMaterial ( const float *c, bool emissive, bool vertex_color );
	size_t AddPoint ( const float *p );
	size_t AddPoint ( const mglPoint p );
	size_t AddColor ( const float *c );
};
//-----------------------------------------------------------------------------
typedef std::list<u3dModel>  u3dModel_list;
//-----------------------------------------------------------------------------
class u3dBall
{
public:
	mglPoint center;
	float radius;
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
	void line_plot ( float *p1, float *p2, float *c1, float *c2 );
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
	u3dPointSet ( const std::string& name, mglGraphIDTF *Graph );
	void point_plot ( const mglPoint& p );
	void print_model_resource ( std::ofstream& ostrtmp );
};
//-----------------------------------------------------------------------------
typedef std::list<u3dPointSet>  u3dPointSet_list;
//-----------------------------------------------------------------------------
struct u3dTriangle
{
	size_t pid0; // ids of points
	size_t pid1;
	size_t pid2;
	size_t mid; // id of triangle material
	size_t cid0; // ids of point colors
	size_t cid1;
	size_t cid2;
};
typedef std::vector<u3dTriangle> u3dTriangle_list;
//-----------------------------------------------------------------------------
/// Class contain meshes which are presented as a set of triangles. Indexes of vertexes, index of material, indexes of vertex colors are stored for each triangle. But really there are two modes, with and without per-vertex colors. If vertex colors are used, colors change smoothly along the surface, but transparency and lighting are disabled. If there are no vertex colors, the appearance is affected by materials, transparency and lighting are possible, but a large number of different materials results in slowly displayed models. Boolean parameter vertex_color_flag controls the mode. If you are drawing something in a small set of colors, it is advised to set it to false in order to get lighting for your model. (If exacly one color is used the per-vertex coloring is disabled automatically).
class u3dMesh : public u3dModel
{
public:
	bool disable_compression;
	u3dMesh ( const std::string& name, mglGraphIDTF *Graph,
				const bool& vertex_color, const bool& disable_compression ) : u3dModel ( name, Graph, vertex_color )
	{
		this->disable_compression = disable_compression;
	}
	u3dTriangle_list  Triangles;
	void quad_plot ( float *pp0,float *pp1,float *pp2,float *pp3,
						float *cc0,float *cc1,float *cc2,float *cc3 );
	void quad_plot_n ( float *pp0,float *pp1,float *pp2,float *pp3,
						float *cc0,float *cc1,float *cc2,float *cc3,
						float *nn0,float *nn1,float *nn2,float *nn3 );
	void trig_plot ( float *pp0,float *pp1,float *pp2,
						float *cc0,float *cc1,float *cc2 );
	void trig_plot_n ( float *pp0,float *pp1,float *pp2,
						float *cc0,float *cc1,float *cc2,
						float *nn0,float *nn1,float *nn2 );
	void print_model_resource ( std::ofstream& ostrtmp );
	void AddTriangle ( size_t pid0, size_t pid1, size_t pid2, size_t mid,
						size_t cid0 = SIZE_MAX, size_t cid1 = SIZE_MAX, size_t cid2 = SIZE_MAX );
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
	void VertexColor(bool enable)  { vertex_color_flag = enable ; }
	void Compression(bool enable)  { disable_compression_flag = !enable ; }
	void Unrotate(bool enable)     { unrotate_flag = enable ; }
	/// Flush() finishes current PointSet, LineSet and Mesh. Use Flush() to separate unnamed objects in the scene. The named objects or object groups are separeted by StartGroup/EndGroup
	void Flush() { points_finished = lines_finished = mesh_finished = true; };
	void Light ( bool enable )	{UseLight=enable;};
	void Light ( int n,mglPoint p, mglColor c=NC, float br=0.5, bool infty=true );
	void InPlot ( float x1,float x2,float y1,float y2 );
	void Clf ( mglColor Back=NC );
	/// Print string \a str in position \a p with font size \a size.
	void Putsw(mglPoint p,const wchar_t *text,const char *font=0,float size=-1,char dir=0,float shift=0);
	/// Print string \a str with font size \a size in position \a p along direction \a l.
	float Putsw(mglPoint p,mglPoint l,const wchar_t *text,char font='t',float size=-1);

	void Ball ( float x,float y,float z,mglColor col=RC,float alpha=1 );
	void WriteIDTF ( const char *fname,const char *descr=0 );
	friend class u3dModel;
	friend class u3dMesh;
	friend class u3dBall;
	bool vertex_color_flag;			///< use per-vertex color in meshes
	bool disable_compression_flag;	///< disable mesh compression (normally is true). One don't need the number of polygons reduced automatically to save space, but you may have to unset it if you use unpatched (unfixed) U3D library.
	bool unrotate_flag;			///< do not rotate the scene as a whole, in this case you have to set camera position externally.
	/// Objects can belong to groups. StartGroup() and EndGroup() do what the names imply.
	void StartGroup ( const char *name );
	void StartAutoGroup ( const char *name );
	void EndGroup();
	u3dGroup* GetCurrentGroup();
protected:
	mglColor def_col;

	void ball ( float *p,float *c );
	void UnitBall ();

	void point_plot ( const mglPoint& p );
	void line_plot ( const mglPoint& p0, const mglPoint& p1 );
	void trig_plot ( const mglPoint& p0, const mglPoint& p1, const mglPoint& p2 );
	void quad_plot ( const mglPoint& p0, const mglPoint& p1, const mglPoint& p2, const mglPoint& p3 );
	void line_plot ( float *p1,float *p2,float *c1,float *c2,bool all=false );
	void line_plot_s ( float *p1,float *p2,float *c1,float *c2,bool all=false );
	void trig_plot ( float *p0,float *p1,float *p2,
						float *c0,float *c1,float *c2 );
	void trig_plot_n ( float *p0,float *p1,float *p2,
						float *c0,float *c1,float *c2,
						float *n0,float *n1,float *n2 );
	void quad_plot ( float *p0,float *p1,float *p2,float *p3,
						float *c0,float *c1,float *c2,float *c3 );
	void quad_plot_a ( float *p0,float *p1,float *p2,float *p3,
						float a0,float a1,float a2,float a3,float alpha );
	void quad_plot_n ( float *p0,float *p1,float *p2,float *p3,
						float *c0,float *c1,float *c2,float *c3,
						float *n0,float *n1,float *n2,float *n3 );
	void mark_plot ( float *pp, char type );
	void font_line ( float *p, unsigned char *r,bool thin=true );

	void surf_plot ( long n,long m,float *pp,float *cc,bool *tt );
	void arrow_plot ( float *p1,float *p2,char st );
	void quads_plot(long n, float *pp, float *cc, bool *tt);

	bool points_finished;	///< do not add any more to the current point set
	bool lines_finished;	///< do not add any more to the current line set
	bool mesh_finished;		///< do not add any more to the current mesh

	inline float fixalpha ( float alpha ) { return UseAlpha ? (alpha<0?-alpha:alpha) : 1.0f; };

	u3dMaterial_list Materials;		///< global list of all materials used in all models.
	size_t AddMaterial ( const u3dMaterial& Material );

	u3dPointSet_list  PointSets;	///< global list of u3dPointSets
	u3dPointSet& GetPointSet();
	u3dLineSet_list  LineSets;	///< global list of u3dLineSets
	u3dLineSet& GetLineSet();
	u3dMesh_list     Meshes;	///< global list of u3Meshes
	u3dMesh& GetMesh();

	u3dBall_list     Balls;		///< global list of u3Balls

	u3dLight_list Lights;
	void SetAmbientLight ( mglColor c=WC, float br=-1.0 );
	void AddLight ( mglPoint p, mglColor color=WC, float br=0.5, bool infty=true );

	u3dGroup*	CurrentGroup;
	u3dGroup_list	Groups;

	float* col2col ( const float *c, const float *n, float *r );
	void MakeTransformMatrix( float position[4][4], float invpos[4][4] );
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
