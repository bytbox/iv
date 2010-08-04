package actions

import (
	"display"
	"os"
)

var actionList = map[string]Action{
	"quit": QuitAction{},
	"down": DownAction{},
	"left": LeftAction{},
	"right": RightAction{},
	"up": UpAction{},
}

// Get() returns the Action associated with the string
func Get(name string) Action {
	action, ok := actionList[name]
	if ok {
		return action
	}
	return &VoidAction{}
}

type Action interface {
	Act(Callback) os.Error
}

type Callback interface {
	Shutdown() os.Error
	ActiveDisplay() *display.Display
	AuxDisplay() *display.Display
}

type VoidAction struct{}
func (a VoidAction) Act(callback Callback) os.Error {
	return nil
}

type QuitAction struct{}
func (a QuitAction) Act(callback Callback) os.Error {
	err := callback.Shutdown()
	if err != nil {
		return err
	}
	os.Exit(0)
	return nil
}

type RightAction struct{}
func (RightAction) Act(callback Callback) (err os.Error) {
	callback.ActiveDisplay().CursorRight()
	return
}

type LeftAction struct{}
func (LeftAction) Act(callback Callback) (err os.Error) {
	return nil
}

type UpAction struct{}
func (UpAction) Act(callback Callback) (err os.Error) {
	return nil
}

type DownAction struct{}
func (DownAction) Act(callback Callback) (err os.Error) {
	return nil
}
