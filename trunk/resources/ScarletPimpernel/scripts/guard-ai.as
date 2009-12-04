/***
 * hesperus: guard-ai.as
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

State@ state;

class State
{
	void enter(ScriptYoke@ yoke)
	{
		// No implementation needed - to be overridden in subclasses
	}

	void exit(ScriptYoke@ yoke)
	{
		// No implementation needed - to be overridden in subclasses
	}

	void process(ScriptYoke@ yoke)
	{
		// No implementation needed - to be overridden in subclasses
	}
}

void change_state(State@ newState, ScriptYoke@ yoke)
{
	if(state !is null) state.exit(yoke);
	@state = @newState;
	state.enter(yoke);
}

class TestState : State
{
	void enter(ScriptYoke@ yoke)
	{
		yoke.goto_position(14,6,6);
	}

	void process(ScriptYoke@ yoke)
	{
		if(!yoke.subyoke_active())
		{
			change_state(@OtherState(), yoke);
		}
	}
}

class OtherState : State
{
	void enter(ScriptYoke@ yoke)
	{
		yoke.goto_position(25,20,6);
	}

	void process(ScriptYoke@ yoke)
	{
		if(!yoke.subyoke_active())
		{
			change_state(@TestState(), yoke);
		}
	}
}

// TODO

void init(ScriptYoke@ yoke)
{
	change_state(@TestState(), yoke);
}

void process(ScriptYoke@ yoke)
{
	if(state !is null) state.process(yoke);
}
