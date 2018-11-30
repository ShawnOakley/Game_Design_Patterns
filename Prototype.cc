// http://gameprogrammingpatterns.com/prototype.html

// The Prototype pattern offers a solution. The key idea is that an object can spawn other objects similar to itself.

class Monster
{
public:
  virtual ~Monster() {}
  virtual Monster* clone() = 0;

  // Other stuff...
};

// Consists of a Spawner and one or more Prototypes

// Spawner class
class Spawner
{
public:
  Spawner(Monster* prototype)
  : prototype_(prototype)
  {}

  Monster* spawnMonster()
  {
    return prototype_->clone();
  }

private:
  Monster* prototype_;
};

// Example Prototype class

class Ghost : public Monster {
public:
  Ghost(int health, int speed)
  : health_(health),
    speed_(speed)
  {}

  virtual Monster* clone()
  {
    return new Ghost(health_, speed_);
  }

private:
  int health_;
  int speed_;
};

// To create a spawner class for each type, 
// we create the prototype instance, then create
// a spawner holding that prototype

Monster* ghostPrototype = new Ghost(15, 3);
Spawner* ghostSpawner = new Spawner(ghostPrototype)

// Clones both the class of the prototype and its state
// So can subclass the prototype and have different spawners for different
// variants


// ALTERNATE APPROACH
// Could make spawn functions in the monster class for variants

Monster* spawnGhost()
{
  return new Ghost();
}

// Spawner class can simply store a function pointer
typedef Monster* (*SpawnCallback)();

class Spawner
{
public:
  Spawner(SpawnCallback spawn)
  : spawn_(spawn)
  {}

  Monster* spawnMonster()
  {
    return spawn_();
  }

private:
  SpawnCallback spawn_;
};

// So to create a spawner, would be 
Spawner* ghostSpawner = new Spawner(spawnGhost);


// Can also use C++ templates for this
class Spawner
{
public:
  virtual ~Spawner() {}
  virtual Monster* spawnMonster() = 0;
};

template <class T>
class SpawnerFor : public Spawner
{
public:
  virtual Monster* spawnMonster() { return new T(); }
};

// Invoking it with
Spawner* ghostSpawner = new SpawnerFor<Ghost>();


// SpawnerFor sets the type that the spawner produces

// Prototype most useful for attribute inheritance
// Specify prototype in data descriptors
// E.g., as json

{
  "name": "goblin grunt",
  "minHealth": 20,
  "maxHealth": 30,
  "resists": ["cold", "poison"],
  "weaknesses": ["fire", "light"]
}

{
  "name": "goblin wizard",
  "prototype": "goblin grunt",
  "spells": ["fire ball", "lightning bolt"]
}

{
  "name": "goblin archer",
  "prototype": "goblin grunt",
  "attacks": ["short bow"]
}