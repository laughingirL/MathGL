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
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "mgl/mgl_ab.h"
//-----------------------------------------------------------------------------
#define __STDC_LIMIT_MACROS
#include <stdint.h>
# include <list>
# include <vector>
# include <deque>
# include <string>
# include <sstream>
# include <iostream>
#ifndef SIZE_MAX
#define SIZE_MAX size_t(-1)
#endif
//-----------------------------------------------------------------------------
/// u3d structures
class mglGraphIDTF;

struct u3dNode
{
	float position[4][4];
	std::string parent;
	std::string resource;
	bool both_visible;
	std::string type;
	std::string name;
	u3dNode(): both_visible(true) {};
	void print(std::ostringstream& ostr);
};

struct u3dLight
{
	std::string type;
	mglColor color;
	float position[4][4];
	float intensity;
	float attenuation;
	std::string name;
	void print_light_resource(std::ostringstream& ostr);
	void print_node(std::ostringstream& ostr);
};
typedef std::list<u3dLight>  u3dLight_list;

class u3dMaterial 
{
	public:
		mglColor color;
		float opacity;
		bool emissive;
		bool vertex_color;
		std::string name;
		u3dMaterial() : opacity(1.0f), emissive(false), vertex_color(false) {};
		bool operator==(const u3dMaterial& Material) {
			return (   this->color         ==  Material.color
					&& this->opacity       ==  Material.opacity
					&& this->emissive      ==  Material.emissive
					&& this->vertex_color  ==  Material.vertex_color);
		}
		void print_material(std::ostringstream& ostr);
		void print_shader(std::ostringstream& ostr);
};
typedef std::deque<u3dMaterial> u3dMaterial_list;

typedef std::vector<mglPoint> mglPoint_list;

typedef std::vector<size_t> ModelMaterial_list;

class u3dModel
{
	public:
//	float B[9];			///< Transformation matrix
//	float xPos;			///< Shifting plot in X-direction
//	float yPos;			///< Shifting plot in Y-direction
//	float zPos;			///< Shifting plot in depth
		mglPoint_list Points;
		std::vector<mglColor> PointColors;

		bool vertex_color;
		ModelMaterial_list ModelMaterials;
		bool both_visible;
		mglGraphIDTF *Graph;
		std::string name;
		std::string parent;
		u3dModel(const std::string name, mglGraphIDTF *Graph, const bool& vertex_color);
		void print_node(std::ostringstream& ostr);
		void print_shading_modifier(std::ostringstream& ostr);
		size_t AddModelMaterial(const float *c, bool emissive, bool vertex_color);
		size_t AddPoint(float *p);
		size_t AddPoint(mglPoint p);
		size_t AddColor(const float *c);
};

struct u3dLine
{
	size_t pid1;
	size_t pid2;
	size_t mid;
};
typedef std::vector<u3dLine> u3dLine_list;

class u3dLineSet : public u3dModel
{
	public:
	u3dLineSet(const std::string& name, mglGraphIDTF *Graph) :
		u3dModel(name, Graph, false) { }
	u3dLine_list  Lines;
	void line_plot (float *p1, float *p2, float *c1, float *c2);
	void print_model_resource(std::ostringstream& ostrtmp);
};

typedef std::list<u3dLineSet>  u3dLineSet_list;

class u3dPointSet : public u3dModel
{
	public:
		u3dPointSet(const std::string& name, mglGraphIDTF *Graph);
		void point_plot (const mglPoint& p);
		void print_model_resource(std::ostringstream& ostrtmp);
};

typedef std::list<u3dPointSet>  u3dPointSet_list;

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

class u3dMesh : public u3dModel
{
	public:
		bool disable_compression;
		u3dMesh(const std::string& name, mglGraphIDTF *Graph,
				const bool& vertex_color, const bool& disable_compression) : u3dModel(name, Graph, vertex_color)
		{
			this->disable_compression = disable_compression;
		}
		u3dTriangle_list  Triangles;
		void quad_plot(float *pp0,float *pp1,float *pp2,float *pp3,
					   float *cc0,float *cc1,float *cc2,float *cc3);
		void quad_plot_n(float *pp0,float *pp1,float *pp2,float *pp3,
						 float *cc0,float *cc1,float *cc2,float *cc3,
	   float *nn0,float *nn1,float *nn2,float *nn3);
		void trig_plot(float *pp0,float *pp1,float *pp2,
					   float *cc0,float *cc1,float *cc2);
		void trig_plot_n(float *pp0,float *pp1,float *pp2,
						 float *cc0,float *cc1,float *cc2,
	   float *nn0,float *nn1,float *nn2);
		void print_model_resource(std::ostringstream& ostrtmp);
		void AddTriangle(size_t pid0, size_t pid1, size_t pid2, size_t mid,
						 size_t cid0 = SIZE_MAX, size_t cid1 = SIZE_MAX, size_t cid2 = SIZE_MAX);
};

