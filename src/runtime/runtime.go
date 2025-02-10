package runtime

import (
	"fmt"
)

type Var struct {
	V     interface{}
	Type  string
	Scope int16
}

func Run(ast []map[string]interface{}, scope int16) {
	varsTable := make(map[string]Var)
	for _, node := range ast {
		name := node["name"].(string)
		switch {
		case name == "varDeclaration":
			var id = node["id"].(string)
			var value = node["value"].(string)
			var typeOfVar = node["type"].(string)
			varsTable[id] = Var{
				V:     value,
				Type:  typeOfVar,
				Scope: scope,
			}
			continue
		case name == "fnCall":
			var call = node["call"].(string)
			if call == "printLog" {
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
			}
		}
	}
}
