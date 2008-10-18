/* mgl_idtf.cpp is part of Math Graphic Library
 * Copyright (C) 2008 Michail Vidiassov <mathgl.abalakin@gmail.com>
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
#include "mgl/mgl_gl.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
#ifdef WIN32
#define bzero(a,b) memset(a,0,b)
#endif
#include "mgl/mgl_idtf.h"
#include <iomanip>
#ifndef MAXFLOAT
#define MAXFLOAT	1e30
#endif

const bool dbg = true;
//-----------------------------------------------------------------------------
/// Create mglGraph object in OpenGL mode.
HMGL mgl_create_graph_idtf()
{	return new mglGraphIDTF;	}
/// Create mglGraph object in OpenGL mode.
uintptr_t mgl_create_graph_idtf_()
{	return uintptr_t(new mglGraphIDTF);	}
//-----------------------------------------------------------------------------
// helper output routines
//-----------------------------------------------------------------------------
inline std::string f2s ( float x )
{
	std::ostringstream o;
	o << x;
	return o.str();
};
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
//-----------------------------------------------------------------------------
// u3d object methods
//-----------------------------------------------------------------------------
void u3dNode::print ( std::ostringstream& ostr )
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
	{
		ostr << "\t\t\t\t" << IDTFPrintVector4 ( this->position[i] ) << "\n";
	}
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

void u3dLight::print_light_resource ( std::ostringstream& ostr )
{
	ostr
	<< "\t\tRESOURCE_NAME \"" << this->name << "\"\n"
	<< "\t\tLIGHT_TYPE \"" << this->type << "\"\n"
	<< "\t\tLIGHT_COLOR " << this->color.r << " "  << this->color.g << " " << this->color.b << "\n"
	<< "\t\tLIGHT_ATTENUATION " << this->attenuation << " 0 0\n"
	<< "\t\tLIGHT_INTENSITY " << this->intensity << "\n";
};
void u3dLight::print_node ( std::ostringstream& ostr )
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
	bzero ( Light.position, sizeof ( Light.position ) );
	Light.position[0][0] = 1.0f;
	Light.position[1][1] = 1.0f;
	Light.position[2][2] = 1.0f;
	Light.position[3][3] = 1.0f;
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
	float pp[3] = {p.x, p.y, p.z};;

	bzero ( Light.position, sizeof ( Light.position ) );
	Light.position[0][0] = 1.0f;
	Light.position[1][1] = 1.0f;
	Light.position[2][2] = 1.0f;
	Light.position[3][3] = 1.0f;
	if ( infty )
	{
		Light.type = "DIRECTIONAL";
//		NormScale ( pp, 1 );
		float a, b, c;
		a = pp[0]; b = pp[1]; c = pp[2];
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
//		PostScale ( pp, 1 );
		Light.position[3][0] = pp[0];
		Light.position[3][1] = pp[1];
		Light.position[3][2] = pp[2];
	}

	Lights.push_back ( Light );
};

void u3dMaterial::print_material ( std::ostringstream& ostr )
{
	ostr
	<< "\t\tRESOURCE_NAME \"" << this->name << "\"\n"
//	  << "\t\tMATERIAL_AMBIENT 0 0 0\n"
	<< "\t\tMATERIAL_AMBIENT "  << 0.125*this->color.r << " " << 0.125*this->color.g << " " << 0.125*this->color.b  << "\n"
	<< "\t\tMATERIAL_DIFFUSE "  << this->color.r << " " << this->color.g << " " << this->color.b  << "\n"
//	  << "\t\tMATERIAL_SPECULAR 0 0 0\n";
//	  << "\t\tMATERIAL_DIFFUSE 0 0 0\n"
	<< "\t\tMATERIAL_SPECULAR "  << this->color.r << " " << this->color.g << " " << this->color.b  << "\n";
//	  << "\t\tMATERIAL_SPECULAR 1 1 1\n";
	if ( this->emissive )
		ostr << "\t\tMATERIAL_EMISSIVE "  << this->color.r << " " << this->color.g << " " << this->color.b  << "\n";
	else
//		ostr << "\t\tMATERIAL_EMISSIVE "  << 0.125*this->color.r << " " << 0.125*this->color.g << " " << 0.125*this->color.b  << "\n";
		ostr << "\t\tMATERIAL_EMISSIVE 0 0 0\n";
	ostr
	<< "\t\tMATERIAL_REFLECTIVITY 0.5\n"
	<< "\t\tMATERIAL_OPACITY " << this->opacity << "\n";
}
void u3dMaterial::print_shader ( std::ostringstream& ostr )
{
	ostr << "\t\tRESOURCE_NAME \"" << this->name << "\"\n";
	if ( this->vertex_color )
	{
		ostr << "\t\tATTRIBUTE_USE_VERTEX_COLOR \"TRUE\"\n";
	}
	ostr << "\t\tSHADER_MATERIAL_NAME \"" << this->name << "\"\n";
	ostr << "\t\tSHADER_ACTIVE_TEXTURE_COUNT 0\n";
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

u3dPointSet& mglGraphIDTF::AddPointSet ( std::string name )
{
	if ( name.empty() || name.size() == 0 )
	{
		if ( !CurrentName.empty() && CurrentName.size() != 0 )
		{
			name = CurrentName;
			CurrentName.clear();
		}
		else if ( !CurrentGroup.empty() && CurrentGroup.size() != 0 )
			name = CurrentGroup + "Points" + i2s ( PointSets.size() );
		else
			name = "Points" + i2s ( PointSets.size() );
	}
	for ( u3dNode_list::iterator it = this->Nodes.begin(); it != this->Nodes.end(); ++it )
		if ( *it == name )
			{	name = "Node" + i2s ( Nodes.size() ); break; }
	u3dPointSet PointSet = u3dPointSet ( name, this );
	PointSets.push_back ( PointSet );
	Nodes.push_back ( name );
	points_finished = false;
	return ( PointSets.back() );
}
// Get the last point set or start a new one if things have changed
u3dPointSet& mglGraphIDTF::GetPointSet()
{
	if ( points_finished )
		return AddPointSet();
	else
		return PointSets.back();
}

// Add a new LineSet named name, if the name is duplicate - change it
u3dLineSet& mglGraphIDTF::AddLineSet ( std::string name )
{
	if ( name.empty() || name.size() == 0 )
	{
		if ( !CurrentName.empty() && CurrentName.size() != 0 )
		{
			name = CurrentName;
			CurrentName.clear();
		}
		else if ( !CurrentGroup.empty() && CurrentGroup.size() != 0 )
			name = CurrentGroup + "Lines" + i2s ( LineSets.size() );
		else
			name = "Lines" + i2s ( LineSets.size() );
	}
	for ( u3dNode_list::iterator it = this->Nodes.begin(); it != this->Nodes.end(); ++it )
		if ( *it == name )
			{	name = "Node" + i2s ( Nodes.size() ); break; }
	u3dLineSet LineSet = u3dLineSet ( name, this );
	LineSets.push_back ( LineSet );
	Nodes.push_back ( name );
	lines_finished = false;
	return ( LineSets.back() );
}
// Get the last line set or start a new one if things have changed
u3dLineSet& mglGraphIDTF::GetLineSet()
{
	if ( lines_finished )
		return AddLineSet();
	else
		return LineSets.back();
}

// Add a new Mesh named name, if the name is duplicate - change it
u3dMesh& mglGraphIDTF::AddMesh ( std::string name )
{
	if ( name.empty() || name.size() == 0 )
	{
		if ( !CurrentName.empty() && CurrentName.size() != 0 )
		{
			name = CurrentName;
			CurrentName.clear();
		}
		else if ( !CurrentGroup.empty() && CurrentGroup.size() != 0 )
			name = CurrentGroup + "Mesh" + i2s ( Meshes.size() );
		else
			name = "Mesh" + i2s ( Meshes.size() );
	}
	for ( u3dNode_list::iterator it = this->Nodes.begin(); it != this->Nodes.end(); ++it )
		if ( *it == name )
			{	name = "Node" + i2s ( Nodes.size() ); break; }
	u3dMesh Mesh = u3dMesh ( name, this, this->vertex_color_flag, this->disable_compression_flag );
	Meshes.push_back ( Mesh );
	Nodes.push_back ( name );
	mesh_finished = false;
	return ( Meshes.back() );
}
// Get the last line set or start a new one if things have changed
u3dMesh& mglGraphIDTF::GetMesh()
{
	if ( mesh_finished )
		return AddMesh();
	else
		return Meshes.back();
}

size_t u3dModel::AddPoint ( float *p )
{
	mglPoint point = mglPoint ( p[0], p[1], p[2] );
	for ( size_t i=0; i< this->Points.size(); i++ )
		if ( this->Points[i] == point )
			return i;
	this->Points.push_back ( point );
	return ( this->Points.size()-1 );
}

size_t u3dModel::AddPoint ( mglPoint point )
{
	for ( size_t i=0; i< this->Points.size(); i++ )
		if ( this->Points[i] == point )
			return i;
	this->Points.push_back ( point );
	return ( this->Points.size()-1 );
}

size_t u3dModel::AddColor ( const float *c )
{
	mglColor color = mglColor ( c[0], c[1], c[2] );
	for ( size_t i=0; i< this->PointColors.size(); i++ )
		if ( this->PointColors[i] == color )
			return i;
	this->PointColors.push_back ( color );
	return ( this->PointColors.size()-1 );
};

void u3dMesh::AddTriangle ( size_t pid0, size_t pid1, size_t pid2, size_t mid,
                            size_t cid0, size_t cid1, size_t cid2 )
{
	u3dTriangle triangle;
	triangle.pid0 = pid0;
	triangle.pid1 = pid1;
	triangle.pid2 = pid2;
	triangle.mid  = mid;
	triangle.cid0 = cid0;
	triangle.cid1 = cid1;
	triangle.cid2 = cid2;
	this->Triangles.push_back ( triangle );
};

u3dModel::u3dModel ( const std::string name, mglGraphIDTF *Graph, const bool& vertex_color )
		: both_visible ( true )
{
	this->name = name;
	this->Graph = Graph;
	this->vertex_color = vertex_color;
	this->parent = Graph->GetCurrentGroup();
}
size_t u3dModel::AddModelMaterial ( const float *c, bool emissive, bool vertex_color )
{
	u3dMaterial Material;
	Material.color = mglColor ( c[0], c[1], c[2] );
//	Material.color = color;
	Material.opacity = this->Graph->fixalpha ( c[3] );
//	Material.opacity = alpha;
	Material.emissive = emissive;
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
void u3dModel::print_node ( std::ostringstream& ostr )
{
	u3dNode Node;
	Node.name = name;
	bzero ( Node.position, sizeof ( Node.position ) );
	Node.position[0][0] = 1.0f;
	Node.position[1][1] = 1.0f;
	Node.position[2][2] = 1.0f;
	Node.position[3][3] = 1.0f;
//	for(int i=0; i<4; i++)
//		for(int j=0; j<4; j++)
//			Node.position[i][j] = this->B[j][i];
	Node.type = "MODEL";
	Node.both_visible = this->both_visible;
	Node.parent = this->parent;
	Node.print ( ostr );
}
void u3dModel::print_shading_modifier ( std::ostringstream& ostr )
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
// typedef std::list<u3dModel>  u3dModel_list;
// u3dModel_list Models;
// void AddModel(u3dModel& Model)
// {
//   Models.push_back(Model);
//}
void u3dBall::print_node ( std::ostringstream& ostr )
{
	u3dNode Node;
	Node.name = name;
	Node.resource = "UnitBall";
	bzero ( Node.position, sizeof ( Node.position ) );
	Node.position[0][0] = this->radius;
	Node.position[1][1] = this->radius;
	Node.position[2][2] = this->radius;
	Node.position[3][0] = this->center.x;
	Node.position[3][1] = this->center.y;
	Node.position[3][2] = this->center.z;
	Node.position[3][3] = 1.0f;
	Node.type = "MODEL";
	Node.both_visible = false;
	Node.parent = this->parent;
	Node.print ( ostr );
}

void u3dBall::print_shading_modifier ( std::ostringstream& ostr )
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
		u3dModel ( name, Graph, false )
{
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
	this->AddModelMaterial ( color, true, false );
	this->both_visible = false;
}
void u3dPointSet::point_plot ( const mglPoint& p )
{
	this->AddPoint ( p );
}

void u3dPointSet::print_model_resource ( std::ostringstream& ostrtmp )
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
		<< ( this->Points[pid].x-0.5f ) *2.0f << " "
		<< ( this->Points[pid].y-0.5f ) *2.0f << " "
		<< ( this->Points[pid].z-0.5f ) *2.0f << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t}\n";
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

void u3dLineSet::print_model_resource ( std::ostringstream& ostrtmp )
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
		<< ( this->Points[pid].x-0.5f ) *2.0f << " "
		<< ( this->Points[pid].y-0.5f ) *2.0f << " "
		<< ( this->Points[pid].z-0.5f ) *2.0f << "\n";
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

	float color[4];
	if ( !this->vertex_color )
	{
		color[0] = ( cc0[0]+cc1[0]+cc2[0] ) /3.0f;
		color[1] = ( cc0[1]+cc1[1]+cc2[1] ) /3.0f;
		color[2] = ( cc0[2]+cc1[2]+cc2[2] ) /3.0f;
		color[3] = ( cc0[3]+cc1[3]+cc2[3] ) /3.0f;
	}
	else
	{
		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
		color[3] = 1.0f;
	}
	size_t mid = this->AddModelMaterial ( color, false, this->vertex_color );
	if ( this->vertex_color )
	{
		size_t cid0 = this->AddColor ( cc0 );
		size_t cid1 = this->AddColor ( cc1 );
		size_t cid2 = this->AddColor ( cc2 );
		this->AddTriangle ( pid0, pid1, pid2, mid, cid0, cid1, cid2 );
	}
	else
	{
		this->AddTriangle ( pid0, pid1, pid2, mid );
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

	float color[4];
	if ( !this->vertex_color )
	{
		color[0] = ( cc0[0]+cc1[0]+cc2[0] ) /3.0f;
		color[1] = ( cc0[1]+cc1[1]+cc2[1] ) /3.0f;
		color[2] = ( cc0[2]+cc1[2]+cc2[2] ) /3.0f;
		color[3] = ( cc0[3]+cc1[3]+cc2[3] ) /3.0f;
	}
	else
	{
		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
		color[3] = 1.0f;
	}
	size_t mid = this->AddModelMaterial ( color, false, this->vertex_color );
	if ( this->vertex_color )
	{
		float cc[3];
		size_t cid0 = this->AddColor ( this->Graph->col2col ( cc0, nn0, cc ) );
		size_t cid1 = this->AddColor ( this->Graph->col2col ( cc1, nn1, cc ) );
		size_t cid2 = this->AddColor ( this->Graph->col2col ( cc2, nn2, cc ) );
		this->AddTriangle ( pid0, pid1, pid2, mid, cid0, cid1, cid2 );
	}
	else
	{
		this->AddTriangle ( pid0, pid1, pid2, mid );
	}
}
//-----------------------------------------------------------------------------
void u3dMesh::print_model_resource ( std::ostringstream& ostrtmp )
{
	size_t numMaterials   = this->ModelMaterials.size();
	size_t numPoints      = this->Points.size();
	size_t numPointColors = this->PointColors.size();
	size_t numTriangles   = this->Triangles.size();

	if ( numTriangles == 0 )	return;

	if ( numPointColors == 1 ) // if there is just one color in the model - make the corresponding material
	{
// fprintf(stderr, "opa \n");
		this->ModelMaterials.pop_back();
		float c[4] = { this->PointColors[0].r, this->PointColors[0].g, this->PointColors[0].b, 1.0f };
		this->AddModelMaterial ( c, false, false );
		numPointColors = 0;
		this->PointColors.clear();
	}

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
	<< "\t\t\tMODEL_DIFFUSE_COLOR_COUNT " << numPointColors << "\n"
	<< "\t\t\tMODEL_SPECULAR_COLOR_COUNT 0\n"
	<< "\t\t\tMODEL_TEXTURE_COORD_COUNT 0\n"
	<< "\t\t\tMODEL_BONE_COUNT 0\n"
	<< "\t\t\tMODEL_SHADING_COUNT " << numMaterials << "\n"
	<< "\t\t\tMODEL_SHADING_DESCRIPTION_LIST {\n";
	for ( size_t cid=0; cid < numMaterials; cid++ )
	{
		ostrtmp
		<< "\t\t\t\tSHADING_DESCRIPTION " << cid << " {\n"
		<< "\t\t\t\t\tTEXTURE_LAYER_COUNT 0\n"
		<< "\t\t\t\t\tSHADER_ID " << cid << "\n"
		<< "\t\t\t\t}\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tMESH_FACE_POSITION_LIST {\n";
	for ( size_t id=0; id < numTriangles; id++ )
	{
		ostrtmp << "\t\t\t\t"
		<< this->Triangles[id].pid0 << " "
		<< this->Triangles[id].pid1 << " "
		<< this->Triangles[id].pid2 << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	ostrtmp << "\t\t\tMESH_FACE_SHADING_LIST {\n";
	for ( size_t id=0; id < numTriangles; id++ )
	{
		ostrtmp << "\t\t\t\t" << this->Triangles[id].mid << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	if ( numPointColors > 0 )
	{
		ostrtmp << "\t\t\tMESH_FACE_DIFFUSE_COLOR_LIST {\n";
		for ( size_t id=0; id < numTriangles; id++ )
		{
			ostrtmp << "\t\t\t\t"
			<< this->Triangles[id].cid0 << " "
			<< this->Triangles[id].cid1 << " "
			<< this->Triangles[id].cid2 << "\n";
		}
		ostrtmp << "\t\t\t}\n";
	}

	ostrtmp << "\t\t\tMODEL_POSITION_LIST {\n";
	for ( size_t pid=0; pid < numPoints; pid++ )
	{
		ostrtmp << "\t\t\t\t"
		<< ( this->Points[pid].x-0.5f ) *2.0f << " "
		<< ( this->Points[pid].y-0.5f ) *2.0f << " "
		<< ( this->Points[pid].z-0.5f ) *2.0f << "\n";
	}
	ostrtmp << "\t\t\t}\n";

	if ( numPointColors > 0 )
	{
		ostrtmp << "\t\t\tMODEL_DIFFUSE_COLOR_LIST {\n";
		for ( size_t cid=0; cid < numPointColors; cid++ )
		{
			ostrtmp << "\t\t\t\t"
			<< this->PointColors[cid].r << " "
			<< this->PointColors[cid].g << " "
			<< this->PointColors[cid].b << "\n";
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
		vertex_color_flag ( false ), disable_compression_flag ( true ),
		points_finished ( true ), lines_finished ( true ), mesh_finished ( true )
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
	Nodes.clear();
	CurrentGroup.clear();
	CurrentName.clear();
	PointSets.clear();
	LineSets.clear();
	Meshes.clear();
	Balls.clear();
	Materials.clear();
	points_finished = true;
	lines_finished = true;
	mesh_finished = true;
}
void mglGraphIDTF::StartGroup ( const char *name )
{
	CurrentGroup = name;
	if ( CurrentGroup == "<NULL>" ) return;
	for ( u3dGroup_list::iterator it = Groups.begin(); it != Groups.end(); ++it )
	{
		if ( CurrentGroup == *it ) return;
	}
	for ( u3dNode_list::iterator it = this->Nodes.begin(); it != this->Nodes.end(); ++it )
		if ( *it == CurrentGroup )
			{	CurrentGroup = "Node" + i2s ( Nodes.size() ); break; }
	Groups.push_back ( CurrentGroup );
	Nodes.push_back ( CurrentGroup );
	points_finished = true;
	lines_finished = true;
	mesh_finished = true;
}
void mglGraphIDTF::EndGroup()
{
	CurrentGroup.clear();
	points_finished = true;
	lines_finished = true;
	mesh_finished = true;
}
const std::string& mglGraphIDTF::GetCurrentGroup()
{
	return CurrentGroup;
}
void mglGraphIDTF::SetName ( const char *name )
{
	if ( name != NULL && strlen ( name ) !=0 )
		CurrentName = name;
	else
		CurrentName.clear();
}
const std::string& mglGraphIDTF::GetName()
{
	return CurrentName;
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::UnitBall ( )
{
	const size_t ThetaResolution = 10;
	const size_t PhiResolution   = 10;
	mglPoint pnt;
	mglPoint nrm;
	mglPoint Center = mglPoint ( 0.5f, 0.5f, 0.5f );
	float Radius = 0.5f;
	u3dMesh& Mesh = AddMesh ( "UnitBall" );
	Mesh.both_visible=false;
	Mesh.vertex_color=false;

	mesh_finished = true;

	float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	Mesh.AddModelMaterial ( color, false, false );

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
		u3dTriangle triangle;
		triangle.pid0 =  phiResolution*i + numPoles;
		triangle.pid1 = ( phiResolution* ( i+1 ) % base ) + numPoles;
		triangle.pid2 =  0;
		triangle.mid  = 0;
		triangle.cid0 = triangle.cid1 = triangle.cid2 = SIZE_MAX;
		Mesh.Triangles.push_back ( triangle );
	}

	// around south pole
	size_t numOffset = phiResolution - 1 + numPoles;
	for ( size_t i=0; i < ThetaResolution; i++ )
	{
		u3dTriangle triangle;
//	triangle.pid0 = phiResolution*i + numOffset;
//	triangle.pid1 = ((phiResolution*(i+1)) % base) + numOffset;
		triangle.pid1 = phiResolution*i + numOffset;
		triangle.pid0 = ( ( phiResolution* ( i+1 ) ) % base ) + numOffset;
		triangle.pid2 = numPoles - 1;
		triangle.mid  = 0;
		triangle.cid0 = triangle.cid1 = triangle.cid2 = SIZE_MAX;
		Mesh.Triangles.push_back ( triangle );
	}

	// bands in-between poles
	for ( size_t i=0; i < ThetaResolution; i++ )
	{
		for ( size_t j=0; j < ( phiResolution-1 ); j++ )
		{
			u3dTriangle triangle;
			triangle.pid0 = phiResolution*i + j + numPoles;
			triangle.pid1 = triangle.pid0 + 1;
			triangle.pid2 = ( ( phiResolution* ( i+1 ) +j ) % base ) + numPoles + 1;
			triangle.mid  = 0;
			triangle.cid0 = triangle.cid1 = triangle.cid2 = SIZE_MAX;
			Mesh.Triangles.push_back ( triangle );
			triangle.pid1 = triangle.pid2;
			triangle.pid2 = triangle.pid1-1;
			Mesh.Triangles.push_back ( triangle );
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::Ball ( float x,float y,float z,mglColor col,float alpha )
{
	if ( alpha==0 || !ScalePoint ( x,y,z ) )	return;
	if ( !col.Valid() )	col = mglColor ( 1.,0.,0. );
	alpha = Transparent ? alpha : 1.0f;
	alpha = UseAlpha ? alpha : 1.0f;
	u3dBall ball;

	float p[3] = {x,y,z};
	PostScale ( p,1 );
	if (PenWidth<0)
	{
		point_plot ( mglPoint ( p[0],p[1],p[2] ) );
		return;
	}
	p[0] = (p[0] - 0.5f)*2.0f;
	p[1] = (p[1] - 0.5f)*2.0f;
	p[2] = (p[2] - 0.5f)*2.0f;
	ball.center = mglPoint ( p[0],p[1],p[2] );
	ball.radius = 2.0f*fabs(PenWidth)/500;
	ball.Graph = this;
	ball.parent = this->GetCurrentGroup();
	ball.name = "Ball" + i2s(Balls.size()) ;

	u3dMaterial Material;
	Material.color = col;
	Material.opacity = alpha;
	Material.emissive = false;
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
#define pnt(x, y)  ( p + ss*mglPoint((float)x, (float)y, 0.0f))
	mglPoint p = mglPoint ( x, y, z );
	mglPoint p1;
	mglPoint p2;
	float ss=MarkSize*0.35*font_factor;
	if ( type=='.' || ss==0 )
	{
		point_plot ( p );
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
	if ( dbg ) fprintf ( stderr, "quad_plot\n p0 %f %f %f\n p1 %f %f %f\n p2 %f %f %f\n p3 %f %f %f\n",
		                     p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z );
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
void mglGraphIDTF::point_plot ( const mglPoint& p )
{
	GetPointSet().point_plot ( p );
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
	u3dMesh& Mesh = AddMesh();
	mesh_finished = true;
	for ( i=0;i<n*m;i++ )
	{
		Mesh.Points.push_back ( mglPoint ( pp[3*i], pp[3*i+1], pp[3*i+2] ) );
		float nn = sqrt ( ns[3*i]*ns[3*i]+ns[3*i+1]*ns[3*i+1]+ns[3*i+2]*ns[3*i+2] );
		if ( nn != 0.0 ) {ns[3*i]/=nn; ns[3*i+1]/=nn; ns[3*i+2]/=nn; }
	}
	if ( cc )
	{
		if ( vertex_color_flag )
		{
			float color [4] = {1.0f, 1.0f, 1.0f, 1.0f};
			Mesh.AddModelMaterial ( color, false, true );
			for ( i=0;i<n*m;i++ )
			{
				float col [4];
				col2col ( cc+4*i, ns+3*i, col );
				Mesh.PointColors.push_back ( mglColor ( col[0], col[1], col[2] ) );
			}
		}
#define AddTri(i1, i2, i3)	\
			if (Mesh.vertex_color)							\
			{									\
				Mesh.AddTriangle(i0+(i1), i0+(i2), i0+(i3), 0, i0+(i1), i0+(i2), i0+(i3));	\
			}									\
			else									\
			{									\
				float col [4];							\
				size_t mid;							\
				col[0] = (c[4*(i1)+0] + c[4*(i2)+0] + c[4*(i3)+0])/3.0f;	\
				col[1] = (c[4*(i1)+1] + c[4*(i2)+1] + c[4*(i3)+1])/3.0f;	\
				col[2] = (c[4*(i1)+2] + c[4*(i2)+2] + c[4*(i3)+2])/3.0f;	\
				col[3] = (c[4*(i1)+3] + c[4*(i2)+3] + c[4*(i3)+3])/3.0f;	\
				mid = Mesh.AddModelMaterial(col, false, false);			\
				Mesh.AddTriangle(i0+(i1), i0+(i2), i0+(i3), mid);		\
			}
		for ( i=0;i<n-1;i++ )	for ( j=0;j<m-1;j++ )
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
	}
	else
	{
		Mesh.AddModelMaterial ( CDef, false, false );
		for ( i=0;i<n-1;i++ )	for ( j=0;j<m-1;j++ )
			{
				i0 = i+n*j;
				if ( !tt || ( tt[i0] && tt[i0+1] && tt[i0+n] && tt[i0+1+n] ) )
					{ Mesh.AddTriangle ( i0, i0+1, i0+n, 0 ); Mesh.AddTriangle ( i0+1, i0+n+1, i0+n, 0 ); }
				else if ( tt[i0] && tt[i0+1] && tt[i0+n] )
					Mesh.AddTriangle ( i0, i0+1, i0+n, 0 );
				else if ( tt[i0] && tt[i0+1] && tt[i0+n+1] )
					Mesh.AddTriangle ( i0, i0+1, i0+n+1, 0 );
				else if ( tt[i0] && tt[i0+n+1] && tt[i0+n] )
					Mesh.AddTriangle ( i0, i0+n+1, i0+n, 0 );
				else if ( tt[i0+n+1] && tt[i0+1] && tt[i0+n] )
					Mesh.AddTriangle ( i0+n+1, i0+1, i0+n, 0 );
			}
	}
	delete []ns;
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
void mglGraphIDTF::InPlot ( float x1,float x2,float y1,float y2 )
{
	mglGraphAB::InPlot ( x1,x2,y1,y2 );
	points_finished = lines_finished = mesh_finished = true;
}
//-----------------------------------------------------------------------------
void mglGraphIDTF::WriteIDTF ( const char *fname,const char *descr )
{
	time_t now;
	time ( &now );
	std::ostringstream ostr;

	Lights.clear();
	if ( UseLight )
	{
		SetAmbientLight();
		LightScale();
		for ( int i=0; i<10; i++ )
			if ( nLight[i] )
				AddLight ( mglPoint ( pLight[3*i], pLight[3*i+1], pLight[3*i+2] ),
				           mglColor ( cLight[3*i], cLight[3*i+1], cLight[3*i+2] ),
				           bLight[i], iLight[i] );
	}

	FILE *fp = fopen ( fname,"wt" );
	ostr
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
	for ( u3dGroup_list::iterator it = this->Groups.begin(); it != this->Groups.end(); ++it )
	{
		u3dNode Node;
		Node.name = *it;
		bzero ( Node.position, sizeof ( Node.position ) );
		Node.position[0][0] = 1.0f;
		Node.position[1][1] = 1.0f;
		Node.position[2][2] = 1.0f;
		Node.position[3][3] = 1.0f;
		Node.type = "GROUP";
		Node.print ( ostr );
	}
	for ( u3dLight_list::iterator it = this->Lights.begin(); it != this->Lights.end(); ++it )
	{
		it->print_node ( ostr );
	}
	for ( u3dPointSet_list::iterator it = this->PointSets.begin(); it != this->PointSets.end(); ++it )
	{
		it->print_node ( ostr );
	}
	for ( u3dLineSet_list::iterator it = this->LineSets.begin(); it != this->LineSets.end(); ++it )
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

	fwrite ( ostr.str().c_str(), 1, strlen ( ostr.str().c_str() ), fp );
	ostr.str ( "" );

	fclose ( fp );
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
