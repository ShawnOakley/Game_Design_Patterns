// Definition
// A game loop runs continuously during gameplay. 
// Each turn of the loop, it processes user input without blocking, updates the game state, and renders the game. 
// It tracks the passage of time to control the rate of gameplay.


// The difference between an “engine” and a “library”: 
// With libraries, you own the main game loop and call into the library. 
// An engine owns the loop and calls into your code.

// variable or fluid time step:
// choose a time step to advance based on how much real time passed since the last frame. 
// The longer the frame takes, the bigger steps the game takes. It always keeps up with real time because it will take bigger and bigger steps to get there. 

// Introducing time*delta-T
double previous = getCurrentTime();
double lag = 0.0;
while (true)
{
  double current = getCurrentTime();
  double elapsed = current - previous;
  previous = current;
  lag += elapsed;

  processInput();

  while (lag >= MS_PER_UPDATE)
  {
    update();
    lag -= MS_PER_UPDATE;
  }

  render();
}

// MS_PER_UPDATE is just the granularity we use to update the game. 
// The shorter this step is, the more processing time it takes to catch up to real time. 
// The longer it is, the choppier the gameplay is. Ideally, you want it pretty short, often faster than 60 FPS, so that the game simulates with high fidelity on fast machines.

// Interpolating intermediate points
// Conveniently, we actually know exactly how far between update frames we are when we render: it’s stored in lag. 
// We bail out of the update loop when it’s less than the update time step, not when it’s zero. That leftover amount? That’s how far into the next frame we are.
// When we go to render, we’ll pass that in:

render(lag / MS_PER_UPDATE);

// The renderer knows each game object and its current velocity. 
// Say that bullet is 20 pixels from the left side of the screen and is moving right 400 pixels per frame. 
// If we are halfway between frames, then we’ll end up passing 0.5 to render(). So it draws the bullet half a frame ahead, at 220 pixels. Ta-da, smooth motion.

// Unity Monobehaviour lifecycle
// http://whatiseeinit.blogspot.com/2012/10/unity3d-monobehaviour-lifecycle.html

// Options for Event Loop
// 1) Platform’s event loop
    // - simple
    // - plays nice with platform
    // - lose control over timing
// 2) Game engine’s loop
    // - subject to particulars of engine
// 3) Own event loop
    // - total control
    // - have to manage interface with platform

// Power consumption management options
// 1) Run as fast as you can
// 2) Clamp the frame rate

// Gameplay speed control options
// 1) Fixed time step with no synchronization
    // - simple
    // - speed effected by hardware and game complexity
// 2)Fixed time step with synchronization
    // - adds slowdown to avoid game running too fast
    // - still can run too slowly
// 3) Fixed update time step, variable rendering/
    // - use of time * delta time to avoid slow cases
    // - more complex


