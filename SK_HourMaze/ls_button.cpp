/*****************************************
* ls_button.cpp
* Created by: loomsoft
* Website   : http://loomsoft.net
* Email     : jay@loomsoft.net
* Copyright : 2006 loomsoft
*
* Version: 1.1
*
* Details:
*         ls_button is a simple class 
* used in conjunction with 
* the allegro game programming library. It 
* provides an easy to use environment for 
* utilizing bitmap "buttons" in allegro.
*
* Additional Contributors:
*	TeamTerradactyl
*****************************************/

#include "ls_button.h"
#include <string.h> // For memcpy

template <class T>void ls_button::validate(T item, T min, T max)
{
		if(item < min)
				item = min;
		else if(item > max)
				item = max;
}

// This function initializes the button, pointing it's member bitmaps to the bitmaps
// passed as arguments. It also sets up which mouse detection mode to use:
// BOUNDING_BOX (0) - Use bounding box detection for mouse over/click
// PIXEL_PERFECT (1) - Use bounding pixel perfect detection for mouse over/click
void ls_button::initialize(BITMAP *bitmap_on, BITMAP *bitmap_down, BITMAP *bitmap_off, SAMPLE *sound_on, SAMPLE *sound_down, SAMPLE *sound_off, int MOUSE_DETECTION_MODE, int MOUSE_CLICK_MODE)
{
		// Set the bitmap pointers
		on_bitmap = bitmap_on;
		down_bitmap = bitmap_down;
		off_bitmap = bitmap_off;

		// Set the sound pointers
		on_sound = sound_on;
		down_sound = sound_down;
		off_sound = sound_off;

		// Set all the modes that were passed.
		mouse_detection_mode = MOUSE_DETECTION_MODE;
		mouse_sound_status = MOUSE_SOUND_STATUS_OFF;
		mouse_status = NOT_ON_FOCUS;
		mouse_click_mode = MOUSE_CLICK_MODE;

		// Set the bounding box to be the dimensions of the off_bitmap.
		bound_left = 0;
		bound_top = 0;
		bound_right = off_bitmap->w;
		bound_bottom = off_bitmap->h;

		// Set the x/y position to 0,0
		x_position = 0;
		y_position = 0;

		// Set the masking color to the default "magic pink".
		masking_color = makecol(255,0,255);

		// Turn off the bounding box display feature
		show_bbox = FALSE;

		// Set the volume/panning to the default.
		sound_volume = 255;
		sound_panning = 127;

		// Set the original frame to draw the button to off_bitmap
		draw_frame = DRAW_FRAME_OFF;

		// Set the button to an active state.
		is_active = TRUE;
}

// Internalizes the bitmap pointed to by the argument
// Updates the passed in pointer to point to the new internalized bitmap
int ls_button::internalize_bitmap(BITMAP *&bmp)
{
	// Only internalize the bitmap if its a valid pointer
	if(bmp != NULL)
	{
		BITMAP *tmp = create_bitmap(bmp->w, bmp->h);
		if(tmp == NULL)
		{
			return FALSE;
		}
		
		blit(bmp, tmp, 0, 0, 0, 0, bmp->w, bmp->h);
		
		bmp = tmp;
	}
	
	return TRUE;
}

// Internalizes the sample pointed to by argument
// Updates the passed in pointer to point to the new internalized sample
int ls_button::internalize_sample(SAMPLE *&smp)
{
	// Onlt internalize the sample if it's a valid pointer
	if(smp != NULL)
	{
		SAMPLE *tmp = create_sample(smp->bits, smp->stereo, smp->freq, smp->len);
		if(tmp == NULL)
		{
			return FALSE;
		}
		
		memcpy(tmp->data, smp->data, smp->len * ((smp->bits==8) ? 1 : sizeof(short)) * ((smp->stereo) ? 2 : 1)); 
		smp = tmp;
	}
	
	return TRUE;
}

int ls_button::internalize(void)
{
	// If we are already internalized, do nothing
	if(internalized == TRUE)
		return TRUE;
	
	// Internalize the bitmaps
	if(!internalize_bitmap(on_bitmap))
		return FALSE;
	if(!internalize_bitmap(down_bitmap))
		return FALSE;
	if(!internalize_bitmap(off_bitmap))
		return FALSE;
	
	// Internalize the samples
	if(!internalize_sample(on_sound))
		return FALSE;
	if(!internalize_sample(off_sound))
		return FALSE;
	if(!internalize_sample(down_sound))
		return FALSE;
	
	// We are now internalized.
	internalized = TRUE;
	return TRUE;
}

