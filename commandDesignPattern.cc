// http://gameprogrammingpatterns.com/command.html

// BASIC EXAMPLE OF COMMAND PATTERN
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

// Step 3: In input handler, store a pointer to a command for each button

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


// Sample code in the actor that takes commands and runs it
Command* command = inputHandler.handleInput();
if (command)
{
    command->execute(actor);
}

// USE OF COMMAND PATTERN TO UNDO AND REDO
// Example for a move unit command
class MoveUnitCommand : public Command
{
public:
  MoveUnitCommand(Unit* unit, int x, int y)
  : unit_(unit),
  x_(x),
  y_(y)
  {}

    virtual void execute()
    {
      unit_->moveTo(x_,y_);
    }

  private:
      Unit* unit_;
      int x_, y_;
};

// To allow for undoing, need to track.
// Hence must create an instance very time a move is invoked
// Example:
Command* handleInput()
{
  Unit* unit = getSelectedUnit();

  if (isPressed(BUTTON_UP)) {
    // Move unit up one
    int destY = unit->y() - 1;
    return new MoveUnitCommand(unit, unit->x(), destY);
  }

  if (isPressed(BUTTON_DOWN)) {
    // Move unit down one
    int destY = unit->y() + 1;
    return new MoveUnitCommand(unit, unit->x(), destY);
  }

  return NULL;
}

// Define an operation in the command class that undoes the action of the commands
class Command
{
public:
  virtual -Command() {}
  virtual void execute() = 0;
  virtual void undo() = 0;
};

// Update command to have undo support
class MoveUnitCommand : public Command
{
public:
  MoveUnitCommand(Unit* unit, int x, int y)
  : unit_(unit),
  xBefore_(0),
  yBefore_(0),
  x_(x),
  y_(y),
  {}

    virtual void execute()
    {
      xBefore_ = unit_=>x();
      yBefore_ = unit_->y();
      unit_->moveTo(x_,y_);
    }

  private:
      Unit* unit_;
      int xBefore_, yBefore_;
      int x_, y_;
};
