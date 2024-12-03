package main

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"sync"
	"time"

	maelstrom "github.com/jepsen-io/maelstrom/demo/go"
)

var (
	counter int
	mu      sync.Mutex
)

func main() {
	n := maelstrom.NewNode()

	// Register a handler for the "generate" message that responds with an "generate_ok" and a unique ID.
	n.Handle("generate", func(msg maelstrom.Message) error {
		// Unmarshal the message body as an loosely-typed map.
		var body map[string]any
		if err := json.Unmarshal(msg.Body, &body); err != nil {
			return err
		}
		id := fmt.Sprintf("%s-%d", n.ID(), time.Now().UnixNano())
		// Get current counter value and increment it.
		mu.Lock()
		body["id"] = id
		incrementCounter()
		mu.Unlock()

		// Update the message type.
		body["type"] = "generate_ok"

		return n.Reply(msg, body)
	})

	// Execute the node's message loop. This will run until STDIN is closed.
	if err := n.Run(); err != nil {
		log.Printf("ERROR: %s", err)
		os.Exit(1)
	}
}

func getCounter() int {
	return counter
}

func incrementCounter() {
	counter++
}
