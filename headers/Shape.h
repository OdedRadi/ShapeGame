#pragma once

class Shape: public CObject 
{
	DECLARE_SERIAL (Shape)
public: 
	Shape();
	virtual ~Shape();
	virtual void setCenter(CPoint center){}
	virtual CPoint getCenter()
	{
		return center;
	}
	virtual void setColor(COLORREF mColor)
	{
		color=mColor;
	}
	virtual COLORREF getColor()
	{
		return color;
	}
	virtual void Draw(CDC *dc){}
	virtual bool Contains(CPoint point){ return false; }
	void Serialize (CArchive& ar)
	{
		CObject::Serialize (ar);
		if (ar.IsStoring ())
		{
			ar << center;
			for(int i=0; i<4; i++)
			{
				ar << p[i].x;
				ar << p[i].y;
			}
			ar << color;
		}
		else
		{
			ar >> center;
			for(int i=0; i<4; i++)
			{
				ar >> p[i].x;
				ar >> p[i].y;
			}
			ar >> color;
		}
	}
protected:
	CPoint center;
	CPoint p[4];
	COLORREF color;
};

class MPolygon: public Shape
{
	DECLARE_SERIAL (MPolygon)
public:
	MPolygon();

	void Draw(CDC *dc, int edges)
	{
		CBrush myBrush, *oldBrush;
		myBrush.CreateSolidBrush(color);
		oldBrush=dc->SelectObject(&myBrush);

		dc->Polygon(p,edges);

		dc->SelectObject(oldBrush);
	}
};

class MSquare: public MPolygon
{
	DECLARE_SERIAL (MSquare)

public:
	MSquare();
	MSquare(CPoint center)
	{
		setCenter(center);
	}

	void setCenter(CPoint center)
	{
		this->center=center;
		p[0].x=center.x-40;
		p[0].y=center.y-40;
		p[1].x=center.x+40;
		p[1].y=center.y-40;
		p[2].x=center.x+40;
		p[2].y=center.y+40;
		p[3].x=center.x-40;
		p[3].y=center.y+40;
	}

	void Draw(CDC *dc)
	{
		MPolygon::Draw(dc, 4);
	}

	bool Contains(CPoint point)
	{
		if(point.x>=p[0].x && point.x<=p[2].x && point.y>=p[0].y && point.y<=p[2].y)
			return true;
		return false;
	}
};

class MParall: public MPolygon
{
	DECLARE_SERIAL (MParall)
public:
	MParall();
	MParall(CPoint center)
	{
		setCenter(center);
	}

	void setCenter(CPoint center)
	{
		this->center=center;
		p[0].x=center.x-30;
		p[0].y=center.y-20;
		p[1].x=center.x+30;
		p[1].y=center.y-60;
		p[2].x=center.x+30;
		p[2].y=center.y+20;
		p[3].x=center.x-30;
		p[3].y=center.y+60;
	}
	
	void Draw(CDC *dc)
	{
		MPolygon::Draw(dc, 4);
	}

	bool Contains(CPoint point)
	{
		if(point.x>=p[3].x && point.x<=p[1].x && point.y>=p[1].y && point.y<=p[3].y)
			return true;
		return false;
	}
};

class MTriangle: public MPolygon
{
	DECLARE_SERIAL(MTriangle)
public:
	MTriangle();
	MTriangle(CPoint center)
	{
		setCenter(center);	
	}
	
	void setCenter(CPoint center)
	{
		this->center=center;
		p[0].x=center.x-40;
		p[0].y=center.y+30;
		p[1].x=center.x;
		p[1].y=center.y-30;
		p[2].x=center.x+40;
		p[2].y=center.y+30;
		p[3].x=0;
		p[3].y=0;
	}

	void Draw(CDC *dc)
	{
		MPolygon::Draw(dc, 3);
	}

	bool Contains(CPoint point)
	{
		if(point.x>=p[0].x && point.x<=p[2].x && point.y>=p[1].y && point.y<=p[0].y)
			return true;
		return false;
	}
};

class MCircle: public Shape
{
	DECLARE_SERIAL(MCircle)
public:
	MCircle();
	MCircle(CPoint center)
	{
		setCenter(center);
	}
	
	void setCenter(CPoint center)
	{
		this->center=center;
		p[0].x=center.x-40;
		p[0].y=center.y-40;
		p[1].x=center.x+40;
		p[1].y=center.y+40;
		p[2].x=0;
		p[2].y=0;
		p[3].x=0;
		p[3].y=0;
	}

	void Draw(CDC *dc)
	{
		CBrush myBrush, *oldBrush;
		myBrush.CreateSolidBrush(color);
		oldBrush=dc->SelectObject(&myBrush);

		dc->Ellipse(p[0].x, p[0].y, p[1].x, p[1].y);

		dc->SelectObject( oldBrush );
	}

	bool Contains(CPoint point)
	{
		if(point.x>=p[0].x && point.x<=p[1].x && point.y>=p[0].y && point.y<=p[1].y)
			return true;
		return false;
	}
};