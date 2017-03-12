#pragma once

//Timer 
class Timer
{
public:
	Timer() : _stopAt(0), _active(false), _elapsed(0)
	{}

	Timer(float stopAt) : _stopAt(stopAt), _active(false), _elapsed(0)
	{};

	~Timer(){};

	bool UpdateTimer(float dt) {
		if(!_active) return false;
			
		_elapsed += dt;

		if (_elapsed < _stopAt) return false;

		//reset
		_active = false;
		_elapsed = 0;

		return true;
	}

	void Activate(bool active = true) {
		_active = active;
	}

	void Set(float stopAt) {
		_stopAt = stopAt;
		_active = false;
	}

private:
	bool _active;
	float _elapsed;
	float _stopAt;
};
