/**
  * Keyboard interface to listen for keypresses
  */

/**
  * Name:   keyboard_init
  * Args:   None
  * Return: None
  *
  * Initializes the keyboard to receive input
  */
void keyboard_init();

/**
  * Name:   keyboard_get_key
  * Args:   None
  * Return: char - The character currently being pressed
  *
  * Queries the keyboard for any key being pressed
  */
char keyboard_get_key();

/**
  * Name:   keyboard_next_key
  * Args:   None
  * Return: char - The next character to be pressed
  *
  * Waits for the keyboard to hit a new key, and returns the key
  */
char keyboard_next_key();

/**
  * Name:   keyboard_register_keypressed
  * Args:   void callback(char) - A function to be called when a key is pressed
  * Return: None
  *
  * Calls the supplied function on each keypress, and supplies it with the 
  * key that was just pressed
  */
void keyboard_register_keypressed(void (*callback)(char));

/**
  * Name:   keyboard_register_keyreleased
  * Args:   void callback(char) - A function to be called when a key is released
  * Return: None
  *
  * Calls the supplied function each time a key is released, and supplies it 
  * with the key that was just released
  */
void keyboard_register_keyreleased(void (*callback)(char));

