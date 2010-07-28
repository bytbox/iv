package cursesview

import (
	"buffer"
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

type cView struct {
	win *curses.Window

	splitMode   int
	mainDisplay *display.Display
	auxDisplay  *display.Display
}

func NewCursesView() *cView {
	return &cView{}
}

func (v *cView) Init() os.Error {
	v.splitMode = NOSPLIT
	curses.Initscr() /* start ncurses */
	if curses.Stdwin == nil {
		v.Shutdown()
		return &errors.IVError{"Could not initialize curses"}
	}
	v.win = curses.Stdwin
	curses.Start_color() /* enable the use of color */
	curses.Noecho()      /* don't immediately echo characters */
	v.win.Keypad(true)
	return nil
}

func (v *cView) Shutdown() os.Error {
	curses.Endwin()
	return nil
}

// Start() starts the control loop for the curses view.
func (v *cView) Start() os.Error {
	// for now, don't make it a seperate goroutine
	return v.control()
}

// The control loop for the curses view
func (v *cView) control() os.Error {
	for {
		v.Refresh()
		inp := int32(v.win.Getch())
		v.win.Addch(0, 0, inp, 0)
	}
	return nil
}

func (v *cView) Hide() os.Error {
	return errors.NotImplementedError()
}

func (v *cView) Refresh() os.Error {
	_, maxy := v.win.Getmax()
	// clear and draw the background
	v.win.Clear()
	for i :=0; i<maxy; i++ {
		v.win.Addch(0, i, '~', curses.A_BOLD)
	}

	// now actually refresh the window
	return v.win.Refresh()
}

func (v *cView) OpenFile(filename string) os.Error {
	return errors.NotImplementedError()
}

func (v *cView) OpenBuffer(buffer *buffer.Buffer) os.Error {
	v.mainDisplay = &display.Display{}
	v.mainDisplay.Init(buffer)
	return nil
}
