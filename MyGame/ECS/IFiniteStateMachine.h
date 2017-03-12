#pragma once

//A simple Finite State Machine
template<typename T>
class IFiniteStateMachine
{
public:

	IFiniteStateMachine() : _timeInState(0.0f)
	{
	}

	//On entering a new state
	virtual void EnterState		(T state) {}

	//Update call per frame
	virtual void UpdateState	(T state) {}

	//On exiting state
	virtual void ExitState		(T state) {}

	//Switches the FSM on/off
	void Activate(bool active) {_active = active;}

	//Set FSM current state
	void SetState(T state)
	{
		ExitState(_state);
		_state = state;
		_timeInState = 0.0f;
		EnterState(_state);
	}
	
	//Update FSM
	void UpdateFSM(float delta_time)
	{
		if (!_active) return;

		_timeInState += delta_time;
		UpdateState(_state);
	}

	float	GetTimeInState()	const { return _timeInState; }
	T		GetState()			const { return _state; }

private:
	float	_timeInState;
	T		_state;
	bool	_active;
};