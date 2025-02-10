package runtime

import "fmt"

func Run(ast []map[string]interface{}) {
	for _, node := range ast {
		call := node["call"].(string)
		//name := node["name"].(string)

		switch {
		case call == "printLog":
			args := node["args"].([]map[string]interface{})
			for _, arg := range args {
				w := arg["w"].(string)
				fmt.Printf("%s ", w)
			}
		}
	}
}
