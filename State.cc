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