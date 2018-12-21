// Subclass Sandbox

// A base class defines an abstract sandbox method and several provided operations. 
// Marking them protected makes it clear that they are for use by derived classes. 
// Each derived sandboxed subclass implements the sandbox method using the provided operations.

// Example: Creating highly variant 'superpower' subclasses for game.
// What we want is to give each of the gameplay programmers who is implementing a superpower 
// a set of primitives they can play with. You want your power to play a sound? 
// Here’s your playSound() function. You want particles? Here’s spawnParticles(). 
// We’ll make sure these operations cover everything you need to do so that you don’t need to 
// include random headers and nose your way into the rest of the codebase.

// Superpower class will end up coupled to different game systems, 
// but derived classes will not.  Instead, only coupled to base class.
// Centralize need for modifications in Superpower.

// When to use Subclass Sandbox
// 1) Have a base class with a number of derived classes.
// 2) The base class is able to provide all of the operations that a derived class
// may need to perform
// 3) There is behavioral overlap in the subclasses and you want to make it easier to share 
// code between them
// 4) You want to minimize coupling between those derived classes and the rest of the program

// Issue -- Brittle Base Class problem
// “Inheritance” is a bad word in many programming circles these days, 
// and one reason is that base classes tend to accrete more and more code. 
// This pattern is particularly susceptible to that.

// Since subclasses go through their base class to reach the rest of the game, 
// the base class ends up coupled to every system any derived class needs to talk to. 
// Of course, the subclasses are also intimately tied to their base class.
// That spiderweb of coupling makes it very hard to change the base class without breaking something 
// See the 'brittle base class' problem: https://en.wikipedia.org/wiki/Fragile_base_class 

class Superpower
{
public:
  virtual ~Superpower() {}

protected:
  virtual void activate() = 0;

  void move(double x, double y, double z)
  {
    // Code here...
  }

  void playSound(SoundId sound, double volume)
  {
    // Code here...
  }

  void spawnParticles(ParticleType type, int count)
  {
    // Code here...
  }
};

// The activate() method is the sandbox method. Since it is virtual and abstract, subclasses must override it. 
// This makes it clear to someone creating a power subclass where their work has to go.

// The other protected methods, move(), playSound(), and spawnParticles(), are the provided operations. 
// These are what the subclasses will call in their implementation of activate().

// Example:
// Superpower with control flow based on player attributes

class Superpower
{
protected:
  double getHeroX()
  {
    // Code here...
  }

  double getHeroY()
  {
    // Code here...
  }

  double getHeroZ()
  {
    // Code here...
  }

  // Existing stuff...
};

class SkyLaunch : public Superpower
{
protected:
  virtual void activate()
  {
    if (getHeroZ() == 0)
    {
      // On the ground, so spring into the air.
      playSound(SOUND_SPROING, 1.0f);
      spawnParticles(PARTICLE_DUST, 10);
      move(0, 0, 20);
    }
    else if (getHeroZ() < 10.0f)
    {
      // Near the ground, so do a double jump.
      playSound(SOUND_SWOOP, 1.0f);
      move(0, 0, getHeroZ() - 20);
    }
    else
    {
      // Way up in the air, so do a dive attack.
      playSound(SOUND_DIVE, 0.7f);
      spawnParticles(PARTICLE_SPARKLES, 1);
      move(0, 0, -getHeroZ());
    }
  }
};

// Since Superpower base class gives access to state, can have control flow.

// CONSIDERATIONS:

// What operations should be provided?
// Rules of thumb
// 1) If a provided operation is only used by one or a few subclasses, you don’t get a lot of bang for your buck. 
// You’re adding complexity to the base class, which affects everyone, but only a couple of classes benefit.

// 2) When you call a method in some other corner of the game, it’s less intrusive if that method doesn’t modify any state. 
// It still creates a coupling, but it’s a “safe” coupling because it can’t break anything in the game.
// Calls that do modify state, on the other hand, more deeply tie you to those parts of the codebase, 
// and you need to be much more cognizant of that. 
// That makes them good candidates for being rolled up into provided operations in the more visible base class.

// 3) If the implementation of a provided operation only forwards a call to some outside system, then it isn’t adding much value. 
// In that case, it may be simpler to call the outside method directly.
// However, even simple forwarding can still be useful:

void playSound(SoundId sound, double volume)
{
  soundEngine_.play(sound, volume);
}

// It’s just forwarding the call to some soundEngine_ field in Superpower. 
// The advantage, though, is that it keeps that field encapsulated in Superpower so subclasses can’t poke at it.

// Should methods be provided directly, or through objects that contain them?
//Shunting provided operations into auxiliary classes like this can do a few things for you:
// 1) It reduces the number of methods in the base class. In the example here, we went from three methods to just a single getter.
// 2) Code in the helper class is usually easier to maintain. Core base classes like Superpower, despite our best intentions, tend to be tricky to change since so much depends on them. By moving functionality over to a less coupled secondary class, we make that code easier to poke at without breaking things.
// 3) It lowers the coupling between the base class and other systems. When playSound() was a method directly on Superpower, our base class was directly tied to SoundId and whatever audio code the implementation called into. Moving that over to SoundPlayer reduces Superpower‘s coupling to the single SoundPlayer class, which then encapsulates all of its other dependencies.

// How does the base class get the state that it needs?
// 1) Pass it to the base class constructor
class Superpower
{
public:
  Superpower(ParticleSystem* particles)
  : particles_(particles)
  {}

  // Sandbox method and other operations...

private:
  ParticleSystem* particles_;    
};

class SkyLaunch : public Superpower
{
public:
  SkyLaunch(ParticleSystem* particles)
  : Superpower(particles)
  {}
};

// 2) Do two-stage initialization
Superpower* createSkyLaunch(ParticleSystem* particles)
{
  Superpower* power = new SkyLaunch();
  power->init(particles);
  return power;
}

// 3) Make state static
class Superpower
{
public:
  static void init(ParticleSystem* particles)
  {
    particles_ = particles;
  }

  // Sandbox method and other operations...

private:
  static ParticleSystem* particles_;
};

// 4) Use a service locator:
class Superpower
{
protected:
  void spawnParticles(ParticleType type, int count)
  {
    ParticleSystem& particles = Locator::getParticles();
    particles.spawn(type, count);
  }

  // Sandbox method and other operations...
};
