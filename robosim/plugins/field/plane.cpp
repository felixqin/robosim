#include "plane.h"


/***********************************************************************/


Plane::Plane(dWorldID world, dSpaceID space, Tag tag, int number, FieldParam const& fp)
	:
	world_(world),
	space_(space),
	tag_(tag),
	number_(number),
	lx_(fp.ground.length),ly_(fp.ground.width),
	tex_id_(TEXTURE_NONE)
{
	dGeomID plane = dCreatePlane(space,0,0,1,0);

	ground_ = CreateGenericPart(this,0);
	GPAddGeom(ground_,plane);
	ground_->setColor(0,1,0,1);
	ground_->setSurface(fp.ground.surface);

	create_bar(space,fp.bar);
	create_door(space,fp);
}
void Plane::create_bar(dSpaceID space, BarParam const& bp)
{
	bar_ = CreateGenericPart(this,0);

	float w = bp.width, h = bp.height;
	float hh = h/2;
	float px = (lx_+w)/2;
	float py = (ly_+w)/2;

	dGeomID geom = dCreateBox(space,w,ly_,h);
	dGeomSetPosition(geom,px,0,hh);
	GPAddGeom(bar_,geom);

	geom = dCreateBox(space,lx_,w,h);
	dGeomSetPosition(geom,0,py,hh);
	GPAddGeom(bar_,geom);

	geom = dCreateBox(space,w,ly_,h);
	dGeomSetPosition(geom,-px,0,hh);
	GPAddGeom(bar_,geom);

	geom = dCreateBox(space,lx_,w,h);
	dGeomSetPosition(geom,0,-py,hh);
	GPAddGeom(bar_,geom);

	bar_->setColor(1,1,1,1);
	bar_->setSurface(bp.surface);
}
void Plane::create_door(dSpaceID space,FieldParam const& fp)
{
	float lx1=fp.door.deep, ly1=fp.door.width, lz1=fp.door.thickness;
	float lx2=fp.door.deep, ly2=fp.door.thickness, lz2=fp.door.height-fp.door.thickness;

	float px1=(fp.ground.length-fp.door.deep)/2, py1=0, pz1=fp.door.height-fp.door.thickness/2;
	float px21=px1, py21=fp.door.width/2, pz21=lz2/2;
	float px22=px21, py22=-py21, pz22=pz21;

	door1_ = CreateGenericPart(this,0);

	dGeomID geom = dCreateBox(space,lx1,ly1,lz1);
	dGeomSetPosition(geom,px1,py1,pz1);
	GPAddGeom(door1_,geom);

	geom = dCreateBox(space,lx2,ly2,lz2);
	dGeomSetPosition(geom,px21,py21,pz21);
	GPAddGeom(door1_,geom);

	geom = dCreateBox(space,lx2,ly2,lz2);
	dGeomSetPosition(geom,px22,py22,pz22);
	GPAddGeom(door1_,geom);

	door1_->setColor(0.8,0.8,0.8,1);
	door1_->setSurface(fp.bar.surface);

	//////////////////////////////////////////
	door2_ = CreateGenericPart(this,0);

	geom = dCreateBox(space,lx1,ly1,lz1);
	dGeomSetPosition(geom,-px1,py1,pz1);
	GPAddGeom(door2_,geom);

	geom = dCreateBox(space,lx2,ly2,lz2);
	dGeomSetPosition(geom,-px21,py21,pz21);
	GPAddGeom(door2_,geom);

	geom = dCreateBox(space,lx2,ly2,lz2);
	dGeomSetPosition(geom,-px22,py22,pz22);
	GPAddGeom(door2_,geom);

	door2_->setColor(0.8,0.8,0.8,1);
	door2_->setSurface(fp.bar.surface);
}

Plane::~Plane()
{
	DestroyGenericPart(ground_);
	DestroyGenericPart(bar_);
	DestroyGenericPart(door1_);
	DestroyGenericPart(door2_);
}

void Plane::render(IDrawing *draw) const
{
	draw->setColor(1.0f,1.0f,1.0f,1);
	draw->setTexture(tex_id_);
	draw->drawRectangle(lx_,ly_);

	draw->setTexture(TEXTURE_STONE);
	bar_->render(draw);
	door1_->render(draw);
	door2_->render(draw);
}

/******************************************************************************/
//对于场地,不可设置它的位移与转动量
void Plane::getPosition(float &x,float &y,float &z) const
{
	x=y=z=0;
}
void Plane::setPosition(float,float,float)
{
}
void Plane::getQuaternion(float &w,float &x,float &y,float &z) const
{
	w=1;
	x=y=z=0;
}
void Plane::setQuaternion(float, float, float, float)
{
}

