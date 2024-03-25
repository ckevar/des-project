package main

import (
	"fmt"
	"net"
	"os"
)

// Available operations on Action sets
const NON_AS	uint8 = 0
const UPDATE_AS	uint8 = 1
const NEW_AS	uint8 = 2

const UI_UPDATE_AS 	= "update"
const UI_NEW_AS 	= "new"
const UI_GET_AS		= "get"
const UI_GET_ALL 	= "all"

const AS_NAME_SIZE		= 8
const MIN_INPUT_ARG 	= 4

func uiop_to_bin(usr_method string) uint8 {
	switch {
	case usr_method == UI_UPDATE_AS:
		return UPDATE_AS
	case usr_method == UI_NEW_AS:
		return NEW_AS
	default:
		fmt.Println("Error: one of the following methods is required")
		fmt.Println("\tupdate, new")
		return NON_AS
	}
}

func validate_as_name(as_name string) string {
	if len(as_name) > AS_NAME_SIZE {
		as_name = as_name[:AS_NAME_SIZE]
		fmt.Println("Warning: Action Set name truncated")
		fmt.Println("\t", as_name)
	} else if len(as_name) < AS_NAME_SIZE {
		for len(as_name) < AS_NAME_SIZE {
			as_name += " "
		}
	}
	return as_name
}

func main() {
	if (len(os.Args) < MIN_INPUT_ARG) {
		fmt.Println("Error: usage <IP:PORT> <Operation> <Action Set Name>")
		return
	}

	var addr_port = os.Args[1]		// Server <IP>:<PORT>
	var ui_operation = os.Args[2]	// Operation requested 
	var ui_as_name = os.Args[3]		// Action Set name
	var method = NON_AS

	method = uiop_to_bin(ui_operation)
	if method == NON_AS {
		return
	}

	ui_as_name = validate_as_name(ui_as_name)
	
	if method == UPDATE_AS && len(os.Args) <= MIN_INPUT_ARG {
		fmt.Println("Error: A parameter to be updated is needed")
		return
	}

	conn, err := net.Dial("tcp", addr_port)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	defer conn.Close()

	// send data to the server
	data := []byte("Hello, Server!")
	 _, err = conn.Write(data)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}
}
