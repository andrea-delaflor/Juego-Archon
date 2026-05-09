#pragma once

#include<cmath>

struct Vector2D
{
	double x{};
	double y{};
	Vector2D(double _x = 0.0, double _y = 0.0) : x(_x), y(_y) {}
	double modulo() const; // Modulo del vector
	double argumento() const; // Argumento del vector
	Vector2D unitario() const; // Devuelve un vector unitario
	Vector2D operator - (const Vector2D&) const; // Resta de vectores
	Vector2D operator + (const Vector2D&) const; // Suma de vectores
	double operator *(const Vector2D&) const; //  Producto escalar
	Vector2D operator *(double) const; // Producto por un escalar
	
};

inline double Vector2D::modulo() const
{
	return sqrt(x * x + y * y);
}

inline double Vector2D::argumento() const
{
	return atan2(y, x);
}

inline Vector2D Vector2D::unitario() const
{
	auto mod = modulo();
	if (mod > 0.00001) return { x / mod, y / mod };
	return { x,y };
}

inline Vector2D Vector2D::operator - (const Vector2D& v) const
{
	return { x - v.x , y - v.y };
}
inline Vector2D Vector2D::operator + (const Vector2D& v) const
{
	return { x + v.x , y + v.y };
}

inline double Vector2D::operator *(const Vector2D& v) const
{
	return{ x * v.x + y * v.y };
}

inline Vector2D Vector2D::operator *(double n) const
{
	return{ x * n, y * n };
}

inline Vector2D operator * (double f, const Vector2D& v) {
	return { v.x * f, v.y * f };
}