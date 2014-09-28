
package main

import(
  "fmt"
  "path/filepath"
  "os/exec"
  "strings"
)

func main() {
  files, _ := filepath.Glob("../mapgen/map-10[0]*.xml")
  fmt.Println(files)
  for _,element := range files {
    out := strings.Split(element, "/")[2]
    out = strings.Split(out, ".")[0] + ".gexf"
    fmt.Println(out)
    command := exec.Command("prun", "-no-panda", "../src/cgc", "1", "-G", "-m",
    element, "-g", out)
    command.Run()
  }
}
