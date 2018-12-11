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