typedef std::list<u3dMesh>  u3dMesh_list;

typedef std::list<std::string>  u3dGroup_list;
//-----------------------------------------------------------------------------
/// Class implements the creation of different mathematical plots under OpenGL
class mglGraphIDTF : public mglGraphAB
{
public:
	mglGraphIDTF();
	~mglGraphIDTF();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Flush() { points_finished = lines_finished = mesh_finished = true; };
	void Light(int n,mglPoint p, mglColor c=NC, float br=0.5, bool infty=true);
	void InPlot(float x1,float x2,float y1,float y2);
	void Clf(mglColor Back=NC);
	
	void Ball(float x,float y,float z,mglColor col=RC,float alpha=1);
	void WriteIDTF(const char *fname,const char *descr=0);
	friend class u3dModel;
	friend class u3dMesh;
	bool vertex_color_flag;	///< use per-vertex color in meshes
	bool disable_compression_flag;	///< disable mesh compression
	void StartGroup(const std::string& name);
	void EndGroup();
	const std::string& GetCurrentGroup();
protected:
	mglColor def_col;

	void ball(float *p,float *c);
	
	void point_plot(const mglPoint& p);
	void line_plot(const mglPoint& p0, const mglPoint& p1);
	void trig_plot(const mglPoint& p0, const mglPoint& p1, const mglPoint& p2);
	void quad_plot(const mglPoint& p0, const mglPoint& p1, const mglPoint& p2, const mglPoint& p3);
	void line_plot(float *p1,float *p2,float *c1,float *c2,bool all=false);
	void line_plot_s(float *p1,float *p2,float *c1,float *c2,bool all=false);
	void trig_plot(float *p0,float *p1,float *p2,
					float *c0,float *c1,float *c2);
	void trig_plot_n(float *p0,float *p1,float *p2,
					float *c0,float *c1,float *c2,
					float *n0,float *n1,float *n2);
	void quad_plot(float *p0,float *p1,float *p2,float *p3,
					float *c0,float *c1,float *c2,float *c3);
	void quad_plot_a(float *p0,float *p1,float *p2,float *p3,
					float a0,float a1,float a2,float a3,float alpha);
	void quad_plot_n(float *p0,float *p1,float *p2,float *p3,
					float *c0,float *c1,float *c2,float *c3,
					float *n0,float *n1,float *n2,float *n3);
	void mark_plot(float *pp, char type);
	void font_line(float *p, unsigned char *r,bool thin=true);

	void surf_plot(long n,long m,float *pp,float *cc,bool *tt);
	void arrow_plot(float *p1,float *p2,char st);

	bool points_finished;	///< do not add any more to the current point set
	bool lines_finished;	///< do not add any more to the current line set
	bool mesh_finished;	///< do not add any more to the current mesh

	inline float fixalpha(const float& alpha) { return UseAlpha ? alpha : 1.0f; };

	u3dMaterial_list Materials;
	size_t AddMaterial(const u3dMaterial& Material);

	u3dPointSet_list  PointSets;
	u3dPointSet& AddPointSet(std::string = "");
	u3dPointSet& GetPointSet();

	u3dLineSet_list  LineSets;
	u3dLineSet& AddLineSet(std::string = "");
	u3dLineSet& GetLineSet();

	u3dMesh_list     Meshes;
	u3dMesh& AddMesh(std::string = "");
	u3dMesh& GetMesh();

	u3dLight_list Lights;
	void SetAmbientLight(mglColor c=WC, float br=-1.0);
	void AddLight(mglPoint p, mglColor color=WC, float br=0.5, bool infty=true);

	std::string	CurrentGroup;
	u3dGroup_list	Groups;

	float* col2col(const float *c, const float *n, float *r);

};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
