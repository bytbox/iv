package actions

import (
	"display"
	"os"
)

var actionList = map[string]Action{
	"quit": QuitAction{},
}

// Get() returns the Action associated with the string
func Get(name string) Action {
	return actionList[name]
}

type Action interface {
	Act(Callback) os.Error
}

type Callback interface {
	Shutdown() os.Error
	ActiveDisplay() *display.Display
}

type VoidAction struct {}
func (a VoidAction) Act(callback Callback) os.Error {
	return nil
}

type QuitAction struct {}
func (a QuitAction) Act(callback Callback) os.Error {
	return callback.Shutdown()
}
