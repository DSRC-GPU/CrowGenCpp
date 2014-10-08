
package main

import(
  "fmt"
  "path/filepath"
  "os/exec"
  "strings"
)

func main() {
  files, _ := filepath.Glob("../mapgen/map-*.xml")
  // fmt.Println(files)
  for i := 0; i < len(files); i++ {
    element := files[i]
    out := strings.Split(element, "/")[2]
    out = strings.Split(out, ".")[0] + ".gexf"
    var size int
    fmt.Sscanf(out, "map-%d.gexf", &size)
    if size > 10000 {
      fmt.Println("Skipping gexf with", size, "nodes.")
      continue
    }
    fmt.Println(out)

    command := exec.Command("prun", "-no-panda", "../src/cgc", "1", "-G", "-t",
    "50", "-m", element, "-g", out)
    command.Run()
  }
}
