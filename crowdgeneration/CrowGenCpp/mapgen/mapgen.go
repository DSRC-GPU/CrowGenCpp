
package main

import(
  "fmt"
  "flag"
  "bufio"
  "os"
  "strconv"
)


func main() {
  crowd := `<crowd>
  <groupdescriptor gid="1">
    <population>%v</population>
    <sources>
      <source type="spawn">
        <xborder start="0" end="100"></xborder>
        <yborder start="505" end="605"></yborder>
      </source>
    </sources>
    <sinks>
    </sinks>
    <map width="1000" height="1000" rows="4" cols="1">
      2
      2
      6
      8
    </map>
  </groupdescriptor>
  <groupdescriptor gid="2">
    <population>%v</population>
    <sources>
      <source type="spawn">
        <xborder start="900" end="1000"></xborder>
        <yborder start="395" end="495"></yborder>
      </source>
    </sources>
    <sinks>
    </sinks>
    <map width="1000" height="1000" rows="4" cols="1">
      2
      4
      8
      8
    </map>
  </groupdescriptor>
</crowd>`

  var numpeople = flag.Int("numpeople", 100, "The maximum number of people in the crowd");
  flag.Parse()
  fmt.Println(*numpeople)

  for i := 100; i <= *numpeople; i += 100 {
    halfpeople := i / 2
    outfile, _ := os.Create("map-" + strconv.Itoa(i) + ".xml")
    printer := bufio.NewWriter(outfile)
    fmt.Fprintf(printer, crowd, halfpeople, halfpeople)
    printer.Flush()
    outfile.Close()
  }
}
