package cursesview

import (
	"buffer"
	"conf"
	"curses"
	"display"
	"errors"
	"os"
)

// The split modes
const (
	NOSPLIT = iota
	HSPLIT
	VSPLIT
)

var Initstring string

type cView struct {
	win         *curses.Window
	splitMode   int
	mainDisplay *display.Display
	auxDisplay  *display.Display
	promptLine  string
	config      *conf.Configuration
	message     string
}

func NewCursesView() *cView {
	return &cView{}
}

func (v *cView) Init(config *conf.Configuration) os.Error {
	// set the configuration
	v.config = config
	v.splitMode = NOSPLIT
	v.message = Initstring
	return nil
}

func (v *cView) Shutdown() os.Error {
	curses.Endwin()
	return nil
}

// Start() starts the control loop for the curses view.
func (v *cView) Start() os.Error {
	curses.Initscr() /* start ncurses */
	if curses.Stdwin == nil {
		v.Shutdown()
		return &errors.IVError{"Could not initialize curses"}
	}
	v.win = curses.Stdwin
	curses.Start_color() /* enable the use of color */
	curses.Noecho()      /* don't immediately echo characters */
	v.win.Keypad(true)
	// for now, don't make it a seperate goroutine
	return v.control()
}

// The control loop for the curses view
func (v *cView) control() os.Error {
	// generate the keymap
	v.config.UpdateKeymap()
	for {
		// refresh and get the next character
		v.Refresh()
		inp := conf.Key(v.win.Getch())
		// look up the character in the configuration table
		action, ok := v.config.ActiveKeymap[inp]
		if ok {
			action.Act(v)
		}
	}
	return nil
}

func (v *cView) Hide() os.Error {
	return errors.NotImplementedError()
}

func (v *cView) Refresh() os.Error {
	maxx, maxy := v.win.Getmax()
	// clear the window
	v.win.Clear()

	// draw the display(s)
	switch {
	case v.splitMode == NOSPLIT:
		v.drawDisplay(v.mainDisplay, 0, 0, maxx, maxy-1)
	case v.splitMode == VSPLIT:
		v.drawDisplay(v.mainDisplay, 0, 0, maxx, (maxy-1)/2)
		v.drawDisplay(v.auxDisplay, 0, (maxy - 1/2), maxx, maxy-1)
	case v.splitMode == HSPLIT:
		v.drawDisplay(v.mainDisplay, 0, 0, maxx/2, maxy-1)
		v.drawDisplay(v.mainDisplay, maxx/2, 0, maxx, maxy-1)
	}

	// draw the message or prompt at the bottom
	v.win.Addstr(0, maxy-1, v.message, 0)

	// now actually refresh the window
	return v.win.Refresh()
}

// drawDisplay() draws the buffer within the specified coordinates
func (v *cView) drawDisplay(disp *display.Display, startx, starty, maxx, maxy int) {
	// update the view's information
	disp.Height = maxy - starty
	disp.Width = maxx - startx
	b := disp.Buffer
	// for each line
	for lineno := disp.FirstLine(); lineno < disp.LastLine(); lineno++ {
		line := b.Lines[lineno]
		y := lineno - disp.FirstLine() + starty
		v.win.Addstr(0, y, line, 0)
	}
	// clear this section of the screen
	for y := disp.LastLine() + starty; y < maxy; y++ {
		v.win.Addch(0, y, '~', curses.A_BOLD)
	}
}

func (v *cView) ActiveDisplay() *display.Display {
	return v.mainDisplay
}

func (v *cView) AuxDisplay() *display.Display {
	return v.auxDisplay
}

func (v *cView) SetMessage(msg string) {
	v.message = msg
}

func (v *cView) Prompt(string) string {
	return ""
}

func (v *cView) OpenFile(filename string) os.Error {
	b, err := buffer.NewBufferFromFile(filename)
	if err != nil {
		return err
	}
	return v.OpenBuffer(b)
}

func (v *cView) OpenBuffer(buffer *buffer.Buffer) os.Error {
	v.mainDisplay = &display.Display{}
	v.mainDisplay.Init(buffer)
	return nil
}
