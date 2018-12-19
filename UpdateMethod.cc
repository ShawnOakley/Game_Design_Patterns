// Update Method
// Definition:
    // The game world maintains a collection of objects. 
    // Each object implements an update method that simulates one frame of the object’s behavior. 
    // Each frame, the game updates every object in the collection.

// In ECS (Entity Component System) update lives on the Component, not the Entity
// Avoids subclassing the Entity

// Issues - 
// (1) deltaTime in Update
// Many games use a variable time step. 
// In those, each turn of the game loop may simulate a larger or smaller slice of time depending on how 
// long it took to process and render the previous frame.
// That means that each update() call needs to know how far the hand of the virtual clock has swung, 
// so you’ll often see the elapsed time passed in. 


// Example:
void Skeleton::update(double elapsed)
{
  if (patrollingLeft_)
  {
    x -= elapsed;
    if (x <= 0)
    {
      patrollingLeft_ = false;
      x = -x;
    }
  }
  else
  {
    x += elapsed;
    if (x >= 100)
    {
      patrollingLeft_ = true;
      x = 100 - (x - 100);
    }
  }
}

// How are dormant objects handled?
// Maintain an action of 'live' objects and only call update on those.