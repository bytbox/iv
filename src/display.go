package display

import (
	"buffer"
)

// A Display represents a display of a buffer. Each buffer may have multiple
// displays, although they will all share the same content.
type Display struct {
	Buffer     *buffer.Buffer // the buffer
	topLine    int            // the first visible line
	LeftColumn int            // the leftmost visible column
	Width      int            // the available width
	Height     int            // the available height
	yPos       int            // the absolute Y-position of the cursor
	xPos       int            // the absolute X-position of the cursor
	Config map[string]interface{}
}

func (d *Display) Init(b *buffer.Buffer) {
	d.Buffer = b
}

//
// Display information
//

// CursorX returns the zero-indexed x position, relative to the screen, at
// which the cursor should be displayed.
func (d *Display) CursorX() int {
	return d.xPos
}

// CursorY returns the zero-indexed y position, relative to the screen, at
// which the cursor should be displayed.
func (d *Display) CursorY() int {
	return d.yPos-d.topLine;
}

// FirstLine returns the zero-indexed line number of the first visible line.
func (d *Display) FirstLine() int {
	return d.topLine
}

// LastLine returns the zero-indexed line number of the last visible line.
func (d *Display) LastLine() int {
	if d.topLine+d.Height < len(d.Buffer.Lines)-1 {
		return d.topLine+d.Height
	}
	return len(d.Buffer.Lines)-1
}

//
// Cursor movement
//

func (d *Display) CursorDown() {

}

func (d *Display) CursorUp() {

}

func (d *Display) CursorLeft() {

}

func (d *Display) CursorRight() {

}


//
// Page movement (separate from the cursor)
//

func (d *Display) PageDown() {

}

func (d *Display) PageUp() {

}

func (d *Display) PageLeft() {

}

func (d *Display) PageRight() {

}