// This function will custom set the bounds to use for the bounding box when
// testing mouse input for the button.
// The bounds are set using a reference point of 0,0 on the buttons own bitmap,
// so if you wanted to set the bounding box to be 5 pixels greater than the
// actual bitmap you would pass these arguments
// set_bounds(-5, bitmap->w+5, -5, bitmap->h +5)
void ls_button::set_bounds(int left_bound, int right_bound, int top_bound, int bottom_bound)
{
	if(mouse_detection_mode == BOUNDING_BOX)
	{
		bound_left = left_bound;
		bound_right = right_bound;
		bound_top = top_bound;
		bound_bottom = bottom_bound;
	}
}

// This function will custom set the masking color to be used when using pixel
// perfect collision detection. The default is set to rbg (255,0,255)
void ls_button::set_masking_color(int red, int blue, int green)
{
	validate(red, 0, 255);
	validate(blue, 0, 255);
	validate(green, 0, 255);

	masking_color = makecol(red,blue,green);
}

// Sets if the drawing function should show the bounding box.
void ls_button::set_show_bounding_box(int value)
{
	show_bbox = value;
}

// Sets the sound volume for the button
void ls_button::set_sound_volume(int value)
{
	validate(value, 0, 255);

	sound_volume = value;
}

// Gets the sound volume for the button
int ls_button::get_sound_volume(void)
{
	return sound_volume;
}

// Sets the sound panning for the button
void ls_button::set_sound_panning(int value)
{
	validate(value, 0, 255);

	sound_panning = value;
}

// Gets the sound panning for the button
int ls_button::get_sound_panning(void)
{
	return sound_panning;
}

// Stops all the sounds being played by the button
void ls_button::stop_sounds(void)
{
	if(off_sound != NULL)
		stop_sample(off_sound);
	if(on_sound != NULL)
		stop_sample(on_sound);
	if(down_sound != NULL)
		stop_sample(down_sound);
}

// Sets the active state for the button
void ls_button::set_active(int value)
{
	is_active = value;
}

// Sets the coordinates for the button
void ls_button::set_coords(int x, int y)
{
	x_position = x;
	y_position = y;
}

// Gets the x coordinate of the button
int ls_button::get_x_coord(void)
{
	return x_position;
}

// Gets the y coordinate of the button
int ls_button::get_y_coord(void)
{
	return y_position;
}

// Set an identifying tag for the button
void ls_button::set_tag(int value)
{
  tag = value;
}

// Gets the identifying tag for the button
int ls_button::get_tag(void)
{
  return tag;
}

// This function polls the button and updates all things necessary to keep the button working.
void ls_button::poll(void)
{
	if(!is_active)
	{	
		return;
	}

	// If the mouse position is within the bounding box of the button
	if(is_mouse_over())
	{
		// If we are using pixel perfect detection mode
		if(mouse_detection_mode == PIXEL_PERFECT)
		{
			if(getpixel(off_bitmap, mouse_x-x_position,mouse_y-y_position) != masking_color)
			{
				if(mouse_b & 1)
				{
					draw_frame = DRAW_FRAME_DOWN;						

					if(mouse_sound_status != MOUSE_SOUND_STATUS_DOWN)
					{
						mouse_sound_status = MOUSE_SOUND_STATUS_DOWN;
					
						stop_sounds();	
						if(down_sound != NULL)
							play_sample(down_sound, sound_volume, sound_panning, 1000, FALSE);
					}
				}
				else
				{
					draw_frame = DRAW_FRAME_ON;

					if(mouse_sound_status != MOUSE_SOUND_STATUS_ON)
					{
						mouse_sound_status = MOUSE_SOUND_STATUS_ON;
					
						stop_sounds();	
						if(on_sound != NULL)
							play_sample(on_sound, sound_volume, sound_panning, 1000, FALSE);
					}
				}
				
			}
			else
			{

				draw_frame = DRAW_FRAME_OFF;
			
				if(mouse_status != MOUSE_STATUS_DOWN)
				{
					mouse_status = NOT_ON_FOCUS;
				}

				if(mouse_sound_status != MOUSE_SOUND_STATUS_OFF)
				{
						mouse_sound_status = MOUSE_SOUND_STATUS_OFF;
					
						stop_sounds();	
						if(off_sound != NULL)
							play_sample(off_sound, sound_volume, sound_panning, 1000, FALSE);
				}
			}
		}
		else
		{
			if(mouse_b & 1)
			{
				draw_frame = DRAW_FRAME_DOWN;

				if(mouse_sound_status != MOUSE_SOUND_STATUS_DOWN)
				{
					mouse_sound_status = MOUSE_SOUND_STATUS_DOWN;
				
					stop_sounds();	
					if(down_sound != NULL)
						play_sample(down_sound, sound_volume, sound_panning, 1000, FALSE);
				}
			}
			else
			{

				draw_frame = DRAW_FRAME_ON;

				if(mouse_sound_status != MOUSE_SOUND_STATUS_ON)
				{
					mouse_sound_status = MOUSE_SOUND_STATUS_ON;
			
					stop_sounds();	
					if(on_sound != NULL)
						play_sample(on_sound, sound_volume, sound_panning, 1000, FALSE);
				}
			}
		}
	}
	else
	{
		draw_frame = DRAW_FRAME_OFF;

		if(mouse_status != MOUSE_STATUS_DOWN)
		{
			mouse_status = NOT_ON_FOCUS;
		}

		if(mouse_sound_status != MOUSE_SOUND_STATUS_OFF)
		{
			mouse_sound_status = MOUSE_SOUND_STATUS_OFF;

			stop_sounds();
			if(off_sound != NULL)
				play_sample(off_sound, sound_volume, sound_panning, 1000, FALSE);
		}
	}
}

