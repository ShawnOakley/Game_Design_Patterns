// http://gameprogrammingpatterns.com/command.html


// Step 1: Define a base class that represents a triggerable game command
class Command
{
public:
  virtual -Command() {}
  virtual void execute() = 0;
};

// Step 2: Create subclasses for each of the different game actions
class JumpCommand : public Command
{
public:
  virtual void execute() { jump(); }
};

class FireCommand : public Command
{
public:
  virtual void execute() {fireGun(); }
};

// ... etc.

// Step 3: In input handler, storea pointer to a command for each button

class InputHandler
{
public:
  void handleInput();

private:
  Command* buttonX_;
  Command* buttonY_;
  Command* buttonA_;
  Command* buttonB_;
}

// Step 4: The input handler delectes to those pointers
class InputHandler::handleInput()
{
  if (isPressed(BUTTON_X)) buttonX_->execute();
  else if (isPressed(BUTTON_Y)) buttonY_->execute();
  else if (isPressed(BUTTON_A)) buttonA_->execute();
  else if (isPressed(BUTTON_B)) buttonB_->execute();
}
