#pragma once

//Defines a numerical range
class Range
{
public:
	Range(int min, int value, int max) : _min(min), _value(value), _max(max){}
	Range() : _min(0), _value(0), _max(0) {}

	~Range(){};

	//Modifies value by delta
	void Delta(int delta){
		_value += delta;

		if (_value > _max)	_value = _max;
		if (_value < _min)	_value = _min;
	}

	//Modifies min by delta
	void DeltaMin(int delta) {
		_min += delta;

		if(_value < _min) _value = _min;
		if(_max < _min)	_max = _min;
	}

	//Modifies max by delta
	void DeltaMax(int delta) {
		_max += delta;

		if (_value > _max)	_value = _max;
		if (_max < _min)	_min = _max;
	}

	//Sets value to min
	void ToMinimum() {
		_value = _min;
	}

	//Sets value to max
	void ToMaximum() {
		_value = _max;
	}

	bool AtMinimum() { return _value == _min; }
	bool AtMaximum() { return _value == _max; }

	int Min() { return _min; }
	int Max() { return _max; }
	int Value() { return _value; }

private:
	int _min; int _value; int _max;
};

