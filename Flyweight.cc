
// Initial example class
class Tree
{
  private:
    Mesh mesh_;
    Texture bark_;
    Texture leaves_;
    Vector position_;
    double height_;
    double thickness_;
    Color barkTint_;
    Color leafTint_;
}

// First, split model in half.
// Put all common elements in separate subclasses

class TreeModel
{
  private:
    Mesh mesh_;
    Texture bark_;
    Texture leaves_;
}

// Then each instances retains reference to the TreeModel
class Tree
{
  private:
    TreeModel* model_;
    Vector position_;
    double height_;
    double thickness_;
    Color barkTint_;
    Color leafTint_;
}

// Similar to TypeObject pattern
// Difference is that TypeObject is about minimizing number classes
// by switching between types in a single class

// Flyweight useful wrt to the GPU with 'instanced rendering'
// Provide 1) common data and 2) list of instances and params
// that will be used to vary common data each time it's drawn

// Separates data into two kinds
// 1) intrinsic state == 'context-free' stuff
// 2) extrinsic sate == stuff that is unique to the instance

// Example 2 -- Game terrain tiles

// Store terrain instances in world and have tiles point to shared
// terrain type instances

class World
{
public:
  World()
  : grassTerrain_(1, false, GRASS_TEXTURE),
    hillTerrain_(3, false, HILL_TEXTURE),
    riverTerrain_(2, true, RIVER_TEXTURE)
  {}

private:
  Terrain grassTerrain_;
  Terrain hillTerrain_;
  Terrain riverTerrain_;

  // Other stuff...
};

// Pain the world with the tile types
void World::generateTerrain()
{
  // Fill the ground with grass.
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y <HEIGHT; y++)
    {
      // Sprinkle some hills.
      if (random(10) == 0)
      {
        tiles_[x][y] = &hillTerrain_;
      }
      else
      {
        tiles_[x][y] = &grassTerrain_;
      }
    }
  }
  // Lay a river
  int x = random(WIDTH);
  for (int y = 0; y < HEIGHT; y++) {
    tiles_[x][y] = &riverTerrain_;
  }
}

// Expose the terrain object directly on the world
const Terrain& World::getTile(int x , int y) const
{
  return *tiles_[x][y];
}

// Can now get movement cost from the tile
int cost = world.getTile(2, 3).getMovementCost();