// This function tests to see if the mouse is "over" the button
int ls_button::is_mouse_over(void)
{
	if (mouse_x > (bound_left + x_position) &&
		mouse_x < (bound_right + x_position) &&
		mouse_y > (bound_top + y_position) &&
		mouse_y < (bound_bottom + y_position))
	{
		return TRUE;
	}

	return FALSE;
}

// This function tests to see if the button is clicked.
int ls_button::is_clicked(void)
{
	if(!is_active)
	{
		return FALSE;
	}

	if(is_mouse_over())
	{
		if(!(mouse_b & 1) && mouse_status == NOT_ON_FOCUS)
		{
			mouse_status = MOUSE_STATUS_UP;
		}

		if(mouse_b & 1 && mouse_click_mode == CLICK_MODE_CONSTANT)
		{
			return TRUE;
		}

		if(mouse_b & 1 && mouse_status != NOT_ON_FOCUS)
		{
			mouse_status = MOUSE_STATUS_DOWN;
		}
		else if(!(mouse_b & 1) && mouse_status == MOUSE_STATUS_DOWN)
		{
			mouse_status = MOUSE_STATUS_UP;
			
			// If we are using pixel perfect detection mode
			if(mouse_detection_mode == PIXEL_PERFECT)
			{
				if(getpixel(off_bitmap, mouse_x-x_position,mouse_y-y_position) != masking_color)
				{
					return TRUE;
				}
			}
			else 
			{
				return TRUE;
			}
		}

		return FALSE;
	}
	else if(!(mouse_b & 1))
	{
		mouse_status = NOT_ON_FOCUS;
	}

	return FALSE;
}


// This function will draw the button to the screen
void ls_button::draw_button(BITMAP *bitmap_buffer)
{
	if(!is_active)
	{
		return;
	}
		
	if(draw_frame == DRAW_FRAME_DOWN)
	{
		if(down_bitmap != NULL)
		{
			acquire_screen();
			draw_sprite(bitmap_buffer, down_bitmap, x_position, y_position);
			release_screen();
		}
	}
	else if(draw_frame == DRAW_FRAME_ON)
	{
		if(on_bitmap != NULL)
		{
			acquire_screen();
			draw_sprite(bitmap_buffer, on_bitmap, x_position, y_position);
			release_screen();
		}
	}
	else if(draw_frame == DRAW_FRAME_OFF)
	{
		if(off_bitmap != NULL)
		{
			acquire_screen();
			draw_sprite(bitmap_buffer, off_bitmap, x_position, y_position);
			release_screen();
		}
	}
				
	if(show_bbox)
	{
		rect(bitmap_buffer, bound_left+x_position, bound_top+y_position, bound_right+x_position, bound_bottom+y_position, makecol(255,0,0));
	}
}


// Constructor
ls_button::ls_button()
{
	// Set all values to either NULL or 0 so the button
	// has some sort of default values before being initialized
	on_bitmap = NULL;
	down_bitmap = NULL;
	off_bitmap = NULL;
		
	on_sound = NULL;
	down_sound = NULL;
	off_sound = NULL;

	x_position = 0;
	y_position = 0;

	bound_left = 0;
	bound_right = 0;
	bound_top = 0;
	bound_bottom = 0;
}

// Destructor
ls_button::~ls_button()
{
	if(internalized)
	{
		if(on_bitmap != NULL)
		{
			destroy_bitmap(on_bitmap);
		}
		
		if(down_bitmap != NULL)
		{
			destroy_bitmap(down_bitmap);
		}
		
		if(off_bitmap != NULL)
		{
			destroy_bitmap(off_bitmap);
		}
		
		if(on_sound != NULL)
		{
			destroy_sample(on_sound);
		}
		
		if(down_sound != NULL)
		{
			destroy_sample(down_sound);
		}
		
		if(off_sound != NULL)
		{
			destroy_sample(off_sound);
		}
	}
}
