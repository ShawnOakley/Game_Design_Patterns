// ByteCode

// Similar to Interpreter pattern:
// https://en.wikipedia.org/wiki/Interpreter_pattern
// Allows you to express composable behavior in forms of data
// Expects an internal tree of objects that represents the code
// Often used to generate bytecode.  
// In order to compile that to bytecode, you’ll recursively walk the tree, just like you do to interpret it with the Interpreter pattern. 
// The only difference is that instead of executing a primitive piece of behavior immediately, 
// you output the bytecode instruction to perform that later.

// Definition:
// An instruction set defines the low-level operations that can be performed. 
// A series of instructions is encoded as a sequence of bytes. 
// A virtual machine executes these instructions one at a time, using a stack for intermediate values. 
// By combining instructions, complex high-level behavior can be defined.

// When to use it:
// Use it when you have a lot of behavior you need to define and your game’s implementation language isn’t a good fit because:
// 1) It’s too low-level, making it tedious or error-prone to program in.
// 2) Iterating on it takes too long due to slow compile times or other tooling issues.
// 3) It has too much trust. If you want to ensure the behavior being defined can’t break the game, you need to sandbox it from the rest of the codebase.

// Caveat: Bytecode is slower than native code, so it isn’t a good fit for performance-critical parts of your engine. 

// Requires a front-end:
// Much like the Gang of Four’s Interpreter pattern, it’s assumed that you also have some way to generate the bytecode. 
// Usually, users author their behavior in some higher-level format, and a tool translates that to the bytecode that our virtual machine understands. 
// In other words, a compiler.

// Goal: Turn a programmatic API into an instruction set.

// Example API for, e.g., bytecode that casts spells

void setHealth(int wizard, int amount);
void setWisdom(int wizard, int amount);
void setAgility(int wizard, int amount);
void playSound(int soundId);
void spawnParticles(int particleType);

// Define instruction set
// To encode a spell in data, we store an array of enum values. 
// We’ve only got a few different primitives, so the range of enum values easily fits into a byte. 
// This means the code for a spell is just a list of bytes — ergo “bytecode”.

enum Instruction
{
  INST_SET_HEALTH      = 0x00,
  INST_SET_WISDOM      = 0x01,
  INST_SET_AGILITY     = 0x02,
  INST_PLAY_SOUND      = 0x03,
  INST_SPAWN_PARTICLES = 0x04
};

// To execute a single instruction, we see which primitive it is and dispatch to the right API method:
switch (instruction)
{
  case INST_SET_HEALTH:
    setHealth(0, 100);
    break;

  case INST_SET_WISDOM:
    setWisdom(0, 100);
    break;

  case INST_SET_AGILITY:
    setAgility(0, 100);
    break;

  case INST_PLAY_SOUND:
    playSound(SOUND_BANG);
    break;

  case INST_SPAWN_PARTICLES:
    spawnParticles(PARTICLE_FLAME);
    break;
}

// In this way, our interpreter forms the bridge between code world and data world. 
// We can wrap this in a little VM that executes an entire spell like so:

class VM
{
public:
  void interpret(char bytecode[], int size)
  {
    for (int i = 0; i < size; i++)
    {
      char instruction = bytecode[i];
      switch (instruction)
      {
        // Cases for each instruction...
      }
    }
  }
};

// Not very expressive

// To execute a complex nested expression, you start with the innermost subexpressions. 
// You calculate those, and the results flow outward as arguments to the expressions that contain them until eventually, the whole expression has been evaluated.

// Since our data is flattened, we’ll have to use the order of the instructions to control that. 
// We’ll do it the same way your CPU does — with a stack.

class VM
{
public:
  VM()
  : stackSize_(0)
  {}

  // Other stuff...

private:
  static const int MAX_STACK = 128;
  int stackSize_;
  int stack_[MAX_STACK];
};

// The VM maintains an internal stack of values. 
// Whenever a bit of data needs to work its way from one instruction to another, it gets there through the stack.

// Like the name implies, values can be pushed onto or popped off of the stack, so let’s add a couple of methods for that:
class VM
{
private:
  void push(int value)
  {
    // Check for stack overflow.
    assert(stackSize_ < MAX_STACK);
    stack_[stackSize_++] = value;
  }

  int pop()
  {
    // Make sure the stack isn't empty.
    assert(stackSize_ > 0);
    return stack_[--stackSize_];
  }

  // Other stuff...
};

// When an instruction needs to receive parameters, it pops them off the stack like so:

switch (instruction)
{
  case INST_SET_HEALTH:
  {
    int amount = pop();
    int wizard = pop();
    setHealth(wizard, amount);
    break;
  }

  case INST_SET_WISDOM:
  case INST_SET_AGILITY:
    // Same as above...

  case INST_PLAY_SOUND:
    playSound(pop());
    break;

  case INST_SPAWN_PARTICLES:
    spawnParticles(pop());
    break;
}

/// To get some values onto that stack, we need one more instruction: a literal. 
// The trick is to take advantage of the fact that our instruction stream is a sequence of bytes — we can stuff the number directly in the byte array. 
// We define another instruction type for a number literal like so:

case INST_LITERAL:
{
  // Read the next byte from the bytecode.
  int value = bytecode[++i];
  push(value);
  break;
}

// Code for reading states
// As you can see, these work with the stack in both directions. 
// They pop a parameter to determine which wizard to get the stat for, and then they look up the stat’s value and push that back onto the stack.
case INST_GET_HEALTH:
{
  int wizard = pop();
  push(getHealth(wizard));
  break;
}

case INST_GET_WISDOM:
case INST_GET_AGILITY:
  // You get the idea...

// Goal is to make behavior sandboxed.

// The bytecode can’t do anything malicious or reach out into weird parts of the game engine because we’ve only defined a few instructions that touch the rest of the game.
// We control how much memory it uses by how big of a stack we create, and we’re careful to make sure it can’t overflow that. 
// We can even control how much time it uses. In our instruction loop, we can track how many we’ve executed and bail out if it goes over some limit.

// Controlling execution time isn’t necessary in our sample because we don’t have any instructions for looping. We could limit execution time by limiting the total size of the bytecode. 
// This also means our bytecode isn’t Turing-complete.

// How to make a higher-level way of authoring the bytecode?
// Build a UI
// Building a graphical interface to let users define their behavior, especially if the people using it won’t be highly technical. 
// Writing text that’s free of syntax errors is difficult for people who haven’t spent years getting used to a compiler yelling at them.