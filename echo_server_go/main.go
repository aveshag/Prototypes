package main

import (
	"flag"
	config "echo_server/config" // packgage is imported as server but using config alias
	"echo_server/server" // packgage is imported as server
)



func setFlags() {
	flag.StringVar(&config.Address, "address", "127.0.0.1", "address of server")
	flag.IntVar(&config.Port, "port", 1234, "port of =server")
	flag.Parse()
}

func main() {
	setFlags()
	server.StartServer()
}
