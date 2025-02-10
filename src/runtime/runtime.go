package runtime

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type Var struct {
	V     interface{}
	Type  string
	Scope int16
}

func Run(ast []map[string]interface{}, scope int16) interface{} {
	varsTable := make(map[string]Var)
	for _, node := range ast {
		name := node["name"].(string)
		switch {
		case name == "varDeclaration":
			var id = node["id"].(string)
			if value, ok := node["value"].(string); ok {
				var typeOfVar = node["type"].(string)
				varsTable[id] = Var{
					V:     value,
					Type:  typeOfVar,
					Scope: scope,
				}
				continue
			} else if valueMap, ok := node["value"].(map[string]interface{}); ok {
				var typeOfVar = node["type"].(string)
				varsTable[id] = Var{
					V:     Run([]map[string]interface{}{valueMap}, 0),
					Type:  typeOfVar,
					Scope: scope,
				}
				continue
			}
		case name == "fnCall":
			var call = node["call"].(string)
			switch call {
			case "printLog":
				args := node["args"].([]map[string]interface{})
				for _, arg := range args {
					w := arg["w"].(string)
					str := arg["str"].(bool)
					if str {
						fmt.Printf("%s ", w)
					} else {
						fmt.Printf("%s ", varsTable[w].V)
					}
				}
				fmt.Print("\n")
			case "input":
				args := node["args"].([]map[string]interface{})
				fmt.Print(args[0]["w"])
				reader := bufio.NewReader(os.Stdin)
				text, _ := reader.ReadString('\n')
				text = strings.Replace(text, "\n", "", -1)
				return text
			}
		}
	}
	return nil
}
