// State --
// Use of inite state machines
// Basic idea:
// 1) A fixed set of states that the machine can be in. For our example, that’s standing, jumping, ducking, and diving.
// 2) The machine can only be in one state at a time. Our heroine can’t be jumping and standing simultaneously. In fact, preventing that is one reason we’re going to use an FSM.
// 3) A sequence of inputs or events is sent to the machine. In our example, that’s the raw button presses and releases.
// 4) Each state has a set of transitions, each associated with an input and pointing to a state. When an input comes in, if it matches a transition for the current state, the machine changes to the state that transition points to.

// Use of enum -- handful of flags where only one is true at a time
enum State
{
    STATE_STANDING,
    STATE_JUMPING,
    STATE_DUCKING,
    STATE_DIVING
}

// Example use of state
// Use enum to define state
void Heroine::handleInput(Input input)
{
    switch(state_)
    {
        case STATE_STANDING:
            if (input == PRESS_B)
            {
                state_ = STATE_JUMPING;
                yVelocity_ = JUMP_VELOCITY;
                setGraphics(IMAGE_JUMP)
            }
            else if (input == PRESS_DOWN)
            {
                state_ = STATE_DUCKING;
                setGraphics(IMAGE_DUCK);
            }
            break;

        case STATE_JUMPING:
            if (input == PRESS_DOWN)
            {
                state_ = STATE_DIVING;
                setGraphics(IMAGE_DIVE);
            }
            break;
            
        case STATE_DUCKING:
            // handle reset of power move charger
            if (input == PRESS_DOWN)
            {
                state_ = STATE_DUCKING;
                chargeTIme_ = 0;
                setGraphics(IMAGE_DUCK);
            }
            if (input == RELEASE_DOWN)
            {
                state_ = STATE_STANDING;
                setGraphics(IMAGE_STAND);
            }
            break;
    }
}

// handle charging of an attack
void Heroine::update()
{
  if (state_ == STATE_DUCKING)
  {
    chargeTime_++;
    if (chargeTime_ > MAX_CHARGE)
    {
      superBomb();
    }
  }
}

// The State Pattern
// Gang of Four definition
// Allow an object to alter its behavior
// when its internal state changes.
// The object will appear to change its class.

// 1) Define interface for the state
// All behavior that is state-dependent 
// becomes a virtual method in the interface

class HeroineState
{
public:
  virtual ~HeroineState() {}
  virtual void handleInput(Heroine& heroine, Input input) {}
  virtual void update(Heroine& heroine) {}
};

// 2) Define a class for each state
//  Methods determine characters behavior when in that state

//Example:

class DuckingState : public HeroineState
{
public:
  DuckingState()
  : chargeTime_(0)
  {}

  virtual void handleInput(Heroine& heroine, Input input) {
    if (input == RELEASE_DOWN)
    {
      // Change to standing state...
      heroine.setGraphics(IMAGE_STAND);
    }
  }

  virtual void update(Heroine& heroine) {
    chargeTime_++;
    if (chargeTime_ > MAX_CHARGE)
    {
      heroine.superBomb();
    }
  }

private:
  int chargeTime_;
};

// 3) Delegate to the state
// Give the Heroine a pointer to her current state
// Delegate to the state

class Heroine
{
    public:
        virtual void handleInput(Input input);
        {
            state_->handleInput(*this, input);
        }

        virtual void update()
        {
            state_->update(*this);
        }

        private:
            HeroineState* state_;
};

// In order to change state, need to assign state_ to point to a different HeroineState object

// Different kinds of state objects
// 1) Static states
// Example:

class HeroineState
{
public:
  static StandingState standing;
  static DuckingState ducking;
  static JumpingState jumping;
  static DivingState diving;

  // Other code...
};

// So to make the heroine jump
// standing state would call jumping from heroine
if (input == PRESS_B)
{
  heroine.state_ = &HeroineState::jumping;
  heroine.setGraphics(IMAGE_JUMP);
}

// 2) Instantiated state
// However, if state needs to track something internally 
// E.g., a timer.  We can return new state from handleInput call and set that to state_

void Heroine::handleInput(Input input)
{
  HeroineState* state = state_->handleInput(*this, input);
  if (state != NULL)
  {
    delete state_;
    state_ = state;
  }
}



// IMPROVEMENTS:
// Concurrent State Machines --
// Two state machines running concurrently in same class

// Hierarchical State Machine -- 
// A state can have a superstate (making itself a substate). When an event comes in, if the substate doesn’t handle it, it rolls up the chain of superstates. In other words, it works just like overriding inherited methods.


// Define a base class
class OnGroundState : public HeroineState
{
public:
  virtual void handleInput(Heroine& heroine, Input input)
  {
    if (input == PRESS_B)
    {
      // Jump...
    }
    else if (input == PRESS_DOWN)
    {
      // Duck...
    }
  }
};


// Then each substate inherits it
class DuckingState : public OnGroundState
{
public:
  virtual void handleInput(Heroine& heroine, Input input)
  {
    if (input == RELEASE_DOWN)
    {
      // Stand up...
    }
    else
    {
      // Didn't handle input, so walk up hierarchy.
      OnGroundState::handleInput(heroine, input);
    }
  }
};

