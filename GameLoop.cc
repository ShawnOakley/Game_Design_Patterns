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