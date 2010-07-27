package errors

import "fmt"

// IVError represents an error originating in the IV editor
type IVError struct {
	Message string
}

func (ce IVError) String() string {
	return fmt.Sprintf("iv: %s", ce.Message)
}

func NotImplementedError() IVError {
	return IVError{"not yet implemented"}
}
