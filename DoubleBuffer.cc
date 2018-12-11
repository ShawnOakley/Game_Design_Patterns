// Double Buffer Pattern

// Intent: Cause a series of sequential operations to appear instantaneous or simultaneous.
// Description:
// Instead of a single framebuffer, we have two. 
// One of them represents the current frame. It’s the one the video hardware is reading from. 
// The GPU can scan through it as much as it wants whenever it wants.

// Meanwhile, our rendering code is writing to the other framebuffer. 
// When our rendering code is done drawing the scene, it swaps the buffers. 
// This tells the video hardware to start reading from the second buffer now instead of the first one. 
// As long as it times that switch at the end of a refresh, we won’t get any tearing, and the entire scene will appear all at once.

// When to use it:
// 1) We have some state that is being modified incrementally.

// 2) That same state may be accessed in the middle of modification.

// 3) We want to prevent the code that’s accessing the state from seeing the work in progress.

// 4) We want to be able to read the state and we don’t want to have to wait while it’s being written.


// Example Code:
class Framebuffer
{
public:
  Framebuffer() { clear(); }

  void clear()
  {
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
      pixels_[i] = WHITE;
    }
  }

  void draw(int x, int y)
  {
    pixels_[(WIDTH * y) + x] = BLACK;
  }

  const char* getPixels()
  {
    return pixels_;
  }

private:
  static const int WIDTH = 160;
  static const int HEIGHT = 120;

  char pixels_[WIDTH * HEIGHT];
};

class Scene
{
public:
  Scene()
  : current_(&buffers_[0]),
    next_(&buffers_[1])
  {}

  void draw()
  {
    next_->clear();

    next_->draw(1, 1);
    // ...
    next_->draw(4, 3);

    swap();
  }

  Framebuffer& getBuffer() { return *current_; }

private:
  void swap()
  {
    // Just switch the pointers.
    Framebuffer* temp = current_;
    current_ = next_;
    next_ = temp;
  }

  Framebuffer  buffers_[2];
  Framebuffer* current_;
  Framebuffer* next_;
};