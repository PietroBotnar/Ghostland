#pragma once

// A class for 3D vectors, with operator overloading for convenient programming

class CVector3f {
public:
	// Constructors
	CVector3f();
	CVector3f(float x_, float y_, float z_);
	CVector3f(int x_, int y_, int z_);
	CVector3f(double x_, double y_, double z_);
	CVector3f(const CVector3f &that);

	// Destructor
	virtual ~CVector3f();

	// Addition, subtraction
	CVector3f operator+(const CVector3f &that) const;
	CVector3f operator-(const CVector3f &that) const;
	CVector3f &operator+=(const CVector3f &that);
	CVector3f &operator-=(const CVector3f &that);

	// Post mutiplication or division with scalar
	CVector3f operator*(const float scalar) const;
	CVector3f operator/(const float scalar) const;
	CVector3f &operator*=(const float scalar);
	CVector3f &operator/=(const float scalar);

	// Pre-multiplication or division by a scalar
	friend CVector3f operator *(const float scalar, const CVector3f& that) {return that * scalar;}
	friend CVector3f operator /(const float scalar, const CVector3f& that) {return that / scalar;}

	// Equality tests
	bool operator==(const CVector3f &that) const;
	bool operator!=(const CVector3f &that) const;

	// Dot product uses * symbol with two vectors
	float operator*(const CVector3f &that) const;

	// Cross product uses % symbol with two vectors
	CVector3f operator%(const CVector3f &that) const;
	CVector3f &operator%=(const CVector3f &that);

	// Vector length and normalisation
	float Length();
	void Normalise();


	// Limit the x, y, or z values to a certain range
	void ClipX(const float xmin, const float xmax);
	void ClipY(const float ymin, const float ymax);
	void ClipZ(const float zmin, const float zmax);

	//limits all values to a max
	void Truncate(const float max);

	//returns a rotated vector
	CVector3f RotateAboutAxis(const float angle, const CVector3f axis);

	//returns the distance on the 2d xz plane
	float XZDistance(CVector3f &other);

	// Note: It would be possible (and consistent practice) to make the x, y, and z variables private, and then implement Get and Set methods. 
	// However, doing this creates some awkward code and is detrimental to learning.  Therefore, they have been made public.
	float x;
	float y;
	float z;
};

