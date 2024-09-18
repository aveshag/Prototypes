package server

import (
	server "echo_server/config"
	"fmt"
	"io"
	"log"
	"net"
)

func readFromBuffer(conn net.Conn) ([]byte, error) {
	// reading 512 bytes in one go
	var msg []byte = make([]byte, 512)
	_, err := conn.Read(msg)
	if err != nil {
		return nil, err
	}
	fmt.Printf("Message from client: %s\n", string(msg[:]))
	return msg, nil
}

func writeToBuffer(conn net.Conn, msg []byte) error {
	msg = []byte(fmt.Sprintf("Message from server: %s\n", string(msg[:])))
	_, err := conn.Write(msg)
	return err
}

func StartServer() {
	log.Println("Starting tcp server...")

	socket := server.Address + ":" + fmt.Sprint(server.Port)
	log.Printf("Socket: %s", socket)

	ln, err := net.Listen("tcp", socket)
	if err != nil {
		panic(err)
	}

	for {
		conn, err := ln.Accept()

		if err != nil {
			panic(err)
		}
		fmt.Printf("Connecting client %s\n", conn.RemoteAddr().String())

		for {
			// Reading 512 byte chunks and writing same to client
			msg, err := readFromBuffer(conn)
			if err != nil {
				conn.Close()
				if err == io.EOF {
					fmt.Printf("Terminating client %s\n", conn.RemoteAddr().String())
					break
				}
				log.Println(err)
			}

			err = writeToBuffer(conn, msg)
			if err != nil {
				log.Println(err)
			}
		}
	}

}
