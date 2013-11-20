/*****************************************
* ls_button.h
* Created by: loomsoft
* Website   : http://loomsoft.net
* Email     : jay@loomsoft.net
* Copyright : 2006 loomsoft
*
* Version: 1.1
*
* Details:
*         ls_button class
* used in conjunction with 
* the allegro game programming library. It 
* provides an easy to use environment for 
* utilizing bitmap "buttons" in allegro.
*
* Additional Contributors:
*	TeamTerradactyl
*****************************************/

#pragma once
#ifndef _LS_BUTTON_
#define _LS_BUTTON_

#include <allegro.h>

/*#define LS_BUTTON_BOUNDING_BOX 0
#define LS_BUTTON_PIXEL_PERFECT 1

#define LS_BUTTON_MOUSE_SOUND_STATUS_ON 0
#define LS_BUTTON_MOUSE_SOUND_STATUS_DOWN 1
#define LS_BUTTON_MOUSE_SOUND_STATUS_OFF 3

#define LS_BUTTON_CLICK_MODE_RELEASE 0
#define LS_BUTTON_CLICK_MODE_CONSTANT 1

#define LS_BUTTON_MOUSE_STATUS_DOWN 0
#define LS_BUTTON_MOUSE_STATUS_UP 1
#define LS_BUTTON_NOT_ON_FOCUS 3

#define LS_BUTTON_DRAW_FRAME_ON 0
#define LS_BUTTON_DRAW_FRAME_DOWN 1
#define LS_BUTTON_DRAW_FRAME_OFF 2*/

class ls_button //The LS Button class for simple bitmap button routines
{
private:
	int x_position;// X position of the button on the screen
	int y_position;// Y position of the button on the screen
	int draw_frame;// Which image to draw

  int tag; // Identifying tag

	// These integers are used to store the bounds for the bounding box which is used
	// during testing the mouse input for the button. One might question as to using
	// bound left, and bound top -- since the x_position and the y_position would be the 
	// same exact value. The fact is that those values have their own variable because
	// the user may custom-set the bounds for the button if they choose.
	int bound_left; // The left bound for the bounding box.
	int bound_right; // The right bound for the bounding box.
	int bound_top; // The top bound for the bounding box.
	int bound_bottom; // The bottom bound for the bounding box.
	
	int is_active; // The state as to whether the button is "active" or not

	int masking_color; // The color used for testing with pixel perfect
	int sound_volume; // Holds the numerical value for the volume of the sound
	int sound_panning; // Holds the numerical value for the panning of the sound
	int show_bbox; // Variable to determine whether to show the bounding box or not

	int mouse_status; // Holds the status for the button being pushed down or not
	int mouse_sound_status; // Used to determine when to play sounds;
	int mouse_detection_mode; // Which detection mode is being used?
	int mouse_click_mode; // Which clicking mode will we use
	
	int internalized; // Flag determining whether the bitmap and sound data have been internalized or not

	template <class T> void validate(T, T, T);
	
	// Internal constants for differentiating between sound on/down/off
	static const int MOUSE_SOUND_STATUS_ON = 0;
	static const int MOUSE_SOUND_STATUS_DOWN = 1;
	static const int MOUSE_SOUND_STATUS_OFF = 3;

	// Internal constants for differentiating between different moust statuses
	static const int MOUSE_STATUS_DOWN = 0;
	static const int MOUSE_STATUS_UP = 1;
	static const int NOT_ON_FOCUS = 3;

	// Internal constants for frame drawing on the different button states
	static const int DRAW_FRAME_ON = 0;
	static const int DRAW_FRAME_DOWN = 1;
	static const int DRAW_FRAME_OFF = 2;
	
	// Internalizes the bitmap pointed to by the argument
	// Updates the passed in pointer to point to the new internalized bitmap
	int internalize_bitmap(BITMAP *&);
	
	// Internalizes the sample pointed to by the argument
	// Updates the passed in pointer to point to the new internalized sample
	int internalize_sample(SAMPLE *&);

// Protected Members of LS Button
protected:
	//Please keep in mind that LS Button assumes the 
	//use of equal dimensioned bitmap images for each 
	//state of the button.
	BITMAP *on_bitmap;//Bitmap to display while the mouse cursor is "on" the button.
	BITMAP *down_bitmap;//Bitmap to display while the mouse button is pressed down while "on" the button.
	BITMAP *off_bitmap;//Bitmap to display while the mouse cursor is "off" the button.

	SAMPLE *on_sound;// Sound to play when the mouse moves "over" the button.
	SAMPLE *down_sound;// Sound to play when the mouse button is pressed down while "on" the button.
	SAMPLE *off_sound;// Sound to play when the mouse moves "off" the button.


//Public members of LS Button
public:
	// Default ctor/dtor
	ls_button();
	virtual ~ls_button();

	static const int BOUNDING_BOX = 0;
	static const int PIXEL_PERFECT = 1;
	
	static const int CLICK_MODE_RELEASE = 0;
	static const int CLICK_MODE_CONSTANT = 1;
	
	
	// This function initializes the button, pointing it's member bitmaps to the bitmaps
	// passed as arguments. It also sets up which mouse detection mode to use:
	// LS_BUTTON_BOUNDING_BOX (0) - Use bounding box detection for mouse over/click
	// LS_BUTTON_PIXEL_PERFECT (1) - Use bounding pixel perfect detection for mouse over/click
	void initialize(BITMAP *bitmap_on, BITMAP *bitmap_down, BITMAP *bitmap_off, SAMPLE *sound_on, SAMPLE *sound_down, SAMPLE *sound_off, int MOUSE_DETECTION_MODE, int MOUSE_CLICK_MODE);
	
	// Method should only be called after initialization. Makes internal copies of the image/sound data.
	// If pointers set in initialize are null the internalize will do nothing for that given component
	int internalize(void);

	// This function will custom set the bounds to use for the bounding box when
	// testing mouse input for the button.
	// The bounds are set using a reference point of 0,0 on the buttons own bitmap,
	// so if you wanted to set the bounding box to be 5 pixels greater than the
	// actual bitmap you would pass these arguments
	// set_bounds(-5, bitmap->w+5, -5, bitmap->h +5)
	void set_bounds(int left_bound, int right_bound, int top_bound, int bottom_bound);

	// This function will custom set the masking color to be used when using pixel
	// perfect collision detection. The default is set to rbg (255,0,255)
	void set_masking_color(int red, int blue, int green);

	// Sets if the drawing function should show the bounding box.
	void set_show_bounding_box(int value);

	// Sets the sound volume for the button
	void set_sound_volume(int value);

	// Gets the sound volume for the button
	int get_sound_volume(void);

	// Sets the sound panning for the button
	void set_sound_panning(int value);

	// Gets the sound panning for the button
	int get_sound_panning(void);

	// Stops all sounds being played by the button
	void stop_sounds(void);

	// Sets the active state for the button
	void set_active(int value);
	
	// Gets the active state for the button
	int get_active(void);

	// Sets the coordinates of the button
	void set_coords(int x, int y);

	// Gets the x coordinate of the button
	int get_x_coord(void);

	// Gets the y coordinate of the button
	int get_y_coord(void);

  // Set an identifying tag for the button
  void set_tag(int value);

  // Gets the identifying tag for the button
  int get_tag(void);

	// This function tests to see if the button is clicked.
	int is_clicked(void);

	// This function determines whether the mouse is "over" the button
	int is_mouse_over(void);

	// This function polls the button, and updates all things necessary to keep it working
	void poll(void);

	// This function will draw the button to the screen
	void draw_button(BITMAP *bitmap_buffer);


};

#endif
