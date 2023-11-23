#pragma once
#include <qpainter.h>
class Node
{
public:
	Node();
	Node(int value, QPoint coordiantes);
	Node(int value, int x, int y);

	void setValue(int value);
	void setCoordinates(QPoint coordinates);
	void setCoordinates(int x, int y);
	void setX(int x);
	void setY(int y);

	QPoint getCoordinates();
	int getX();
	int getY();
	int getValue();



private:
	int m_value;
	QPoint m_coordinates;
};

